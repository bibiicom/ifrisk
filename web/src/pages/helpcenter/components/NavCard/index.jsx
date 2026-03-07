import React from 'react'
import styles from './index.module.less'

const NavCard = ({ title, description, isActive, onClick }) => {
  return (
    <button
      onClick={onClick}
      className={`${styles.card} ${isActive ? styles.active : ''}`}
    >
      <h3 className={styles.title}>{title}</h3>
      <p className={styles.description}>{description}</p>
    </button>
  )
}

export default NavCard
