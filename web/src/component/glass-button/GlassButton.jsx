import React from 'react'
import { Link } from 'react-router-dom'
import styles from './index.module.less'

const GlassButton = ({ 
  children, 
  type = 'primary', 
  to, 
  onClick, 
  className = '',
  disabled = false
}) => {
  const buttonContent = (
    <div className={`${styles.button} ${styles[type]} ${className}`} disabled={disabled}>
      {children}
    </div>
  )

  if (to) {
    return (
      <Link to={to} className={styles.link}>
        {buttonContent}
      </Link>
    )
  }

  return (
    <button 
      onClick={onClick} 
      disabled={disabled}
      className={styles.buttonWrapper}
    >
      {buttonContent}
    </button>
  )
}

export default GlassButton