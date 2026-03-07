import React, { useMemo } from 'react';
import { merge } from 'lodash';
import Echarts from './Echarts';
import { colors } from '@/config/color';

const PieChart = (props) => {
  const { color = colors, data = [], unit = '%', labelLineColor = '#fff', option, seriesOption, ...other } = props;

  const options = useMemo(() => {
    const safeData = Array.isArray(data) && data.length > 0 ? data : [];

    const series = {
      type: 'pie',
      radius: '50%',
      center: ['50%', '50%'],
      label: {
        show: true,
        position: 'outside',
        formatter: `{b}: {d}${unit}`,
        color: '#fff',
        fontSize: 12,
        textBorderWidth: 0,
      },
      labelLine: {
        show: true,
        length: 10,
        length2: 8,
        lineStyle: {
          color: labelLineColor,
          width: 1,
        },
      },
      data: safeData,
      ...seriesOption,
    };

    return merge(
      {},
      {
        backgroundColor: 'transparent',
        color,
        tooltip: {
          trigger: 'item',
          formatter: '{b}: {c} ({d}%)',
          backgroundColor: '#111',
          borderColor: '#fff',
          borderWidth: 1,
          textStyle: {
            color: '#fff',
            fontSize: 12,
          },
        },
        legend: {
          show: true,
          bottom: 0,
          left: 'center',
          itemHeight: 8,
          itemWidth: 8,
          type: 'scroll',
          textStyle: {
            color: '#888',
            fontSize: 11,
          },
        },
        series: [series],
      },
      option || {},
    );
  }, [data, color, unit, option, seriesOption]);

  return <Echarts option={options} {...other} />;
};

export default PieChart;
