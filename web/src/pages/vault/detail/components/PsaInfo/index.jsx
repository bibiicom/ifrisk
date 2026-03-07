import React from 'react'
import styles from './index.module.less'
import intl from '../../../../../component/intl'

const PsaInfo = ({ grade, gradeLabel, certNumber, year, VarietyPedigree, subject }) => {
    return (
        <div className={styles.container}>
            {/* 水印 */}
            <div className={styles.watermark}>PSA</div>

            <div className={styles.header}>
                <div className={styles.headerLeft}>
                    <div className={styles.psaLogo}>
                        <span>PSA</span>
                    </div>
                    <div className={styles.headerInfo}>
                        <h3 className={styles.title}>{intl.get('vault_psa_title')}</h3>
                        <p className={styles.subtitle}>{intl.get('vault_psa_subtitle')}</p>
                    </div>
                </div>
                <div className={styles.headerRight}>
                    <p className={styles.gradeNumber}>{grade}</p>
                    <p className={styles.gradeLabel}>{gradeLabel}</p>
                </div>
            </div>

            <div className={styles.infoGrid}>
                <div className={styles.infoItem}>
                    <p className={styles.infoLabel}>{intl.get('vault_psa_cert_number')}</p>
                    <p className={styles.infoValue}>{certNumber || '-'} </p>
                </div>

                <div className={styles.infoItem}>
                    <p className={styles.infoLabel}>{intl.get('vault_psa_year_label')}</p>
                    <p className={styles.infoValue}>{year} {VarietyPedigree}</p>
                </div>

                <div className={styles.infoItem}>
                    <p className={styles.infoLabel}>{intl.get('vault_psa_subject')}</p>
                    <p className={styles.infoValue}>{subject || '-'} </p>
                </div>

                <div className={styles.infoItem}>
                    <p className={styles.infoLabel}>{intl.get('vault_psa_verify_status')}</p>
                    <div className={styles.statusIndicator}>
                        <span className={styles.statusDot}></span>
                        <span className={styles.statusText}>{intl.get('vault_psa_verified')}</span>
                    </div>
                </div>
            </div>

            <a
                href={`https://www.psacard.com/cert/${certNumber}`}
                target="_blank"
                rel="noopener noreferrer"
                className={styles.verifyLink}
            >
                {intl.get('vault_psa_verify_link')}
            </a>
        </div>
    )
}

export default PsaInfo
