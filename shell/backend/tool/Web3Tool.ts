import {ethers} from 'ethers';

// ERC721合约ABI，包含mint方法
const ERC721_ABI = [
    "function mint(address to, uint256 tokenId) external",
    "function mint(address to, uint256 tokenId, bytes calldata data) external",
    "function safeMint(address to) external returns (uint256)",
    "function safeMint(address to, bytes calldata data) external returns (uint256)",
    "function safeMint(address to, uint256 tokenId) external",
    "function safeMint(address to, uint256 tokenId, bytes calldata data) external",
    "function safeMint(address to, string memory uri) external returns (uint256)",
    "function safeMint(address to, uint256 tokenId, string memory uri) external"
];

export class Web3Tool {
    
    /**
     * 在Arbitrum网络上铸造NFT
     * @param contractAddress NFT合约地址
     * @param to 接收NFT的地址
     * @param tokenId NFT的tokenId（可选，如果合约支持自动生成则不需要）
     * @param uri NFT元数据URI（可选，用于指定图片等元数据）
     * @param privateKey 用于签名交易的私钥
     * @returns 交易哈希
     */
    static async mintNFT(
        contractAddress: string,
        to: string,
        tokenId?: number,
        uri?: string,
        privateKey?: string
    ): Promise<string> {
        try {
            // Arbitrum网络配置 - 直接写在方法中
            const arbitrumRpcUrl = "https://arb1.arbitrum.io/rpc";
            
            // 如果没有提供私钥，使用默认私钥（实际生产环境中应该从安全的地方获取）
            const defaultPrivateKey = "0x..."; // 实际使用时需要替换为真实私钥
            const signerPrivateKey = privateKey || defaultPrivateKey;
            
            // 创建Arbitrum网络提供者
            const provider = new ethers.JsonRpcProvider(arbitrumRpcUrl);
            
            // 创建签名者
            const signer = new ethers.Wallet(signerPrivateKey, provider);
            
            // 创建合约实例
            const contract = new ethers.Contract(contractAddress, ERC721_ABI, signer);
            
            let tx: { wait: () => any; hash: string | PromiseLike<string>; };
            
            // 根据合约支持的mint方法类型选择合适的调用方式
            if (uri) {
                // 如果提供了元数据URI，使用支持URI的mint方法
                if (tokenId !== undefined) {
                    // 提供了tokenId和uri
                    tx = await contract.safeMint(to, tokenId, uri);
                } else {
                    // 只提供了uri，tokenId由合约自动生成
                    tx = await contract.safeMint(to, uri);
                }
            } else {
                // 没有提供元数据URI，使用普通mint方法
                if (tokenId !== undefined) {
                    // 提供了tokenId
                    try {
                        tx = await contract.safeMint(to, tokenId);
                    } catch (error) {
                        tx = await contract.mint(to, tokenId);
                    }
                } else {
                    // tokenId由合约自动生成
                    tx = await contract.safeMint(to);
                }
            }
            
            // 等待交易确认
            await tx.wait();
            
            // 返回交易哈希
            return tx.hash;
        } catch (error) {
            console.error("NFT铸造失败:", error);
            throw error;
        }
    }

}