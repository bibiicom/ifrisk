import React, { useState, useEffect } from "react";
import { App, Spin, Empty } from "antd";
import { SyncOutlined } from "@ant-design/icons";
import intl from '@/component/intl';
import { getRecordedNftList } from "../../../../api/nft";
import RecordedCard from "./RecordedCard";
import CommonPagination from "../../../../component/commonPagination";
import styles from "./index.module.less";

const PAGE_SIZE = 10;

const RecordedNftList = () => {
    const { message } = App.useApp();
    const [list, setList] = useState([]);
    const [loading, setLoading] = useState(false);
    const [currentPage, setCurrentPage] = useState(1);
    const [total, setTotal] = useState(0);

    // 加载已补录的卡片列表
    const loadRecordedList = async (page = 1) => {
        setLoading(true);
        try {
            const result = await getRecordedNftList({ page, limit: PAGE_SIZE });
            if (result.code !== 0) {
                throw new Error(result.msg || intl.get('nft_get_recorded_failed'));
            }
            setList(result.data?.cardList || []);
            setTotal(result.data?.total || 0);
        } catch (error) {
            message.error(error.message || intl.get('nft_get_recorded_failed'));
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        loadRecordedList(currentPage);
    }, [currentPage]);

    const handleRefresh = () => {
        loadRecordedList(currentPage);
    };

    return (
        <>
            {/* 刷新按钮 */}
            <div className={styles.toolbar}>
                <button className={styles.refreshBtn} onClick={handleRefresh} disabled={loading}>
                    <SyncOutlined spin={loading} />
                </button>
            </div>

            {/* 列表内容 */}
            {loading ? (
                <div className={styles.loading}><Spin size="large" tip={intl.get('nft_loading')} /></div>
            ) : list.length === 0 ? (
                <div className={styles.empty}><Empty description={intl.get('nft_no_data')} /></div>
            ) : (
                <div className={styles.grid}>
                    {list.map((card) => (
                        <RecordedCard key={card.Id} card={card} onRefresh={handleRefresh} />
                    ))}
                </div>
            )}

            {/* 分页 */}
            <CommonPagination
                current={currentPage}
                total={total}
                pageSize={PAGE_SIZE}
                onChange={setCurrentPage}
            />
        </>
    );
};

export default RecordedNftList;
