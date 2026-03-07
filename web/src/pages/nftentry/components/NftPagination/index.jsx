import React from "react";
import { LeftOutlined, RightOutlined } from "@ant-design/icons";
import styles from "./index.module.less";

const NftPagination = ({ totalPages, activePage, onPageChange }) => {
  if (totalPages <= 1) return null;

  const pages = Array.from({ length: totalPages }, (_, i) => i + 1)
    .filter((page) => {
      return (
        page === 1 ||
        page === totalPages ||
        Math.abs(page - activePage) <= 2
      );
    })
    .reduce((acc, page, idx, arr) => {
      if (idx > 0 && page - arr[idx - 1] > 1) {
        acc.push("...");
      }
      acc.push(page);
      return acc;
    }, []);

  return (
    <div className={styles.pagination}>
      <button
        className={styles.btn}
        disabled={activePage === 1}
        onClick={() => onPageChange(activePage - 1)}
      >
        <LeftOutlined />
      </button>

      {pages.map((item, idx) =>
        item === "..." ? (
          <span
            key={`ellipsis-${idx}`}
            className={styles.ellipsis}
          >
            ...
          </span>
        ) : (
          <button
            key={item}
            className={`${styles.btn} ${
              activePage === item ? styles.active : ""
            }`}
            onClick={() => onPageChange(item)}
          >
            {item}
          </button>
        )
      )}

      <button
        className={styles.btn}
        disabled={activePage === totalPages}
        onClick={() => onPageChange(activePage + 1)}
      >
        <RightOutlined />
      </button>
    </div>
  );
};

export default NftPagination;
