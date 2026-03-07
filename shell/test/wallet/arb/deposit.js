const fs = require("fs");
const {ethers} = require("ethers");
const {request} = require('undici');
const {join, dirname} = require("node:path");

const ACTION_URL = "http://127.0.0.1/admin/v1.0/SendAccountAction";
const ACTION_721_URL = "http://127.0.0.1:8080/backend/v1/common/addCard";
const NOTIFY_URL = "http://127.0.0.1:8080/backend/v1/common/notify";

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

const ERC721_TRANSFER_TOPIC = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";

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
            const provider = new ethers.providers.JsonRpcProvider(url, {name: "arbitrum", chainId: 42161});
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
        let retryCount = 0;
        const maxRetries = 3;
        let success = false;

        while (!success && retryCount < maxRetries) {
            try {
                const filter = contract.filters.Transfer(null, address);
                const logs = await contract.queryFilter(filter, start, toBlock);
                let tmpTransfers = [];
                for (let i = logs.length - 1; i >= 0; i--) {
                    const log = logs[i];
                    if (log.transactionHash === lastHash) {
                        stop = true;
                        break;
                    }
                    const block = await provider.getBlock(log.blockNumber);
                    tmpTransfers.push({
                        token: symbol,
                        hash: log.transactionHash,
                        from: log.args.from,
                        to: log.args.to,
                        value: ethers.utils.formatUnits(log.args.value, decimals),
                        block: log.blockNumber,
                        time: new Date(block.timestamp * 1000).toLocaleString()
                    });
                }
                transfers.push(...tmpTransfers);
                success = true;
            } catch (err) {
                retryCount++;
                if (retryCount > maxRetries) {
                    console.error(`queryFilter 达到最大重试次数，跳过此区块范围`, err.message);
                    break;
                }
                console.warn(`queryFilter 出错，重试中... (${retryCount}/${maxRetries})`, err.message);
                const delay = Math.pow(2, retryCount) * 1000; // 指数退避策略
                await new Promise(r => setTimeout(r, delay));
            }
        }
        toBlock = start - 1;
    }

    return transfers;
}

async function scan721Incremental(provider, address, lastHash, step = STEP_BLOCKS, maxBlocks = INITIAL_SCAN_BLOCKS) {
    const currentBlock = await provider.getBlockNumber();
    let fromBlock = Math.max(0, currentBlock - maxBlocks);
    let toBlock = currentBlock;
    let transfers = [];
    let stop = false;

    while (!stop && toBlock >= fromBlock) {
        const start = Math.max(fromBlock, toBlock - step + 1);
        let retryCount = 0;
        const maxRetries = 3;
        let success = false;

        while (!success && retryCount < maxRetries) {
            try {
                const logs = await provider.getLogs({
                    fromBlock: start,
                    toBlock: toBlock,
                    topics: [
                        ERC721_TRANSFER_TOPIC,
                        null,
                        ethers.utils.hexZeroPad(address, 32), // 查转入记录
                    ],
                });
                let tmpTransfers = [];
                for (let i = logs.length - 1; i >= 0; i--) {
                    const log = logs[i];
                    if (log.transactionHash === lastHash) {
                        stop = true;
                        break;
                    }
                    if (log.topics.length !== 4) continue;
                    const block = await provider.getBlock(log.blockNumber);
                    tmpTransfers.push({
                        hash: log.transactionHash,
                        block: log.blockNumber,
                        contract: log.address,
                        from: ethers.utils.hexStripZeros(log.topics[1]),
                        to: ethers.utils.hexStripZeros(log.topics[2]),
                        tokenId: ethers.BigNumber.from(log.topics[3]).toString(),
                        timestamp: block.timestamp * 1000
                    });
                }
                transfers.push(...tmpTransfers);
                success = true;
            } catch (err) {
                retryCount++;
                if (retryCount > maxRetries) {
                    console.error(`721 转账记录达到最大重试次数，跳过此区块范围`, err.message);
                    break;
                }
                console.warn(`721 转账记录出错，重试中... (${retryCount}/${maxRetries})`, err.message);
                const delay = Math.pow(2, retryCount) * 1000; // 指数退避策略
                await new Promise(r => setTimeout(r, delay));
            }
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
            LocalID: transfer.hash.slice(-20)
        };
        console.log('payload', payload);
        try {
            await request(NOTIFY_URL, {
                method: 'POST',
                headers: {'content-type': 'application/json'},
                body: JSON.stringify({nAction: "Deposit", TxHash: transfer.hash, ...payload}),
            });
        } catch (err) {
            console.error(`通知失败, TxHash: ${transfer.hash}`, err.message);
        }
        const {body} = await request(ACTION_URL, {
            method: 'POST',
            headers: {'content-type': 'application/json'},
            body: JSON.stringify(payload)
        });
        const res = await body.json();
        if (res.errorCode !== 0) {
            console.error(`接口调用失败:${res.errorCode}`, res.errorMsg);
            return;
        }
        console.log(`已发送 ${transfer.token} 转账到接口, TxHash: ${transfer.hash}`, res.result[0].data.Balance);
        try {
            // 记录到文件 当前目录 /run/wallet/arb/ 目标 /run/client/nginx/hisw/deposit/{payload.AccountID}.csv
            const filePath = join(process.cwd(), '../../client/nginx/hisw/deposit', `${payload.AccountID}.csv`);
            const dirPath = dirname(filePath);
            // 确保目录存在
            fs.mkdirSync(dirPath, {recursive: true});
            const fileContent = `${Date.now()},${payload.Amount},${payload.Currency},${transfer.hash}\n`;
            fs.appendFileSync(filePath, fileContent);
        } catch (e) {
            console.error(`记录到文件失败:${e.message}`);
        }
    } catch (err) {
        console.error(`接口调用失败, TxHash: ${transfer.hash}`, err.message);
    }
}

async function call721Api(transfer) {
    try {
        const payload = transfer;
        console.log('payload', payload);
        const {body} = await request(ACTION_721_URL, {
            method: 'POST',
            headers: {'content-type': 'application/json'},
            body: JSON.stringify(payload)
        });
        const res = await body.json();
        if (res.code !== 0) {
            console.error(`接口调用失败:${res.code}`, res.msg);
            return;
        }
        console.log(`已发送 ${transfer.contract} ${transfer.tokenId}  转账到接口, TxHash: ${transfer.hash}`);
        try {
            // 记录到文件 当前目录 /run/wallet/arb/ 目标 /run/client/nginx/hisw/deposit721/{transfer.from}.csv
            const filePath = join(process.cwd(), '../../client/nginx/hisw/deposit721', `${transfer.from}.csv`);
            const dirPath = dirname(filePath);
            // 确保目录存在
            fs.mkdirSync(dirPath, {recursive: true});
            const fileContent = `${Date.now()},${transfer.contract},${transfer.tokenId},${transfer.hash}\n`;
            fs.appendFileSync(filePath, fileContent);
        } catch (e) {
            console.error(`记录到文件失败:${e.message}`);
        }
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
    // 扫描 ERC721 新增转入记录
    let all721Transfers = [];
    console.log(`扫描 ERC721 新增转入记录...`);
    const nftTransfers = await scan721Incremental(provider, address, lastTx.ERC721 || null);
    if (nftTransfers.length > 0) {
        lastTx.ERC721 = nftTransfers[0].hash;
        all721Transfers = all721Transfers.concat(nftTransfers);
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

    if (all721Transfers.length > 0) {
        all721Transfers.sort((a, b) => a.block - b.block);
        // 统一调用接口
        for (const t of all721Transfers) {
            await call721Api(t);
        }
    } else {
        console.log("没有新增721转入记录");
    }
}

main();