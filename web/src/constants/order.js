import intl from '../component/intl'

import { Type as PositionType } from './position'

export const Type = {
  LIMIT: 'limit',
  PLAN: 'plan',
  STOP_PROFIT_LOSS: 'stop_profit_loss',
  SUBSCRIPTION: 'subscription',
  createOptions () {
    return [
      { key: this.LIMIT, text: intl.get('limit_order') },
      { key: this.SUBSCRIPTION, text: intl.get('launchpad2_subscribe') },
      { key: this.PLAN, text: intl.get('plan_order') }
      // { key: this.STOP_PROFIT_LOSS, text: intl.get('stop_profit_loss') }
    ]
  },
  transferText (type) {
    switch (type) {
      case this.LIMIT: return intl.get('limit_order')
      case this.SUBSCRIPTION: return intl.get('launchpad2_subscribe')
      case this.PLAN: return intl.get('plan_order')
      // case this.STOP_PROFIT_LOSS: return intl.get('stop_profit_loss')
    }
  }
}

export const Direction = {
  BUY: '0',
  SELL: '1'
}

export const OffsetFlag = {
  OPEN: '0',
  CLOSE: '1',
  FORCE_CLOSE: '2',
  CLOSE_TODAY: '3',
  CLOSE_YESTERDAY: '4',
  CLOSE_ALL: '5',
  CLOSE_APPOINT_ORDER: '6',
  CLOSE_APPOINT_TRADE: '7',
  CLOSE_MAX: '8'
}

export const InstructType = {
  NORMAL: '0',
  FAK: '1',
  FOK: '2',
  ONLY_MAKER: '3',
  NOT_MATCH: '4'
}

export const PriceType = {
  LIMIT: '0',
  ANY: '1',
  // 市价用最优10档代替
  // MARKET: '1',
  MARKET: '4',
  OPPONENT: '2',
  OPPONENT_5: '3',
  OPPONENT_10: '4',
  OPPONENT_20: '5',
  OPPONENT_30: '6',
  BEST_MARKER: '7',
  transfer: function (type) {
    switch (type) {
      case this.LIMIT: return intl.get('limit_price')
      case this.ANY:
      case this.MARKET: return intl.get('market_price')
      case this.OPPONENT: return intl.get('opponent_price')
      case this.OPPONENT_5: return intl.get('best_5')
      case this.OPPONENT_10: return intl.get('best_10')
      case this.OPPONENT_20: return intl.get('best_20')
      case this.OPPONENT_30: return intl.get('best_30')
      case this.BEST_MARKER: return intl.get('best_marker')
    }
  }
}

export const VolumeMode = {
  PERCENT: '0',
  VALUE: '1'
}

export const LimitOrderStatus = {
  ALL: '',
  ALL_TRADE: '1',
  PARTIAL_TRADE: '2',
  PARTIAL_TRADE_PARTIAL_REVOKE: '3',
  UNSOLD: '4',
  ALL_REVOKE: '6',
  createOptions () {
    return [
      { key: '', text: intl.get('all') },
      { key: this.UNSOLD, text: intl.get('unsold') },
      { key: this.ALL_TRADE, text: intl.get('all_trade') },
      { key: this.PARTIAL_TRADE, text: intl.get('partial_trade') },
      { key: this.PARTIAL_TRADE_PARTIAL_REVOKE, text: intl.get('partial_trade_partial_revoke') },
      { key: this.ALL_REVOKE, text: intl.get('has_revoked') }
    ]
  },
  transfer: function (status) {
    switch (status) {
      case this.ALL: return intl.get('all')
      case this.ALL_TRADE: return intl.get('all_trade')
      case this.PARTIAL_TRADE: return intl.get('partial_trade')
      case this.PARTIAL_TRADE_PARTIAL_REVOKE: return intl.get('partial_trade_partial_revoke')
      case this.UNSOLD: return intl.get('unsold')
      case this.ALL_REVOKE: return intl.get('has_revoked')
      default: return intl.get('unknow_status')
    }
  }
}

export const TriggerOrderType = {
  POSITION_STOP_PROFIT_LOSS: '1',
  ORDER_STOP_PROFIT_LOSS: '2',
  PLAN: '3',
  transfer: function (type) {
    switch (type) {
      case this.POSITION_STOP_PROFIT_LOSS: return intl.get('position_stop_profit_loss')
      case this.ORDER_STOP_PROFIT_LOSS: return intl.get('order_stop_profit_loss')
      case this.PLAN: return intl.get('plan_order')
    }
  }
}

export const TriggerOrderStatus = {
  ALL: '',
  NOT_IN_QUEUE: '0',
  NOT_TRIGGER: '1',
  TRIGGER_SUCCESS: '2',
  TRIGGER_FAILED: '3',
  REVOKE: '4',
  createOptions () {
    return [
      { key: '', text: intl.get('all') },
      { key: this.NOT_TRIGGER, text: intl.get('wait_effect') },
      { key: this.TRIGGER_SUCCESS, text: intl.get('trigger_success') },
      { key: this.TRIGGER_FAILED, text: intl.get('trigger_failed') },
      { key: this.REVOKE, text: intl.get('has_revoked') }
    ]
  },
  transfer: function (status) {
    switch (status) {
      case this.ALL: return intl.get('all')
      case this.NOT_IN_QUEUE:
      case this.NOT_TRIGGER: return intl.get('wait_effect')
      case this.TRIGGER_SUCCESS: return intl.get('trigger_success')
      case this.TRIGGER_FAILED: return intl.get('trigger_failed')
      case this.REVOKE: return intl.get('has_revoked')
    }
  }
}

export const TriggerOrderTriggerPriceType = {
  NORMAL: 'normal',
  LAST_PRICE: '0',
  MARK_PRICE: '1',
  TARGET_INDEX_PRICE: '2',
  createOptions () {
    return [
      { key: this.LAST_PRICE, text: intl.get('last_price') },
      { key: this.MARK_PRICE, text: intl.get('mark_price') },
      { key: this.TARGET_INDEX_PRICE, text: intl.get('target_index_price') }
    ]
  },
  transfer: function (type) {
    switch (type) {
      case this.LAST_PRICE: return intl.get('last_price')
      case this.MARK_PRICE: return intl.get('mark_price')
      case this.TARGET_INDEX_PRICE: return intl.get('target_index_price')
    }
  }
}

export const TradeDirection = {
  ALL: '',
  BUY_IN: '0',
  SELL_OUT: '1',
  OPEN_LONG: '2',
  OPEN_SHORT: '3',
  CLOSE_LONG: '4',
  CLOSE_SHORT: '5',
  ENFORCE_CLOSE_LONG: '6',
  ENFORCE_CLOSE_SHORT: '7',
  createSpotOptions () {
    return [
      { key: this.ALL, text: intl.get('all') },
      { key: this.BUY_IN, text: intl.get('buy_in') },
      { key: this.SELL_OUT, text: intl.get('sell_out') }
    ]
  },
  createContractOptions () {
    return [
      { key: this.ALL, text: intl.get('all') },
      { key: this.OPEN_LONG, text: intl.get('open_long') },
      { key: this.OPEN_SHORT, text: intl.get('open_short') },
      { key: this.CLOSE_LONG, text: intl.get('close_long') },
      { key: this.CLOSE_SHORT, text: intl.get('close_short') },
      { key: this.ENFORCE_CLOSE_LONG, text: intl.get('enforce_close_long') },
      { key: this.ENFORCE_CLOSE_SHORT, text: intl.get('enforce_close_short') }
    ]
  },
  transferText (direction) {
    switch (direction) {
      case this.ALL: return intl.get('all')
      case this.BUY_IN: return intl.get('buy_in')
      case this.SELL_OUT: return intl.get('sell_out')
      case this.OPEN_LONG: return intl.get('open_long')
      case this.OPEN_SHORT: return intl.get('open_short')
      case this.CLOSE_LONG: return intl.get('close_long')
      case this.CLOSE_SHORT: return intl.get('close_short')
      case this.ENFORCE_CLOSE_LONG: return intl.get('enforce_close_long')
      case this.ENFORCE_CLOSE_SHORT: return intl.get('enforce_close_short')
    }
  },
  trasnferParams (direction) {
    switch (direction) {
      case this.ALL: return {}
      case this.BUY_IN:
      case this.OPEN_LONG: return { Direction: Direction.BUY, OffsetFlag: OffsetFlag.OPEN }
      case this.SELL_OUT:
      case this.OPEN_SHORT: return { Direction: Direction.SELL, OffsetFlag: OffsetFlag.OPEN }
      case this.CLOSE_LONG: return { Direction: Direction.SELL, OffsetFlag: OffsetFlag.CLOSE }
      case this.CLOSE_SHORT: return { Direction: Direction.BUY, OffsetFlag: OffsetFlag.CLOSE }
      case this.ENFORCE_CLOSE_LONG: return { Direction: Direction.SELL, OffsetFlag: OffsetFlag.ENFORCE_CLOSE }
      case this.ENFORCE_CLOSE_SHORT: return { Direction: Direction.BUY, OffsetFlag: OffsetFlag.ENFORCE_CLOSE }
    }
  },
  transferDirectionOffsetFlagText (order, positionType) {
    const offsetFlag = order.OffsetFlag
    const isBuy = order.Direction === Direction.BUY
    if (positionType === PositionType.SINGLE) {
      if (offsetFlag === OffsetFlag.FORCE_CLOSE) {
        return isBuy ? intl.get('force_buy_in') : intl.get('force_sell_out')
      }
      return isBuy ? intl.get('buy_in') : intl.get('sell_out')
    }
    switch (offsetFlag) {
      case OffsetFlag.OPEN: {
        return isBuy ? intl.get('buy_in_open_much') : intl.get('sell_out_open_void')
      }
      case OffsetFlag.CLOSE_ALL:
      case OffsetFlag.CLOSE: {
        return isBuy ? intl.get('buy_in_close_void') : intl.get('sell_out_close_much')
      }
      case OffsetFlag.FORCE_CLOSE: {
        return isBuy ? intl.get('force_close_void') : intl.get('force_close_much')
      }
    }
  }
}
