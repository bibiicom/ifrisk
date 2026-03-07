import React, { useState, useCallback } from 'react'
import { Tabs, Table } from 'antd'
import intl from '@/component/intl'
import { getDepositHistoryCsv, getWithdrawHistoryCsv } from '../../../../api/account'
import { parseCsvToRecords } from '../../../../utils/csvParser'
import CommonModal from '../../../../component/commonModal'
import styles from './index.module.less'

const PAGE_SIZE = 10

const DepositWithdrawModal = ({ visible, onCancel, address }) => {
    const [activeTab, setActiveTab] = useState('deposit')
    const [depositData, setDepositData] = useState([])
    const [withdrawData, setWithdrawData] = useState([])
    const [loading, setLoading] = useState(false)

    const fetchData = useCallback(async (type) => {
        if (!address) return
        setLoading(true)
        try {
            const fetchFn = type === 'deposit' ? getDepositHistoryCsv : getWithdrawHistoryCsv
            const csv = await fetchFn(address)
            const { records } = parseCsvToRecords(csv)
            const dataWithKeys = records.map((item, index) => ({ ...item, key: index }))
            if (type === 'deposit') {
                setDepositData(dataWithKeys)
            } else {
                setWithdrawData(dataWithKeys)
            }
        } catch (error) {
            console.error(`Failed to fetch ${type} history:`, error)
        } finally {
            setLoading(false)
        }
    }, [address])

    // 每次切换tab都重新请求数据
    const handleTabChange = useCallback((key) => {
        setActiveTab(key)
        fetchData(key)
    }, [fetchData])

    // Modal打开时请求当前tab数据，关闭时清空数据
    const handleAfterOpenChange = useCallback((open) => {
        if (open) {
            fetchData(activeTab)
        } else {
            setDepositData([])
            setWithdrawData([])
        }
    }, [activeTab, fetchData])

    const getExplorerUrl = (hash) => {
        if (!hash) return '#'
        return `https://arbiscan.io/tx/${hash}`
    }

    const columns = [
        {
            title: intl.get('wallet_assets_modal_time'),
            dataIndex: 'timestamp',
            key: 'timestamp',
            width: 180,
        },
        {
            title: intl.get('wallet_assets_modal_amount'),
            dataIndex: 'amount',
            key: 'amount',
            width: 120,
        },
        {
            title: intl.get('wallet_assets_modal_currency'),
            dataIndex: 'currency',
            key: 'currency',
            width: 100,
        },
        {
            title: intl.get('wallet_assets_modal_tx_hash'),
            dataIndex: 'txHash',
            key: 'txHash',
            ellipsis: true,
            render: (hash) => (
                <a
                    href={getExplorerUrl(hash)}
                    target="_blank"
                    rel="noopener noreferrer"
                    className={styles.hashLink}
                >
                    {hash}
                </a>
            ),
        },
    ]

    const tabItems = [
        {
            key: 'deposit',
            label: intl.get('wallet_assets_modal_deposit_tab'),
            children: (
                <Table
                    columns={columns}
                    dataSource={depositData}
                    loading={loading && activeTab === 'deposit'}
                    pagination={depositData.length > PAGE_SIZE ? { pageSize: PAGE_SIZE, showSizeChanger: false } : false}
                    size="small"
                />
            ),
        },
        {
            key: 'withdraw',
            label: intl.get('wallet_assets_modal_withdraw_tab'),
            children: (
                <Table
                    columns={columns}
                    dataSource={withdrawData}
                    loading={loading && activeTab === 'withdraw'}
                    pagination={withdrawData.length > PAGE_SIZE ? { pageSize: PAGE_SIZE, showSizeChanger: false } : false}
                    size="small"
                />
            ),
        },
    ]

    return (
        <CommonModal
            title={intl.get('wallet_assets_history')}
            open={visible}
            onCancel={onCancel}
            afterOpenChange={handleAfterOpenChange}
            footer={null}
            width={700}
            className={styles.modal}
        >
            <Tabs
                activeKey={activeTab}
                onChange={handleTabChange}
                items={tabItems}
            />
        </CommonModal>
    )
}

export default DepositWithdrawModal
