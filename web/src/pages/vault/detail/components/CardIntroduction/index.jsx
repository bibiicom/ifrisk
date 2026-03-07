import React from 'react'
import styles from './index.module.less'

const CardIntroduction = ({ description }) => {
    return (
        <div className={styles.container}>
            <div className={styles.quoteIcon}>&ldquo;</div>
            <p className={styles.description}>{description}</p>
        </div>
    )
}

export default CardIntroduction
