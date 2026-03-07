import React, { useState, useContext, useRef, forwardRef, memo, useEffect } from 'react'

import ExchangeContext from '../context'
import RootContext from '../../rootContext'

import Checkbox from '../../../component/checkbox'
import Button from '../../../component/button'
import Message from '../../../component/message'
import { Clear } from '../../../component/icons'
import intl from '../../../component/intl'
import useScreenSize, { ScreenSize } from '../../../component/hooks/useScreenSize'
import ComponentContext from '../../../component/context'
import Toggle from '../../../component/toggle'
import Row from '../../../component/row'

import Section from '../components/section'
import { ConfirmModal } from '../modal'

import Position from './position'
import CurrentOrder from './currentOrder'
import Assets from './assets'

import { Type, TriggerOrderType } from '../../../constants/order'

import { revokeLimitOrder, revokeTriggerOrder } from '../../../api/order'

import { UPDATE_DELAY } from '../config'

import './index.less'
import FundingRate from './fundingrate'

// const TAB_CURRENT_POSITION = 'current_position'
const TAB_ALL_POSITION = 'all_position'
const TAB_CURRENT_ORDER = 'current_order'
// const TAB_HISTORY_ORDER = 'history_order'
// const TAB_FINANCIAL_RECORD = 'financial_record'
const TAB_FUNDING = 'history_funding_rate'
const TAB_ASSETS = 'assets'

export default memo(forwardRef(({ onLeverClick, onLimitOrderChange }, ref) => {
  const { currentSymbol } = useContext(ExchangeContext)
  const { theme } = useContext(ComponentContext)
  const { userInfo } = useContext(RootContext)

  const screenSize = useScreenSize()

  const [tab, setTab] = useState(TAB_CURRENT_ORDER)
  const [currentOrderType, setCurrentOrderType] = useState(Type.LIMIT)
  const [revokeOrderCount, setRevokeOrderCount] = useState(0)
  const [orderType, setOrderType] = useState(Type.LIMIT)

  // // 添加日志监控 revokeOrderCount 变化
  // useEffect(() => {
  //   console.log('=== DataQuery 组件状态 ===')
  //   console.log('revokeOrderCount 更新为:', revokeOrderCount)
  //   console.log('按钮是否禁用:', revokeOrderCount === 0)
  //   console.log('========================')
  // }, [revokeOrderCount])

  const revokeAllOrder = () => {
    // console.log('=== 点击全撤按钮 ===')
    // console.log('1. currentSymbol:', currentSymbol)
    // console.log('2. currentOrderType:', currentOrderType)
    // console.log('3. revokeOrderCount:', revokeOrderCount)
    // console.log('====================')
    
    if (currentSymbol?.InstrumentID) {
      ConfirmModal.open({
        isMobile: screenSize === ScreenSize.MIN,
        title: intl.get('revoke_order'),
        content: intl.get('confirm_revoke_all_order'),
        onConfirm: async () => {
          try {
            switch (currentOrderType) {
              case Type.LIMIT: {
                await revokeLimitOrder({
                  ExchangeID: currentSymbol.ExchangeID,
                  InstrumentID: currentSymbol?.InstrumentID,
                  MemberID: userInfo?.MemberID
                })
                break
              }
              case Type.PLAN: {
                await revokeTriggerOrder({
                  ExchangeID: currentSymbol.ExchangeID,
                  InstrumentID: currentSymbol?.InstrumentID,
                  TriggerOrderType: TriggerOrderType.PLAN,
                  MemberID: userInfo?.MemberID
                })
                break
              }
              case Type.STOP_PROFIT_LOSS: {
                await revokeTriggerOrder({
                  ExchangeID: currentSymbol.ExchangeID,
                  InstrumentID: currentSymbol?.InstrumentID,
                  TriggerOrderType: `${TriggerOrderType.POSITION_STOP_PROFIT_LOSS}${TriggerOrderType.ORDER_STOP_PROFIT_LOSS}`,
                  MemberID: userInfo?.MemberID
                })
                break
              }
            }
            if (tab === TAB_CURRENT_ORDER) {
              setTimeout(() => { ref.current.updateData() }, UPDATE_DELAY)
            }
            Message.ok({ message: intl.get('revoke_all_order_success'), config: { theme } })
          } catch (e) {
            Message.error({ message: `${intl.get('revoke_all_order_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}`, config: { theme } })
          }
        },
        config: { theme }
      })
    }
  }

  const renderTitle = () => {
    switch (tab) {
      case TAB_CURRENT_ORDER: return (
        <Row style={{ marginLeft: 'auto' }}>
          <Toggle
            style={{alignSelf: 'flex-start' , marginRight: '12px'}}
            value={orderType}
            dataSource={Type.createOptions()}
            onChange={value => {
              setOrderType(value)
              if (ref.current?.updateData) {
                ref.current.updateData(value)
              }
            }}
          />
          <Button
            className={'exchange-data-query-title-button'}
            type="tertiary"
            size="small"
            disabled={revokeOrderCount === 0}
            onClick={revokeAllOrder}>
            <span>{intl.get('all_revoke_order')}</span>
          </Button>
        </Row>
      )
      // case TAB_HISTORY_ORDER: {
      //   return (
      //     <Checkbox
      //       className="exchange-data-query-title-checkbox"
      //       label={intl.get('hidden_other')}
      //       checked={otherHistoryOrderHide}
      //       onChecked={setOtherHistoryOrderHide}/>
      //   )
      // }
    }
  }

  const renderContent = () => {
    switch (tab) {
      case TAB_CURRENT_ORDER: return <CurrentOrder ref={ref} orderType={orderType} onOrderTypeChange={setCurrentOrderType} onLimitOrderChange={onLimitOrderChange} onRevokeCountChange={setRevokeOrderCount}/>
      case TAB_ASSETS: return <Assets ref={ref} />
      case TAB_ALL_POSITION: return <Position ref={ref} key="all" visible={true}/>
      // case TAB_HISTORY_ORDER: return <HistoryOrder ref={ref} hideOther={otherHistoryOrderHide}/>
      // case TAB_FINANCIAL_RECORD: return <FinancialRecord ref={ref}/>
      case TAB_FUNDING: return <FundingRate ref={ref}/>
    }
  }

  return (
    <Section
      className={`exchange-data-query-${theme}`}
      onTabChange={setTab}
      tab={tab}
      title={renderTitle()}
      // tabs={screenSize === ScreenSize.MIN ? [
      //   // { key: TAB_CURRENT_POSITION, text: intl.get('current_position') },
      //   { key: TAB_ALL_POSITION, text: intl.get('all_position') },
      //   { key: TAB_CURRENT_ORDER, text: intl.get('current_order') },
      //   { key: TAB_ASSETS, text: intl.get('assets') }
      // ] : [
      //   // { key: TAB_CURRENT_POSITION, text: intl.get('current_position') },
      //   { key: TAB_ALL_POSITION, text: intl.get('all_position') },
      //   { key: TAB_CURRENT_ORDER, text: intl.get('current_order') },
      //   // { key: TAB_HISTORY_ORDER, text: intl.get('history_order') },
      //   // { key: TAB_FINANCIAL_RECORD, text: intl.get('financial_record') },
      //   { key: TAB_ASSETS, text: intl.get('assets') }
      // ]}>
      tabs={[
        { key: TAB_CURRENT_ORDER, text: intl.get('current_order') },
        { key: TAB_ASSETS, text: intl.get('assets') },
        // { key: TAB_ALL_POSITION, text: intl.get('all_position') },
        // { key: TAB_FUNDING, text: intl.get('history_funding_rate') }
      ]}>
      {/* <Position
        key="current"
        ref={ref}
        visible={tab === TAB_ALL_POSITION}
        onLeverClick={onLeverClick}
        onLimitOrderChange={onLimitOrderChange}/> */}
      {renderContent()}
    </Section>
  )
}))
