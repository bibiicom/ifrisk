import {EventHandler} from "../base/EventDecorator";
import {App} from "../base/App";
import {LocalRemote} from "../remote/LocalRemote";
import {TimeUtil} from "../tool/TimeUtil";
import {CardActionEvent, MessageEvent} from "../const/LarkInterface";

export class MessageHandler {
    /**
     * 处理消息事件
     * @param msg 事件数据
     */
    @EventHandler("im.message.receive_v1")
    async onMessage(msg: MessageEvent) {
        try {

            // console.log("im.message.receive_v1:", JSON.stringify(msg));
            let content = JSON.parse(msg.event.message.content);
            msg.event.message.content = content;
            if (content.text == "申请上架列表") {
                await this.doSendApplyList(msg);
                return;
            } else if (content.text == "申请入库列表") {
                await this.doSendStorageApplyList(msg);
                return;
            } else if (content.text == "申请出库列表") {
                await this.doSendOutboundApplyList(msg);
                return;
            } else if (content.text == "文档列表") {
                await this.doSendDocumentList(msg);
                return;
            } else if (content.text == "申购到期列表") {
                await this.doGetSubscriptionExpiryList(msg);
                return;
            }
        } catch (e) {
            console.error("onMessage error:", e);
        }
    }

    @EventHandler("card.action.trigger")
    async onCardAction(msg: CardActionEvent) {
        try {
            // console.log("card.action.trigger:", msg);
            // console.log("card.action.trigger:", JSON.stringify(msg));
            const action = msg.event.action;
            if (!action || !action.value) {
                return;
            }
            const actionValue = action.value;
            if (actionValue.func == "documentList" && actionValue.action == "page") {
                // 发送文档列表
                await this.sendDocumentList(msg.event.context.open_chat_id, actionValue.page);
            } else {
                // 未处理的函数
                console.log("未处理的函数:", actionValue.func, msg.event.action.value);
            }
        } catch (e) {
            console.error("onCardAction error:", msg.event.action.value, e);
        }
    }

    async doSendApplyList(msg: MessageEvent) {
        // 发送申请列表 larkCardID ctp_AAXrxY6sQLfu
        let applyCardList = await LocalRemote.getApplyCardList();
        let md_text = "";
        for (let cardInfo of applyCardList) {
            md_text += `
- **CardID:** ${cardInfo.Id}
  **Owner:** ${cardInfo.Owner}
  **Name:** ${cardInfo.Name}
  **Contract:** ${cardInfo.Contract}
  **TokenID:** ${cardInfo.TokenID}
  **Time:** ${TimeUtil.format(cardInfo.updatedAt)}<hr>`;
        }

        let content = {
            "type": "template",
            "data": {
                // 卡片 ID，参数必填。
                "template_id": "ctp_AAXrxY6sQLfu",
                "template_variable":
                    {
                        // 如果卡片模板内设置了卡片变量，则可以在此处为变量（key）赋值（value）。
                        "md_text": md_text.length > 0 ? md_text : "暂无申请"
                    }
            }
        };
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: msg.event.message.chat_id,
                msg_type: 'interactive',
                content: JSON.stringify(content),
            },
        });
    }

    async doSendStorageApplyList(msg: MessageEvent) {
        // 发送入库申请列表 larkCardID ctp_AAXQ2n94Uphn
        let storageApplyCardList = await LocalRemote.getStorageApplyCardList();
        let md_text = "";
        for (let cardInfo of storageApplyCardList) {
            // 格式化更新时间
            let updateTimeStr = TimeUtil.format(cardInfo.updatedAt);
            md_text += `
- **ApplyID:** ${cardInfo.Id}
  **AccountID:** ${cardInfo.AccountID}
  **Email:** ${cardInfo.Email}
  **Time:** ${updateTimeStr}<hr>`;
        }

        let content = {
            "type": "template",
            "data": {
                // 卡片 ID，参数必填。
                "template_id": "ctp_AAXQ2n94Uphn",
                "template_variable":
                    {
                        // 如果卡片模板内设置了卡片变量，则可以在此处为变量（key）赋值（value）。
                        "md_text": md_text.length > 0 ? md_text : "暂无申请"
                    }
            }
        };
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: msg.event.message.chat_id,
                msg_type: 'interactive',
                content: JSON.stringify(content),
            },
        });
    }

    async doSendOutboundApplyList(msg: MessageEvent) {
        // 发送出库申请列表 larkCardID ctp_AAXQ2n94kqXG
        let outboundApplyCardList = await LocalRemote.getOutboundApplyCardList();
        let md_text = "";
        for (let outboundApply of outboundApplyCardList) {
            // 格式化更新时间
            let updateTimeStr = TimeUtil.format(outboundApply.updatedAt);
            md_text += `
- **ApplyID:** ${outboundApply.Id}
  **AccountID:** ${outboundApply.AccountID}
  **Name:** ${outboundApply.Name}
  **Address:** ${outboundApply.Address}
  **CardName:** ${outboundApply.CardInfo?.Name || "未知"}
  **Time:** ${updateTimeStr}<hr>`;
        }

        let content = {
            "type": "template",
            "data": {
                // 卡片 ID，参数必填。
                "template_id": "ctp_AAXQ2n94kqXG",
                "template_variable":
                    {
                        // 如果卡片模板内设置了卡片变量，则可以在此处为变量（key）赋值（value）。
                        "md_text": md_text.length > 0 ? md_text : "暂无申请"
                    }
            }
        };
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'chat_id',
            },
            data: {
                receive_id: msg.event.message.chat_id,
                msg_type: 'interactive',
                content: JSON.stringify(content),
            },
        });
    }

    async doSendDocumentList(msg: MessageEvent) {
        await this.sendDocumentList(msg.event.message.chat_id, 1);
    }

    async doGetSubscriptionExpiryList(msg: MessageEvent) {
        // 获取申购到期列表
        let result = await LocalRemote.getSubscriptionExpiryList({
            page: 1,
            pageSize: 10,
        });

        let elements = [];
        elements.push({
            tag: "div",
            text: {
                tag: "lark_md",
                content: `**申购到期列表**`
            }
        });

        if (result.rows.length === 0) {
            elements.push({
                tag: "div",
                text: {
                    tag: "plain_text",
                    content: "暂无"
                }
            });
        } else {
            for (let item of result.rows) {
                let closeTimeStr = TimeUtil.format(item.CloseTime);
                const roundVolume = Number(item.RoundVolume);
                let needSubscribeVolume = roundVolume - item.SubscribeVolume > 0 ? roundVolume - item.SubscribeVolume : 0;
                let percent = item.SubscribeVolume / roundVolume * 100;
                elements.push({
                    tag: "hr"
                });
                let md_text = `
**ID:** ${item.Id}
**Currency:** ${item.Currency}
**申购进度:** ${item.SubscribeVolume} / ${roundVolume}
**申购占比:** ${percent.toFixed(2)}%
**需补充:** ${needSubscribeVolume}
**结束时间:** ${closeTimeStr}`.trim();
                elements.push({
                    tag: "div",
                    text: {
                        tag: "lark_md",
                        content: md_text
                    }
                });
            }
        }

        let buttons = [];
        // 按钮跳转 https://applink.larksuite.com/T93rmrxTFis7
        buttons.push({
            tag: "button",
            text: {
                tag: "plain_text",
                content: "申购审批"
            },
            url: "https://applink.larksuite.com/T93rmrxTFis7",
            type: "primary"
        });
        elements.push({
            tag: "action",
            actions: buttons
        });

        const cardContent = {
            config: {
                wide_screen_mode: true
            },
            elements: elements
        };

        await App.BotUtil.sendChatCardMsg(msg.event.message.chat_id, cardContent);

    }

    private async sendDocumentList(chatId: string, page: number, pageSize: number = 3) {
        const documentTypeMap = {
            1: "新闻",
            2: "公告",
            100: "行业热点",
            200: "新卡上线",
            201: "卡片下线",
        };
        let documentList = await LocalRemote.getAnnouncementListAndCount({
            type: null,
            subType: null,
            page: page,
            pageSize: pageSize,
        });
        const totalPages = Math.ceil(documentList.count / pageSize);

        let elements = [];

        elements.push({
            tag: "div",
            text: {
                tag: "lark_md",
                content: `**文档列表** (第 ${page}/${totalPages} 页) 共 ${documentList.count} 条`
            }
        });

        if (documentList.rows.length === 0) {
            elements.push({
                tag: "div",
                text: {
                    tag: "plain_text",
                    content: "暂无文档"
                }
            });
        } else {
            for (let document of documentList.rows) {
                let updateTimeStr = TimeUtil.format(document.createdAt);
                elements.push({
                    tag: "hr"
                });
                elements.push({
                    tag: "div",
                    text: {
                        tag: "lark_md",
                        content: `**DocumentID:** ${document.Id}\n**Title:** ${document.Title}\n**Type:** ${documentTypeMap[document.Type]}${document.SubType ? ' > ' + documentTypeMap[document.SubType] : ''}\n**DocUrl:** ${document.DocUrl}\n**Time:** ${updateTimeStr}`
                    }
                });
            }
        }

        elements.push({
            tag: "hr"
        });

        const buttons = [];
        buttons.push({
            tag: "button",
            text: {
                tag: "plain_text",
                content: `第${page - 1}页`
            },
            type: "primary",
            value: {
                func: "documentList",
                action: "page",
                page: page - 1,
            },
            disabled: page === 1
        });

        buttons.push({
            tag: "button",
            text: {
                tag: "plain_text",
                content: `第${page + 1}页`
            },
            type: "primary",
            value: {
                func: "documentList",
                action: "page",
                page: page + 1,
            },
            disabled: page === totalPages
        });

        elements.push({
            tag: "action",
            actions: buttons
        });

        const cardContent = {
            config: {
                wide_screen_mode: true
            },
            elements: elements
        };

        await App.BotUtil.sendChatCardMsg(chatId, cardContent);
    }

}