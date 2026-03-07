import React, { useState, useEffect, useMemo, useContext } from 'react'
import { App } from 'antd'
import Decimal from 'decimal.js'
import intl from '@/component/intl'
import { launchpadSubscription } from '@/api/launchpad'
import { useAccountAssets } from '@/component/hooks'
import RootContext from '@/pages/rootContext'
import { formatSignificantDigits } from '@/utils/format'
import styles from './index.module.less'

const PurchaseModal = ({ isOpen, onClose, onSuccess, detail }) => {
    const { message } = App.useApp()
    const [quantity, setQuantity] = useState('') // 改为数量
    const [isConfirming, setIsConfirming] = useState(false)
    const { userInfo } = useContext(RootContext)
    const { assets, run: fetchAssets } = useAccountAssets()

    const { launchpadInfo } = detail || {}
    const pricePerShare = new Decimal(launchpadInfo?.Price || 0)

    // 获取最小和最大申购金额
    const minPrice = Number(launchpadInfo?.MinPrice || 0)
    const maxPrice = Number(launchpadInfo?.MaxPrice || 0)

    // 获取用户已申购金额
    const totalPrice = Number(detail?.totalPrice || 0)

    // 获取 USDC 可用余额
    const usdcBalance = useMemo(() => {
        if (!assets || !Array.isArray(assets)) return 0
        const usdc = assets.find((item) => item.Currency === 'USDC')
        return usdc ? Number(usdc.Available || 0) : 0
    }, [assets])

    // 计算预计金额 = 输入数量 × 单价
    const estimatedAmount = useMemo(() => {
        if (!quantity || pricePerShare.lte(0)) return 0
        try {
            const qty = new Decimal(quantity)
            return qty.mul(pricePerShare).toNumber()
        } catch {
            return 0
        }
    }, [quantity, pricePerShare])

    const quantityNum = parseFloat(quantity) || 0
    const isBalanceLow = estimatedAmount > usdcBalance

    // 计算申购后的总金额
    const newTotalPrice = totalPrice + estimatedAmount

    // 验证金额是否在范围内
    const isBelowMin = minPrice > 0 && newTotalPrice < minPrice
    const isAboveMax = maxPrice > 0 && newTotalPrice > maxPrice

    // 获取用户资产
    useEffect(() => {
        if (isOpen && userInfo?.MemberID) {
            fetchAssets({ AccountID: userInfo.MemberID })
        }
    }, [isOpen, userInfo?.MemberID, fetchAssets])

    // 重置表单
    useEffect(() => {
        if (!isOpen) {
            setQuantity('')
        }
    }, [isOpen])

    if (!isOpen) return null

    const handleInputChange = (e) => {
        const value = e.target.value
        // 只允许输入整数,不允许小数点
        if (value === '' || /^\d+$/.test(value)) {
            setQuantity(value)
        }
    }

    const handleQuickAdd = (val) => {
        const current = parseFloat(quantity) || 0
        const newQuantity = current + val
        // 计算对应的金额,不超过可用余额
        const newAmount = new Decimal(newQuantity).mul(pricePerShare).toNumber()
        if (newAmount <= usdcBalance) {
            setQuantity(String(newQuantity))
        } else {
            // 计算最大可购买数量
            const maxQuantity = new Decimal(usdcBalance).div(pricePerShare).toNumber()
            setQuantity(String(Math.floor(maxQuantity)))
        }
    }

    const handleConfirm = async () => {
        if (isBalanceLow || quantityNum <= 0 || isBelowMin || isAboveMax) return

        // 检查申购是否已结束
        const closeTime = launchpadInfo?.CloseTime
        if (closeTime) {
            const now = Date.now()
            const closeTimestamp = new Date(closeTime).getTime()
            if (now > closeTimestamp) {
                message.error(intl.get('launchpad2_subscription_ended'))
                return
            }
        }

        setIsConfirming(true)
        try {
            const res = await launchpadSubscription({
                launchpadID: launchpadInfo?.Id,
                volume: String(quantityNum), // 提交数量
            })

            if (res.code === 0) {
                onSuccess({ shares: quantityNum, amount: estimatedAmount })
            } else {
                message.error(res.msg || intl.get('launchpad2_subscription_failed'))
            }
        } catch (err) {
            message.error(err?.msg || intl.get('launchpad2_subscription_failed'))
        } finally {
            setIsConfirming(false)
        }
    }

    return (
        <div className={styles.overlay}>
            <div className={styles.backdrop} onClick={isConfirming ? undefined : onClose} />

            <div className={styles.modal}>
                {isConfirming ? (
                    <div className={styles.processing}>
                        <div className={styles.spinner}>
                            <div className={styles.spinnerTrack}></div>
                            <div className={styles.spinnerFill}></div>
                        </div>
                        <div className={styles.processingText}>
                            <h3>{intl.get('launchpad2_processing')}</h3>
                            <p>{intl.get('launchpad2_verifying')}</p>
                        </div>
                    </div>
                ) : (
                    <>
                        <div className={styles.header}>
                            <h2>{intl.get('launchpad2_subscribe')}</h2>
                            <button className={styles.closeBtn} onClick={onClose}>
                                <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor">
                                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2.5" d="M6 18L18 6M6 6l12 12" />
                                </svg>
                            </button>
                        </div>

                        <div className={styles.body}>
                            <div className={styles.inputSection}>
                                <div className={styles.amountInput}>
                                    <span className={styles.currency}>#</span>
                                    <input
                                        type="text"
                                        value={quantity}
                                        onChange={handleInputChange}
                                        placeholder="0"
                                        autoFocus
                                    />
                                </div>
                                <p className={styles.inputLabel}>{intl.get('launchpad2_subscription_quantity')}</p>
                            </div>

                            <div className={styles.quickBtns}>
                                {[100, 1000, 5000].map(val => (
                                    <button
                                        key={val}
                                        className={styles.quickBtn}
                                        onClick={() => handleQuickAdd(val)}
                                    >
                                        +{val.toLocaleString()}
                                    </button>
                                ))}
                            </div>

                            <div className={styles.info}>
                                <InfoRow
                                    label={intl.get('launchpad2_price_per_share')}
                                    value={`${formatSignificantDigits(pricePerShare.toNumber())}`}
                                />
                                <InfoRow
                                    label={intl.get('launchpad2_wallet_balance')}
                                    value={`${usdcBalance.toLocaleString(undefined, { minimumFractionDigits: 2 })}`}
                                    warning={isBalanceLow}
                                />
                                <InfoRow
                                    label={intl.get('launchpad2_estimated_amount')}
                                    value={`$${estimatedAmount.toLocaleString(undefined, { minimumFractionDigits: 2, maximumFractionDigits: 2 })}`}
                                    highlight
                                />
                            </div>

                            <button
                                className={styles.confirmBtn}
                                disabled={isBalanceLow || quantityNum <= 0 || isBelowMin || isAboveMax}
                                onClick={handleConfirm}
                            >
                                {isBalanceLow
                                    ? intl.get('launchpad2_insufficient_funds')
                                    : isBelowMin
                                        ? intl.get('launchpad2_below_min_amount')
                                        : isAboveMax
                                            ? intl.get('launchpad2_above_max_amount')
                                            : quantityNum <= 0
                                                ? intl.get('launchpad2_enter_quantity')
                                                : intl.get('launchpad2_confirm')}
                            </button>
                        </div>
                    </>
                )}
            </div>
        </div>
    )
}

const InfoRow = ({ label, value, warning, highlight }) => (
    <div className={styles.infoRow}>
        <span className={styles.infoLabel}>{label}</span>
        <span className={`${styles.infoValue} ${warning ? styles.warning : ''} ${highlight ? styles.highlight : ''}`}>
            {value}
        </span>
    </div>
)

export default PurchaseModal
