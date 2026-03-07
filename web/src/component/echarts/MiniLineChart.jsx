import React, { useMemo } from 'react'
import Echarts from './Echarts'
import { getChangeColor } from '@/utils/format'
import { ColorTheme } from '@/constants/theme'

/**
 * 迷你折线图组件 - 用于排行榜等场景
 * 
 * @param {Array} data - 图表数据数组
 * @param {number} change - 涨跌幅,用于确定颜色
 * @param {number} width - 图表宽度
 * @param {number} height - 图表高度
 * @param {string} colorTheme - 颜色主题配置,可选 ('buy-rise-green' | 'buy-rise-red'),默认 'buy-rise-green'
 */
const MiniLineChart = ({
    data = [],
    change = 0,
    width = 100,
    height = 40,
    colorTheme = ColorTheme.BUY_RISE_GREEN
}) => {
    const option = useMemo(() => {
        const color = getChangeColor(change, colorTheme)

        return {
            backgroundColor: 'transparent',
            grid: {
                left: 0,
                right: 0,
                top: 0,
                bottom: 0,
            },
            tooltip: {
                trigger: 'axis',
                axisPointer: {
                    type: 'line',
                    lineStyle: {
                        color: color,
                        width: 1,
                        type: 'solid',
                    },
                },
                backgroundColor: 'rgba(0, 0, 0, 0.8)',
                borderColor: color,
                borderWidth: 1,
                textStyle: {
                    color: '#fff',
                    fontSize: 11,
                },
                padding: [4, 8],
                formatter: (params) => {
                    if (params && params[0]) {
                        return `${params[0].value.toFixed(2)}`
                    }
                    return ''
                },
            },
            xAxis: {
                type: 'category',
                show: false,
                boundaryGap: false,
            },
            yAxis: {
                type: 'value',
                show: false,
                scale: true,
            },
            series: [
                {
                    type: 'line',
                    data: data,
                    smooth: false,
                    symbol: 'none',
                    lineStyle: {
                        width: 1.5,
                        color: color,
                    },
                    areaStyle: {
                        color: 'transparent',
                    },
                },
            ],
        }
    }, [data, change, colorTheme])

    return (
        <Echarts
            option={option}
            width={width}
            height={height}
            renderer="svg"
            useResizeHandler={false}
        />
    )
}

export default MiniLineChart
