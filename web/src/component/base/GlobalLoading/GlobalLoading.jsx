import React from "react";
import { Spin } from "antd";

/**
 * 全局 Loading 组件
 * @param {boolean} visible - 是否显示 loading
 * @param {string} tip - 提示文字
 * @param {string} size - Spin 大小，可选 'small' | 'default' | 'large'
 */
const GlobalLoading = ({ visible = false, tip = "加载中...", size = "large" }) => {
  if (!visible) return null;

  return (
    <div
      style={{
        position: "fixed",
        top: 0,
        left: 0,
        right: 0,
        bottom: 0,
        background: "rgba(0, 0, 0, 0.45)",
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
        zIndex: 9999,
        flexDirection: "column",
        gap: "16px",
      }}
    >
      <Spin size={size} />
      <div style={{ color: "#fff", fontSize: "16px" }}>{tip}</div>
    </div>
  );
};

export default GlobalLoading;
