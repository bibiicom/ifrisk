import React, { useMemo } from 'react';
import intl from '@/component/intl';
import styles from './Timeline.module.less';
import dayjs from 'dayjs';
import utc from 'dayjs/plugin/utc';

dayjs.extend(utc);

const Timeline = ({ info }) => {
  // 时间轴节点数据
  const stats = useMemo(() => {
    const format = (time) => {
      if (!time) return '';
      const d = dayjs(time);
      if (!d.isValid()) return '';
      return d.utc().format('YYYY-MM-DD HH:mm:ss') + ' (UTC)';
    };

    return [
      {
        label: intl.get('launchpad2_upcoming'),
        value: '', // 无具体时间，第一个点默认点亮
      },
      {
        label: intl.get('launchpad2_subscription_period'),
        value: format(info?.OpenTime),
      },
      {
        label: intl.get('launchpad2_subscription_end'),
        value: format(info?.CloseTime),
      },
      {
        label: intl.get('launchpad2_trading_period'),
        value: format(info?.TradeTime),
      },
    ];
  }, [info]);

  // 计算当前所在阶段索引
  const currentPhaseIndex = useMemo(() => {
    const now = Date.now();

    const openTime = info?.OpenTime ? new Date(info.OpenTime).getTime() : null;
    const closeTime = info?.CloseTime ? new Date(info.CloseTime).getTime() : null;
    const tradeTime = info?.TradeTime ? new Date(info.TradeTime).getTime() : null;

    let index = 0; // 第一个点默认点亮

    if (openTime && now >= openTime) {
      index = 1;
    }
    if (closeTime && now >= closeTime) {
      index = 2;
    }
    if (tradeTime && now >= tradeTime) {
      index = 3;
    }

    return index;
  }, [info]);

  return (
    <div className={styles.timelineSection}>
      <div className={styles.horizontalTimeline}>
        {stats.map((stat, index) => {
          const isActiveNode = index <= currentPhaseIndex;
          const isActiveLine = index < currentPhaseIndex;

          return (
            <div key={index} className={styles.timelineItem}>
              <div className={styles.timelineNode}>
                {isActiveNode ? (
                  <div className={styles.solidDot} />
                ) : (
                  <div className={styles.hollowDot} />
                )}
                {index < stats.length - 1 && (
                  <div className={`${styles.timelineLine} ${isActiveLine ? styles.activeLine : ''}`} />
                )}
              </div>
              <div className={styles.timelineContent}>
                <div className={styles.statLabel}>{stat.label}</div>
                <div className={styles.statValue}>{stat.value}</div>
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
};

export default Timeline;