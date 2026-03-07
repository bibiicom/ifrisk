import React, { PureComponent } from 'react'

import Web3 from 'web3'

import { depositWithdrawal } from '../api/account'
import { get, post } from '../network/http'

import authHOC from './common/hoc/authHOC'

import Button from '../component/button'

import axios from 'axios'

@authHOC(true)
class TestPage extends PureComponent {
  fromTo = 'spot_to_swap'

  state = {
    result: ''
  }

  getAmount () {
    return Math.round(Math.random() * 10) + (Math.round(Math.random() * 100) / 100)
  }

  spotToSwapDeposit () {
    return depositWithdrawal({
      AccountID: 'test01',
      ActionType: '6',
      Amount: this.getAmount(),
      Currency: 'USDT',
      OtherAccountID: 'test01',
      OtherSettlementGroup: 'SPOT',
      SettlementGroup: 'Spot',
      UserID: 'test01'
    })
  }

  swapToSpotDeposit () {
    return depositWithdrawal({
      AccountID: 'test01',
      ActionType: '6',
      Amount: this.getAmount(),
      Currency: 'USDT',
      OtherAccountID: 'test01',
      OtherSettlementGroup: 'Spot',
      SettlementGroup: 'SPOT',
      UserID: 'test01'
    })
  }

  deposit () {
    if (this.fromTo === 'spot_to_swap') {
      this.spotToSwapDeposit().then(
        result => {
          console.log(result)
        }
      ).catch(error => {
        // console.log(error)
      }).finally(
        _ => {
          this.fromTo = 'swap_to_spot'
          this.timer = setTimeout(() => {
            this.deposit()
          }, 2000)
        }
      )
    } else {
      this.swapToSpotDeposit().then(
        result => {
          console.log(result)
        }
      ).catch(error => {
        // console.log(error)
      }).finally(
        _ => {
          this.fromTo = 'spot_to_swap'
          this.timer = setTimeout(() => {
            this.deposit()
          }, 2000)
        }
      )
    }
  }

  componentDidMount () {
    // this.deposit()
    // const web3 = new Web3(window.ethereum)
    // console.log(web3)
    // web3.eth.getAccounts().then(result => { console.log(result) }).catch(error => { console.log(error) })
    // web3.eth.getBalance('0x7947e92776Ef2e52c710D69cF34801BC181f8fEB').then(result => { console.log(result) }).catch(error => { console.log(error) })

    // web3.eth.getWork().then(result => { console.log(result) }).catch(error => { console.log(error) })

    let testCancel
    for (let i = 0; i < 100; i++) {
      // axios.get('http://localhost:3000/swap/query/v1.0/Instrument?ProductGroup=Spot&pageIndex=1&pageSize=30', {
      //   cancelToken: new axios.CancelToken(cancel => {
      //     testCancel = cancel
      //   })
      // })
      // get('query/v1.0/Instrument', { ProductGroup: 'Spot', pageIndex: 1, pageSize: 30 }, true, {
      //   cancelToken: new axios.CancelToken(cancel => {
      //     testCancel = cancel
      //   })
      // })
      // if (testCancel) {
      //   console.log(11111)
      //   testCancel({ code: -100 })
      // }
    }
  }

  sendTransaction () {
    const ethereum = window.ethereum
    if (ethereum.selectedAddress) {
      window.ethereum.request({
        method: 'eth_sendTransaction',
        params: [
          {
            from: ethereum.selectedAddress,
            to: '0x2f318C334780961FB129D2a6c30D0763d9a5C970',
            value: Web3.utils.toHex(Web3.utils.toWei('1', 'ether'))
            // gasPrice: web3.utils.toWei('0.1'),
            // gas: '0x2710'
          }
        ]
      }).then((txHash) => console.log(txHash)).catch((error) => console.log(error))
    } else {
      console.log('connect......')
    }
  }

  connectWallet = () => {
    const ethereum = window.ethereum
    if (!ethereum.selectedAddress) {
      window.ethereum.request({
        method: 'eth_requestAccounts'
      }).then((accounts) => console.log('accounts', accounts)).catch((error) => console.log(error))
    } else {
      console.log('connect......')
    }
    //   window.ethereum.enable()
    // }
  }

  componentWillUnmount () {
    clearTimeout(this.timer)
  }

  render () {
    const { result } = this.state
    return (
      <div>
        <Button onClick={this.connectWallet}>连接</Button>
        <Button style={{ marginLeft: 20 }} onClick={this.sendTransaction}>转账</Button>
        <p
          style={{ display: 'flex', flexWrap: 'wrap', padding: 10 }}>
          {result}
        </p>
      </div>
    )
  }
}

export default TestPage
