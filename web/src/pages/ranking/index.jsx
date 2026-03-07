import React, { useState, useEffect } from 'react'
import { Spin } from 'antd'
import { getMarkets } from '@/api/symbol'
import { getHoldRank } from '@/api/ranking'
import { ProductGroup } from '@/constants/symbol'
import styles from './index.module.less'
import StatsCards from './components/StatsCards'
import RankingTable from './components/RankingTable'
import authHOC from '../common/hoc/authHOC'

const Ranking = () => {
    const [rankingData, setRankingData] = useState([])
    const [holdData, setHoldData] = useState([])
    const [loading, setLoading] = useState(false)
    const [activeTab, setActiveTab] = useState('hold') // hot, hold, gain, loss, turnover

    // 请求真实数据
    useEffect(() => {
        const fetchData = async () => {
            setLoading(true)
            try {
                const markets = await getMarkets(ProductGroup.SPOT)

                // 处理市场数据
                const dataArray = Object.entries(markets).map(([instrumentId, market]) => ({
                    symbol:  instrumentId,
                    instrumentId: instrumentId,
                    price: market.N || 0,
                    change24h: (market.Change || 0) * 100,
                    high24h: market.H2 || market.H || 0,
                    low24h: market.L2 || market.L || 0,
                    volume24h: market.V2 || market.V || 0, // 当日成交量
                    turnover24h: market.T2 || market.T || 0, // 当日成交额
                    chartData: Array.from({ length: 24 }, () => Math.random() * 100), // 暂时使用模拟数据
                    RE: market.RE || '' // 添加 RE 字段用于 tooltip
                }))

                // 默认按成交额排序 (for stats calculation)
                const sortedData = [...dataArray].sort((a, b) => b.turnover24h - a.turnover24h)
                
                // 添加排名
                const rankedData = sortedData.map((item, index) => ({
                    ...item,
                    rank: index + 1
                }))

                setRankingData(rankedData)
            } catch (error) {
                console.error('获取排行榜数据失败:', error)
            } finally {
                setLoading(false)
            }
        }

        fetchData()
    }, [activeTab])

    // 点击持有榜时请求数据
    useEffect(() => {
        const fetchHoldData = async () => {
            if (activeTab === 'hold') {
                setLoading(true)
                try {
                    const holdResponse = await getHoldRank()
                    console.log(holdResponse)
                    if (holdResponse.data && holdResponse.code === 0) {
                        const holdRankData = holdResponse.data.rank
                        
                        // 获取市场数据以获取最新价
                        const markets = await getMarkets(ProductGroup.SPOT)
                        
                        // 合并最新价和涨跌幅到持有榜数据
                        const mergedData = holdRankData.map(item => {
                            // 使用 instrumentID 字段匹配市场数据
                            const market = markets[item.instrumentID] || {}
                            
                            return {
                                ...item,
                                symbol: item.instrumentID,  // 添加 symbol 字段供图表使用
                                price: market.N || 0,  // N 字段是最新价
                                change24h: (market.Change || 0) * 100,  // Change 字段是24小时涨跌幅
                                RE: market.RE || '' // 添加 RE 字段用于 tooltip
                            }
                        })
                        
                        setHoldData(mergedData)
                    }
                } catch (error) {
                    console.error('获取持有榜数据失败:', error)
                } finally {
                    setLoading(false)
                }
            }
        }

        fetchHoldData()
    }, [activeTab])

    const getDisplayData = () => {
        if (activeTab === 'hold') {
            return holdData
        }

        let sortedData = [...rankingData]
        
        switch (activeTab) {
            case 'hot':
                // 热门榜按成交量排序
                sortedData.sort((a, b) => b.volume24h - a.volume24h)
                break
            case 'gain':
                // 涨幅榜
                sortedData.sort((a, b) => b.change24h - a.change24h)
                break
            case 'loss':
                // 跌幅榜
                sortedData.sort((a, b) => a.change24h - b.change24h)
                break
            case 'turnover':
                // 成交额榜
                sortedData.sort((a, b) => b.turnover24h - a.turnover24h)
                break
            default:
                break
        }

        // 重新计算排名
        return sortedData.map((item, index) => ({
            ...item,
            rank: index + 1
        }))
    }

    return (
        <div className={styles.container}>
            {/* 统计卡片 */}
            <StatsCards 
                marketData={rankingData}
                holdData={holdData}
                activeTab={activeTab}
                onTabChange={setActiveTab}
            />

            {/* 排名表格 */}
            {loading ? (
                <div className={styles.loading}>
                    <Spin size="large" />
                </div>
            ) : (
                <RankingTable 
                    data={getDisplayData()} 
                    type={activeTab}
                />
            )}
        </div>
    )
}

export default authHOC(true)(Ranking)  
