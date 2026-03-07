import React, { useState, useContext, useEffect } from "react";
import { useSearchParams } from "react-router-dom";
import authHOC from "../common/hoc/authHOC";
import RootContext from "../rootContext";
import { Status } from "../../constants/auth";
import EmptyState from "@/component/emptyState";
import NftTabs from "./components/NftTabs";
import OwnedNftList from "./components/OwnedNftList";
import RecordedNftList from "./components/RecordedNftList";
import NftHistory from "./components/NftHistory";
import styles from "./index.module.less";

const NftEntry = () => {
  const { authStatus } = useContext(RootContext);
  const [searchParams] = useSearchParams();
  const [activeTab, setActiveTab] = useState("owned"); // 'owned' | 'recorded' | 'history'

  // 从 URL 参数读取 tab 和 historyTab
  useEffect(() => {
    const tab = searchParams.get("tab");
    if (tab && ["owned", "recorded", "history"].includes(tab)) {
      setActiveTab(tab);
    }
  }, [searchParams]);

  // 未登录显示空状态
  if (authStatus !== Status.LOGIN) {
    return <EmptyState />;
  }

  return (
    <div className={styles.page}>
      <NftTabs activeTab={activeTab} onChange={setActiveTab} />

      {activeTab === "owned" && <OwnedNftList />}
      {activeTab === "recorded" && <RecordedNftList />}
      {activeTab === "history" && <NftHistory />}
    </div>
  );
};

export default authHOC(true)(NftEntry);
