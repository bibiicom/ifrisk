import React, { useState, useEffect, useMemo } from "react";
import { App, Spin, Empty, Tooltip } from "antd";
import { SearchOutlined, SyncOutlined } from "@ant-design/icons";
import intl from '@/component/intl';
import CommonCard from "../../../../component/commonCard";
import CommonPagination from "../../../../component/commonPagination";
import { getArbitrumNfts, transferNftToPlatform } from "../../../../api/nft";
import styles from "./index.module.less";

const PAGE_SIZE = 10;

const OwnedNftList = ({ onRecordSuccess }) => {
    const { modal, message } = App.useApp();
    const [transferring, setTransferring] = useState(null);
    const [list, setList] = useState([]);
    const [loading, setLoading] = useState(false);
    const [searchValue, setSearchValue] = useState("");
    const [currentPage, setCurrentPage] = useState(1);
    const cardStyles = CommonCard.styles;

    // 加载个人持有的 NFT
    const loadOwnedNfts = async () => {
        setLoading(true);
        try {
            const nfts = await getArbitrumNfts();
            setList(nfts || []);
        } catch (error) {
            message.error(error.message || intl.get('nft_get_list_failed'));
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        loadOwnedNfts();
    }, []);

    // 前端搜索过滤
    const filteredList = useMemo(() =>
        (list || []).filter((nft) =>
            nft.name?.toLowerCase().includes(searchValue.toLowerCase()) ||
            (nft.collection || "").toLowerCase().includes(searchValue.toLowerCase())
        ), [list, searchValue]);

    // 前端分页
    const paginatedList = useMemo(() =>
        (filteredList || []).slice((currentPage - 1) * PAGE_SIZE, currentPage * PAGE_SIZE),
        [filteredList, currentPage]);

    const totalPages = Math.ceil((filteredList || []).length / PAGE_SIZE);

    const handleSearch = (e) => {
        setSearchValue(e.target.value);
        setCurrentPage(1);
    };

    const handleRefresh = () => loadOwnedNfts();

    // 链上入库
    const handleRecord = async (nft) => {
        if (transferring || !nft) return;
        setTransferring(nft.id);
        try {
            const result = await transferNftToPlatform(nft.contractAddress, nft.tokenId);
            message.success(intl.get('nft_record_success').replace('{hash}', result.txHash.slice(0, 10) + '...'));
            setList((prev) => prev.filter((item) => item.id !== nft.id));
            onRecordSuccess?.(nft.id);
        } catch (error) {
            const errCode = error?.code || error?.error?.code;
            const errMsg = error?.message || error?.error?.message || "";
            if (errCode === 4001 || errCode === "ACTION_REJECTED" || errCode === "USER_REJECTED" || /user rejected/i.test(errMsg)) {
                message.warning(intl.get('nft_record_cancelled'));
            } else {
                message.error(error.message || intl.get('nft_record_failed'));
            }
        } finally {
            setTransferring(null);
        }
    };

    const handleOpenRecordModal = (nft) => {
        let modalInstance;
        modalInstance = modal.confirm({
            title: intl.get('nft_record_modal_title'),
            content: intl.get('nft_record_modal_content').replace('{name}', nft.name),
            okText: intl.get('nft_record_modal_ok'),
            cancelText: intl.get('nft_record_modal_cancel'),
            style: { top: "30%" },
            onOk: async () => {
                modalInstance?.update?.({ cancelButtonProps: { disabled: true } });
                return handleRecord(nft);
            },
        });
    };

    const renderContent = (nft) => (
        <>
            <div className={styles.cardName}>{nft.name}</div>
            <Tooltip title={nft.contractAddress}>
                <div className={styles.cardAddress}>
                    {nft.contractAddress.slice(0, 6)}...{nft.contractAddress.slice(-4)}
                </div>
            </Tooltip>
        </>
    );

    const renderFooter = (nft) => (
        <button className={cardStyles.cardBtn} onClick={() => handleOpenRecordModal(nft)} disabled={transferring === nft.id}>
            {transferring === nft.id ? intl.get('nft_recording') : intl.get('nft_record_button')}
        </button>
    );

    return (
        <>
            <div className={styles.searchRow}>
                <div className={styles.searchBox}>
                    <SearchOutlined className={styles.searchIcon} />
                    <input type="text" placeholder={intl.get('nft_search_placeholder')} value={searchValue} onChange={handleSearch} />
                </div>
                <button className={styles.refreshBtn} onClick={handleRefresh} disabled={loading}>
                    <SyncOutlined spin={loading} />
                </button>
            </div>

            {loading ? (
                <div className={styles.loading}>
                    <Spin size="large" >
                        <div style={{ minHeight: 100 }} />
                    </Spin>
                </div>
            ) : (paginatedList || []).length === 0 ? (
                <div className={styles.empty}><Empty description={intl.get('nft_no_data')} /></div>
            ) : (
                <div className={styles.grid}>
                    {(paginatedList || []).map((nft) => (
                        <CommonCard key={nft.id} image={nft.image} imageAlt={nft.name} content={renderContent(nft)} footer={renderFooter(nft)} />
                    ))}
                </div>
            )}

            {totalPages > 1 && (
                <CommonPagination
                    current={currentPage}
                    total={filteredList.length}
                    pageSize={PAGE_SIZE}
                    onChange={setCurrentPage}
                />
            )}
        </>
    );
};

export default OwnedNftList;
