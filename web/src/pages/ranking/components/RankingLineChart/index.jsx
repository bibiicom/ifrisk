import React, { useState, useEffect } from 'react'
import { getLastKLine } from '@/api/contract'
import MiniLineChart from '@/component/echarts/MiniLineChart'

const RankingLineChart = ({
    symbol,
    change24h,
    colorTheme,
    width = 100,
    height = 40
}) => {
    const [data, setData] = useState([])

    useEffect(() => {
        const fetchData = async () => {
            if (!symbol) return

            try {
                // getLastKLine(symbolId, exchangeId, interval, count, config)
                const response = await getLastKLine(symbol, 'SCARD', '1h', 24)

                const responseData = response
                let rawData = []
                
                if (Array.isArray(responseData)) {
                     rawData = responseData
                } else if (responseData && Array.isArray(responseData.result)) {
                     rawData = responseData.result
                } else if (responseData && Array.isArray(responseData.data)) {
                     rawData = responseData.data
                }

                // Process data for chart (expecting array of numbers)
                if (rawData.length > 0) {
                    // Check if data is array of arrays (like [timestamp, open, high, low, close, volume])
                    const firstItem = rawData[0]
                    if (Array.isArray(firstItem)) {
                        // 按时间戳排序(升序),确保最新的时间在最后
                        const sortedData = [...rawData].sort((a, b) => a[0] - b[0])
                        // Based on screenshot, index 4 is the closing price
                        // Format seems to be: [timestamp, open, high, low, close, volume, ...]
                        const chartData = sortedData.map(item => parseFloat(item[4]))
                        setData(chartData)
                    } else if (typeof firstItem === 'object') {      
                        // Extract Close price (common fields: C, Close, c, close)
                        const chartData = rawData.map(item => {
                            return parseFloat(item.C || item.Close || item.c || item.close || item.price || item.p || 0)
                        })
                        setData(chartData)
                    } else {
                        setData(rawData)
                    }
                } else {
                    setData([])
                }

            } catch (error) {
                console.error(`Error fetching data for ${symbol}:`, error)
            }
        }

        fetchData()
    }, [symbol])

    // 如果没有数据,显示占位符
    if (!data || data.length === 0) {
        return (
            <div style={{ width, height, display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
                <span style={{ fontSize: '12px', color: 'var(--text-secondary)' }}>-</span>
            </div>
        )
    }

    return (
        <MiniLineChart
            data={data}
            change={change24h}
            width={width}
            height={height}
            colorTheme={colorTheme}
        />
    )
}

export default RankingLineChart
