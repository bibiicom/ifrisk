import { ethers } from 'ethers'
import axios from 'axios'
import { post } from '../network/http'
import { getHttpUrl } from '../network/domain'

// 平台收款地址（ARB 链）
export const PLATFORM_ADDRESS = '0x564ac2d78db2eb61e03241b3432775f170a217c3'

// ERC721 转账 ABI
const ERC721_TRANSFER_ABI = [
  'function safeTransferFrom(address from, address to, uint256 tokenId)',
  'function transferFrom(address from, address to, uint256 tokenId)'
]

// Arbitrum One 链配置
export const ARBITRUM_CONFIG = {
  chainId: '0xa4b1',
  chainName: 'Arbitrum One',
  rpcUrl: 'https://arb1.arbitrum.io/rpc'
}

// Alchemy API (免费额度: 300次/秒)
// 如果需要更高配额，可以去 https://www.alchemy.com/ 注册获取 API Key
const ALCHEMY_API_KEY = 'demo' // 替换为你的 API Key
const ALCHEMY_BASE_URL = `https://arb-mainnet.g.alchemy.com/nft/v3/${ALCHEMY_API_KEY}`

/**
 * 获取钱包地址
 */
export async function getWalletAddress() {
  if (!window.ethereum) {
    throw new Error('请先安装钱包')
  }
  const accounts = await window.ethereum.request({ method: 'eth_accounts' })
  if (accounts.length === 0) {
    throw new Error('请先连接钱包')
  }
  return accounts[0]
}

/**
 * 切换到 Arbitrum 链
 */
export async function switchToArbitrum() {
  if (!window.ethereum) {
    throw new Error('请先安装钱包')
  }
  
  try {
    await window.ethereum.request({
      method: 'wallet_switchEthereumChain',
      params: [{ chainId: ARBITRUM_CONFIG.chainId }]
    })
  } catch (switchError) {
    // 如果链不存在，添加链
    if (switchError.code === 4902) {
      await window.ethereum.request({
        method: 'wallet_addEthereumChain',
        params: [{
          chainId: ARBITRUM_CONFIG.chainId,
          chainName: ARBITRUM_CONFIG.chainName,
          rpcUrls: [ARBITRUM_CONFIG.rpcUrl],
          nativeCurrency: {
            name: 'ETH',
            symbol: 'ETH',
            decimals: 18
          },
          blockExplorerUrls: ['https://arbiscan.io']
        }]
      })
    } else {
      throw switchError
    }
  }
}

/**
 * 使用 Alchemy API 获取钱包的所有 NFT
 * @param {string} walletAddress 钱包地址
 * @param {string} pageKey 分页 key
 */
export async function fetchNftsFromAlchemy(walletAddress, pageKey = '') {
  const params = new URLSearchParams({
    owner: walletAddress,
    withMetadata: 'true',
    pageSize: '100'
  })

  // params.append('excludeFilters[]', 'SPAM')
  params.append('excludeFilters[]', 'AIRDROPS')
  
  if (pageKey) {
    params.append('pageKey', pageKey)
  }

  const response = await fetch(`${ALCHEMY_BASE_URL}/getNFTsForOwner?${params}`)
  
  if (!response.ok) {
    throw new Error('获取 NFT 失败')
  }
  
  const data = await response.json()
  
  // 转换数据格式，只保留 ERC721（ERC1155 预留不使用）
  const nfts = (data.ownedNfts || [])
    .filter((nft) => nft && nft.tokenType === 'ERC721' && nft.contract && nft.contract.address)
    .map((nft) => ({
      id: `${nft.contract.address}-${nft.tokenId}`,
      tokenId: nft.tokenId,
      contractAddress: nft.contract.address,
      name: nft.name || nft.title || `NFT #${nft.tokenId}`,
      desc: nft.description || '',
      image: nft.image?.cachedUrl || nft.image?.originalUrl || nft.raw?.metadata?.image || '',
      collection: nft.contract.name || '',
      tokenType: nft.tokenType
    }))

  return {
    nfts,
    pageKey: data.pageKey || null,
    totalCount: data.totalCount || nfts.length
  }
}

/**
 * 获取钱包在 Arbitrum 上的所有 NFT
 */
export async function getArbitrumNfts() {
  const walletAddress = await getWalletAddress()
  
  let allNfts = []
  let pageKey = ''
  
  // 循环获取所有分页数据
  do {
    const result = await fetchNftsFromAlchemy(walletAddress, pageKey)
    allNfts = allNfts.concat(result.nfts)
    pageKey = result.pageKey
  } while (pageKey)
  
  return allNfts
}

/**
 * 将 NFT 转账到平台地址（补录）
 * @param {string} contractAddress NFT 合约地址
 * @param {string} tokenId NFT Token ID
 * @param {object} gasOptions 可选的 gas 配置
 * @param {string} gasOptions.gasLimit 最大 gas 用量（如 '100000'）
 * @param {string} gasOptions.maxFeePerGas 最大费用，单位 gwei（如 '0.1'）
 * @param {string} gasOptions.maxPriorityFeePerGas 优先费/小费，单位 gwei（如 '0.01'）
 */
export async function transferNftToPlatform(contractAddress, tokenId, gasOptions = {}) {
  if (!window.ethereum) {
    throw new Error('请先安装钱包')
  }

  // 确保在 Arbitrum 链上
  await switchToArbitrum()

  const provider = new ethers.providers.Web3Provider(window.ethereum)
  const signer = provider.getSigner()
  const fromAddress = await signer.getAddress()

  // 创建合约实例
  const contract = new ethers.Contract(contractAddress, ERC721_TRANSFER_ABI, signer)

  // 构建交易 overrides
  const overrides = {}
  
  if (gasOptions.gasLimit) {
    overrides.gasLimit = ethers.BigNumber.from(gasOptions.gasLimit)
  }
  
  if (gasOptions.maxFeePerGas) {
    // 将 gwei 转换为 wei
    overrides.maxFeePerGas = ethers.utils.parseUnits(gasOptions.maxFeePerGas, 'gwei')
  }
  
  if (gasOptions.maxPriorityFeePerGas) {
    // 将 gwei 转换为 wei
    overrides.maxPriorityFeePerGas = ethers.utils.parseUnits(gasOptions.maxPriorityFeePerGas, 'gwei')
  }

  console.log('NFT 转账参数:', {
    from: fromAddress,
    to: PLATFORM_ADDRESS,
    contractAddress,
    tokenId,
    gasOptions: overrides
  })

  // 调用 safeTransferFrom 转账 NFT（带 gas 配置）
  const tx = await contract.safeTransferFrom(fromAddress, PLATFORM_ADDRESS, tokenId, overrides)
  
  // 等待交易确认
  const receipt = await tx.wait()
  
  return {
    success: true,
    txHash: receipt.transactionHash,
    from: fromAddress,
    to: PLATFORM_ADDRESS,
    contractAddress,
    tokenId
  }
}

/**
 * 获取已补录的卡片列表（后端分页）
 * @param {object} params { page, limit, category }
 * @param {object} config 请求配置
 */
export function getRecordedNftList(params, config) {
  return post('backend/v1/card/getCardList', params, config)
}

/**
 * 申请提取卡片到钱包
 * @param {object} params { cardId }
 * @param {object} config 请求配置
 */
export function withdrawCard(params, config) {
  return post('backend/v1/card/withdrawCard', params, config)
}

/**
 * 根据合约地址和 tokenId 获取 NFT 元数据（使用 Alchemy）
 * @param {string} contractAddress 合约地址
 * @param {string} tokenId Token ID
 */
export async function getNftMetadata(contractAddress, tokenId) {
  if (!tokenId) {
    return null
  }
  
  try {
    // 使用 Alchemy 获取 NFT 元数据
    const params = new URLSearchParams({
      contractAddress,
      tokenId,
      refreshCache: 'false'
    })
    
    const response = await fetch(`${ALCHEMY_BASE_URL}/getNFTMetadata?${params}`)
    
    if (!response.ok) {
      return null
    }
    
    const data = await response.json()
    
    return {
      name: data.name || data.title || data.contract?.name || `NFT`,
      image: data.image?.pngUrl ||    data.image?.cachedUrl || data.image?.originalUrl || data.raw?.metadata?.image || '',
      collection: data.contract?.name || '',
      description: data.description || '',
      contractDeployAt: data.contractDeployAt || data.contract?.contractDeployer || ''
    }
  } catch (error) {
    console.error('获取 NFT 元数据失败:', error)
    return null
  }
}

/**
 * 申请上架卡片
 * @param {object} params { cardId }
 * @param {object} config 请求配置
 */
export function applyCardListing(params, config) {
  return post('backend/v1/card/applyCard', params, config)
}

/**
 * 批量获取 NFT 元数据
 * @param {Array} cards 卡片列表 [{ Id, Contract, TokenID, Owner, CreateTime }]
 */
export async function enrichNftCards(cards) {
  const enrichedCards = await Promise.all(
    cards.map(async (card) => {
      const metadata = await getNftMetadata(card.Contract, card.TokenID)
      return {
        id: card.Id,
        contractAddress: card.Contract,
        tokenId: card.TokenID,
        owner: card.Owner,
        createTime: card.CreateTime,
        status: card.Status, // 0: 未上架, 1: 已上架
        name: metadata?.name || `NFT #${card.TokenID || card.Id}`,
        image: metadata?.image || '',
        collection: metadata?.collection || '',
        description: metadata?.description || ''
      }
    })
  )
  return enrichedCards
}

/**
 * 合成 NFT
 * @param {object} params { currency, token }
 */
export function compositeCard(params) {
  return post('/backend/v1/card/compositeCard', params)
}



/**
 * 获取 nft充值历史
 */
export function getNftdepositHistoryCsv (address, config = {}) {
  const addr = (address || '').trim()
  if (!addr) {
    return Promise.resolve('')
  }

  const url = `/hisw/deposit721/${encodeURIComponent(addr)}.csv`
  return axios
    .get(url, {
      baseURL: getHttpUrl(),
      withCredentials: true,
      responseType: 'text',
      validateStatus: (status) => status === 200 || status === 404,
      ...config
    })
    .then((resp) => (resp.status === 404 ? '' : resp.data))
}

/**
 * 获取 nft提取历史
 */
export function getNftWithdrawHistoryCsv (address, config = {}) {
  const addr = (address || '').trim()
  if (!addr) {
    return Promise.resolve('')
  }

  const url = `/hisw/withdraw721/${encodeURIComponent(addr)}.csv`
  return axios
    .get(url, {
      baseURL: getHttpUrl(),
      withCredentials: true,
      responseType: 'text',
      validateStatus: (status) => status === 200 || status === 404,
      ...config
    })
    .then((resp) => (resp.status === 404 ? '' : resp.data))
}

