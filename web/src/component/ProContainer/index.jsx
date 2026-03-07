import React from "react";
import styles from "./index.module.less";

const ProContainer = ({ children, heightFull = false, className = "" }) => {
  return (
    <div
      className={`${styles.proContainer} ${heightFull ? styles.heightFull : ""
        } ${className}`}
      style={{ maxHeight: heightFull ? "calc(100vh - 56px - 360px)" : "auto", overflow: heightFull ? "hidden" : "auto" }}
    >
      {children}
    </div>
  );
};

export default ProContainer;
