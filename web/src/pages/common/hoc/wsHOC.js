import React, { PureComponent } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'

export default (wsOpenSuccessFuc, unsubscribe) => WrapperComponent => {
  @connect(state => {
    return {
      wsConnectSuccess: state.app.get('wsConnectSuccess')
    }
  }, dispatch => {
    return bindActionCreators({}, dispatch)
  })
  class WsComponent extends PureComponent {
    componentDidMount () {
      this.wsOpen(this.props.wsConnectSuccess)
    }

    componentDidUpdate (prevProps, prevState, snapshot) {
      this.wsOpen(!prevProps.wsConnectSuccess && this.props.wsConnectSuccess)
    }

    wsOpen (isOpen) {
      if (isOpen && typeof wsOpenSuccessFuc === 'function') {
        wsOpenSuccessFuc(this.props)
      }
    }

    componentWillUnmount () {
      if (this.props.wsConnectSuccess) {
        if (typeof unsubscribe === 'function') {
          unsubscribe(this.props)
        }
      }
    }

    render () {
      return <WrapperComponent { ...this.props }/>
    }
  }

  return WsComponent
}
