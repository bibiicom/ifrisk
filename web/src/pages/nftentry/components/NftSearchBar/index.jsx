import React from "react";
import { SearchOutlined, SyncOutlined } from "@ant-design/icons";
import styles from "./index.module.less";

const NftSearchBar = ({ value, onChange, loading, onRefresh }) => {
  return (
    <div className={styles.searchRow}>
      <div className={styles.searchBox}>
        <SearchOutlined className={styles.searchIcon} />
        <input
          type="text"
          placeholder="搜索 NFT 名称或合集"
          value={value}
          onChange={onChange}
        />
      </div>
      <button
        className={styles.refreshBtn}
        onClick={onRefresh}
        disabled={loading}
      >
        <SyncOutlined spin={loading} />
      </button>
    </div>
  );
};

export default NftSearchBar;
