import React, { useState, useEffect, useContext } from "react";
import { useParams, useSearchParams } from "react-router-dom";
import { message } from "antd";
import RootContext from "../../../../rootContext";
import authHOC from "../../../../common/hoc/authHOC";
import { fetchLaunchpadInfoById } from "@/api/launchpad";
import { getNftMetadata } from "@/api/nft";
import ActivityRules from "./ActivityRules";
import ProductImageGallery from "./ProductImageGallery";
import SubscriptionRightPanel from "./SubscriptionRightPanel";
import Timeline from "./Timeline";
import styles from "./SubscriptionDetail.module.less";
import { formatDecimals } from "@/utils/number";

const SubscriptionDetail = () => {
  const { projectId } = useParams();
  const { authStatus } = useContext(RootContext);

  // 项目详情数据，由接口返回
  const [projectData, setProjectData] = useState({});
  const [nftMetadata, setNftMetadata] = useState(null);

  // 获取详情
  const getProjectDetail = async () => {
    try {
      const res = await fetchLaunchpadInfoById({ id: projectId });
      if (res?.code === 0 && res?.data) {
        // 合并接口返回：launchpadInfo + cardInfo(Contract/TokenID)
        const launchpadInfo = res?.data?.launchpadInfo || {};
        const cardInfo = res?.data?.cardInfo || {};
        const address = cardInfo?.Contract || launchpadInfo?.Address;
        const tokenId = cardInfo?.TokenID || launchpadInfo?.TokenId || launchpadInfo?.TokenID;

        setProjectData({
          ...launchpadInfo,
          Address: address,
          TokenId: tokenId,
        });

        // 获取 NFT 元数据
        if (address && tokenId) {
          const metadata = await getNftMetadata(address, tokenId);
          setNftMetadata(metadata);
        }
      }
    } catch (err) {
      message.error(err?.msg || "获取项目详情失败");
    }
  };

  // 根据路由参数 id 获取项目详情
  useEffect(() => {
    if (!projectId) return;
    getProjectDetail();
  }, [projectId]);

  console.log(projectData, "-----");

  return (
    <div className={styles.container}>
      <div className={styles.mainContent}>
        {/* Left and Middle Section Wrapper */}
        <div className={styles.leftMiddleWrapper}>
          {/* Top Row: Image Gallery and Product Info */}
          <div className={styles.topRow}>
            {/* Product Image Gallery */}
            <ProductImageGallery />

            <div className={styles.productInfo}>
              {/* Product Title */}
              <h1 className={styles.title}>{projectData?.Name || "--"}</h1>

              {/* Info Row: Seller, Edition, ID */}
              <div className={styles.infoRow}>
                <div className={styles.infoItem}>
                  <span className={styles.infoLabel}>合约创建者</span>
                  <span className={styles.infoValue}>
                    {nftMetadata?.contractDeployAt || projectData.owner || "--"}
                  </span>
                </div>
                <div className={styles.infoItem}>
                  <span className={styles.infoLabel}>地址</span>
                  <span className={styles.infoValue}>
                    {projectData?.Address || "--"}
                  </span>
                </div>
                <div className={styles.infoItem}>
                  <span className={styles.infoLabel}>令牌ID</span>
                  <span className={styles.infoValue}>
                    {projectData?.TokenId || "--"}
                  </span>
                </div>
              </div>

              {/* Price Section */}
              <div className={styles.priceSection}>
                <div className={styles.currentPriceLabel}>当前价格</div>
                <div className={styles.priceRow}>
                  <div className={styles.currentPrice}>
                    {formatDecimals(projectData?.Price) || "--"}$
                  </div>
                </div>
              </div>
            </div>
          </div>

          {/* Timeline Section - Full Width Below */}
          <Timeline info={projectData} />
        </div>

        {/* Right side - Subscription Details */}
        <SubscriptionRightPanel
          info={projectData}
          authStatus={authStatus}
          onRefreshDetail={getProjectDetail}
        />
      </div>

      {/* Activity Rules Section */}
      <ActivityRules data={projectData} />
    </div>
  );
};

export default authHOC(true)(SubscriptionDetail);
