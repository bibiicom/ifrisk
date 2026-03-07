import React, { useContext, useState, useMemo } from 'react'
import { useLocation } from 'react-router-dom'
import RootContext from '../../rootContext'

import Row from '../../../component/row'
import Select from '../../../component/select'
import Switch from '../../../component/switch'
import Radio from '../../../component/radio'
import Modal from '../../../component/modal/Modal'

import intl from '../../../component/intl'

import { Theme, ColorTheme } from '../../../constants/theme'

import { THEME_KEY, COLOR_THEME_KEY, CONVERTED_CURRENCY_KEY, ORDER_CONFIRM_KEY, LOCALE_KEY } from '../../storageConfig'

import {
  ConvertedCurrency
} from '../../../constants/convertedCurrency'
import './index.less'

const Setting = ({ onClose }) => {
  const { config, setConfig } = useContext(RootContext)
  const [theme, setTheme] = useState(config[THEME_KEY])
  const [colorTheme, setColorTheme] = useState(config[COLOR_THEME_KEY])
  const [convertedCurrency, setConvertedCurrency] = useState(config[CONVERTED_CURRENCY_KEY])
  const [orderConfirm, setOrderConfirm] = useState(config[ORDER_CONFIRM_KEY])
  const langText = useMemo(() => {
    switch (config[LOCALE_KEY]) {
      case 'zh-CN': return '简体中文'
      case 'zh-HK': return '繁体中文'
      case 'ko-KR': return '한국인'
      case 'ja-JP': return '日本語'
      case 'ru-RU': return 'Русский язык'
      default: return 'English'
    }
  }, [config[LOCALE_KEY]])
  const { pathname } = useLocation()

  // 使用 CSS 变量版本，自动适配主题
  return (
    <Modal
      className="header-setting-modal-auto"
      title={intl.get('setting')}
      width={400}
      buttons={[
        {
          children: intl.get('cancel'),
          text: true,
          type: 'tertiary',
          onClick: onClose
        }, {
          children: intl.get('confirm'),
          onClick: () => {
            setConfig({
              [THEME_KEY]: theme,
              [COLOR_THEME_KEY]: colorTheme,
              [CONVERTED_CURRENCY_KEY]: convertedCurrency,
              [ORDER_CONFIRM_KEY]: orderConfirm
            })
            onClose()
          }
        }
      ]}
      onClose={onClose}>
      <Select
        popoverRowStyle={{ justifyContent: 'space-between' }}
        prefix={intl.get('converted_currency')}
        dataSource={ConvertedCurrency.createOptions()}
        value={ConvertedCurrency.transferKey(convertedCurrency)}
        onSelected={(data) => { setConvertedCurrency(data.key) }}
        renderPopoverRow={data => {
          return (
            <>
              <span>{data.text}</span>
              <span>{ConvertedCurrency.transferText(data.key)}</span>
            </>
          )
        }}
      />
      {/* <Row
        className="header-setting-modal-row">
        <span className="header-setting-modal-row-title">{intl.get('theme')}</span>
        <Row>
          <Radio
            className="header-setting-modal-row-radio"
            label={intl.get('dark')}
            checked={theme === Theme.DARK}
            onChecked={ checked => { checked && setTheme(Theme.DARK) }}/>
          <Radio
            className="header-setting-modal-row-radio"
            label={intl.get('light')}
            checked={theme === Theme.LIGHT}
            onChecked={ checked => { checked && setTheme(Theme.LIGHT) }}/>
        </Row>
      </Row> */}
      <Row className="header-setting-modal-row">
        <span className="header-setting-modal-row-title">{intl.get('language')}</span>
        <Select
          style={{ fontSize: 13, hight: 40, width: 90 }}
          value={langText}
          dataSource={[
            { key: 'zh-CN', text: '简体中文' },
            // { key: 'zh-HK', text: '繁体中文' },
            { key: 'en-US', text: 'English' },
            // { key: 'ko-KR', text: '한국인' },
            // { key: 'ja-JP', text: '日本語' },
            // { key: 'ru-RU', text: 'Русский язык' }
          ]}
          onSelected={(data) => {
            window.location.href = `/${data.key}${pathname}`
          }}
        />
      </Row>
      <Row
        className="header-setting-modal-row">
        <span className="header-setting-modal-row-title">{intl.get('rise_fail_color')}</span>
        <Row
          className="header-setting-modal-row-right-container">
          <Row>
            <Radio
              className="header-setting-modal-row-radio"
              label={intl.get('green_rise_red_fail')}
              checked={colorTheme === ColorTheme.BUY_RISE_GREEN}
              onChecked={checked => { checked && setColorTheme(ColorTheme.BUY_RISE_GREEN) }} />
            <Radio
              className="header-setting-modal-row-radio"
              label={intl.get('red_rise_green_fail')}
              checked={colorTheme === ColorTheme.BUY_RISE_RED}
              onChecked={checked => { checked && setColorTheme(ColorTheme.BUY_RISE_RED) }} />
          </Row>
        </Row>
      </Row>
      <Row
        className="header-setting-modal-row">
        <span className="header-setting-modal-row-title">{intl.get('order_confirm')}</span>
        <Switch
          open={orderConfirm}
          onChange={setOrderConfirm} />
      </Row>
    </Modal>
  )
}

export default Setting
