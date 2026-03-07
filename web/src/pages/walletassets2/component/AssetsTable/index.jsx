import React from 'react';
import { Spin } from 'antd';
import intl from '@/component/intl';
import { formatDecimals } from '../../../../utils/number';
import { formatDecimal } from '../../../../utils/format';
import styles from './index.module.less';

const AssetsTable = (props) => {
  const { loading, assetsTableData, pieColors } = props;

  return (
    <div className={styles.assetsTableContainer}>
      <Spin spinning={loading} size="small">
        <table className={styles.assetsTable}>
          <colgroup>
            <col style={{ width: '30%' }} />
            <col style={{ width: '25%' }} />
            <col style={{ width: '25%' }} />
            <col style={{ width: '20%' }} />
          </colgroup>
          <thead>
            <tr>
              <th>{intl.get('wallet_assets_currency')}</th>
              <th className={styles.numCell}>{intl.get('wallet_assets_amount')}</th>
              <th className={styles.numCell}>{intl.get('wallet_assets_value')}</th>
              <th className={styles.numCell}>{intl.get('wallet_assets_percentage')}</th>
            </tr>
          </thead>
          <tbody>
            {assetsTableData.length > 0 ? (
              assetsTableData.map((asset, index) => (
                <tr key={index}>
                  <td>{asset.currency}</td>
                  <td className={styles.numCell}>{formatDecimals(asset.balance, 4)}</td>
                  <td className={styles.numCell}>{formatDecimals(asset.value, 4)}</td>
                  <td className={styles.numCell} style={{ color: pieColors[index % pieColors.length] }}>
                    {asset.percentage < 0.01 ? '<0.01%' : `${formatDecimal(asset.percentage, 2)}%`}
                  </td>
                </tr>
              ))
            ) : (
              <tr>
                <td colSpan="4" style={{ textAlign: 'center', padding: '20px' }}>

                </td>
              </tr>
            )}
          </tbody>
        </table>
      </Spin>
    </div>
  );
};

export default AssetsTable;
