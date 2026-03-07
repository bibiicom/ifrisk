import React, { useState, useEffect } from 'react';
import { Table, Select, Form, Button, Space, Typography,  } from 'antd';
import { SearchOutlined } from '@ant-design/icons';
import dayjs from 'dayjs';
import utc from 'dayjs/plugin/utc';
import { getFinishOrderList } from '@/api/order';
import { formatDecimals } from '@/utils/number';
import authHOC from '../common/hoc/authHOC';
import intl from '@/component/intl';

dayjs.extend(utc);

const { Title } = Typography;
const { Option } = Select;

const HistoryOrders2 = () => {
  const [form] = Form.useForm();
  const [data, setData] = useState([]);
  const [loading, setLoading] = useState(false);
  const [pagination, setPagination] = useState({
    current: 1,
    pageSize: 10,
    total: 0,
  });

  const fetchData = async (page = 1, pageSize = 10, filters = {}) => {
    setLoading(true);
    try {
      const response = await getFinishOrderList({
        page,
        limit: pageSize,
        ...filters,
      });
      if (response.code === 0) {
        setData(response.data.list);
        setPagination(prev => ({
          ...prev,
          current: page,
          pageSize,
          total: response.data.total,
        }));
      }
    } catch (error) {
      console.error('Failed to fetch order history:', error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchData();
  }, []);

  const handleSearch = (values) => {
    fetchData(1, pagination.pageSize, values);
  };

  const handleReset = () => {
    form.resetFields();
    fetchData(1, pagination.pageSize, {});
  };

  const handleTableChange = (pagination) => {
    fetchData(pagination.current, pagination.pageSize, form.getFieldsValue());
  };

  const columns = [
    {
      title: intl.get('history_orders_instrument_id'),
      dataIndex: 'InstrumentID',
      key: 'InstrumentID',
      width: 180,
      ellipsis: true,
    },
    {
      title: intl.get('history_orders_insert_time'),
      dataIndex: 'InsertTime',
      width: 250,
      key: 'InsertTime',
      render: (text) => dayjs(Number(text)).utc().format('YYYY-MM-DD HH:mm:ss') + ' (UTC)',
    },
    {
      title: intl.get('history_orders_direction'),
      dataIndex: 'Direction',
      width: 100,
      key: 'Direction',
      render: (text) => (text === '0' ? intl.get('history_orders_direction_buy') : intl.get('history_orders_direction_sell')),
    },
    {
      title: intl.get('history_orders_volume'),
      dataIndex: 'Volume',
      key: 'Volume',
      render: (text) => formatDecimals(text, 3),
    },
    {
      title: intl.get('history_orders_price'),
      dataIndex: 'Price',
      key: 'Price',
      render: (text) => formatDecimals(text, 3),
    },
    {
      title: intl.get('history_orders_volume_traded'),
      dataIndex: 'VolumeTraded',
      key: 'VolumeTraded',
      render: (text) => formatDecimals(text, 3),
    },
    {
      title: intl.get('history_orders_trade_price'),
      dataIndex: 'TradePrice',
      key: 'TradePrice',
      render: (text) =>text? formatDecimals(text, 3):'-',
    },
    {
      title: intl.get('history_orders_close_profit'),
      dataIndex: 'CloseProfit',
      key: 'CloseProfit',
      render: (text) => formatDecimals(text, 3),
    },
    {
      title: intl.get('history_orders_fee'),
      dataIndex: 'Fee',
      key: 'Fee',
      render: (text) => formatDecimals(text, 3),
    },
  ];

  return (
    <div style={{ maxWidth: '1400px', margin: '0 auto', paddingTop: 24, paddingBottom: 24 }}>

      <Title level={2} style={{ marginBottom: 24 }}>{intl.get('history_orders_title')}</Title>
      <Form
        form={form}
        layout="inline"
        onFinish={handleSearch}
        style={{ marginBottom: 24 }}
      >
        {/* <Form.Item name="tradeType" label="交易类型">
          <Select placeholder="请选择交易类型" style={{ width: 200 }}>
            <Option value="0">限价单</Option>
            <Option value="1">市价单</Option>
          </Select>
        </Form.Item> */}

        <Form.Item name="direction" label={intl.get('history_orders_direction_label')}>
          <Select placeholder={intl.get('history_orders_direction_placeholder')} style={{ width: 200 }} allowClear>
            <Option value="0">{intl.get('history_orders_direction_buy')}</Option>
            <Option value="1">{intl.get('history_orders_direction_sell')}</Option>
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
        rowKey="OrderID"
        pagination={pagination}
        onChange={handleTableChange}
        bordered
      />

    </div>
  );
};

export default authHOC(true)(HistoryOrders2) ;
