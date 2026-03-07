import React, { useMemo } from 'react'
import {merge} from 'lodash'
import Echarts from './Echarts'

/**
 * 通用空心饼图（环形图）基础组件
 * props:
 * - data: ECharts pie series data 数组
 * - titleOption?: object ECharts title 配置（如居中显示文字）
 * - extraOption?: object 额外配置，会与基础配置 merge
 * 组件本身宽高为 100%，具体尺寸由外层容器控制
 */

const getBaseOption = (data) => {
  const safeData = Array.isArray(data) && data.length
    ? data
    : [
        { value: 0, name: '我的' },
        { value: 0, name: '其他' },
      ]

  return {
  backgroundColor: 'transparent',
  tooltip: {
    trigger: 'item',
    formatter: (params) => {
      if (!params) return ''
      const { name, value, percent } = params
      return `${name}: ${value} (${percent}%)`
    },
    textStyle: { color: '#fff', fontSize: 11 },
    backgroundColor: '#111',
    borderColor: '#333',
    borderWidth: 1,
  },
  legend: { show: false },
  series: [
    {
      type: 'pie',
      radius: ['55%', '85%'],
      avoidLabelOverlap: false,
      label: { show: false },
      labelLine: { show: false },
      data: safeData,
    },
  ],
  }
}

const HoldRatioDonut = ({ data = [], titleOption, extraOption, ...otherProps }) => {
  const option = useMemo(
    () => merge({}, getBaseOption(data), titleOption ? { title: titleOption } : {}, extraOption || {}),
    [data, titleOption, extraOption],
  )

  return <Echarts option={option} {...otherProps} />
}

export default HoldRatioDonut
