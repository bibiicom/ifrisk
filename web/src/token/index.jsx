import React from 'react'
import BTCIcon from './btc'
import ETHIcon from './eth'
import USDTIcon from './usdt'

const TokenIcon = ({ name }) => {
  switch (name) {
    case 'BTC':
      return <BTCIcon/>
    case 'ETH':
      return <ETHIcon/>
    case 'USDT':
      return <USDTIcon/>
    default:
      return <div></div>
  }
}

export default TokenIcon
