import React from 'react'
import { motion } from 'framer-motion'
import GlassCard from '../GlassCard/GlassCard'
import intl from '../../../../component/intl'
import styles from './FeaturesSection.module.less'
import TradingCardIcon from '@/assets/images/features/trading-card.svg'
import CertificationIcon from '@/assets/images/features/certification.svg'
import BreakingBoundariesIcon from '@/assets/images/features/breaking-boundaries.svg'
import UltraFastIcon from '@/assets/images/features/ultra-fast.svg'
import AssetSecurityIcon from '@/assets/images/features/asset-security.svg'
import ProfessionalAnalysisIcon from '@/assets/images/features/professional-analysis.svg'

const features = [
  {
    iconSrc: TradingCardIcon,
    titleKey: 'home_feature_trading_card',
    descriptionKey: 'home_feature_trading_card_desc'
  },
  {
    iconSrc: CertificationIcon,
    titleKey: 'home_feature_certification',
    descriptionKey: 'home_feature_certification_desc'
  },
  {
    iconSrc: BreakingBoundariesIcon,
    titleKey: 'home_feature_boundary',
    descriptionKey: 'home_feature_boundary_desc'
  },
  {
    iconSrc: UltraFastIcon,
    titleKey: 'home_feature_fast',
    descriptionKey: 'home_feature_fast_desc'
  },
  {
    iconSrc: AssetSecurityIcon,
    titleKey: 'home_feature_security',
    descriptionKey: 'home_feature_security_desc'
  },
  {
    iconSrc: ProfessionalAnalysisIcon,
    titleKey: 'home_feature_analysis',
    descriptionKey: 'home_feature_analysis_desc'
  }
]

const FeaturesSection = () => {
  const containerVariants = {
    hidden: { opacity: 0 },
    visible: {
      opacity: 1,
      transition: {
        staggerChildren: 0.1
      }
    }
  }

  const itemVariants = {
    hidden: { opacity: 0, y: 40 },
    visible: {
      opacity: 1,
      y: 0,
      transition: {
        duration: 0.5,
        ease: [0.4, 0, 0.2, 1]
      }
    }
  }

  return (
    <motion.section
      className={styles.featuresSection}
      variants={containerVariants}
      initial="hidden"
      whileInView="visible"
      viewport={{ once: true, margin: "-100px" }}
    >
      <div className={styles.container}>
        <motion.div variants={itemVariants} className={styles.header}>
          <h2 className={styles.title}>
            {intl.get('home_why_choose')}
            <span className={styles.highlight}>scardex</span>
          </h2>
          <p className={styles.subtitle}>
            {intl.get('home_features_subtitle')}
          </p>
        </motion.div>

        <div className={styles.grid}>
          {features.map((feature, index) => {
            const title = intl.get(feature.titleKey)
            const description = intl.get(feature.descriptionKey)

            return (
            <motion.div
              key={index}
              variants={itemVariants}
              className={styles.gridItem}
            >
              <GlassCard variant="compact" className={styles.featureCard}>
                <div className={styles.featureIcon}>
                  <img src={feature.iconSrc} alt={title} />
                </div>
                <h3 className={styles.featureTitle}>{title}</h3>
                <p className={styles.featureDesc}>{description}</p>
              </GlassCard>
            </motion.div>
            )
          })}
        </div>
      </div>
    </motion.section>
  )
}

export default FeaturesSection
