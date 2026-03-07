import {BaseClass} from "../core/BaseClass";
import {App} from "../App";

export class BotUtil extends BaseClass {
    public notifyChatId = "oc_f25da90d6b40a2b9a221f0a5ab4e5960";
    public moneyChatId = "oc_28747bb4a364fc3008745af63a88a733";

    public async sendMDMsgToNotify(msg: string) {
        let msgContent = {
            zh_cn: {
                content: [[
                    {
                        tag: "md",
                        text: msg,
                    }
                ]]
            },
        }
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: this.notifyChatId,
                msg_type: 'post',
                content: JSON.stringify(msgContent),
            },
        });
    }

    public async sendMDMsgToMoney(msg: string) {
        let msgContent = {
            zh_cn: {
                content: [[
                    {
                        tag: "md",
                        text: msg,
                    }
                ]]
            },
        }
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: this.moneyChatId,
                msg_type: 'post',
                content: JSON.stringify(msgContent),
            },
        });
    }


    public async sendPrivateMsg(userId: string, msg: string) {
        let msgContent = {
            text: msg
        }
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'user_id',
            },
            data: {
                receive_id: userId,
                msg_type: 'text',
                content: JSON.stringify(msgContent),
            },
        });
    }

    public async sendChatMdMsg(chatId: string, msg: string) {
        let msgContent = {
            zh_cn: {
                content: [[
                    {
                        tag: "md",
                        text: msg,
                    }
                ]]
            },
        }
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: chatId,
                msg_type: 'post',
                content: JSON.stringify(msgContent),
            },
        });
    }

    public async sendChatCardMsg(chatId: string, msgContent: any) {
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: chatId,
                msg_type: 'interactive',
                content: JSON.stringify(msgContent),
            },
        });
    }

}

