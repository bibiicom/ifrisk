import React, { useState, useMemo, useEffect } from "react";
import { useSearchParams } from "react-router-dom";
import { Tabs } from "antd";
import intl from '@/component/intl';
import ChainHistory from "./components/ChainHistory";
import EntityInbound from "./components/EntityInbound";
import EntityOutbound from "./components/EntityOutbound";
import styles from "./index.module.less";

const NftHistory = () => {
  const [searchParams] = useSearchParams();
  const [activeTab, setActiveTab] = useState("deposit");

  // 从 URL 参数读取 historyTab
  useEffect(() => {
    const historyTab = searchParams.get("historyTab");
    if (historyTab && ["deposit", "withdraw", "entity", "entityOutbound"].includes(historyTab)) {
      setActiveTab(historyTab);
    }
  }, [searchParams]);

  const tabItems = useMemo(
    () => [
      { key: "deposit", label: intl.get('nft_history_deposit') },
      { key: "withdraw", label: intl.get('nft_history_withdraw') },
      { key: "entity", label: intl.get('nft_history_entity') },
      { key: "entityOutbound", label: intl.get('nft_history_entity_outbound') },
    ],
    []
  );

  const renderContent = () => {
    switch (activeTab) {
      case "deposit":
        return <ChainHistory type="deposit" />;
      case "withdraw":
        return <ChainHistory type="withdraw" />;
      case "entity":
        return <EntityInbound />;
      case "entityOutbound":
        return <EntityOutbound />;
      default:
        return null;
    }
  };

  return (
    <div className={styles.wrap}>
      <Tabs items={tabItems} activeKey={activeTab} onChange={setActiveTab} />
      {renderContent()}
    </div>
  );
};

export default NftHistory;
