export interface ApprovalEvent {
    uuid: string,
    event: {
        app_id: string,
        approval_code: string,
        instance_code: string,
        instance_operate_time: string,
        operate_time: string,
        status: string,
        tenant_key: string,
        type: string,
        uuid: string,
    },
    token: string,
    ts: string,
    type: string,
    instanceData: InstanceData,
}

export interface InstanceData {
    approval_name: string;
    start_time?: string | undefined;
    end_time: string;
    user_id: string;
    open_id: string;
    serial_number: string;
    department_id: string;
    status: "PENDING" | "APPROVED" | "REJECTED" | "CANCELED" | "DELETED";
    uuid: string;
    form: string;
    parseForm?: {
        id: string,
        name: string,
        value: any,
    }[];
    task_list: Array<{
        id: string;
        user_id: string;
        open_id?: string;
        status: "PENDING" | "APPROVED" | "REJECTED" | "TRANSFERRED" | "DONE";
        node_id?: string;
        node_name?: string;
        custom_node_id?: string;
        type?: "AND" | "OR" | "AUTO_PASS" | "AUTO_REJECT" | "SEQUENTIAL";
        start_time: string;
        end_time?: string;
    }>;
    comment_list: Array<{
        id: string;
        user_id: string;
        open_id: string;
        comment: string;
        create_time: string;
        files?: Array<{
            url?: string;
            file_size?: number;
            title?: string;
            type?: string;
        }>;
    }>;
    timeline: Array<{
        type: "START" | "PASS" | "REJECT" | "AUTO_PASS" | "AUTO_REJECT" | "REMOVE_REPEAT" | "TRANSFER" | "ADD_APPROVER_BEFORE" | "ADD_APPROVER" | "ADD_APPROVER_AFTER" | "DELETE_APPROVER" | "ROLLBACK_SELECTED" | "ROLLBACK" | "CANCEL" | "DELETE" | "CC";
        create_time: string;
        user_id?: string;
        open_id?: string;
        user_id_list?: Array<string>;
        open_id_list?: Array<string>;
        task_id?: string;
        comment?: string;
        cc_user_list?: Array<{
            user_id?: string;
            cc_id?: string;
            open_id?: string;
        }>;
        ext: string;
        node_key?: string;
        files?: Array<{
            url?: string;
            file_size?: number;
            title?: string;
            type?: string;
        }>;
    }>;
    modified_instance_code?: string | undefined;
    reverted_instance_code?: string | undefined;
    approval_code: string;
    reverted?: boolean | undefined;
    instance_code: string;
}

export interface MessageEvent {
    schema: string,
    header: {
        event_id: string,
        token: string,
        create_time: string,
        event_type: string,
        tenant_key: string,
        app_id: string,
    },
    event: {
        message: {
            chat_id: string,
            chat_type: string,
            content: any,
            create_time: string,
            message_id: string,
            message_type: string,
            update_time: string,
            user_agent: string,
        },
        sender: {
            sender_id: {
                open_id: string,
                union_id: string,
                user_id: string,
            },
            sender_type: string,
            tenant_key: string,
        },
    },
}

export interface CardActionEvent {
    schema: string,
    header: {
        event_id: string,
        token: string,
        create_time: string,
        event_type: string,
        tenant_key: string,
        app_id: string,
    },
    event: {
        operator: {
            open_id: string,
            union_id: string,
            user_id: string,
            tenant_key: string,
        },
        token: string
        action: {
            value: {
                func: string,
                action: string,
                page: number,
            },
            tag: string,
        },
        host: string,
        context: {
            open_message_id: string,
            open_chat_id: string,
        },
    },
}