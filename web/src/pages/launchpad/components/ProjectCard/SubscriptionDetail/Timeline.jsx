import React, { useMemo } from 'react';
import styles from './Timeline.module.less';

const Timeline = ({ info }) => {
  // 时间轴节点数据
  const stats = useMemo(() => {
    const format = (time) => {
      if (!time) return '';
      const d = new Date(time);
      if (Number.isNaN(d.getTime())) return '';
      return d.toLocaleString();
    };

    return [
      {
        label: '预展期',
        value: '', // 无具体时间，第一个点默认点亮
      },
      {
        label: '申购期',
        value: format(info?.OpenTime),
      },
      {
        label: '申购结束',
        value: format(info?.CloseTime),
      },
      {
        label: '交易期',
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
