import React from "react";
import { Button } from "antd";
import intl from '@/component/intl';
import styles from "./index.module.less";

const NftTabs = ({ activeTab, onChange }) => {
  return (
    <div className={styles.tabs}>
      <Button
        type="default"
        className={`${styles.tab} ${activeTab === "owned" ? styles.active : ""}`}
        onClick={() => onChange("owned")}
      >
        {intl.get('nft_tab_owned')}
      </Button>
      <Button
        type="default"
        className={`${styles.tab} ${activeTab === "recorded" ? styles.active : ""}`}
        onClick={() => onChange("recorded")}
      >
        {intl.get('nft_tab_recorded')}
      </Button>
      <Button
        type="default"
        className={`${styles.tab} ${activeTab === "history" ? styles.active : ""}`}
        onClick={() => onChange("history")}
      >
        {intl.get('nft_tab_history')}
      </Button>
    </div>
  );
};

export default NftTabs;
