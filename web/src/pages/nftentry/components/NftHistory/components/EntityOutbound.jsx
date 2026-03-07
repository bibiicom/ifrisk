import React, { useEffect, useState, useMemo } from "react";
import { Table, Empty, message } from "antd";
import dayjs from "dayjs";
import intl from '@/component/intl';
import { getOutboundApplyList } from "@/api/entityCard";
import { DEFAULT_PAGE_SIZE } from "../utils";

const EntityOutbound = () => {
    const STATUS_MAP = {
        0: intl.get('nft_history_status_pending'),
        1: intl.get('nft_history_status_approved'),
        2: intl.get('nft_history_status_rejected'),
    };
    const [loading, setLoading] = useState(false);
    const [list, setList] = useState([]);
    const [total, setTotal] = useState(0);
    const [page, setPage] = useState(1);

    const fetchList = async (currentPage = 1) => {
        setLoading(true);
        try {
            const res = await getOutboundApplyList({ page: currentPage, limit: DEFAULT_PAGE_SIZE });
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

    const columns = useMemo(
        () => [
            {
                title: intl.get('nft_outbound_date'),
                dataIndex: "createdAt",
                key: "createdAt",
                width: 180,
                render: (value) => (value ? dayjs(value).format("YYYY-MM-DD HH:mm") : "-"),
            },
            {
                title: intl.get('nft_outbound_recipient'),
                dataIndex: "Name",
                key: "Name",
                width: 120,
            },
            {
                title: intl.get('nft_outbound_phone'),
                dataIndex: "Phone",
                key: "Phone",
                width: 150,
            },
            {
                title: intl.get('nft_outbound_email'),
                dataIndex: "Email",
                key: "Email",
                width: 200,
            },
            {
                title: intl.get('nft_outbound_address'),
                dataIndex: "Address",
                key: "Address",
                ellipsis: true,
            },
            {
                title: intl.get('nft_outbound_postal_code'),
                dataIndex: "PostalCode",
                key: "PostalCode",
                width: 100,
            },
            {
                title: intl.get('nft_outbound_status'),
                dataIndex: "Status",
                key: "Status",
                width: 150,
                render: (value) => STATUS_MAP[value] || intl.get('nft_card_status_unknown'),
            },
        ],
        []
    );

    return (
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
    );
};

export default EntityOutbound;
