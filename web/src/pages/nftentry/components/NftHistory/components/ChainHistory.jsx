import React, { useContext, useEffect, useMemo, useState } from "react";
import { Empty, Table, message } from "antd";
import dayjs from "dayjs";
import intl from '@/component/intl';
import { getNftdepositHistoryCsv, getNftWithdrawHistoryCsv } from "@/api/nft";
import RootContext from "@/pages/rootContext";
import { DEFAULT_PAGE_SIZE, shorten, safeToNumber, parseHistoryCsv } from "../utils";

const ChainHistory = ({ type = "deposit" }) => {
    const [loading, setLoading] = useState(false);
    const [rows, setRows] = useState([]);
    const [page, setPage] = useState(1);

    const { userInfo } = useContext(RootContext);
    const userId = String(userInfo?.UserID || "").trim();

    useEffect(() => {
        setRows([]);
        setPage(1);
    }, [userId]);

    useEffect(() => {
        let cancelled = false;

        const run = async () => {
            if (!userId) {
                setLoading(false);
                return;
            }
            setRows([]);
            setLoading(true);

            try {
                const text = type === "deposit"
                    ? await getNftdepositHistoryCsv(userId)
                    : await getNftWithdrawHistoryCsv(userId);
                const next = text ? parseHistoryCsv(text) : [];
                if (!cancelled) setRows(next);
            } catch (e) {
                if (!cancelled) message.error(e?.message || intl.get('nft_history_get_failed'));
            } finally {
                if (!cancelled) setLoading(false);
            }
        };

        run();
        return () => { cancelled = true; };
    }, [userId, type]);

    const columns = useMemo(
        () => [
            {
                title: intl.get('nft_history_time'),
                dataIndex: "timestamp",
                key: "timestamp",
                width: 180,
                render: (value) => {
                    const ts = safeToNumber(value);
                    if (!ts) return "-";
                    const ms = ts > 10_000_000_000 ? ts : ts * 1000;
                    return dayjs(ms).format("MM-DD HH:mm");
                },
            },
            {
                title: intl.get('nft_history_contract'),
                dataIndex: "contract",
                key: "contract",
                render: (value) => <span title={value}>{shorten(value)}</span>,
            },
            {
                title: intl.get('nft_history_token_id'),
                dataIndex: "tokenId",
                key: "tokenId",
                width: 160,
            },
            {
                title: intl.get('nft_history_tx_hash'),
                dataIndex: "txHash",
                key: "txHash",
                render: (value) => {
                    const tx = String(value || "");
                    if (!tx) return "-";
                    return (
                        <a href={`https://arbiscan.io/tx/${tx}`} target="_blank" rel="noreferrer">
                            {shorten(tx, 10, 6)}
                        </a>
                    );
                },
            },
        ],
        []
    );

    return (
        <Table
            rowKey="key"
            columns={columns}
            dataSource={rows}
            loading={loading}
            pagination={{
                current: page,
                pageSize: DEFAULT_PAGE_SIZE,
                total: rows.length,
                showSizeChanger: false,
                onChange: (p) => setPage(p),
            }}
            locale={{ emptyText: <Empty description={intl.get('nft_history_no_record')} /> }}
        />
    );
};

export default ChainHistory;
