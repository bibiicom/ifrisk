import React, { useState, useEffect, useContext, useMemo } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { DatePicker, Select, Input, InputNumber, Button, Modal, App } from 'antd';
import { SearchOutlined, FilterOutlined } from '@ant-design/icons';
import { fetchLaunchpadInfoById, fetchSubscriptionInfo } from '@/api/launchpad';
import { formatDecimals } from '@/utils/number';
import RootContext from '@/pages/rootContext';
import { Status } from '@/constants/auth';
import authHOC from '@/pages/common/hoc/authHOC';
import ProContainer from '@/component/ProContainer';
import dayjs from 'dayjs';
import styles from './SubscriptionRecords.module.less';

const { RangePicker } = DatePicker;
const { Option } = Select;

const SubscriptionRecords = () => {
  const { message } = App.useApp();
  const { projectId } = useParams();
  const navigate = useNavigate();
  const { userInfo, authStatus } = useContext(RootContext);

  // 状态管理
  const [loading, setLoading] = useState(false);
  const [searchText, setSearchText] = useState('');
  const [sortBy, setSortBy] = useState('time_desc');
  const [currentPage, setCurrentPage] = useState(1);
  const [pageSize] = useState(10);
  const [filterVisible, setFilterVisible] = useState(false);
  const [projectInfo, setProjectInfo] = useState(null);

  // 筛选条件
  const [filters, setFilters] = useState({
    timeRange: null,
    quantityRange: { min: null, max: null }
  });

  // 统计数据
  const [statistics, setStatistics] = useState({
    totalUsers: 0,
    totalQuantity: 0,
    myVolume: 0,
    confirmedRate: 0,
    remainingQuantity: 0
  });

  // 列表数据（allRecords 为源数据，其余通过 useMemo 派生）
  const [/* records */, /* setRecords */] = useState([]); // 占位，避免大改，后续通过 useMemo 计算 records
  const [/* totalRecords */, /* setTotalRecords */] = useState(0);
  const [allRecords, setAllRecords] = useState([]);

  // 获取项目信息
  useEffect(() => {
    if (!projectId) return;
    fetchLaunchpadInfoById({ id: projectId })
      .then((res) => {
        if (res?.code === 0 && res?.data) {
          setProjectInfo(res?.data?.launchpadInfo || {});
        }
      })
      .catch((err) => {
        message.error(err?.msg || '获取项目信息失败');
      });
  }, [projectId]);

  // 获取认购记录数据
  useEffect(() => {
    if (!projectId || !projectInfo) return;
    fetchSubscriptionData();
  }, [projectId, projectInfo]);

  const fetchSubscriptionData = async () => {
    setLoading(true);
    try {
      const res = await fetchSubscriptionInfo({ launchpadID: parseInt(projectId) });
      if (res?.code === 0 && res?.data) {
        const subscriptionList = res.data.subscriptionList || [];
        const stockVolume = parseFloat(res.data.StockVolume || 0);

        // 转换数据格式
        const formattedRecords = subscriptionList.map((item, index) => ({
          id: `${item.accountId}-${item.insertTime}-${index}`,
          walletAddress: item.accountId || item.memberId,
          subscribeTime: item.insertTime || '-',
          quantity: item.volume || 0,
          cost: item.cost || 0,
          timestamp: item.insertTime || 0
        }));

        setAllRecords(formattedRecords);

        // 计算统计数据
        const uniqueUsers = new Set(
          subscriptionList.map(item => item.accountId || item.memberId)
        ).size;

        // 总认购数量 = 所有已认购 volume 之和 + 剩余量
        const subscribedTotal = subscriptionList.reduce(
          (sum, item) => sum + (item.volume || 0),
          0
        );
        const totalQuantity = subscribedTotal + stockVolume;

        // 计算当前地址的认购总量（仅在已登录时）
        let myVolume = 0;
        let confirmedRate = 0;

        const walletAddress =
          userInfo?.WalletAddress ||
          userInfo?.UserID ||
          userInfo?.accountId ||
          '';

        if (authStatus === Status.LOGIN && walletAddress) {
          const lowerAddr = walletAddress.toLowerCase();
          myVolume = subscriptionList.reduce((sum, item) => {
            const acc = (item.accountId || item.memberId || '').toLowerCase();
            if (acc === lowerAddr) {
              return sum + (item.volume || 0);
            }
            return sum;
          }, 0);

          if (totalQuantity > 0 && myVolume > 0) {
            confirmedRate = formatDecimals((myVolume / totalQuantity) * 100, 4);
          }
        }

        setStatistics({
          totalUsers: uniqueUsers,
          totalQuantity,
          myVolume,
          confirmedRate,
          remainingQuantity: stockVolume
        });
      }
    } catch (err) {
      message.error(err?.msg || '获取认购记录失败');
    } finally {
      setLoading(false);
    }
  };

  // 处理分页、排序、筛选（派生数据）
  const { records, totalRecords } = useMemo(() => {
    let filtered = [...allRecords];

    // 搜索过滤
    if (searchText) {
      const keyword = searchText.toLowerCase();
      filtered = filtered.filter((record) =>
        record.walletAddress.toLowerCase().includes(keyword)
      );
    }

    // 时间范围过滤
    if (filters.timeRange && filters.timeRange[0] && filters.timeRange[1]) {
      const startTime = filters.timeRange[0].valueOf() / 1000;
      const endTime = filters.timeRange[1].valueOf() / 1000;
      filtered = filtered.filter(
        (record) => record.timestamp >= startTime && record.timestamp <= endTime
      );
    }

    // 数量范围过滤
    const { min, max } = filters.quantityRange;
    if (min !== null && min !== '') {
      const minVal = parseFloat(min);
      if (!Number.isNaN(minVal)) {
        filtered = filtered.filter((record) => record.quantity >= minVal);
      }
    }
    if (max !== null && max !== '') {
      const maxVal = parseFloat(max);
      if (!Number.isNaN(maxVal)) {
        filtered = filtered.filter((record) => record.quantity <= maxVal);
      }
    }

    // 排序
    filtered.sort((a, b) => {
      switch (sortBy) {
        case 'time_desc':
          return b.timestamp - a.timestamp;
        case 'time_asc':
          return a.timestamp - b.timestamp;
        case 'quantity_desc':
          return b.quantity - a.quantity;
        case 'quantity_asc':
          return a.quantity - b.quantity;
        default:
          return 0;
      }
    });

    const total = filtered.length;

    // 分页
    const startIndex = (currentPage - 1) * pageSize;
    const endIndex = startIndex + pageSize;
    const pageRecords = filtered.slice(startIndex, endIndex);

    return {
      records: pageRecords,
      totalRecords: total,
    };
  }, [allRecords, searchText, filters, sortBy, currentPage, pageSize]);


  const handleFilterApply = () => {
    setFilterVisible(false);
    setCurrentPage(1);
  };

  const handleFilterReset = () => {
    setFilters({
      timeRange: null,
      quantityRange: { min: null, max: null }
    });
  };

  const totalPages = Math.ceil(totalRecords / pageSize) || 1;

  return (
    <ProContainer >
      <div className={styles.container}>
        {/* 顶部导航区 */}
        <div className={styles.header}>
          <div className={styles.headerLeft}>
            <Button
              type="text"
              onClick={() => navigate(-1)}
              className={styles.backButton}
            >
              ← 返回
            </Button>
            <h1 className={styles.title}>{projectInfo?.Name || '加载中...'} 认购记录</h1>
          </div>
          <div className={styles.headerRight}>
            <Button
              icon={<FilterOutlined />}
              onClick={() => setFilterVisible(true)}
              className={styles.filterButton}
            >
              筛选
            </Button>
          </div>
        </div>

        {/* 统计概览区 */}
        <div className={styles.statisticsSection}>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>总认购人数</div>
            <div className={styles.statValue}>{statistics.totalUsers}</div>
          </div>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>我的认购</div>
            <div className={styles.statValue}>
              {authStatus !== Status.LOGIN || !statistics.myVolume
                ? '--'
                : (
                  <>
                    {statistics.myVolume}
                    {statistics.confirmedRate
                      ? `（${statistics.confirmedRate}%）`
                      : ''}
                  </>
                )}
            </div>
          </div>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>总认购数量</div>
            <div className={styles.statValue}>{statistics.totalQuantity}</div>
          </div>
          <div className={styles.statCard}>
            <div className={styles.statLabel}>剩余可认购量</div>
            <div className={styles.statValue}>{statistics.remainingQuantity}</div>
          </div>
        </div>

        {/* 搜索与列表控制区 */}
        <div className={styles.controlSection}>
          <Input
            placeholder="搜索钱包地址"
            prefix={<SearchOutlined />}
            value={searchText}
            onChange={(e) => setSearchText(e.target.value)}
            className={styles.searchInput}
          />
          <Select
            value={sortBy}
            onChange={setSortBy}
            className={styles.sortSelect}
          >
            <Option value="time_desc">按认购时间倒序</Option>
            <Option value="time_asc">按认购时间正序</Option>
            <Option value="quantity_desc">按认购数量倒序</Option>
            <Option value="quantity_asc">按认购数量正序</Option>
          </Select>
        </div>

        {/* 认购记录列表区 */}
        <div className={styles.tableSection}>
          <table className={styles.table}>
            <thead>
              <tr>
                <th style={{ width: '200px' }}>钱包地址</th>
                <th style={{ width: '180px' }}>认购时间</th>
                <th style={{ width: '120px' }}>认购数量</th>
                <th style={{ width: '150px' }}>消耗 USDT</th>
              </tr>
            </thead>
            <tbody>
              {loading ? (
                <tr>
                  <td colSpan="4" className={styles.loadingCell}>加载中...</td>
                </tr>
              ) : records.length === 0 ? (
                <tr>
                  <td colSpan="4" className={styles.emptyCell}>暂无数据</td>
                </tr>
              ) : (
                records.map((record) => (
                  <tr key={record.id} className={styles.tableRow}>
                    <td>{record.walletAddress}</td>
                    <td>{dayjs(record.subscribeTime).format('YYYY-MM-DD HH:mm:ss')}</td>
                    <td>{record.quantity}</td>
                    <td>{record.cost.toFixed(8)}</td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>

        {/* 分页栏 */}
        <div className={styles.pagination}>
          <span className={styles.pageInfo}>
            当前第 {currentPage} 页 / 共 {totalPages} 页（总计 {totalRecords} 条记录）
          </span>
          <div className={styles.pageButtons}>
            <Button
              disabled={currentPage === 1}
              onClick={() => setCurrentPage(currentPage - 1)}
              className={styles.pageButton}
            >
              上一页
            </Button>
            <span className={styles.pageJump}>
              跳转到
              <InputNumber
                min={1}
                max={totalPages}
                value={currentPage}
                onChange={(value) => {
                  if (value) {
                    setCurrentPage(value);
                  }
                }}
                className={styles.pageInput}
              />
              页
            </span>
            <Button
              disabled={currentPage === totalPages}
              onClick={() => setCurrentPage(currentPage + 1)}
              className={styles.pageButton}
            >
              下一页
            </Button>
          </div>
        </div>

        {/* 筛选面板 */}
        <Modal
          title="筛选条件"
          open={filterVisible}
          onOk={handleFilterApply}
          onCancel={() => setFilterVisible(false)}
          okText="应用"
          cancelText="取消"
          className={styles.filterModal}
        >
          <div className={styles.filterContent}>
            <div className={styles.filterItem}>
              <label>认购时间范围</label>
              <RangePicker
                showTime
                value={filters.timeRange}
                onChange={(dates) => setFilters({ ...filters, timeRange: dates })}
                style={{ width: '100%' }}
              />
            </div>
            <div className={styles.filterItem}>
              <label>认购数量范围</label>
              <div className={styles.rangeInputs}>
                <Input
                  type="number"
                  placeholder="最小值"
                  value={filters.quantityRange.min}
                  onChange={(e) => setFilters({
                    ...filters,
                    quantityRange: { ...filters.quantityRange, min: e.target.value }
                  })}
                />
                <span>-</span>
                <Input
                  type="number"
                  placeholder="最大值"
                  value={filters.quantityRange.max}
                  onChange={(e) => setFilters({
                    ...filters,
                    quantityRange: { ...filters.quantityRange, max: e.target.value }
                  })}
                />
              </div>
            </div>
            <Button onClick={handleFilterReset} style={{ marginTop: '16px' }}>
              重置筛选
            </Button>
          </div>
        </Modal>
      </div>
    </ProContainer>
  );
};

export default authHOC(true)(SubscriptionRecords);
