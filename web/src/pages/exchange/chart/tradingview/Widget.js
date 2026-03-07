import DataFeed from './DataFeed'

export default class Widget extends window.TradingView.widget {
  constructor (
    { containerId, corePath },
    symbolId, { resolution, chartType },
    language, timezone,
    { theme, customCssUrl, overrides, loadingBackgroundColor, studiesOverrides },
    resolve, getHistoryData, subscribe, unsubscribe
  ) {
    const defaultOptions = {
      symbol: symbolId,
      interval: resolution,
      fullscreen: false,
      autosize: true,
      container_id: containerId || 'trading_view_container',
      library_path: corePath || '/charting_library/',
      timezone: timezone || 'Asia/Shanghai',
      theme: theme || 'Dark',
      datafeed: new DataFeed(resolve, getHistoryData, subscribe, unsubscribe),
      debug: process.env.NODE_ENV !== 'production',
      locale: language || 'en',
      toolbar_bg: 'rgba(0, 0, 0, 0)',
      custom_css_url: customCssUrl,
      loading_screen: { backgroundColor: loadingBackgroundColor },
      overrides,
      studies_overrides: studiesOverrides,
      disabled_features: [
        'compare_symbol',
        'display_market_status',
        'go_to_date',
        'header_chart_type',
        'header_compare',
        'header_interval_dialog_button',
        'header_resolutions',
        'header_screenshot',
        'header_symbol_search',
        'header_undo_redo',
        'legend_context_menu',
        'show_hide_button_in_legend',
        'show_interval_dialog_on_key_press',
        'snapshot_trading_drawings',
        'symbol_info',
        'timeframes_toolbar',
        'volume_force_overlay',
        'header_widget'
      ],
      enabled_features: [
        'dont_show_boolean_study_arguments',
        'hide_last_na_study_output',
        'move_logo_to_main_pane',
        'same_data_requery',
        'side_toolbar_in_fullscreen_mode',
        'disable_resolution_rebuild',
        'hide_left_toolbar_by_default',
        'keep_left_toolbar_visible_on_small_screens'
      ]
    }
    super(defaultOptions)
    this.onChartReady(() => {
      this.chartReady = true
      const chart = this.chart()
      // 打开画线工具栏
      chart.executeActionById('drawingToolbarAction')
      // const movingAverage = [
      //   { param: 5, color: '#D9D9D9' },
      //   { param: 10, color: '#F5A623' },
      //   { param: 30, color: '#F601FF' },
      //   { param: 60, color: '#1587DD' }
      // ]
      // // 创建均线指标
      // movingAverage.forEach(({ param, color }) => {
      //   chart.createStudy(
      //     'Moving Average', false, false, [param],
      //     {
      //       'plot.color.0': color,
      //       precision: 2
      //     }
      //   )
      // })
      chart.setChartType(chartType)
    })
  }
}
