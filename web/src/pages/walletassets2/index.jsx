import React, { useState, useEffect, useContext, useMemo, useCallback } from 'react';
import Decimal from 'decimal.js';
import { getAssets } from '../../api/account';
import { colors } from '../../config/color';
import RootContext from '../rootContext';
import { Status } from '../../constants/auth';
import authHOC from '../common/hoc/authHOC';
import EmptyState from '@/component/emptyState';
import PreChart from './component/PreChart';
import AssetsTable from './component/AssetsTable';
import AssetsHistory from './component/AssetsHistory';
import styles from './index.module.less';

const WalletAssets2 = authHOC(true)(() => {
  const { authStatus, userInfo } = useContext(RootContext);
  const [assetsData, setAssetsData] = useState([]);
  const [refreshingAssets, setRefreshingAssets] = useState(false);
  const [lastAssetsUpdatedAt, setLastAssetsUpdatedAt] = useState(null);

  // 获取资产价值的工具函数
  const getAssetValue = useCallback((asset) => {
    return asset?.CurrValue || asset?.currvalue || 0;
  }, []);

  const fetchAssets = useCallback(async () => {
    try {
      if (!userInfo?.MemberID) return;
      const result = await getAssets({
        AccountID: userInfo.MemberID
      });
      const dataArray = Array.isArray(result?.data) ? result.data : Array.isArray(result) ? result : [];
      const filterData = dataArray.filter((i) => i?.Currency !== '');
      setAssetsData(filterData);
      setLastAssetsUpdatedAt(Date.now());
      return filterData;
    } catch (error) {
      setAssetsData([]);
      return [];
    }
  }, [userInfo?.MemberID]);

  useEffect(() => {
    if (authStatus === Status.LOGIN && userInfo?.MemberID) {
      fetchAssets();
    } else if (authStatus === Status.LOGOUT) {
      setAssetsData([]);
    }
  }, [authStatus, userInfo?.MemberID, fetchAssets]);

  const handleRefresh = useCallback(async () => {
    if (refreshingAssets) return;
    setRefreshingAssets(true);
    await fetchAssets();
    setRefreshingAssets(false);
  }, [fetchAssets, refreshingAssets]);

  const totalAssets = useMemo(() => {
    if (!assetsData || assetsData.length === 0) return 0;
    return assetsData.reduce((sum, asset) => {
      const value = getAssetValue(asset);
      return new Decimal(sum).add(new Decimal(value)).toNumber();
    }, 0);
  }, [assetsData, getAssetValue]);

  const pieChartData = useMemo(() => {
    if (!assetsData || assetsData.length === 0 || totalAssets === 0) {
      return [];
    }

    return assetsData
      .filter(asset => {
        const value = Number(getAssetValue(asset));
        return !isNaN(value) && value > 0;
      })
      .map((asset, index) => ({
        name: String(asset.Currency || 'UNKNOWN'),
        value: Number(getAssetValue(asset)),
        amount: Number(asset.Balance || 0),
        itemId: `${asset.Currency}_${index}`,
      }));
  }, [assetsData, totalAssets, getAssetValue]);

  const assetsTableData = useMemo(() => {
    if (!assetsData || assetsData.length === 0 || totalAssets === 0) return [];

    const totalDec = new Decimal(totalAssets || 0);
    if (totalDec.lte(0)) return [];

    const rows = assetsData
      .filter(asset => (asset.Balance || 0) > 0)
      .map(asset => {
        const valueDec = new Decimal(getAssetValue(asset));
        const rawPercent = valueDec.div(totalDec).mul(100);
        const roundedPercent = rawPercent.toDecimalPlaces(2, Decimal.ROUND_HALF_UP);

        return {
          currency: asset.Currency,
          balance: asset.Balance,
          value: getAssetValue(asset),
          _valueDec: valueDec,
          _percentDec: roundedPercent,
        };
      });

    const sumRounded = rows.reduce((acc, r) => acc.add(r._percentDec), new Decimal(0));
    const diff = new Decimal(100).sub(sumRounded).toDecimalPlaces(2, Decimal.ROUND_HALF_UP);

    if (!diff.isZero() && rows.length > 0) {
      let targetIndex = 0;
      let maxValue = rows[0]._valueDec;
      for (let i = 1; i < rows.length; i++) {
        if (rows[i]._valueDec.gt(maxValue)) {
          maxValue = rows[i]._valueDec;
          targetIndex = i;
        }
      }
      rows[targetIndex]._percentDec = rows[targetIndex]._percentDec.add(diff);
    }

    return rows.map(r => ({
      currency: r.currency,
      balance: r.balance,
      value: r.value,
      percentage: r._percentDec.toNumber(),
    }));
  }, [assetsData, totalAssets, getAssetValue]);

  const pieColors = useMemo(() => {
    return (colors || []).filter(c => {
      if (typeof c !== 'string') return false;
      const hex = c.trim();
      return !['#000', '#000000', '#111', '#111111', '#222', '#222222', '#333', '#333333', '#444', '#444444', '#4a4a4a', '#4A4A4A'].includes(hex);
    });
  }, []);

  if (authStatus !== Status.LOGIN) {
    return <EmptyState />;
  }

  return (
    <div className={styles.container}>
      <PreChart
        totalAssets={totalAssets}
        lastAssetsUpdatedAt={lastAssetsUpdatedAt}
        handleRefresh={handleRefresh}
        pieChartData={pieChartData}
        pieColors={pieColors}
        loading={refreshingAssets}
        address={userInfo.MemberID}
      />

      <AssetsTable 
        loading={refreshingAssets} 
        assetsTableData={assetsTableData} 
        pieColors={pieColors} 
      />

      <AssetsHistory />
    </div>
  );
});

export default WalletAssets2;
