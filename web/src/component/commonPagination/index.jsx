import React from "react";
import { Pagination } from "antd";
import styles from "./index.module.less";

const CommonPagination = ({
    current,
    total,
    pageSize = 10,
    onChange,
    showSizeChanger = false,
    showQuickJumper = false,
    showTotal = false,
    simple = false,
}) => {
    // 总数为0或不超过一页时不显示分页
    if (total <= pageSize) return null;

    return (
        <div className={styles.paginationWrapper}>
            <Pagination
                current={current}
                total={total}
                pageSize={pageSize}
                onChange={onChange}
                showSizeChanger={showSizeChanger}
                showQuickJumper={showQuickJumper}
                showTotal={showTotal ? (t) => `共 ${t} 条` : undefined}
                simple={simple}
                className={styles.pagination}
            />
        </div>
    );
};

export default CommonPagination;
