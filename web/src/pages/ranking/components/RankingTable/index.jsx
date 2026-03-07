import React, { useContext } from 'react'
import { Tooltip } from 'antd'
import intl from '@/component/intl'
import RootContext from '@/pages/rootContext'
import { COLOR_THEME_KEY } from '@/pages/storageConfig'
import { getChangeColor, formatDecimal, formatChineseNumber } from '@/utils/format'
import RankingLineChart from '../RankingLineChart'
import PriceRangeBar from '../PriceRangeBar'
import { useNavigate } from 'react-router-dom'
import styles from './index.module.less'


const RankingTable = ({ data = [], type = 'hot' }) => {
    const { config } = useContext(RootContext)
    const colorTheme = config[COLOR_THEME_KEY]
    const navigate = useNavigate()

    const handleGoTrade = (instrumentID) => {
        const productGroup = "SPOT"
        navigate(`/exchange?ProductGroup=${productGroup}&InstrumentID=${instrumentID}`)
    }

    if (type === 'hold') {
        return (
            <div className={styles.tableContainer}>
                <div className={styles.tableHeaderHold}>
                    <div className={styles.rankCol}>{intl.get('ranking_table_rank')}</div>
                    <div className={styles.nameCol}>{intl.get('ranking_table_card')}</div>
                    <div className={styles.priceCol}>{intl.get('ranking_table_price')}</div>
                    <div className={styles.chartCol}>{intl.get('ranking_table_chart')}</div>
                    <div className={styles.changeCol}>{intl.get('ranking_table_max_hold_address')}</div>
                    <div className={styles.holdPctCol}>{intl.get('ranking_table_hold_pct')}</div>
                    <div className={styles.highLowCol}>{intl.get('ranking_table_max_hold_amount')}</div>
                    <div className={styles.volumeCol}>{intl.get('ranking_table_total_amount')}</div>
                </div>

                <div className={styles.tableBody}>
                    {data.map((item, index) => (
                        <div key={index} className={styles.tableRowHold} style={{cursor:'pointer'}} onClick={()=>handleGoTrade(item.symbol)}>
                            <div className={styles.rankCol}>{index + 1}</div>
                            <Tooltip title={item?.RE?.replace(/_/g, ' ') || ''}>
                                <div className={styles.nameCol}>
                                    <div className={styles.coinIcon}>
                                        {(item.currency || item.symbol || '').charAt(0)}
                                    </div>
                                    <div className={styles.symbolInfo}>
                                        <span className={styles.symbol}>{item.currency || item.symbol}</span>
                                        <span className={styles.pair}>/USDT</span>
                                    </div>
                                </div>
                            </Tooltip>
                            <div className={styles.priceCol}>
                                <div>
                                    {formatDecimal(item.price, 2)}
                                </div>
                                <div style={{ fontSize: '12px', color: 'var(--text-secondary)' }}>
                                    ≈ $ {formatDecimal(item.price, 2)}
                                </div>
                            </div>
                            <div className={styles.chartCol}>
                                <RankingLineChart
                                    symbol={item.symbol}
                                    change24h={item.change24h || 0}
                                    width={100}
                                    height={40}
                                    colorTheme={colorTheme}
                                />
                            </div>
                            <div className={styles.changeCol} title={item.maxHoldAccountID}>
                                {item.maxHoldAccountID ? `${item.maxHoldAccountID.substring(0, 6)}...${item.maxHoldAccountID.substring(item.maxHoldAccountID.length - 4)}` : '-'}
                            </div>
                            <div className={styles.holdPctCol}>
                                {item.maxHoldAmountPct ? `${parseFloat(item.maxHoldAmountPct).toFixed(2)}%` : '0.00%'}
                            </div>
                            <div className={styles.highLowCol}>
                                {formatChineseNumber(item.holdAmount, 2)}
                            </div>
                            <div className={styles.volumeCol}>
                                {formatChineseNumber(item.totalHoldAmount, 2)}
                            </div>
                        </div>
                    ))}
                </div>
            </div>
        )
    }

    return (
        <div className={styles.tableContainer}>
            <div className={styles.tableHeader}>
                <div className={styles.rankCol}>{intl.get('ranking_table_rank')}</div>
                <div className={styles.nameCol}>{intl.get('ranking_table_card')}</div>
                <div className={styles.priceCol}>{intl.get('ranking_table_price')}</div>
                <div className={styles.changeCol}>{intl.get('ranking_table_change')}</div>
                <div className={styles.chartCol}>{intl.get('ranking_table_chart')}</div>
                <div className={styles.highLowCol}>{intl.get('ranking_table_range')}</div>
                <div className={styles.volumeCol}>{intl.get('ranking_table_volume')}</div>
                <div className={styles.turnoverCol}>{intl.get('ranking_table_turnover')}</div>
            </div>

            <div className={styles.tableBody}>
                {data.map((item) => (
                    <div key={item.rank} className={styles.tableRow} style={{cursor:'pointer'}} onClick={()=>handleGoTrade(item.symbol)}>
                        <div className={styles.rankCol}>{item.rank}</div>
                        <Tooltip title={item?.RE?.replace(/_/g, ' ') || ''}>
                            <div className={styles.nameCol}>
                                <div className={styles.coinIcon}>
                                    {item.symbol.charAt(0)}
                                </div>
                                <div className={styles.symbolInfo}>
                                    <span className={styles.symbol}>{item.symbol}</span>
                                </div>
                            </div>
                        </Tooltip>
                        <div className={styles.priceCol}>
                            <div>
                                {formatDecimal(item.price, 2)}
                            </div>
                            <div style={{ fontSize: '12px', color: 'var(--text-secondary)' }}>
                                ≈ $ {formatDecimal(item.price, 2)}
                            </div>
                        </div>
                        <div
                            className={styles.changeCol}
                            style={{ color: getChangeColor(item.change24h, colorTheme) }}
                        >
                            {parseFloat(item.change24h) >= 0 ? '+' : ''}{item.change24h.toFixed(2)}%
                        </div>
                        <div className={styles.chartCol}>
                            <RankingLineChart
                                symbol={item.instrumentId}
                                change24h={item.change24h}
                                width={100}
                                height={40}
                                colorTheme={colorTheme}
                            />
                        </div>
                        <div className={styles.highLowCol}>
                            <PriceRangeBar
                                lowPrice={item.low24h}
                                highPrice={item.high24h}
                                currentPrice={item.price}
                            />
                        </div>
                        <div className={styles.volumeCol}>
                            {formatChineseNumber(item.volume24h, 2)}
                        </div>
                        <div className={styles.turnoverCol}>
                            {formatChineseNumber(item.turnover24h, 2)}
                        </div>
                    </div>
                ))}
            </div>
        </div>
    )
}

export default RankingTable
