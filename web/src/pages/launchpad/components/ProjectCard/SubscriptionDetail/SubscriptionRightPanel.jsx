import React, { useState, useMemo, useEffect, useContext } from "react";
import { Button, Progress, Input, Select, message } from "antd";
import { useNavigate } from "react-router-dom";
import Decimal from "decimal.js";
import { Status } from "../../../../../constants/auth";
import WalletModal from "@/pages/walletmodal";
import GlobalLoading from "@/component/base/GlobalLoading/GlobalLoading";
import { formatDecimals } from "@/utils/number";
import { calcSubscriptionProgress } from "@/utils/progress";
import styles from "./SubscriptionRightPanel.module.less";
import { useAccountAssets } from "@/component/hooks";
import RootContext from "@/pages/rootContext";
import { launchpadSubscription } from "@/api/launchpad";

const SubscriptionRightPanel = ({ info, authStatus, onRefreshDetail }) => {
  const navigate = useNavigate();
  const [walletModalOpen, setWalletModalOpen] = useState(false);
  const [submitting, setSubmitting] = useState(false);
  const [globalLoading, setGlobalLoading] = useState(false);
  const [messageApi, contextHolder] = message.useMessage();
  const { userInfo } = useContext(RootContext);
  const { assets, run } = useAccountAssets();

  /**
 *    "Id": 1,
            "InstrumentID": "PikachuUSDT",
            "Name": "2023 Pokemon Black Star Promo Pokemon X Van Gogh Pikachu With Grey Felt Hat #085",
            "Summary": "简介！简介！简介！简介！简介！简介！简介！简介！简介！简介！简介！简介！",
            "AllVolume": "2000000000.000000000000000",
            "StockVolume": "2000000000.000000000000000",
            "Price": "0.000002000000000",
            "OpenTime": "2025-12-11T00:00:00.000Z",
            "CloseTime": "2025-12-15T00:00:00.000Z",
            "Round": "A",
            "MinPrice": "0.000010000000000",
            "MaxPrice": "20.000000000000000",
            "TradeTime": "2025-12-16T00:00:00.000Z",
 */
  const priceDetails = [
    { label: "代币总量", value: formatDecimals(info?.AllVolume) ?? "--" },
    { label: "公开发售", value: formatDecimals(info?.RoundVolume) ?? "--" },
    { label: "首发价", value: formatDecimals(info?.Price) ?? "--" },
  ];

  useEffect(() => {
    if (!userInfo || !userInfo.MemberID) return;
    run({ AccountID: userInfo.MemberID });
  }, [userInfo?.MemberID, run]);

  // 申购表单状态（仅在右侧面板内部使用）
  const [availableBalance, setAvailableBalance] = useState(0); // 可用余额
  const [selectedToken, setSelectedToken] = useState("USDT"); // 当前选择的代币，默认 USDT
  const [purchaseAmount, setPurchaseAmount] = useState(""); // 用户输入的价格（支付金额）

  // 预计可购买数量：用用户输入的价格除以 info.Price
  const estimatedAmount = useMemo(() => {
    if (!purchaseAmount || !info?.Price) return "0";
    try {
      const pay = new Decimal(purchaseAmount);
      const price = new Decimal(info.Price);
      if (price.lte(0)) return "0";
      const qty = pay.div(price);
      // 保留 6 位有效数字，避免太长
      return qty.toSignificantDigits(6).toString();
    } catch (e) {
      return "0";
    }
  }, [purchaseAmount, info?.Price]);

  useEffect(() => {
    if (!assets || !Array.isArray(assets)) {
      setAvailableBalance(0);
      return;
    }

    const current = assets.find((item) => item.Currency === selectedToken);
    setAvailableBalance(current ? Number(current.Available || 0) : 0);
  }, [assets, selectedToken]);

  // 判断是否在认购期内：使用接口返回的 OpenTime / CloseTime
  const isInSubscriptionPeriod = () => {
    if (!info || !info.OpenTime || !info.CloseTime) {
      return false;
    }

    const now = Date.now();
    const startTime = new Date(info.OpenTime).getTime();
    const endTime = new Date(info.CloseTime).getTime();

    if (Number.isNaN(startTime) || Number.isNaN(endTime)) return false;

    return now >= startTime && now <= endTime;
  };

  // 处理价格输入变化
  const handlePurchaseAmountChange = (e) => {
    const value = e.target.value;
    // 只允许输入数字和小数点
    if (value === "" || /^\d*\.?\d*$/.test(value)) {
      if (value === "") {
        setPurchaseAmount("");
        return;
      }

      const num = parseFloat(value);
      if (Number.isNaN(num)) return;

      // 不允许超过可用余额：超过时自动设为最大可用金额
      if (num > availableBalance) {
        setPurchaseAmount(String(availableBalance));
        return;
      }

      setPurchaseAmount(value);
    }
  };

  // 处理代币切换
  const handleTokenChange = (value) => {
    setSelectedToken(value);
    // 可用余额会通过 useEffect 自动更新
  };

  // 提交认购
  const handleSubscribe = async () => {
    if (submitting) return;

    // 登录校验
    if (!userInfo || !userInfo.MemberID) {
      messageApi.warning("请先登录账户");
      return;
    }

    if (!info || !info.Id) {
      messageApi.warning("项目信息缺失，无法认购");
      return;
    }

    // 价格校验
    if (!purchaseAmount) {
      messageApi.warning("请输入价格");
      return;
    }

    const priceNum = Number(purchaseAmount);
    if (!Number.isFinite(priceNum) || priceNum <= 0) {
      messageApi.warning("价格必须大于 0");
      return;
    }

    // 最小购买金额校验
    if (info.MinPrice) {
      const minPrice = Number(info.MinPrice);
      if (priceNum < minPrice) {
        messageApi.warning(`购买金额不能小于 ${minPrice} ${selectedToken}`);
        return;
      }
    }

    // 最大购买金额校验
    if (info.MaxPrice) {
      const maxPrice = Number(info.MaxPrice);
      if (priceNum > maxPrice) {
        messageApi.warning(`购买金额不能大于 ${maxPrice} ${selectedToken}`);
        return;
      }
    }

    // 数量校验（预计可申购量）
    const volumeNum = Number(estimatedAmount);
    if (!Number.isFinite(volumeNum) || volumeNum <= 0) {
      messageApi.warning("预计可申购数量必须大于 0");
      return;
    }

    const payload = {
      launchpadID: info.Id,
      accountID: userInfo.MemberID,
      volume: String(estimatedAmount),
      price: String(purchaseAmount),
      currency: selectedToken,
    };

    try {
      setSubmitting(true);
      setGlobalLoading(true);

      await launchpadSubscription(payload);

      messageApi.success("认购成功");

      // 认购成功后刷新详情（进度、剩余额度等）
      if (typeof onRefreshDetail === 'function') {
        onRefreshDetail();
      }

      // 刷新账户资产
      if (userInfo && userInfo.MemberID) {
        run({ AccountID: userInfo.MemberID });
      }

      // 成功后可以按需清空输入
      // setPurchaseAmount("");
    } catch (e) {
      messageApi.error(e?.msg || "认购失败，请稍后重试");
    } finally {
      setSubmitting(false);
      setGlobalLoading(false);
    }
  };

  // 进度：使用公共工具计算已申购数量 / 总量 以及百分比
  const progressInfo = useMemo(() => {
    const totalStr = info?.RoundVolume;
    const remainingStr = info?.StockVolume;

    const { percent, bought, total } = calcSubscriptionProgress(totalStr, remainingStr);

    return {
      percent,
      bought,
      total,
    };
  }, [info?.RoundVolume, info?.StockVolume]);

  return (
    <>
      <GlobalLoading visible={globalLoading} tip="正在上链中，请稍等..." />
      <div className={styles.rightSection}>
        {contextHolder}
        <h1 className={styles.projectTitle}>{info?.Currency || "--"}</h1>

        {/* Progress Section */}
        <div className={styles.progressSection}>
          <div className={styles.progressHeader}>
            <span className={styles.progressLabel}>本轮公开发售</span>
            <span className={styles.progressAmount}>{formatDecimals(info?.RoundVolume) || "--"}</span>
          </div>
          <Progress
            percent={progressInfo.percent}
            strokeColor="#670099"
            trailColor="#fff"
            showInfo={false}
            className={styles.progressBar}
          />
          <div className={styles.progressInfo}>
            <span className={styles.progressPercent}>{progressInfo.percent}%</span>
          </div>
        </div>

        {/* 根据认购期状态和登录状态显示不同内容 */}
        {isInSubscriptionPeriod() && authStatus === Status.LOGIN ? (
          // 认购期内且已登录 - 显示申购表单
          <>
            <div className={styles.subscriptionForm}>
              {/* 可用余额 */}
              <div className={styles.formRow}>
                <span className={styles.formLabel}>可用</span>
                <span className={styles.formValue}>
                  {availableBalance} {selectedToken}
                </span>
              </div>

              {/* 当前仓位 */}
              <div className={styles.formRow}>
                <span className={styles.formLabel}>当前仓位</span>
                <span className={styles.formValue}>0</span>
              </div>

              {/* 价格输入 */}
              <div className={styles.formInputGroup}>
                <div className={styles.inputWrapper}>
                  <Input
                    placeholder="价格"
                    value={purchaseAmount}
                    onChange={handlePurchaseAmountChange}
                    className={styles.amountInput}
                  />
                  <Select
                    value={selectedToken}
                    onChange={handleTokenChange}
                    className={styles.tokenSelect}
                    suffixIcon={<span style={{ color: "#999" }}>▼</span>}
                    getPopupContainer={(trigger) => trigger.parentNode}
                  >
                    <Select.Option value="USDC">USDC</Select.Option>
                    <Select.Option value="USDT">USDT</Select.Option>
                  </Select>
                </div>
              </div>

              {/* 数量展示：预计可购买数量（纯展示，不可编辑） */}
              <div className={styles.formRow}>
                <span className={styles.formLabel}>数量</span>
                <span className={styles.formLabel}>
                  预计可认购 ：{estimatedAmount}
                </span>
              </div>

              {/* 认购按钮 */}
              <Button
                type={progressInfo.percent >= 100 ? 'default' : 'primary'}
                size='small'
                className={styles.subscribeButton}
                loading={submitting}
                onClick={handleSubscribe}
                disabled={progressInfo.percent >= 100}
              >
                {progressInfo.percent >= 100 ? '已售罄' : '认购'}
              </Button>
            </div>

            {/* Divider */}
            <div className={styles.divider}></div>
          </>
        ) : info?.OpenTime && info?.CloseTime && !isInSubscriptionPeriod() ? (
          // 不在认购期 - 显示已结束状态
          <>
            {/* Status Section */}
            <div className={styles.statusSection}>
              <div className={styles.statusMessage}>
                秒杀活动已结束，感谢参与
              </div>
              <Button type="primary" className={styles.viewButton}>
                已结束
              </Button>
            </div>

            {/* Divider */}
            <div className={styles.divider}></div>
          </>
        ) : null}

        {/* 头像和登录部分 - 仅在认购期内且未登录时显示 */}
        {isInSubscriptionPeriod() && authStatus !== Status.LOGIN && (
          <>
            <div className={styles.avatarSection}>
              <div className={styles.avatar}>
                {(info?.Currency || '--').slice(0, 2).toUpperCase()}
              </div>
            </div>

            <div className={styles.verificationSection}>
              <p className={styles.verificationText}>
                请登录您的账户
              </p>
              <Button
                type="default"
                size='small'
                className={styles.verifyButton}
                onClick={() => setWalletModalOpen(true)}
              >
                登录
              </Button>
            </div>
          </>
        )}

        {/* 历史记录按钮 - 始终显示 */}
        <div className={styles.userSection}>
          <span className={styles.userLabel}>
            {isInSubscriptionPeriod() ? '认购中' : '已结束'}
          </span>
          <Button 
            type="link" 
            className={styles.historyButton}
            onClick={() => navigate(`/launchpad/subscription/${info?.Id}/records`)}
          >
            历史记录
          </Button>
        </div>

        {/* Price Details */}
        <div className={styles.priceDetails}>
          {priceDetails.map((detail, index) => (
            <div key={index} className={styles.priceRow}>
              <span className={styles.priceLabel}>{detail.label}</span>
              <span className={styles.priceValue}>{detail.value}</span>
            </div>
          ))}
        </div>

        <WalletModal
          open={walletModalOpen}
          onClose={() => setWalletModalOpen(false)}
        />
      </div>
    </>
  );
};

export default SubscriptionRightPanel;
