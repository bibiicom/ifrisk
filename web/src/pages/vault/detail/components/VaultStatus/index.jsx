import React from 'react'
import styles from './index.module.less'
import intl from '../../../../../component/intl'

const VaultStatus = ({ location }) => {
    return (
        <section className={styles.container}>
            {/* 装饰性背景 */}
            <div className={styles.decorativeBackground}></div>

            <div className={styles.content}>
                <div className={styles.iconSection}>
                    <div className={styles.outerCircle}>
                        <div className={styles.innerCircle}>
                            <svg className={styles.lockIcon} fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="1.5" d="M12 15v2m-6 4h12a2 2 0 002-2v-6a2 2 0 00-2-2H6a2 2 0 00-2 2v6a2 2 0 002 2zm10-10V7a4 4 0 00-8 0v4h8z"></path>
                            </svg>
                        </div>
                    </div>
                    {/* 呼吸灯效果 */}
                    <div className={styles.statusLight}></div>
                </div>

                <div className={styles.infoSection}>
                    <div className={styles.header}>
                        <h2 className={styles.title}>{intl.get('vault_status_title')}</h2>
                        <p className={styles.subtitle}>{intl.get('vault_status_subtitle')}</p>
                    </div>

                    <div className={styles.infoGrid}>
                        <div className={styles.infoCard}>
                            <p className={styles.infoLabel}>{intl.get('vault_status_location')}</p>
                            <p className={styles.infoValue}>{location}</p>
                        </div>
                        <div className={styles.infoCard}>
                            <p className={styles.infoLabel}>{intl.get('vault_status_audit')}</p>
                            <p className={styles.infoValueGreen}>{intl.get('vault_status_audit_value')}</p>
                        </div>
                        <div className={styles.infoCard}>
                            <p className={styles.infoLabel}>{intl.get('vault_status_insurance')}</p>
                            <p className={styles.infoValue}>{intl.get('vault_status_insurance_value')}</p>
                        </div>
                        <div className={styles.infoCard}>
                            <p className={styles.infoLabel}>{intl.get('vault_status_verification')}</p>
                            <p className={styles.infoValue}>{intl.get('vault_status_verification_value')}</p>
                        </div>
                    </div>

                    <p className={styles.disclaimer}>
                        {intl.get('vault_status_disclaimer')}
                    </p>
                </div>
            </div>
        </section>
    )
}

export default VaultStatus
