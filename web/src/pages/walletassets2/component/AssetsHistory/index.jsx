import React, { useContext, useEffect, useState } from "react";
import { Empty } from "antd";
import intl from '@/component/intl';
import LineChart from "../../../../component/echarts/LineChart";
import RootContext from "../../../rootContext";
import { getAccountHistoryCsv } from "../../../../api/account";
import styles from "./index.module.less";

const EMPTY_CHART_DATA = {
  xAxis: [],
  yAxis: [{ name: intl.get('wallet_assets_chart_asset'), data: [] }],
  unit: ["USD"],
};

const PRIMARY_BLUE = "#1890ff";

const parseCsvToChartData = (csvText) => {
  const lines = String(csvText || "")
    .split(/\r?\n/)
    .map((l) => l.trim())
    .filter(Boolean);

  const rawData = [];

  for (let i = 0; i < lines.length; i++) {
    const parts = lines[i].split(",");
    if (parts.length < 2) continue;
    const ts = Number(parts[0]);
    const v = Number(parts[1]);
    if (!isFinite(ts) || !isFinite(v)) continue;
    const tsMs = ts < 1e12 ? ts * 1000 : ts;
    rawData.push({ ts: tsMs, value: v });
  }

  if (rawData.length === 0) {
    return {
      xAxis: [],
      yAxis: [{ name: intl.get('wallet_assets_chart_asset'), data: [] }],
      unit: ["USD"],
    };
  }

  // 按时间排序
  rawData.sort((a, b) => a.ts - b.ts);

  // 从第一条数据的日期开始，补齐30天
  const DAYS = 30;
  const firstTs = rawData[0].ts;
  // 获取第一天的0点
  const firstDate = new Date(firstTs);
  firstDate.setHours(0, 0, 0, 0);
  const startTime = firstDate.getTime();
  const dayMs = 24 * 60 * 60 * 1000;

  const xAxis = [];
  const yData = [];

  for (let i = 0; i < DAYS; i++) {
    const dayStart = startTime + i * dayMs;
    const dayEnd = dayStart + dayMs;

    // 找到这一天范围内最后一条数据
    let dayValue = null;
    for (const item of rawData) {
      if (item.ts >= dayStart && item.ts < dayEnd) {
        dayValue = item.value;
      }
    }

    // 如果这一天有数据之前的天没有数据，使用前一天的值
    if (dayValue === null && yData.length > 0) {
      const lastValue = yData[yData.length - 1];
      // 只有当前一天有值，且当前天在已有数据范围内时才继承
      const lastRawTs = rawData[rawData.length - 1].ts;
      if (lastValue !== null && dayStart <= lastRawTs) {
        dayValue = lastValue;
      }
    }

    const d = new Date(dayStart);
    const mm = String(d.getMonth() + 1).padStart(2, "0");
    const dd = String(d.getDate()).padStart(2, "0");
    xAxis.push(`${mm}-${dd}`);
    yData.push(dayValue);
  }

  return {
    xAxis,
    yAxis: [
      {
        name: intl.get('wallet_assets_chart_asset'),
        data: yData,
      },
    ],
    unit: ["USD"],
  };
};

const fetchAccountHistoryChartData = async (userId) => {
  const address = String(userId || "").trim();
  if (!address) return EMPTY_CHART_DATA;
  const text = await getAccountHistoryCsv(address);
  return parseCsvToChartData(text);
};

const AssetsHistory = () => {
  const { userInfo } = useContext(RootContext);
  const [loading, setLoading] = useState(true);
  const [historyChartData, setHistoryChartData] = useState(EMPTY_CHART_DATA);

  const userId = (userInfo?.UserID || "").trim();

  useEffect(() => {
    let cancelled = false;

    const run = async () => {
      if (!userId) {
        setHistoryChartData(EMPTY_CHART_DATA);
        setLoading(false);
        return;
      }

      setLoading(true);

      try {
        const next = await fetchAccountHistoryChartData(userId);
        if (!cancelled) {
          setHistoryChartData(next);
        }
      } catch (e) {
        if (!cancelled) {
          setHistoryChartData(EMPTY_CHART_DATA);
        }
      } finally {
        if (!cancelled) {
          setLoading(false);
        }
      }
    };

    run();

    return () => {
      cancelled = true;
    };
  }, [userId]);

  return (
    <div className={styles.assetsHistory}>
      <h3 className={styles.title}>{intl.get('wallet_assets_chart_title')}</h3>
      <div className={styles.chartWrapper}>
        {!loading &&
          (!historyChartData?.xAxis?.length ||
            !historyChartData?.yAxis?.[0]?.data?.length) ? (
          <Empty />
        ) : (
          <LineChart
            data={historyChartData}
            color={[PRIMARY_BLUE]}
            height="200px"

            showArea={true}
            symbol="none"
          />
        )}
      </div>
    </div>
  );
};

export default AssetsHistory;
