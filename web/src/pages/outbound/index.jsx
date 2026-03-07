import React, { useState, useContext, useEffect, useCallback } from 'react'
import { useNavigate } from 'react-router-dom'
import { Form, Input, Pagination, Spin, App, Button } from 'antd'
import { QuestionCircleOutlined } from '@ant-design/icons'
import intl from '../../component/intl'
import CommonModal from '../../component/commonModal'
import CommonCard from '../../component/commonCard'
import EmptyState from '@/component/emptyState'
import styles from './Outbound.module.less'
import authHOC from '../common/hoc/authHOC'
import RootContext from '../rootContext'
import { Status } from '../../constants/auth'
import defaultCardImage from '../../assets/images/common/default-card.jpeg'
import { getEntityCardList, outboundApply } from '../../api/entityCard'
import { NFT_ENTRY } from '../routes'

const PAGE_SIZE = 10

const Outbound = () => {
    const { message } = App.useApp()
    const { authStatus } = useContext(RootContext)
    const navigate = useNavigate()
    const [ruleModalOpen, setRuleModalOpen] = useState(false)
    const [outboundModalOpen, setOutboundModalOpen] = useState(false)
    const [selectedCard, setSelectedCard] = useState(null)
    const [isSubmitting, setIsSubmitting] = useState(false)
    const [currentPage, setCurrentPage] = useState(1)
    const [cards, setCards] = useState([])
    const [total, setTotal] = useState(0)
    const [loading, setLoading] = useState(false)
    const [form] = Form.useForm()

    // 获取实体卡片列表
    const fetchCards = useCallback(async (page = 1) => {
        setLoading(true)
        try {
            const result = await getEntityCardList({ page, limit: PAGE_SIZE, category: 2 })
            setCards(result.data?.cardList || [])
            setTotal(result.data?.total || 0)
        } catch (e) {
            console.error('获取卡片列表失败:', e)
            setCards([])
            setTotal(0)
        } finally {
            setLoading(false)
        }
    }, [])

    // 跳转到出库记录
    const handleViewHistory = () => {
        navigate(`${NFT_ENTRY}?tab=history&historyTab=entityOutbound`)
    }

    // 登录后加载数据
    useEffect(() => {
        if (authStatus === Status.LOGIN) {
            fetchCards(currentPage)
        }
    }, [authStatus, currentPage, fetchCards])

    // 页码变化
    const handlePageChange = (page) => {
        setCurrentPage(page)
    }

    // 未登录状态
    if (authStatus !== Status.LOGIN) {
        return <EmptyState />
    }

    // 判断卡片是否可出库 (Status === 0 表示未上架，可以出库)
    const canOutbound = (card) => {
        return card.Status === 1
    }

    // 获取状态文字
    const getStatusText = (status) => {
        switch (status) {          
            case 1: return intl.get('outbound_card_status_not_listed')
            case 2: return intl.get('outbound_card_status_listed')
            case 3: return intl.get('outbound_card_status_withdrawing')
            case 4: return intl.get('outbound_card_status_outbounding')
            default: return intl.get('outbound_card_status_unknown')
        }
    }

    // 打开出库弹框
    const handleOpenOutbound = (card) => {
        setSelectedCard(card)
        setOutboundModalOpen(true)
    }

    // 关闭出库弹框
    const handleCloseOutbound = () => {
        setOutboundModalOpen(false)
        setSelectedCard(null)
        form.resetFields()
    }

    // 提交出库申请
    const handleSubmitOutbound = async (values) => {
        if (isSubmitting) return
        setIsSubmitting(true)
        try {
            await outboundApply({
                cardID: selectedCard?.Id,
                name: values.name,
                phone: values.phone,
                email: values.email,
                postalCode: values.postalCode,
                address: values.address
            })
            message.success('出库申请成功')
            handleCloseOutbound()
            fetchCards(currentPage)
        } catch (e) {
            message.error(e?.errorMsg || e?.message || '出库申请失败')
        } finally {
            setIsSubmitting(false)
        }
    }

    const modalStyles = CommonModal.styles
    const cardStyles = CommonCard.styles

    // 渲染卡片内容
    const renderCardContent = (card) => {
        const isAvailable = canOutbound(card)
        return (
            <>
                <div className={styles.cardName}>{card.Name}</div>
                <div className={`${styles.cardStatus} ${isAvailable ? styles.available : styles.unavailable}`}>
                    {isAvailable ? intl.get('outbound_status_available') : intl.get('outbound_status_unavailable')}
                </div>
                <div className={styles.cardMeta}>
                    <div className={styles.cardMetaItem}>
                        <span className={styles.label}>{intl.get('status')}</span>
                        <span className={styles.value}>{getStatusText(card.Status)}</span>
                    </div>
                </div>
            </>
        )
    }

    // 渲染卡片底部按钮
    const renderCardFooter = (card) => {
        const isAvailable = canOutbound(card)
        return (
            <button
                className={cardStyles.cardBtn}
                disabled={!isAvailable}
                onClick={() => handleOpenOutbound(card)}
            >
                {intl.get('outbound_apply')}
            </button>
        )
    }

    return (
        <div className={styles.container}>
            <div className={styles.content}>
                {/* 头部 */}
                <div className={styles.header}>
                    <h1 className={styles.title}>{intl.get('outbound_title')}</h1>
                    <div className={styles.headerButtons}>
                        <Button
                            type="link"
                            onClick={handleViewHistory}
                        >
                            {intl.get('view_outbound_history')}
                        </Button>
                        <button className={styles.ruleBtn} onClick={() => setRuleModalOpen(true)}>
                            <QuestionCircleOutlined />
                            {intl.get('outbound_rules')}
                        </button>
                    </div>
                </div>

                {/* 卡片列表 */}
                {loading ? (
                    <div className={styles.loadingState}>
                        <Spin size="large" />
                    </div>
                ) : cards.length === 0 ? (
                    <div className={styles.emptyState}>
                        {intl.get('outbound_no_cards')}
                    </div>
                ) : (
                    <>
                        <div className={styles.cardList}>
                            {cards.map(card => (
                                <CommonCard
                                    key={card.Id}
                                    image={card.Img || defaultCardImage}
                                    imageAlt={card.Name}
                                    content={renderCardContent(card)}
                                    footer={renderCardFooter(card)}
                                />
                            ))}
                        </div>
                        {total > PAGE_SIZE && (
                            <div className={styles.pagination}>
                                <Pagination
                                    current={currentPage}
                                    total={total}
                                    pageSize={PAGE_SIZE}
                                    onChange={handlePageChange}
                                    showSizeChanger={false}
                                    showQuickJumper
                                    showTotal={(total) => `${intl.get('total')} ${total} ${intl.get('items')}`}
                                />
                            </div>
                        )}
                    </>
                )}

                {/* 规则弹框 */}
                <CommonModal
                    title={intl.get('outbound_rules_title')}
                    open={ruleModalOpen}
                    onCancel={() => setRuleModalOpen(false)}
                    footer={null}
                    width={600}
                >
                    <div className={styles.ruleContent}>
                        <h3>{intl.get('outbound_rule_condition_title')}</h3>
                        <ul>
                            <li>{intl.get('outbound_rule_condition_1')}</li>
                            <li>{intl.get('outbound_rule_condition_2')}</li>
                            <li>{intl.get('outbound_rule_condition_3')}</li>
                        </ul>
                        <h3>{intl.get('outbound_rule_process_title')}</h3>
                        <ul>
                            <li>{intl.get('outbound_rule_process_1')}</li>
                            <li>{intl.get('outbound_rule_process_2')}</li>
                            <li>{intl.get('outbound_rule_process_3')}</li>
                            <li>{intl.get('outbound_rule_process_4')}</li>
                        </ul>
                        <h3>{intl.get('outbound_rule_notice_title')}</h3>
                        <ul>
                            <li>{intl.get('outbound_rule_notice_1')}</li>
                            <li>{intl.get('outbound_rule_notice_2')}</li>
                        </ul>
                    </div>
                </CommonModal>

                {/* 出库表单弹框 */}
                <CommonModal
                    title={intl.get('outbound_form_title')}
                    open={outboundModalOpen}
                    onCancel={handleCloseOutbound}
                    footer={null}
                    width={500}
                >
                    <Form
                        form={form}
                        layout="vertical"
                        onFinish={handleSubmitOutbound}
                    >
                        <Form.Item
                            name="name"
                            label={intl.get('outbound_form_recipient')}
                            rules={[{ required: true, message: intl.get('outbound_form_recipient_required') }]}
                        >
                            <Input placeholder={intl.get('outbound_form_recipient_placeholder')} />
                        </Form.Item>
                        <Form.Item
                            name="phone"
                            label={intl.get('outbound_form_phone')}
                            rules={[{ required: true, message: intl.get('outbound_form_phone_required') }]}
                        >
                            <Input placeholder={intl.get('outbound_form_phone_placeholder')} />
                        </Form.Item>
                        <Form.Item
                            name="email"
                            label={intl.get('outbound_form_email')}
                            rules={[
                                { required: true, message: intl.get('outbound_form_email_required') },
                                { type: 'email', message: intl.get('outbound_form_email_invalid') }
                            ]}
                        >
                            <Input placeholder={intl.get('outbound_form_email_placeholder')} />
                        </Form.Item>
                        <Form.Item
                            name="postalCode"
                            label={intl.get('outbound_form_postal_code')}
                            rules={[{ required: true, message: intl.get('outbound_form_postal_code_required') }]}
                        >
                            <Input placeholder={intl.get('outbound_form_postal_code_placeholder')} />
                        </Form.Item>
                        <Form.Item
                            name="address"
                            label={intl.get('outbound_form_address')}
                            rules={[{ required: true, message: intl.get('outbound_form_address_required') }]}
                        >
                            <Input.TextArea
                                rows={3}
                                placeholder={intl.get('outbound_form_address_placeholder')}
                            />
                        </Form.Item>
                        <div className={modalStyles.footer}>
                            <button type="button" className={modalStyles.cancelBtn} onClick={handleCloseOutbound}>
                                {intl.get('cancel')}
                            </button>
                            <button type="submit" className={modalStyles.confirmBtn} disabled={isSubmitting}>
                                {isSubmitting ? intl.get('submitting') : intl.get('submit')}
                            </button>
                        </div>
                    </Form>
                </CommonModal>
            </div>
        </div>
    )
}

export default authHOC(true)(Outbound)
