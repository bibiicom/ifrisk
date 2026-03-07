import React, { PureComponent } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'

export default (WrapperComponent) => {
  @connect(state => {
    return {
      theme: state.app.get('theme')
    }
  }, dispatch => {
    return bindActionCreators({}, dispatch)
  })
  class ThemeComponent extends PureComponent {
    render () {
      return <WrapperComponent { ...this.props }/>
    }
  }

  return ThemeComponent
}
