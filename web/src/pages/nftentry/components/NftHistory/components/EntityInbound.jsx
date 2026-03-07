import React, { useEffect, useState, useMemo } from "react";
import { Table, Button, Empty, message } from "antd";
import dayjs from "dayjs";
import intl from '@/component/intl';
import CommonModal from "@/component/commonModal";
import { getStorageApplyList } from "@/api/entityCard";
import { DEFAULT_PAGE_SIZE } from "../utils";
import styles from "./EntityInbound.module.less";
import { formatDecimals } from '@/utils/number'

const EntityInbound = () => {
    const STATUS_MAP = {
        0: intl.get('nft_history_status_pending'),
        1: intl.get('nft_history_status_approved'),
        2: intl.get('nft_history_status_rejected'),
    };

    const CARD_STATUS = {
        0: intl.get('nft_card_status_not_listed'),
        1: intl.get('nft_card_status_listing'),
        2: intl.get('nft_card_status_listed'),
        3: intl.get('nft_card_status_withdrawing'),
        4: intl.get('nft_card_status_outbounding'),
    };

    const getCardStatus = (card) => {
        if (card.Status === 4 && card.deletedAt) {
            return intl.get('nft_card_status_outbounded');
        }
        return CARD_STATUS[card.Status] || intl.get('nft_card_status_unknown');
    };

    const [loading, setLoading] = useState(false);
    const [list, setList] = useState([]);
    const [total, setTotal] = useState(0);
    const [page, setPage] = useState(1);
    const [modalVisible, setModalVisible] = useState(false);
    const [currentCard, setCurrentCard] = useState(null);

    const fetchList = async (currentPage = 1) => {
        setLoading(true);
        try {
            const res = await getStorageApplyList({ page: currentPage, limit: DEFAULT_PAGE_SIZE });
            if (res?.code === 0 && res?.data) {
                setList(res.data.list || []);
                setTotal(res.data.total || 0);
            } else {
                message.error(res?.msg || intl.get('nft_get_recorded_failed'));
            }
        } catch (e) {
            message.error(e?.message || intl.get('nft_get_recorded_failed'));
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        fetchList(page);
    }, [page]);

    const handleViewCard = (record) => {
        setCurrentCard(record.CardInfo);
        setModalVisible(true);
    };

    const columns = useMemo(
        () => [
            {
                title: intl.get('nft_history_date'),
                dataIndex: "createdAt",
                key: "createdAt",
                width: 180,
                render: (value) => (value ? dayjs(value).format("YYYY-MM-DD HH:mm") : "-"),
            },
            {
                title: intl.get('nft_history_email'),
                dataIndex: "Email",
                key: "Email",
            },
            {
                title: intl.get('nft_history_status'),
                dataIndex: "Status",
                key: "Status",
                width: 200,
                render: (value) => STATUS_MAP[value] || intl.get('nft_card_status_unknown'),
            },
            {
                title: intl.get('nft_history_view'),
                key: "action",
                width: 120,
                align: "center",
                render: (_, record) => {
                    if (record.Status === 1 && record.CardInfo) {
                        return (
                            <Button type="link" onClick={() => handleViewCard(record)}>
                                {intl.get('nft_history_card_info')}
                            </Button>
                        );
                    }
                    return "-";
                },
            },
        ],
        []
    );

    return (
        <>
            <Table
                rowKey="Id"
                columns={columns}
                dataSource={list}
                loading={loading}
                pagination={{
                    current: page,
                    pageSize: DEFAULT_PAGE_SIZE,
                    total,
                    showSizeChanger: false,
                    onChange: (p) => setPage(p),
                }}
                locale={{ emptyText: <Empty description={intl.get('nft_history_no_record')} /> }}
            />
            <CommonModal
                title={intl.get('nft_card_detail_title')}
                open={modalVisible}
                onCancel={() => setModalVisible(false)}
                footer={null}
                width={400}
            >
                {currentCard && (
                    <div className={styles.cardInfo}>
                        <div className={styles.row}>
                            <span className={styles.label}>{intl.get('nft_card_detail_name')}</span>
                            <span className={styles.value}>{currentCard.Name || "-"}</span>
                        </div>
                        <div className={styles.row}>
                            <span className={styles.label}>{intl.get('nft_card_detail_valuation')}</span>
                            <span className={styles.value}>{formatDecimals(currentCard.Valuation) || "-"}</span>
                        </div>

                        <div className={styles.row}>
                            <span className={styles.label}>{intl.get('nft_card_detail_status')}</span>
                            <span className={styles.value}>
                                {getCardStatus(currentCard)}
                            </span>
                        </div>

                        <div className={styles.row}>
                            <span className={styles.label}>{intl.get('nft_card_detail_inbound_time')}</span>
                            <span className={styles.value}>
                                {currentCard.createdAt ? dayjs(currentCard.createdAt).format("YYYY-MM-DD HH:mm") : "-"}
                            </span>
                        </div>
                    </div>
                )}
            </CommonModal>
        </>
    );
};

export default EntityInbound;
