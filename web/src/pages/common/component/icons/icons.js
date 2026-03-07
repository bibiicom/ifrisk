import React from 'react'

import BTC from './BTC'
import BSV from './BSV'
import BCH from './BCH'
import EOS from './EOS'
import ETC from './ETC'
import ETH from './ETH'
import LINK from './LINK'
import LTC from './LTC'
import NEO from './NEO'
import TRX from './TRX'
import XRP from './XRP'
import Default from './Default'

import './icons.less'

export function getIcon (type, style, className) {
  switch (type) {
    case 'BTC': return <BTC style={style} className={className}/>
    case 'BSV': return <BSV style={style} className={className}/>
    case 'BCH': return <BCH style={style} className={className}/>
    case 'EOS': return <EOS style={style} className={className}/>
    case 'ETC': return <ETC style={style} className={className}/>
    case 'ETH': return <ETH style={style} className={className}/>
    case 'LINK': return <LINK style={style} className={className}/>
    case 'LTC': return <LTC style={style} className={className}/>
    case 'NEO': return <NEO style={style} className={className}/>
    case 'TRX': return <TRX style={style} className={className}/>
    case 'XRP': return <XRP style={style} className={className}/>
    default: return <Default style={style} className={className}/>
  }
}
