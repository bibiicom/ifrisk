import React, { useState, useEffect } from 'react'
import { Table, Select, Form, Button, Space, Typography, message } from 'antd'
import { SearchOutlined } from '@ant-design/icons'
import dayjs from 'dayjs'
import utc from 'dayjs/plugin/utc'
import { formatDecimals } from '@/utils/number'
import authHOC from '../common/hoc/authHOC'
import intl from '@/component/intl'
import { FlowType } from '@/constants/financialRecord'
import { getAccountDetailList } from '@/api/account'

dayjs.extend(utc)

const { Title } = Typography
const { Option } = Select

const FinancialRecord2 = () => {
  const [form] = Form.useForm()
  const [data, setData] = useState([])
  const [loading, setLoading] = useState(false)
  const [pagination, setPagination] = useState({
    current: 1,
    pageSize: 10,
    total: 0,
  })

  const handleSearch = (values) => {
    fetchData({ ...values, page: 1 })
  }

  const handleReset = () => {
    form.resetFields()
    fetchData({ page: 1 })
  }

  const handleTableChange = (newPagination) => {
    const formValues = form.getFieldsValue()
    fetchData({
      ...formValues,
      page: newPagination.current,
    })
  }

  const fetchData = async (params = {}) => {
    setLoading(true)
    try {
      const requestParams = {
        source: params.flowType || '',
        page: params.page || pagination.current,
        limit: pagination.pageSize,
      }

      const response = await getAccountDetailList(requestParams)

      if (response.code === 0) {
        setData(response.data.list || [])
        setPagination((prev) => ({
          ...prev,
          current: requestParams.page,
          total: response.data.total || 0,
        }))
      } else {
        message.error(response.msg || intl.get('fetch_data_failed'))
      }
    } catch (error) {
      message.error(intl.get('fetch_data_failed'))
    } finally {
      setLoading(false)
    }
  }

  useEffect(() => {
    fetchData()
  }, [])

  const columns = [
    {
      title: intl.get('trade_variety'),
      dataIndex: 'InstrumentID',
      key: 'InstrumentID',
      width: 140,
      ellipsis: true,
        render: (text) =>text || '-',
    },
    {
      title: intl.get('time'),
      dataIndex: 'InsertTime',
      key: 'InsertTime',
      width: 250,
      render: (text) => dayjs(Number(text)).utc().format('YYYY-MM-DD HH:mm:ss') + ' (UTC)',
    },
    {
      title: intl.get('flow_type'),
      dataIndex: 'Source',
      key: 'Source',
      width: 280,
      render: (text) => FlowType.transfer(text),
    },
    {
      title: intl.get('amount'),
      dataIndex: 'Amount',
      key: 'Amount',
      width: 300,
      render: (text) => formatDecimals(text, 8),
    },
    {
      title: intl.get('total_amount'),
      dataIndex: 'Balance',
      key: 'Balance',
      render: (text) => formatDecimals(text, 8),
    },
  ]

  const flowTypeOptions = FlowType.createSwapContractOptions()

  return (
    <div style={{ maxWidth: '1400px', margin: '0 auto', paddingTop: 24, paddingBottom: 24 }}>
      <Title level={2} style={{ marginBottom: 24 }}>
        {intl.get('financial_record')}
      </Title>

      <Form
        form={form}
        layout="inline"
        onFinish={handleSearch}
        style={{ marginBottom: 24 }}
      >
        <Form.Item name="flowType" label={intl.get('flow_type')}>
          <Select
            placeholder={intl.get('flow_type_placeholder')}
            style={{ width: 200 }}
            allowClear
          >
            {flowTypeOptions.map(option => (
              <Option key={option.key} value={option.key}>
                {option.text}
              </Option>
            ))}
          </Select>
        </Form.Item>

        <Form.Item>
          <Space>
            <Button type="primary" htmlType="submit" icon={<SearchOutlined />}>
              {intl.get('history_orders_search')}
            </Button>
            <Button onClick={handleReset}>
              {intl.get('history_orders_reset')}
            </Button>
          </Space>
        </Form.Item>
      </Form>

      <Table
        columns={columns}
        dataSource={data}
        loading={loading}
        rowKey={(record) => record.AccountDetailID}
        pagination={pagination}
        onChange={handleTableChange}
        bordered
      />
    </div>
  )
}

export default authHOC(true)(FinancialRecord2)
