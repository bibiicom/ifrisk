import React from 'react'
import styles from './GlassCard.module.less'

const GlassCard = ({ children, variant = 'default', className = '', ...props }) => {
  const variantClass = styles[variant] || styles.default

  return (
    <div className={`${styles.glassCard} ${variantClass} ${className}`} {...props}>
      <div className={styles.glassInner}>
        {children}
      </div>
    </div>
  )
}

export default GlassCard
