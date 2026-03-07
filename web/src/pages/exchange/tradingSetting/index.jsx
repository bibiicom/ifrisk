import React, { useState, useContext, memo } from 'react'

import RootContext from '../../rootContext'
import ExchangeContext from '../context'

import Button from '../../../component/button'
import Row from '../../../component/row'
import Toggle from '../../../component/toggle'
import useScreenSize, { ScreenSize } from '../../../component/hooks/useScreenSize'
import { Calculator as CalculatorIcon } from '../../../component/icons'
import intl from '../../../component/intl'
import ComponentContext from '../../../component/context'

import { Status } from '../../../constants/auth'
import { MarginModeModal, PositionTypeModal } from '../modal'

import Calculator from './calculator'

import { MarginMode, Direction, Type } from '../../../constants/position'
import VolumeUnitType from '../../../constants/volumeUnitType'
import { ProductGroup } from '../../../constants/symbol'

import './index.less'

const TradingSetting = (({ onLeverClick }) => {
  const { authStatus } = useContext(RootContext)
  const {
    currentSymbol = {},
    marginMode, positionType, levers, volumeUnitType, setVolumeUnitType,
    setMarginMode, setPositionType
  } = useContext(ExchangeContext)

  const { theme } = useContext(ComponentContext)
  const screenSize = useScreenSize()
  const [calculatorVisible, setCalculatorVisible] = useState(false)

  // console.log('currentSymbol', currentSymbol)
  if (ProductGroup.isOpenClose(currentSymbol.ProductGroup)) {
    return (
      <Row
        className="exchange-trading-setting">
        {
          calculatorVisible &&
          <Calculator
            onClose={() => { setCalculatorVisible(false) }} />
        }
        {
          currentSymbol.BaseCurrency &&
          <Toggle
            style={{ height: 26 }}
            value={volumeUnitType === VolumeUnitType.COIN ? currentSymbol.BaseCurrency : VolumeUnitType.SHEET}
            dataSource={[
              { key: currentSymbol.BaseCurrency, text: currentSymbol.BaseCurrency },
              { key: VolumeUnitType.SHEET, text: intl.get('sheet') }
            ]}
            onChange={tab => {
              setVolumeUnitType(tab === VolumeUnitType.SHEET ? VolumeUnitType.SHEET : VolumeUnitType.COIN)
            }} />
        }
        {
          <>
            <Button
              className="exchange-trading-setting-button"
              type="tertiary"
              size="small"
              onClick={() => {
                MarginModeModal.open({
                  isMobile: screenSize === ScreenSize.MIN,
                  symbol: currentSymbol,
                  marginMode,
                  onSuccess: setMarginMode,
                  config: { theme }
                })
              }}>
              {MarginMode.transfer(marginMode)}
            </Button>
            <Button
              className="exchange-trading-setting-button"
              type="tertiary"
              size="small"
              onClick={() => {
                PositionTypeModal.open({
                  isMobile: screenSize === ScreenSize.MIN,
                  positionType,
                  onSuccess: setPositionType,
                  config: { theme }
                })
              }}>
              {Type.transfer(positionType)}
            </Button>
          </>
        }
        {/* <CalculatorIcon style={{ width: 20, height: 20 }} onClick={_ => { setCalculatorVisible(true) }} /> */}
      </Row>
    )
  }
  return (
    <Row
      className="exchange-trading-setting">
      {
        <Button
          className="exchange-trading-setting-button"
          type="tertiary"
          size="small"
          onClick={() => { onLeverClick(Direction.NET) }}>
          {levers.long}X
        </Button>
      }
    </Row>
  )
})

export default memo(TradingSetting)
