const fs = require("fs");
const { request } = require('undici');
const { ethers } = require("ethers");
const { join, dirname } = require("path");

const QUERY_URL = "http://127.0.0.1/admin/v1.0/SendQryOrder?ExchangeID=ARB";
const WITHDRAW_URL = "http://127.0.0.1/admin/v1.0/SendWithdraw";
const QUERY721_URL = "http://127.0.0.1:8080/backend/v1/common/getWithdrawCardList";
const WITHDRAW721_URL = "http://127.0.0.1:8080/backend/v1/common/delCard";
const NOTIFY_URL = "http://127.0.0.1:8080/backend/v1/common/notify";

// ===== 配置 =====
const RPC_URLS = [
  "https://arb1.arbitrum.io/rpc",
  "https://rpc.ankr.com/arbitrum",
  "https://endpoints.omniatech.io/v1/arbitrum/one/public"
];

const PRIVATE_KEY_FILE = "../privateKey.txt";

// ERC20 地址（Arbitrum One 主网）
const TOKENS = {
  ETH: null,
  USDC: "0xaf88d065e77c8cC2239327C5EDb3A432268e5831",
  USDT: "0xfd086bc7cd5c481dcc9c85ebe478a1c0b69fcbb9"
};

// ===== 工具函数 =====
async function getProvider() {
  for (const url of RPC_URLS) {
    try {
      const provider = new ethers.providers.JsonRpcProvider(url, {
        name: "arbitrum",
        chainId: 42161
      });
      await provider.getBlockNumber();
      console.log(`使用节点: ${url}`);
      return provider;
    } catch {
      console.warn(`节点不可用: ${url}`);
    }
  }
  throw new Error("所有 RPC 节点都不可用！");
}

// ===== 主函数 =====
async function main() {
  const { body } = await request(QUERY_URL, { method: 'GET' });
  const res = await body.json();
  // const res = await axios.get(QUERY_URL);
  const orders = res.result;
  console.log('orders', orders.length)
  for (const order of orders) {
    // console.log('order', order)
    const TOKEN = order.data.ClearCurrency;
    const TO = order.data.MemberID;
    const AMOUNT = order.data.FrozenMoney.toString();
    try {
      // 读取私钥
      const PRIVATE_KEY = fs.readFileSync(PRIVATE_KEY_FILE, "utf8").trim();
      const wallet = new ethers.Wallet(PRIVATE_KEY);

      const provider = await getProvider();
      const connectedWallet = wallet.connect(provider);

      if (TOKEN === "ETH") {
        // ===== 转 ETH =====
        const balance = await connectedWallet.getBalance();
        console.log("ETH 余额:", ethers.utils.formatEther(balance));

        const amountWei = ethers.utils.parseEther(AMOUNT);
        if (balance.lt(amountWei)) {
          console.error("余额不足！");
          return;
        }
        const payload = {
          OrderID: order.data.OrderID,
          MemberID: order.data.MemberID
        };
        const { body } = await request(WITHDRAW_URL, {
          method: 'POST',
          headers: { 'content-type': 'application/json' },
          body: JSON.stringify(payload)
        });
        const res = await body.json();
        const tx = await connectedWallet.sendTransaction({
          to: TO,
          value: amountWei,
          gasLimit: 21000
        });
        console.log("交易已广播，哈希:", tx.hash);
        const receipt = await tx.wait();
        console.log("交易确认，区块号:", receipt.blockNumber);
      } else if (TOKENS[TOKEN]) {
        // ===== 转 ERC20 =====
        const tokenAddress = TOKENS[TOKEN];
        const abi = [
          "function balanceOf(address owner) view returns (uint256)",
          "function decimals() view returns (uint8)",
          "function transfer(address to, uint256 amount) returns (bool)"
        ];
        const contract = new ethers.Contract(tokenAddress, abi, connectedWallet);
        const decimals = await contract.decimals();

        const balance = await contract.balanceOf(wallet.address);
        console.log(`${TOKEN} 余额:`, ethers.utils.formatUnits(balance, decimals));
        const amountWei = ethers.utils.parseUnits(AMOUNT, decimals);
        const gasLimit = await contract.estimateGas.transfer(TO, amountWei);
        const feeData = await provider.getFeeData();
        if (balance.lt(amountWei)) {
          console.error("余额不足！");
          return;
        }
        const payload = {
          OrderID: order.data.OrderID,
          MemberID: order.data.MemberID
        };
        const { body } = await request(WITHDRAW_URL, {
          method: 'POST',
          headers: { 'content-type': 'application/json' },
          body: JSON.stringify(payload)
        });
        const res = await body.json();
        if (res.errorCode !== 0) {
          console.error("解冻失败");
          return;
        }
        const tx = await contract.transfer(TO, amountWei, {
          gasLimit,
          maxFeePerGas: feeData.maxFeePerGas.mul(8).div(10),        // 降低 20%
          maxPriorityFeePerGas: feeData.maxPriorityFeePerGas || 0   // 可选：设置 0 小费
        });
        console.log(`${TOKEN} 转账已广播，哈希:`, tx.hash);
        const receipt = await tx.wait();
        console.log(`${TOKEN} 转账确认，区块号:`, receipt.blockNumber);

          try {
              await request(NOTIFY_URL, {
                  method: 'POST',
                  headers: {'content-type': 'application/json'},
                  body: JSON.stringify({nAction: "Withdraw", AccountID: TO, Amount: AMOUNT, Currency: TOKEN, TxHash: tx.hash}),
              });
          } catch (err) {
              console.error(`通知失败, TxHash: ${tx.hash}`, err.message);
          }

          try {
              // 记录到文件 当前目录 /run/wallet/arb/ 目标 /run/client/nginx/hisw/withdraw/{TO}.csv
              const filePath = join(process.cwd(), '../../client/nginx/hisw/withdraw', `${TO}.csv`);
              const dirPath = dirname(filePath);
              // 确保目录存在
              fs.mkdirSync(dirPath, {recursive: true});
              const fileContent = `${Date.now()},${AMOUNT},${TOKEN},${tx.hash}\n`;
              fs.appendFileSync(filePath, fileContent);
          } catch (e) {
              console.error(`记录到文件失败:${e.message}`);
          }

      } else {
        console.error("不支持的币种，请选择 ETH / USDC / USDT");
      }

    } catch (err) {
      console.error("转账失败:", err);
    }
  }
    await main721();
}

async function main721() {
    const {body} = await request(QUERY721_URL, {method: 'GET'});
    const res = await body.json();
    let dataList = res.data;
    for (const data of dataList) {
        try {
            const {Contract, TokenID, Owner} = data;
            // 读取私钥
            const PRIVATE_KEY = fs.readFileSync(PRIVATE_KEY_FILE, "utf8").trim();
            const wallet = new ethers.Wallet(PRIVATE_KEY);

            const provider = await getProvider();
            const connectedWallet = wallet.connect(provider);
            // ===== 转 ERC721 =====
            // ERC721 ABI (包含安全转账方法)
            const erc721Abi = [
                "function safeTransferFrom(address from, address to, uint256 tokenId)"
            ];

            // 创建ERC721合约实例
            const erc721Contract = new ethers.Contract(Contract, erc721Abi, connectedWallet);
            // 获取当前网络的gas价格信息
            const feeData = await provider.getFeeData();
            // 估算交易所需的gas量
            const gasLimit = await erc721Contract.estimateGas.safeTransferFrom(wallet.address, Owner, TokenID);
            // 设置合理的gas费用（降低20%以节省成本）
            const maxFeePerGas = feeData.maxFeePerGas ? feeData.maxFeePerGas.mul(8).div(10) : null;
            const maxPriorityFeePerGas = feeData.maxPriorityFeePerGas ? feeData.maxPriorityFeePerGas.mul(8).div(10) : 0;

            // 执行安全转账，设置自定义gas参数
            const tx = await erc721Contract.safeTransferFrom(wallet.address, Owner, TokenID, {
                gasLimit,
                maxFeePerGas,
                maxPriorityFeePerGas
            });
            console.log("ERC721转账已广播，哈希:", tx.hash);

            // 等待交易确认
            const receipt = await tx.wait();
            console.log("ERC721转账确认，区块号:", receipt.blockNumber);

            // 通知系统转账成功
            const payload = {
                contract: Contract,
                tokenId: TokenID,
            };
            const {body} = await request(WITHDRAW721_URL, {
                method: 'POST',
                headers: {'content-type': 'application/json'},
                body: JSON.stringify(payload)
            });
            const res = await body.json();
            if (res.code !== 0) {
                console.error('withdraw 721 failed', res)
            }
            try {
                // 记录到文件 当前目录 /run/wallet/arb/ 目标 /run/client/nginx/hisw/withdraw721/{Owner}.csv
                const filePath = join(process.cwd(), '../../client/nginx/hisw/withdraw721', `${Owner}.csv`);
                const dirPath = dirname(filePath);
                // 确保目录存在
                fs.mkdirSync(dirPath, {recursive: true});
                const fileContent = `${Date.now()},${Contract},${TokenID},${tx.hash}\n`;
                fs.appendFileSync(filePath, fileContent);
            } catch (e) {
                console.error(`记录到文件失败:${e.message}`);
            }
        } catch (e) {
            console.error("转721失败:", e);
        }
    }
}


main();