import intl from '../component/intl'

import { Direction as OrderDirection } from './order'

export const Type = {
  DOUBLE: '1',
  SINGLE: '2',
  transfer: function (type) {
    switch (type) {
      case this.DOUBLE: return intl.get('two_way')
      case this.SINGLE: return intl.get('single_way')
    }
  }
}

export const Direction = {
  LONG: '0',
  SHORT: '1',
  NET: '2',
  transfer: function (direction) {
    switch (direction) {
      case this.LONG: return intl.get('much')
      case this.SHORT: return intl.get('void')
    }
  }
}

export function transferDirectionTag (position) {
  if (position.PosiDirection === Direction.NET) {
    return position.Direction === OrderDirection.BUY ? intl.get('buy') : intl.get('sell')
  }
  return Direction.transfer(position.PosiDirection)
}

export const MarginMode = {
  ISOLATED: 0,
  CROSS: 1,
  transfer: function (mode) {
    switch (mode) {
      case this.CROSS: return intl.get('full_warehouse')
      case this.ISOLATED: return intl.get('warehouse_by_warehouse')
    }
  }
}
