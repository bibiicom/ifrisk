// 我的持有卡片：展示当前账号在选中币种下的持有数量、占比，并提供合成功能
import React, { useState, useEffect, useMemo } from "react";
import { Button, message } from "antd";
import { SyncOutlined, WalletOutlined } from "@ant-design/icons";
import intl from "@/component/intl";
import HoldRatioDonut from "../../../../component/echarts/HoldRatioDonut";
import ConfirmModal from "../../../../component/modal/ConfirmModal";
import { compositeCard, getNftMetadata } from "../../../../api/nft";
import { getStorage, TOKEN } from "../../../../utils/storage";
import { isSvgImage } from "../../../../utils/image";
import CompositionAnimation from "./CompositionAnimation";
import { useNavigate } from "react-router-dom";
import useTheme from "../../../../hooks/useTheme";
import styles from "./index.module.less";

// selectedCoin：当前选中的币种（BaseCurrency）
// balance：用户持有的数量（来自 currencyRank 接口）
// total：该币种总持有量（来自 currencyRank 接口 totalBalance）
const Holding = ({ selectedCoin, balance, total, instrumentID, onRefresh }) => {
  const { isDark } = useTheme();
  // 合成弹窗是否可见
  const [synthesizeModalVisible, setSynthesizeModalVisible] = useState(false);
  // 是否已经合成成功 / 是否正在合成
  const [synthesized, setSynthesized] = useState(false);
  const [synthesizing, setSynthesizing] = useState(false);

  // 动画相关
  const [showAnimation, setShowAnimation] = useState(false);
  const [nftImage, setNftImage] = useState("");

  const navigate = useNavigate();

  const myAmount = useMemo(() => {
    return Number(balance) || 0;
  }, [balance]);

  // 当切换币种时,重置合成状态
  useEffect(() => {
    setSynthesized(false);
  }, [selectedCoin]);

  // 计算我的持有占比：我的数量 / 总持有量 * 100（总持有量来自排行榜接口 total）
  const getMyPercentage = () => {
    if (!total) return 0;
    return (myAmount / total) * 100;
  };
  const myPercentage = getMyPercentage();

  // donut 图数据：我的占比 + 其他占比
  const holdRatioData = useMemo(
    () => [
      {
        value: myPercentage,
        name: intl.get('buyback_my_label'),
        itemStyle: { color: "#1677ff" },
      },
      {
        value: Math.max(0, 100 - myPercentage),
        name: intl.get('buyback_other_label'),
        itemStyle: { color: isDark ? "#333333" : "#e0e0e0" },
      },
    ],
    [myPercentage, isDark]
  );

  // donut 图中心标题：显示我的持有百分比（保留三位小数）
  const holdRatioTitle = useMemo(
    () => ({
      text: `${myPercentage.toFixed(3)}%`,
      left: "center",
      top: "center",
      textStyle: {
        color: isDark ? "#ffffff" : "#1a1a1a",
        fontSize: 12,
      },
    }),
    [myPercentage, isDark]
  );

  // 点击按钮打开合成弹窗
  const handleSynthesizeClick = () => {
    setSynthesizeModalVisible(true);
  };

  // 合成确认
  const handleSynthesize = async () => {
    const actionLoading = true;
    setSynthesizing(true);
    try {
      const token = getStorage(TOKEN);
      // 调用合成接口，currency 使用当前选中的币种名称
      const res = await compositeCard({
        currency: selectedCoin,
        token,
      });

      // 检查合成状态
      if (res && res?.code === 0) {
        setSynthesized(true);
        setSynthesizeModalVisible(false);

        let nextImage = "";
        const category = res?.data?.Category;

        if (category === 2) {
          // 实体卡片：直接使用返回的 Img 字段
          nextImage = res?.data?.Img || "";
        } else {
          // NFT 卡片 (Category === 1 或无 Category)：通过元数据获取图片
          const tokenId = res?.data?.TokenID;
          const contract = res?.data?.Contract;
          if (tokenId && contract) {
            try {
              const metadata = await getNftMetadata(contract, tokenId);
              nextImage = (metadata && metadata.image) || "";
            } catch (e) {
              console.error("获取 NFT 元数据失败", e);
            }
          }
        }

        // 统一判断：SVG 或空图片不展示动画
        if (!nextImage || isSvgImage(nextImage)) {
          setShowAnimation(false);
          message.success(intl.get('buyback_synthesize_success'));
        } else {
          setNftImage(nextImage);
          setShowAnimation(false);
          requestAnimationFrame(() => setShowAnimation(true));
        }

        // 刷新排行榜（父组件数据）
        if (onRefresh) {
          onRefresh({ resetSelected: true });
        }
      } else {
        message.warning(intl.get('buyback_synthesize_failed'));
      }
    } catch (error) {
      message.error(error.msg || intl.get('buyback_synthesize_failed'));
    } finally {
      setSynthesizing(false);
    }
  };

  const handleGoTrade = (e) => {
    // 避免点击按钮时同时触发卡片的详情跳转
    if (e) {
      e.stopPropagation();
    }
    const productGroup = "SPOT";
    navigate(
      `/exchange?ProductGroup=${productGroup}&InstrumentID=${instrumentID}`
    );
  };

  return (
    // 卡片整体容器
    <div className={styles["buyback-user-card"]}>
      {nftImage ? (
        <CompositionAnimation
          visible={showAnimation}
          imageUrl={nftImage}
          onComplete={() => {
            setShowAnimation(false);
            // message.success("合成成功");
          }}
        />
      ) : null}
      {/* 头部标题区域 */}
      <div className={styles["user-card-header"]}>
        <div style={{ display: "flex", gap: "12px" }}>
          <WalletOutlined />
          <span>{intl.get('buyback_my_holding')}</span>
        </div>
        <div>
          <Button onClick={handleGoTrade}>{intl.get('buyback_go_trade')}</Button>
        </div>
      </div>
      {/* 主体区域：左侧 donut + 右侧数量 + 底部操作按钮 */}
      <div className={styles["user-card-body"]}>
        <div className={styles["user-holding-info"]}>
          {/* 持有占比 donut 图 */}
          <div className={styles["holding-percentage"]}>
            <span className={styles["label"]}>{intl.get('buyback_holding_percentage')}</span>
            <div className={styles["holding-percentage-content"]}>
              <div className={styles["holding-donut-wrapper"]}>
                <HoldRatioDonut
                  data={holdRatioData}
                  titleOption={holdRatioTitle}
                />
              </div>
            </div>
          </div>
          {/* 当前持有数量展示 */}
          <div className={styles["holding-amount"]}>
            <span className={styles["label"]}>{intl.get('buyback_holding_amount')}</span>
            <span className={styles["value"]}>
              {myAmount} {selectedCoin}
            </span>
          </div>
        </div>

        {/* 操作按钮：根据占比展示合成入口 */}
        <div className={styles["user-actions"]}>
          {synthesized ? null : myPercentage >= 100 ? (
            <Button
              type="primary"
              onClick={handleSynthesizeClick}
              icon={<SyncOutlined spin={synthesizing} />}
            >
              {intl.get('buyback_synthesize_card')}
            </Button>
          ) : null}
        </div>
      </div>

      <ConfirmModal
        visible={synthesizeModalVisible}
        onCancel={() => setSynthesizeModalVisible(false)}
        onConfirm={handleSynthesize}
        title={intl.get('buyback_synthesize_title')}
        cancelText={intl.get('buyback_cancel')}
        confirmText={intl.get('buyback_synthesize_confirm')}
        loading={synthesizing}
      >
        <div className={styles.modalTitle}>
          {intl.get('buyback_synthesize_message')} <strong>{selectedCoin}</strong> {intl.get('buyback_synthesize_to_card')}
        </div>

        <div className={styles.modalInfoCard}>
          <div className={styles.infoLabel}>{intl.get('buyback_current_percentage')}</div>
          <div className={styles.infoValue}>{myPercentage.toFixed(3)}%</div>
        </div>

        <div className={styles.modalTip}>{intl.get('buyback_synthesize_tip')}</div>
      </ConfirmModal>
    </div>
  );
};

export default Holding;
