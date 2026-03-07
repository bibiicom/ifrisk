import React, { useState, useEffect, useContext } from 'react'

import Col from '../../component/col'
import intl from '../../component/intl'
import Row from '../../component/row'
import Select from '../../component/select'
import Input from '../../component/input'
import Message from '../../component/message'
import Button from '../../component/button'

import RootContext from '../rootContext'

import { withdrawal } from '../../api/account'

const MyWithdrawal = ({ onCurrencyChange }) => {
  const { userInfo } = useContext(RootContext)

  const [currency, setCurrency] = useState('TBTC')

  const [address, setAddress] = useState('')
  const [amount, setAmount] = useState('')

  useEffect(() => { onCurrencyChange(currency) }, [currency])

  return (
    <Col
      className="blockchain-tranfer-step-container">
      <Row
        className="step-title">
        <span
          className="step-title-tag">1</span>
        <span
          style={{ marginTop: 2 }}>
          {intl.get('choose_withdrawal_currency')}
        </span>
      </Row>
      <p
        className="step-select-title">
        {intl.get('currency')}
      </p>
      <Select
        className="step-select"
        value={currency}
        dataSource={['TBTC']}
        onSelected={setCurrency}/>
      <Row
        style={{ marginTop: 30 }}
        className="step-title">
        <span
          className="step-title-tag">2</span>
        <span
          style={{ marginTop: 2 }}>
          {intl.get('input_withdrawal_currency_info')}
        </span>
      </Row>
      <p
        className="step-select-title">
        {intl.get('withdrawal_currency_addr')}
      </p>
      <Input
        className="step-input"
        value={address}
        onChange={setAddress}/>
      <p
        style={{ marginTop: 12 }}
        className="step-tip">
        {intl.get('withdrawal_currency_tip').replace('{currency}', currency)}
      </p>
      <p className="step-tip">{intl.get('withdrawal_deposit_currency_tip')}</p>
      <p
        style={{ marginTop: 14 }}
        className="step-select-title">
        {intl.get('withdrawal_currency_volume')}
      </p>
      <Input
        className="step-input"
        precision={6}
        value={amount}
        onChange={setAmount}/>
      <Button
        style={{ height: 42, maxWidth: 380, marginTop: 30 }}
        onClick={async () => {
          try {
            await withdrawal({ user_id: userInfo.UserID, currency, address, amount: +amount })
            Message.ok({ message: intl.get('withdrawal_success') })
          } catch (e) {
            Message.error({ message: `${intl.get('withdrawal_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}` })
          }
        }}>
        {intl.get('withdrawal')}
      </Button>
    </Col>
  )
}

// @authHOC(true, true)
// class Withdrawal extends PureComponent {
//   render () {
//     return (
//       <Col
//         className="withdrawal">
//         <Row
//           className="withdrawal-step-container">
//           <Col
//             className="withdrawal-step withdrawal-step-left">
//             <Row
//               className="withdrawal-step-title">
//               <span
//                 className="withdrawal-step-title-tag">1</span>
//               <span
//                 style={{ marginTop: 2 }}>
//                 {intl.get('choose_withdrawal_currency')}
//               </span>
//             </Row>
//             <p
//               className="withdrawal-step-select-title">
//               {intl.get('currency')}
//             </p>
//             <Select
//               className="withdrawal-step-select"
//               valueKey="currency"
//               dataSource={[]}/>
//             <Row
//               className="withdrawal-step-left-min-volume-tip">
//               <svg
//                 viewBox="0 0 1024 1024">
//                 <path
//                   d="M512 179.2a332.8 332.8 0 1 1 0 665.6 332.8 332.8 0 0 1 0-665.6z m0 51.2a281.6 281.6 0 1 0 0 563.2 281.6 281.6 0 0 0 0-563.2z"/>
//                 <path
//                   d="M512.768 339.968a38.7072 38.7072 0 1 1 0 77.3632 38.7072 38.7072 0 0 1 0-77.3632zM546.4576 678.6048h-27.2384c-14.2848 0-29.0304-12.3904-29.0304-23.7056V498.0224h-9.728a25.8048 25.8048 0 1 1 0-51.6096s31.488-0.1024 32.3072 0c13.1072 0.9728 22.5792 6.656 22.5792 17.3568v163.2768h11.1104a25.8048 25.8048 0 1 1 0 51.5584z"/>
//               </svg>
//               <Col>
//                 <p
//                   style={{ marginTop: 3 }}>

//                 </p>
//                 <p>{intl.get('withdrawal_currency_min_volume_tip2')}</p>
//               </Col>
//             </Row>
//             <p className="withdrawal-step-left-tip">{intl.get('withdrawal_deposit_currency_tip1')}</p>
//           </Col>
//           <Col
//             className="withdrawal-step withdrawal-step-right">
//             <Row
//               className="withdrawal-step-title">
//               <span
//                 className="withdrawal-step-title-tag">2</span>
//               <span
//                 style={{ marginTop: 2 }}>
//                 {intl.get('input_withdrawal_currency_info')}
//               </span>
//             </Row>
//             <p
//               className="withdrawal-step-select-title">
//               {intl.get('withdrawal_currency_network')}
//             </p>
//             <Select
//               className="withdrawal-step-select"/>
//             <p
//               style={{ marginTop: 14 }}
//               className="withdrawal-step-select-title">
//               {intl.get('withdrawal_currency_addr')}
//             </p>
//             <input
//               className="withdrawal-step-right-input"
//               placeholder={intl.get('input_withdrawal_currency_addr')}/>
//             <p
//               style={{ marginTop: 14 }}
//               className="withdrawal-step-select-title">
//               {intl.get('withdrawal_currency_volume')}
//             </p>
//             <Input
//               style={{ width: '100%', marginTop: 4, height: 38 }}
//               valueStyle={{ marginTop: 0 }}
//               valueClassName="withdrawal-step-right-input"
//               showTickButton={false}
//               placeholder={intl.get('input_withdrawal_currency_volume')}
//               precision={2}/>
//             <Button>{intl.get('withdrawal_currency')}</Button>
//           </Col>
//         </Row>
//         <Col
//           className="withdrawal-table-container">
//           <p className="withdrawal-table-title">{intl.get('withdrawal_currency_record')}</p>
//           <Table
//             className="withdrawal-table"
//             columns={[
//               { title: intl.get('time') },
//               { title: intl.get('currency') },
//               { title: intl.get('volume') },
//               { title: intl.get('status') }
//             ]}/>
//         </Col>
//       </Col>
//     )
//   }
// }

// export default Withdrawal

export default MyWithdrawal
