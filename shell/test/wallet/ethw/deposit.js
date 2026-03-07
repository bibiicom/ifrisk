const fs = require("fs");
const { ethers } = require("ethers");
const { request } = require('undici');

const ACTION_URL = "http://18.141.186.102:9527/admin/v1.0/SendAccountAction";

// 多个公共 RPC
const RPC_LIST = [
  "https://arb1.arbitrum.io/rpc",
  "https://arbitrum.blockpi.network/v1/rpc/public",
  "https://arbitrum.publicnode.com",
  "https://arb-mainnet.public.blastapi.io"
];

// ERC20 ABI
const ERC20_ABI = [
  "event Transfer(address indexed from, address indexed to, uint256 value)",
  "function decimals() view returns (uint8)",
  "function symbol() view returns (string)"
];

// 支持的 Token
const TOKENS = {
  USDC: "0xaf88d065e77c8cC2239327C5EDb3A432268e5831",
  USDT: "0xfd086bc7cd5c481dcc9c85ebe478a1c0b69fcbb9"
};

const LAST_TX_FILE = "./lastTx.json";
const INITIAL_SCAN_BLOCKS = 10000; // 第一次运行扫描最近多少块
const STEP_BLOCKS = 5000; // 每次扫描的区块步长

// 读取钱包地址
function getAddressFromPrivateKey(filePath) {
  const privateKey = fs.readFileSync(filePath, "utf8").trim();
  return new ethers.Wallet(privateKey).address;
}

// 获取可用 RPC
async function getProvider() {
  for (const url of RPC_LIST) {
    try {
      const provider = new ethers.providers.JsonRpcProvider(url, { name: "arbitrum", chainId: 42161 });
      await provider.getBlockNumber();
      console.log(`已连接 RPC: ${url}`);
      return provider;
    } catch (err) {
      console.warn(`节点不可用: ${url} - ${err.message}`);
    }
  }
  throw new Error("所有 RPC 节点不可用");
}

// 读取本地 lastTx
function readLastTx() {
  if (!fs.existsSync(LAST_TX_FILE)) return {};
  return JSON.parse(fs.readFileSync(LAST_TX_FILE, "utf8"));
}

// 保存 lastTx
function saveLastTx(lastTx) {
  fs.writeFileSync(LAST_TX_FILE, JSON.stringify(lastTx, null, 2));
}

// 分段增量扫描 ERC20 Token
async function scanTokenIncremental(provider, address, symbol, tokenAddr, lastHash, step = STEP_BLOCKS, maxBlocks = INITIAL_SCAN_BLOCKS) {
  const contract = new ethers.Contract(tokenAddr, ERC20_ABI, provider);
  const [decimals] = await Promise.all([contract.decimals()]);
  const currentBlock = await provider.getBlockNumber();

  let fromBlock = Math.max(0, currentBlock - maxBlocks);
  let toBlock = currentBlock;
  let transfers = [];
  let stop = false;

  while (!stop && toBlock >= fromBlock) {
    const start = Math.max(fromBlock, toBlock - step + 1);
    try {
      const filter = contract.filters.Transfer(null, address);
      const logs = await contract.queryFilter(filter, start, toBlock);

      for (let i = logs.length - 1; i >= 0; i--) {
        const log = logs[i];
        if (log.transactionHash === lastHash) {
          stop = true;
          break;
        }
        const block = await provider.getBlock(log.blockNumber);
        transfers.push({
          token: symbol,
          hash: log.transactionHash,
          from: log.args.from,
          to: log.args.to,
          value: ethers.utils.formatUnits(log.args.value, decimals),
          block: log.blockNumber,
          time: new Date(block.timestamp * 1000).toLocaleString()
        });
      }
    } catch (err) {
      console.warn(`queryFilter 出错，重试中...`, err.message);
      await new Promise(r => setTimeout(r, 500)); // 半秒延迟再试
      continue;
    }
    toBlock = start - 1;
  }

  return transfers;
}

async function callApi(transfer) {
  try {
    const payload = {
      MemberID: transfer.from.toLowerCase(),     // 使用转入地址
      AccountID: transfer.from.toLowerCase(),    // 使用转入地址
      SettlementGroup: "SG1",
      Amount: Number(transfer.value),
      Currency: transfer.token,
      ActionType: "1",
      LocalID: transfer.block
    };
    console.log('payload', payload);
    const { body } = await request(ACTION_URL, {
      method: 'POST',
      headers: { 'content-type': 'application/json' },
      body: JSON.stringify(payload)
    });
    const res = await body.json();
    if (res.errorCode !== 0) {
      console.error(`接口调用失败:${res.errorCode}`, res.errorMsg);
      return;
    }
    console.log(`已发送 ${transfer.token} 转账到接口, TxHash: ${transfer.hash}`, res.result[0].data.Balance);
  } catch (err) {
    console.error(`接口调用失败, TxHash: ${transfer.hash}`, err.message);
  }
}

// 主程序
async function main() {
  const address = getAddressFromPrivateKey("../privateKey.txt");
  console.log(`查询地址: ${address}\n`);

  const provider = await getProvider();
  const lastTx = readLastTx();
  let allTransfers = [];

  // 扫描每个 ERC20 Token
  for (const [symbol, tokenAddr] of Object.entries(TOKENS)) {
    console.log(`扫描 ${symbol} 新增转入记录...`);
    const tokenTransfers = await scanTokenIncremental(provider, address, symbol, tokenAddr, lastTx[symbol] || null);
    if (tokenTransfers.length > 0) {
      lastTx[symbol] = tokenTransfers[0].hash;
      allTransfers = allTransfers.concat(tokenTransfers);
    }
  }

  saveLastTx(lastTx);

  if (allTransfers.length > 0) {
    allTransfers.sort((a, b) => a.block - b.block);
      // 统一调用接口
    for (const t of allTransfers) {
  	await callApi(t);
    }
  } else {
    console.log("没有新增转入记录");
  }
}

main();
