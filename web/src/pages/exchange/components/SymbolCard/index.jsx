import React from 'react'
import { useNavigate } from 'react-router-dom'
import { EyeOutlined } from '@ant-design/icons'
import intl from '../../../../component/intl'
import styles from './index.module.less'
import  defaultImage from '@/assets/images/common/default-card.jpeg'
import { VAULT_DETAIL } from '../../../routes'

const SymbolCard = ({ symbol, cardData }) => {
  const navigate = useNavigate()
  
  // 优先使用接口返回的卡片数据
  const card = cardData
  const imageUrl = card?.Img || defaultImage
  const title = card?.Name
  const description = card?.Description || intl.get('no_description')
  
  // 跳转到保险库详情页
  const handleViewMore = () => {
    if (card?.Id) {
      navigate(VAULT_DETAIL.replace(':id', card.Id))
    }
  }
  
  return (
    <div className={styles.card}>
      <div className={styles.image}>
        <img 
          src={imageUrl} 
          alt={title}
          onError={(e) => { e.target.src = defaultImage }}
        />
      </div>
      <div className={styles.content}>
        <div className={styles.titleRow}>
          <h3 className={styles.title}>{title}</h3>
          {card?.Id && (
            <EyeOutlined 
              className={styles.viewMoreIcon}
              onClick={handleViewMore}
            />
          )}
        </div>
        <p className={styles.description}>{description}</p>
        {card && (
          <div className={styles.info}>
            {card.CertNumber && (
              <div className={styles.infoItem}>
                <span className={styles.label}>证书编号:</span>
                <span className={styles.value}>{card.CertNumber}</span>
              </div>
            )}
            {card.ItemGrade && (
              <div className={styles.infoItem}>
                <span className={styles.label}>评级:</span>
                <span className={styles.value}>{card.ItemGrade}</span>
              </div>
            )}
            {card.Year && (
              <div className={styles.infoItem}>
                <span className={styles.label}>年份:</span>
                <span className={styles.value}>{card.Year}</span>
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  )
}

export default SymbolCard
