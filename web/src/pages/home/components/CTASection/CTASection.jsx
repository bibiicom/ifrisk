import React, { useState } from 'react'
import { motion } from 'framer-motion'
import WalletModal from '../../../walletmodal'
import intl from '../../../../component/intl'
import GlassButton from '../../../../component/glass-button'
import styles from './CTASection.module.less'

const CTASection = () => {
  const [walletModalOpen, setWalletModalOpen] = useState(false)
  const containerVariants = {
    hidden: { opacity: 0, y: 50 },
    visible: {
      opacity: 1,
      y: 0,
      transition: {
        duration: 0.6,
        ease: [0.4, 0, 0.2, 1]
      }
    }
  }

  return (
    <motion.section
      className={styles.ctaSection}
      variants={containerVariants}
      initial="hidden"
      whileInView="visible"
      viewport={{ once: true, margin: "-100px" }}
    >
      <div className={styles.container}>
        <div className={styles.content}>
          <motion.div
            className={styles.glowOrb}
            animate={{
              scale: [1, 1.2, 1],
              opacity: [0.3, 0.5, 0.3]
            }}
            transition={{
              duration: 4,
              repeat: Infinity,
              ease: 'easeInOut'
            }}
          />

          <h2 className={styles.title}>
            {intl.get('home_cta_title_prefix')}
            <span className={styles.highlight}>{intl.get('home_cta_title_highlight')}</span>
            {intl.get('home_cta_title_suffix')}
          </h2>

          <p className={styles.description}>
            {intl.get('home_cta_description')}
            <br />
            {intl.get('home_cta_description_line2')}
          </p>

          <div className={styles.buttons}>
            <GlassButton
              type="primary"
              onClick={() => setWalletModalOpen(true)}
            >
              <span>{intl.get('home_cta_connect')}</span>
              <svg className={styles.buttonIcon} viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M5 12h14M12 5l7 7-7 7" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
            </GlassButton>

          <WalletModal
            open={walletModalOpen}
            onClose={() => setWalletModalOpen(false)}
          />
            <GlassButton
              type="secondary"
              to="/market"
            >
              <span>{intl.get('home_cta_explore')}</span>
            </GlassButton>
          </div>

          <div className={styles.benefits}>
            <div className={styles.benefitItem}>
              <svg className={styles.benefitIcon} viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M5 13l4 4L19 7" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
              <span>{intl.get('home_encryption_tech')}</span>
            </div>
            <div className={styles.benefitItem}>
              <svg className={styles.benefitIcon} viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M5 13l4 4L19 7" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
              <span>{intl.get('home_blockchain_auth')}</span>
            </div>
            <div className={styles.benefitItem}>
              <svg className={styles.benefitIcon} viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <path d="M5 13l4 4L19 7" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
              <span>{intl.get('home_authority_guarantee')}</span>
            </div>
          </div>
        </div>

        <div className={styles.decorative}>
          <div className={styles.circle} />
          <div className={styles.circle} />
          <div className={styles.circle} />
        </div>
      </div>
    </motion.section>
  )
}

export default CTASection
