import React, { useMemo } from 'react'
import merge from 'lodash.merge'
import Echarts from './Echarts'

/**
 * 基础折线图基础组件
 *
 * props:
 * - xData: string[] 横轴标签
 * - yData: number[] 纵轴数据
 * - extraOption?: object 额外配置，会与基础配置 merge
 *
 * 组件本身宽高为 100%，具体高度由外层容器控制
 */

const getBaseOption = (xData, yData) => ({
  backgroundColor: 'transparent',
  grid: {
    left: 40,
    right: 20,
    top: 20,
    bottom: 30,
  },
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'line',
      lineStyle: {
        color: '#666',
        type: 'dashed',
      },
    },
    textStyle: {
      color: '#fff',
      fontSize: 11,
    },
    backgroundColor: '#111',
    borderColor: '#333',
    borderWidth: 1,
  },
  xAxis: {
    type: 'category',
    data: xData,
    boundaryGap: false,
    axisLine: {
      lineStyle: {
        color: '#555',
      },
    },
    axisLabel: {
      color: '#aaa',
      fontSize: 10,
    },
    axisTick: {
      show: false,
    },
  },
  yAxis: {
    type: 'value',
    axisLine: { show: false },
    axisTick: { show: false },
    splitLine: {
      show: true,
      lineStyle: {
        color: '#333',
        type: 'dashed',
      },
    },
    axisLabel: {
      color: '#aaa',
      fontSize: 10,
    },
  },
  series: [
    {
      type: 'line',
      data: yData,
      smooth: false,
      symbol: 'circle',
      symbolSize: 6,
      showSymbol: true,
      lineStyle: {
        width: 3,
        color: '#1677ff',
      },
      itemStyle: {
        color: '#000000',
        borderColor: '#1677ff',
        borderWidth: 3,
      },
      areaStyle: {
        color: 'transparent',
      },
    },
  ],
})

// data: { x: string | number, y: number }[]
const BasicLineChart = ({ data = [], extraOption, ...otherProps }) => {
  const option = useMemo(() => {
    const xData = (data || []).map((item) => item.x)
    const yData = (data || []).map((item) => item.y)
    return merge({}, getBaseOption(xData, yData), extraOption || {})
  }, [data, extraOption])

  return <Echarts option={option} {...otherProps} />
}

export default BasicLineChart
