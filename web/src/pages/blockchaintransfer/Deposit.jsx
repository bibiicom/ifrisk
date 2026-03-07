import React, { useState, useEffect, useContext } from 'react'
import { CopyToClipboard } from 'react-copy-to-clipboard'

import Col from '../../component/col'
import intl from '../../component/intl'
import Row from '../../component/row'
import Select from '../../component/select'
import Button from '../../component/button'
import Message from '../../component/message'
import { Copy } from '../../component/icons'

import RootContext from '../rootContext'

import { getDepositAddresses, createDepositAddress } from '../../api/account'

const Deposit = ({ onCurrencyChange }) => {
  const { userInfo } = useContext(RootContext)

  const [currency, setCurrency] = useState('TBTC')

  const [address, setAddress] = useState()

  useEffect(() => {
    if (userInfo) {
      const requestAddresses = async () => {
        try {
          const result = await getDepositAddresses({ user_id: userInfo.UserID, blockchain: 'OKT' })
          setAddress(result)
        } catch (e) {}
      }
      setAddress()
      requestAddresses()
    }
  }, [userInfo])

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
          {intl.get('choose_deposit_currency')}
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
          {intl.get('confirm_deposit_currency_info')}
        </span>
      </Row>
      <Row
        style={{ justifyContent: 'space-between', maxWidth: 430 }}
        className="step-select-title">
        <span>{intl.get('deposit_currency_addr')}</span>
        {
          !address && (
            <Button
              className="operate-button"
              onClick={async () => {
                if (userInfo) {
                  try {
                    const result = await createDepositAddress({ user_id: userInfo.UserID, blockchain: 'OKT' })
                    if (result) {
                      setAddress(result)
                      Message.ok({ message: intl.get('create_address_success') })
                    } else {
                      Message.error({ message: intl.get('create_address_failed') })
                    }
                  } catch (e) {
                    Message.error({ message: `${intl.get('create_address_failed')}${e.errorMsg ? `, ${e.errorMsg}` : ''}` })
                  }
                }
              }}>
              {intl.get('add_address')}
            </Button>
          )
        }
      </Row>
      {
        address && (
          <Row
            key={address.address}
            className="step-address">
            <span>{address.address}</span>
            <CopyToClipboard
              text={address.address}
              onCopy={() => { Message.ok({ message: intl.get('copy_success') }) }}>
              <Copy/>
            </CopyToClipboard>
          </Row>
        )
      }
      <p
        style={{ marginTop: 20 }}
        className="step-tip">
        {intl.get('deposit_currency_tip').replace('{currency}', currency)}
      </p>
      <p className="step-tip">{intl.get('withdrawal_deposit_currency_tip')}</p>
    </Col>
  )
}

export default Deposit
