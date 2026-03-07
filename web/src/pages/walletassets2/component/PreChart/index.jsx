import React, { useCallback, useMemo, useState } from 'react';
import { Spin } from 'antd';
import { SyncOutlined } from '@ant-design/icons';
import intl from '@/component/intl';
import PieChart from '../../../../component/echarts/PieChart';
import { formatDecimals } from '../../../../utils/number';
import { formatDecimal } from '../../../../utils/format';
import DepositWithdrawModal from '../DepositWithdrawModal';
import TransferModal from '../TransferModal';
import styles from './index.module.less';

const PreChart = (props) => {
  const {
    totalAssets,
    lastAssetsUpdatedAt,
    handleRefresh,
    pieChartData,
    pieColors,
    loading,
    address,
  } = props;

  const [historyModalVisible, setHistoryModalVisible] = useState(false);
  const [transferModalVisible, setTransferModalVisible] = useState(false);
  const [transferMode, setTransferMode] = useState('deposit'); // 'deposit' 或 'withdraw'


  const tooltipFormatter = useCallback((params) => {
    // 添加安全检查，防止访问未定义的数据
    if (!params || !params.data) {
      return '';
    }

    const name = params?.name ?? '';
    const amount = params?.data?.amount;
    const percent = params?.percent;
    const value = params?.value;

    const amountText = amount !== undefined && amount !== null ? formatDecimal(amount, 8) : '--';
    const valueText = value !== undefined && value !== null ? formatDecimal(value, 8) : '--';
    const percentText = percent !== undefined && percent !== null ? formatDecimal(percent, 2) : '--';

    return `
      <div style="min-width: 160px; padding: 2px 0;">
        <div style="font-size: 13px; font-weight: 600; color: #fff; margin-bottom: 6px;">${name}</div>
        <div style="display:flex; justify-content: space-between; gap: 12px; font-size: 12px; color: rgba(255,255,255,0.85); line-height: 18px;">
          <span style="color: rgba(255,255,255,0.65);">${intl.get('wallet_assets_chart_amount')}</span>
          <span style="color: rgba(255,255,255,0.92);">${amountText}</span>
        </div>
        <div style="display:flex; justify-content: space-between; gap: 12px; font-size: 12px; color: rgba(255,255,255,0.85); line-height: 18px;">
          <span style="color: rgba(255,255,255,0.65);">${intl.get('wallet_assets_chart_percentage')}</span>
          <span style="color: rgba(255,255,255,0.92);">${percentText}%</span>
        </div>
        <div style="display:flex; justify-content: space-between; gap: 12px; font-size: 12px; color: rgba(255,255,255,0.85); line-height: 18px;">
          <span style="color: rgba(255,255,255,0.65);">${intl.get('wallet_assets_chart_value')}</span>
          <span style="color: rgba(255,255,255,0.92);">${valueText}</span>
        </div>
      </div>
    `;
  }, []);

  const pieOption = useMemo(() => {
    return {
      tooltip: {
        trigger: 'item',
        formatter: tooltipFormatter,
        backgroundColor: 'rgba(17, 17, 17, 0.92)',
        borderColor: 'rgba(255, 255, 255, 0.18)',
        borderWidth: 1,
        padding: [10, 12],
        extraCssText: 'border-radius: 10px; box-shadow: 0 8px 28px rgba(0,0,0,0.55);',
        textStyle: {
          color: '#fff',
          fontSize: 12,
        },
      },
    };
  }, [tooltipFormatter]);

  const pieSeriesOption = useMemo(() => {
    return {
      itemStyle: {
        borderColor: 'rgba(255, 255, 255, 0.10)',
        borderWidth: 1,
      },
      label: {
        show: false,
      },
      labelLine: {
        show: false,
      },
    };
  }, []);

  return (
    <div className={styles.assetsSummary} style={{ position: 'relative' }}>
      <div style={{ position: 'absolute', right: 16, top: 12, zIndex: 2, display: 'flex', alignItems: 'center', columnGap: '12px' }}>
        <div
          className={styles.historyBtn}
          onClick={() => {
            setTransferMode('deposit');
            setTransferModalVisible(true);
          }}
        >
          {intl.get('wallet_assets_deposit')}
        </div>
        <div
          className={styles.historyBtn}
          onClick={() => {
            setTransferMode('withdraw');
            setTransferModalVisible(true);
          }}
        >
          {intl.get('wallet_assets_withdraw')}
        </div>
        <div
          className={styles.historyBtn}
          onClick={() => setHistoryModalVisible(true)}
        >
          {intl.get('wallet_assets_history')}
        </div>
        <button
          className={styles.refreshBtn}
          onClick={handleRefresh}
          disabled={loading}
          type="button"
        >
          <SyncOutlined spin={loading} />
        </button>
      </div>
      <div className={styles.totalAssets}>
        <span className={styles.label}>{intl.get('wallet_assets_total')}: </span>
        <span className={styles.value}>{formatDecimals(totalAssets || 0, 4)} USD</span>
        {lastAssetsUpdatedAt ? (
          <div style={{ marginTop: 8, fontSize: 12, color: '#666' }}>
            {intl.get('wallet_assets_updated_at')}：{new Date(lastAssetsUpdatedAt).toLocaleString()}
          </div>
        ) : null}
      </div>
      <div className={styles.pieChartWrapper}>
        {pieChartData.length > 0 ? (
          <PieChart
            data={pieChartData}
            color={pieColors}
            option={pieOption}
            seriesOption={pieSeriesOption}
            height="200px"
            width="300px"
          />
        ) : loading ? (
          <div className={styles.emptyChart}>
            <Spin size="small" />
          </div>
        ) : (
          <div className={styles.emptyChart}>{intl.get('wallet_assets_no_data')}</div>
        )}
      </div>
      <DepositWithdrawModal
        visible={historyModalVisible}
        onCancel={() => setHistoryModalVisible(false)}
        address={address}
      />
      <TransferModal
        visible={transferModalVisible}
        onCancel={() => setTransferModalVisible(false)}
        mode={transferMode}
      />
    </div>
  );
};

export default React.memo(PreChart);
