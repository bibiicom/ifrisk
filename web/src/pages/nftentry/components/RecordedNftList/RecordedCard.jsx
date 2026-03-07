import React, { useState, useEffect } from "react";
import { App, Spin, Tooltip } from "antd";
import intl from '@/component/intl';
import CommonCard from "../../../../component/commonCard";
import { getNftMetadata, applyCardListing, withdrawCard } from "../../../../api/nft";
import defaultCardImg from "../../../../assets/images/common/default-card.jpeg";
import styles from "./index.module.less";

/**
 * 单个卡片组件
 * Category === 1: NFT，需要内部请求元数据
 * Category === 2: 实体卡片，直接使用 cardList 数据
 */
const RecordedCard = ({ card, onRefresh }) => {
    const { modal, message } = App.useApp();
    const [listing, setListing] = useState(false);
    const [withdrawing, setWithdrawing] = useState(false);
    const [nftMetadata, setNftMetadata] = useState(null);
    const [loading, setLoading] = useState(false);
    const cardStyles = CommonCard.styles;

    const isNft = card.Category === 1;
    const isEntity = card.Category === 2;

    // NFT 类型：请求元数据
    useEffect(() => {
        if (isNft && card.Contract && card.TokenID) {
            setLoading(true);
            getNftMetadata(card.Contract, card.TokenID)
                .then((metadata) => setNftMetadata(metadata))
                .catch(() => setNftMetadata(null))
                .finally(() => setLoading(false));
        }
    }, [isNft, card.Contract, card.TokenID]);

    // 获取显示数据
    const displayName = isEntity
        ? card.Name || `${intl.get('nft_recorded_entity_card')} #${card.Id}`
        : nftMetadata?.name || `${intl.get('nft_recorded_nft')} #${card.TokenID || card.Id}`;

    const displayImage = isEntity
        ? card.Img || defaultCardImg
        : nftMetadata?.image || "";

    const displayAddress = isNft && card.Contract
        ? `${card.Contract.slice(0, 6)}...${card.Contract.slice(-4)}`
        : "";

    // 申请上架
    const handleListing = async () => {
        if (listing) return;
        setListing(true);
        try {
            const result = await applyCardListing({ cardId: card.Id });
            if (result.code !== 0) {
                throw new Error(result.msg || intl.get('nft_listing_failed'));
            }
            message.success(intl.get('nft_listing_success'));
            onRefresh?.();
        } catch (error) {
            message.error(error.message || intl.get('nft_listing_failed'));
        } finally {
            setListing(false);
        }
    };

    // 打开上架确认弹窗
    const handleOpenListingModal = () => {
        modal.confirm({
            title: intl.get('nft_listing_modal_title'),
            content: intl.get('nft_listing_modal_content').replace('{name}', displayName),
            okText: intl.get('nft_listing_modal_ok'),
            cancelText: intl.get('nft_listing_modal_cancel'),
            style: { top: "30%" },
            onOk: handleListing,
        });
    };

    // 提取/出库
    const handleWithdraw = async () => {
        if (withdrawing) return;
        setWithdrawing(true);
        try {
            const result = await withdrawCard({ cardId: card.Id });
            if (result.code !== 0) {
                throw new Error(result.msg || intl.get('nft_withdraw_failed'));
            }
            message.success(intl.get('nft_withdraw_success'));
            onRefresh?.();
        } catch (error) {
            message.error(error.message || intl.get('nft_withdraw_failed'));
        } finally {
            setWithdrawing(false);
        }
    };

    // 打开提取确认弹窗
    const handleOpenWithdrawModal = () => {
        const title = isEntity ? intl.get('nft_withdraw_modal_title_entity') : intl.get('nft_withdraw_modal_title_nft');
        const content = isEntity
            ? intl.get('nft_withdraw_modal_content_entity').replace('{name}', displayName)
            : intl.get('nft_withdraw_modal_content_nft').replace('{name}', displayName);

        modal.confirm({
            title,
            content,
            okText: intl.get('nft_withdraw_modal_ok'),
            cancelText: intl.get('nft_withdraw_modal_cancel'),
            style: { top: "30%" },
            onOk: handleWithdraw,
        });
    };

    // 渲染内容区域
    const renderContent = () => (
        <div style={{ height: '48px' }}>
            <div className={styles.cardName}>{displayName}</div>
            {isNft && displayAddress && (
                <Tooltip title={card.Contract}>
                    <div className={styles.cardAddress}>
                        {displayAddress}
                    </div>
                </Tooltip>
            )}
        </div>
    );

    // 渲染底部按钮
    const renderFooter = () => {
        const status = card.Status;

        // 获取状态文字
        const getStatusText = () => {
            switch (status) {  
                case 1: return intl.get('nft_recorded_apply_listing');
                case 2: return intl.get('nft_recorded_listed');
                case 3: return intl.get('nft_recorded_withdrawing');
                case 4: return intl.get('nft_recorded_outbounding');
                default: return intl.get('nft_recorded_unknown_status');
            }
        };

        return (
            <>
                {/* 申请上架按钮已注释 */}
                {/* <button
                    className={cardStyles.cardBtn}
                    onClick={status === 1 ? handleOpenListingModal : undefined}
                    disabled={status !== 1 || listing}
                >
                    {getStatusText()}
                </button> */}
                {/* 只有 NFT 且 Status === 1 才有提取按钮 */}
                {status === 1 && isNft && (
                    <button
                        className={`${cardStyles.cardBtn} ${cardStyles.cardBtnSecondary}`}
                        onClick={handleOpenWithdrawModal}
                        disabled={withdrawing}
                    >
                        {intl.get('nft_recorded_withdraw')}
                    </button>
                )}
            </>
        );
    };

    // NFT 加载中
    if (isNft && loading) {
        return (
            <CommonCard
                image=""
                imageAlt=""
                content={<Spin size="small" />}
                footer={null}
            />
        );
    }

    return (
        <CommonCard
            image={displayImage}
            imageAlt={displayName}
            content={renderContent()}
            footer={renderFooter()}
        />
    );
};

export default RecordedCard;
