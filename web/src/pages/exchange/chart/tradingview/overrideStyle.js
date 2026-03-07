import { Theme, ColorTheme } from '../../../../constants/theme'

export default (theme, colorTheme) => {
  const isDark = theme === Theme.DARK
  const isRiseGreen = colorTheme === ColorTheme.BUY_RISE_GREEN
  return {
    customCssUrl: isDark ? '/chart_style/chart-night.css' : '/chart_style/chart-day.css',
    loadingBackgroundColor: isDark ? '#121212' : '#ffffff',
    overrides: {
      'paneProperties.background': isDark ? '#121212' : '#ffffff',
      'paneProperties.vertGridProperties.color': isDark ? 'rgba(255, 255, 255, .05)' : 'rgba(0, 0, 0, .05)',
      'paneProperties.vertGridProperties.style': 2,
      'paneProperties.horzGridProperties.color': isDark ? 'rgba(255, 255, 255, .05)' : 'rgba(0, 0, 0, .05)',
      'paneProperties.horzGridProperties.style': 2,

      // 'scalesProperties.backgroundColor' : "#f00",
      'scalesProperties.lineColor': isDark ? 'rgba(255, 255, 255, .1)' : 'rgba(0, 0, 0, .1)',
      'scalesProperties.textColor': isDark ? 'rgba(255, 255, 255, .5)' : 'rgba(0, 0, 0, .5)',
      // 'scalesProperties.scaleSeriesOnly': true

      'mainSeriesProperties.candleStyle.upColor': isRiseGreen ? '#2DC08E' : '#F92855',
      'mainSeriesProperties.candleStyle.downColor': isRiseGreen ? '#F92855' : '#2DC08E',
      'mainSeriesProperties.candleStyle.borderUpColor': isRiseGreen ? '#2DC08E' : '#F92855',
      'mainSeriesProperties.candleStyle.borderDownColor': isRiseGreen ? '#F92855' : '#2DC08E',
      'mainSeriesProperties.candleStyle.wickUpColor': isRiseGreen ? '#2DC08E' : '#F92855',
      'mainSeriesProperties.candleStyle.wickDownColor': isRiseGreen ? '#F92855' : '#2DC08E'
    },
    studiesOverrides: {
      'volume.volume.color.0': isRiseGreen ? '#2DC08E' : '#F92855',
      'volume.volume.color.1': isRiseGreen ? '#F92855' : '#2DC08E'
    }
  }
}
