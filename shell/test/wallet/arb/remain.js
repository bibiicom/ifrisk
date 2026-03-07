const { ethers } = require("ethers");

// Arbitrum 主网公共 RPC 列表（免 API Key）
const RPC_LIST = [
  "https://arb1.arbitrum.io/rpc",
  "https://rpc.ankr.com/arbitrum",
  "https://arb-mainnet.public.blastapi.io",
  "https://arbitrum.blockpi.network/v1/rpc/public",
  "https://arbitrum.publicnode.com"
];

// ERC20 最小 ABI
const ERC20_ABI = [
  "function balanceOf(address owner) view returns (uint256)",
  "function decimals() view returns (uint8)",
  "function symbol() view returns (string)"
];

// 常见 Token 地址（Arbitrum 主网）
const TOKENS = {
  USDC: "0xaf88d065e77c8cC2239327C5EDb3A432268e5831",
  USDT: "0xfd086bc7cd5c481dcc9c85ebe478a1c0b69fcbb9"
};

// 要查询的地址
const TARGET_ADDRESS = "0x491a533e93a1d19e186d37179b3d0a6a9ddcc819";

// 尝试连接 RPC，返回可用 provider
async function getProvider() {
  for (const url of RPC_LIST) {
    try {
      const provider = new ethers.providers.JsonRpcProvider(url, {
        name: "arbitrum",
        chainId: 42161
      });
      await provider.getBlockNumber(); // 测试节点是否可用
      console.log(`已连接 RPC: ${url}`);
      return provider;
    } catch (err) {
      console.warn(`节点不可用: ${url} - ${err.message}`);
    }
  }
  throw new Error("所有 Arbitrum 公共节点都不可用");
}

// 查询 ERC20 Token 余额
async function checkTokens(provider, address) {
  for (const [name, contractAddress] of Object.entries(TOKENS)) {
    try {
      const token = new ethers.Contract(contractAddress, ERC20_ABI, provider);
      const [bal, decimals, symbol] = await Promise.all([
        token.balanceOf(address),
        token.decimals(),
        token.symbol()
      ]);
      console.log(`${name} (${symbol}) 余额: ${ethers.utils.formatUnits(bal, decimals)}`);
    } catch (err) {
      console.warn(`查询 ${name} 失败: ${err.message}`);
    }
  }
}

async function main() {
  try {
    const provider = await getProvider();

    // 查询 ARB 余额
    const arbBalance = await provider.getBalance(TARGET_ADDRESS);
    console.log(`\nARB 余额: ${ethers.utils.formatEther(arbBalance)} ARB\n`);

    // 查询 Token 余额
    await checkTokens(provider, TARGET_ADDRESS);

  } catch (err) {
    console.error("查询失败:", err.message);
  }
}

main();
