import React, { useState, useEffect } from 'react'
import { Spin, Empty, Button, Table, Tooltip } from 'antd'
import intl from '@/component/intl'
import CommonModal from '@/component/commonModal'
import CommonPagination from '@/component/commonPagination'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'
import { fetchBuybackHistory } from '@/api/buyback'

dayjs.extend(utc)

const HistoryBuybackModal = () => {
  const [visible, setVisible] = useState(false)
  const [loading, setLoading] = useState(false)
  const [historyData, setHistoryData] = useState([])
  const [pagination, setPagination] = useState({
    current: 1,
    pageSize: 10,
    total: 0
  })

  // 打开弹框时加载数据
  const openModal = () => {
    setVisible(true)
  }

  // 关闭弹框
  const closeModal = () => {
    setVisible(false)
  }

  // 加载历史回购数据
  const loadHistoryData = async (params = {}) => {
    const { current = 1, pageSize = 10 } = params
    setLoading(true)
    try {
      const res = await fetchBuybackHistory({ page: current, limit: pageSize })
      if (res && res.code === 0 && res.data) {
        const list = res.data.launchpadList || []
        const mappedData = list.map((item) => ({
          key: item.Id,
          id: item.Id,
          card: item.BuyBackInfo.InstrumentID,
          user: item.BuyBackInfo?.AccountID || '--',
          amount: item.BuyBackInfo?.Volume || '0',
          time: item.BuyBackInfo?.createdAt || '--',
          price: item.BuyBackInfo?.Price || '0'
        }))
        setHistoryData(mappedData)
        setPagination(prev => ({
          ...prev,
          current,
          pageSize,
          total: res.data.total || 0
        }))
      }
    } catch (error) {
      console.error('Failed to load buyback history:', error)
    } finally {
      setLoading(false)
    }
  }

  // 每次打开弹框时加载数据
  useEffect(() => {
    if (visible) {
      // 重置分页到第一页
      const initialParams = { current: 1, pageSize: 10 }
      setPagination(initialParams)
      loadHistoryData(initialParams)
    }
  }, [visible])

  const formatTime = (timeStr) => {
    if (!timeStr || timeStr === '--') return '--'
    const d = dayjs(timeStr)
    if (!d.isValid()) return '--'
    return d.utc().format('YYYY-MM-DD HH:mm:ss') + ' (UTC)'
  }

  const formatNumber = (num) => {
    if (!num || num === '--' || num === '0') return num
    const number = Number(num)
    if (isNaN(number)) return num
    // 保留4位小数，去除末尾的0
    return number.toFixed(4).replace(/\.?0+$/, '')
  }

  // 定义表格列
  const columns = [
    {
      title: intl.get('buyback_history_card'),
      dataIndex: 'card',
      key: 'card',
    },
    {
      title: intl.get('buyback_history_user'),
      dataIndex: 'user',
      key: 'user',
      render: (text) => {
        if (text === '--') return '--'
        if (text.length > 20) {
          return (
            <Tooltip title={text}>
              <span>{text.substring(0, 6)}...{text.substring(text.length - 4)}</span>
            </Tooltip>
          )
        }
        return text
      },
    },
    {
      title: intl.get('buyback_history_amount'),
      dataIndex: 'amount',
      key: 'amount',
      render: (text) => formatNumber(text),
    },
    {
      title: intl.get('buyback_history_time'),
      dataIndex: 'time',
      key: 'time',
      render: (text) => formatTime(text),
    },
    {
      title: intl.get('buyback_history_price'),
      dataIndex: 'price',
      key: 'price',
      render: (text) => formatNumber(text),
    },
  ]

  return (
    <>
      {/* 打开弹框的按钮 */}
      <Button type="primary" onClick={openModal}>
        {intl.get('buyback_history_button')}
      </Button>

      {/* 历史回购弹框 */}
      <CommonModal
        open={visible}
        title={intl.get('buyback_history_title')}
        onCancel={closeModal}
        width={800}
        footer={null}
      >
        <Spin spinning={loading}>
          {historyData.length === 0 && !loading ? (
            <Empty description={intl.get('buyback_no_data')} />
          ) : (
            <>
              <Table
                columns={columns}
                dataSource={historyData}
                pagination={false}
                size="middle"
              />
             { pagination.total > 0 &&
               <div style={{ marginTop: 16, display: 'flex', justifyContent: 'flex-end' }}>
                <CommonPagination
                  current={pagination.current}
                  total={pagination.total}
                  pageSize={pagination.pageSize}
                  onChange={(page, pageSize) => {
                    loadHistoryData({ current: page, pageSize })
                  }}
                  // showSizeChanger={true}
                  // showQuickJumper={true}
                  showTotal={true}
                />
              </div>
             }
            </>
          )}
        </Spin>
      </CommonModal>
    </>
  )
}

export default HistoryBuybackModal
