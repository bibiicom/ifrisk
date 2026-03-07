import React from 'react';
import { merge, get } from 'lodash';
import Echarts from './Echarts';

const hexToRgba = (hex, alpha = 1) => {
  const r = parseInt(hex.slice(1, 3), 16);
  const g = parseInt(hex.slice(3, 5), 16);
  const b = parseInt(hex.slice(5, 7), 16);
  return `rgba(${r}, ${g}, ${b}, ${alpha})`;
};

const echartsLinear = (color) => {
  return {
    type: 'linear',
    x: 0,
    y: 0,
    x2: 0,
    y2: 1,
    colorStops: [
      { offset: 0, color: hexToRgba(color, 0.3) },
      { offset: 1, color: hexToRgba(color, 0.05) }
    ]
  };
};

const defaultColors = ['#a855f7', '#ec4899', '#f59e0b', '#10b981', '#3b82f6'];

const LineChart = (props) => {
  const {
    data = {},
    color = defaultColors,
    unit = [''],
    symbol = 'circle',
    legendShow = null,
    smooth = false,
    markPoint = false,
    showArea = false,
    option,
    ...other
  } = props;

  const formatSeries = (seriesData) => {
    const arr = [];
    seriesData?.forEach((item, index) => {
      const { lineStyle = {}, name, ...other } = item;
      const series = {
        name,
        type: 'line',
        symbol: symbol,
        smooth,
        symbolSize: 6,
        sampling: 'lttb',
        lineStyle: {
          color: color[index],
          width: 2,
          ...lineStyle,
        },
        areaStyle: {
          color: showArea ? echartsLinear(color[index]) : 'transparent',
        },
        itemStyle: {
          color: color[index],
          borderWidth: 4,
          borderColor: hexToRgba(color[index], 0.4),
        },
        ...other,
      };
      if (markPoint) {
        series.markPoint = markPoint;
      }
      arr.push(series);
    });
    if (!arr[0]) return undefined;
    return arr;
  };

  const yAxis = get(data, 'yAxis', []);

  const options = merge(
    {
      color,
      tooltip: {
        trigger: 'axis',
        backgroundColor: '#111',
        borderColor: '#333',
        borderWidth: 1,
        textStyle: {
          color: '#fff',
          fontSize: 12,
        },
      },
      legend: {
        show: legendShow ?? (yAxis?.length > 1 ? true : false),
        top: 0,
        left: 'center',
        itemHeight: 8,
        width: '80%',
        type: 'scroll',
        textStyle: {
          color: '#888',
        },
      },
      grid: {
        top: 30,
        left: 50,
        right: 20,
        bottom: 30,
        containLabel: true,
      },
      dataZoom: {
        type: 'inside',
        minValueSpan: 2,
      },
      xAxis: {
        type: 'category',
        data: get(data, 'xAxis', []),
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
      yAxis: get(data, 'unit', unit)?.map((unit, i) => {
        return {
          type: 'value',
          name: unit,
          scale: true,
          splitLine: {
            show: i > 0 ? false : true,
            lineStyle: {
              color: '#333',
              type: 'dashed',
            },
          },
          axisLine: { show: false },
          axisTick: { show: false },
          axisLabel: {
            color: '#aaa',
            fontSize: 10,
          },
        };
      }),
      series: formatSeries(yAxis),
    },
    option,
  );

  return <Echarts option={options} {...other} />;
};

export default LineChart;
