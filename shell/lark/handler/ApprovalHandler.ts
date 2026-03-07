import {EventHandler} from "../base/EventDecorator";
import {App} from "../base/App";
import {LocalRemote} from "../remote/LocalRemote";
import * as fs from 'fs';
import * as path from 'path';
import {TimeUtil} from "../tool/TimeUtil";
import {ApprovalEvent} from "../const/LarkInterface";

/**
 * 审批创建 https://www.larksuite.com/approval/admin/approvalList
 * 订阅审批事件 https://open.larksuite.com/document/server-docs/approval-v4/event/event-interface/subscribe
 * 查看指定审批定义 https://open.larksuite.com/document/server-docs/approval-v4/approval/get
 */
export class ApprovalHandler {
    /**
     * 处理审批实例事件
     * @param msg 事件数据
     */
    @EventHandler("approval_instance")
    async onApprovalInstance(msg: ApprovalEvent) {
        try {

            // console.log("审批通过:", msg);
            // 获取审批实例内容
            let res = await App.LarkClient.approval.v4.instance.get({
                path: {
                    instance_id: msg.event.instance_code,
                }
            });
            // console.log("审批实例内容:", res.data);
            msg.instanceData = res.data;
            msg.instanceData.parseForm = JSON.parse(res.data.form);

            // await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, res.data.form);

            if (msg.event.status == "APPROVED" && msg.event.approval_code == "FBEF9B44-17D1-4E4F-8845-F0924783AB58") {
                await this.doAddLaunchpadInfo(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "A8458BFB-7DFD-4852-AD8B-AEDD1915A86B") {
                await this.doUpdateLaunchpadInfo(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "1949DFD8-01C1-4143-8341-0E00DE816347") {
                await this.doCardStorageApply(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "8486FA98-5139-4F9E-A9CC-6FF592CFB9E4") {
                await this.doOutboundApply(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "F7ACD394-B93C-49F2-B1EE-8EA66884C3E7") {
                // 导出文档审批
                await this.doExportDocument(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "31F7BA8B-05AD-43D7-A16C-4BB15665C2D6") {
                // 更新文档审批
                await this.doUpdateDocument(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "76B27D34-74AC-46AB-B635-3BA0E8E53FC4") {
                // 删除文档审批
                await this.doDeleteDocument(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "AD3C37BC-C98A-483D-BA33-F34C0BB5C8BF") {
                // 拒绝审批
                await this.doRejectApply(msg);
            } else if (msg.event.status == "APPROVED" && msg.event.approval_code == "80D9F36B-919C-4F12-A060-0FB446000D09") {
                // 申购审批
                await this.doSubscriptionApproval(msg);
            }
        } catch (e) {
            console.error("审批实例处理失败:", e);
        }
    }

    async doCardStorageApply(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "accountId",
            "widget17670830047090001": "valuation",
            "widget17670841968790001": "img",
            "widget17675928577120001": "name",
            "widget17694954083810001": "imgGroup",
            "widget17694955128760001": "description",
            "widget17694955637110001": "certNumber",
            "widget17694956193930001": "itemGrade",
            "widget17694957150420001": "year",
            "widget17694957396990001": "brandTitle",
            "widget17694957632200001": "subject",
            "widget17694957878240001": "cardNumber",
            "widget17694958035080001": "varietyPedigree",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            payload[key] = parseElement.value;
        }
        let replayMsg: string;
        try {
            let cardModel = await LocalRemote.addCard(payload);
            replayMsg = `添加卡片成功\naccountId: ${cardModel.Owner}\ncardId: ${cardModel.Id}`;
        } catch (e) {
            console.error("添加卡片失败:", e);
            replayMsg = "添加卡片失败:" + e.message;
        }

        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, replayMsg);
    }


    async doAddLaunchpadInfo(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17664713202350001": "cardId",
            "widget17661231905520001": "currency",
            "widget17664714347190001": "name",
            "widget17664714673110001": "volume",
            "widget17664714678320001": "price",
            "widget17661233361110001": "openTime|closeTime",
            // "widget17664716110760001": "tradeTime",
            "widget17664716522580001": "summary",
            "widget17683603233730001": "tradeStandardPct",
            "widget17696730618390001": "haltStandardPct",
            "widget17683604447300001": "feePct",
            "widget17683604849070001": "minPrice",
            "widget17683604873380001": "maxPrice",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            if (key == "openTime|closeTime") {
                let [startKey, endKey] = key.split('|');
                // 转换为时间戳
                payload[startKey] = new Date(parseElement.value.start).getTime();
                payload[endKey] = new Date(parseElement.value.end).getTime();
            } else {
                payload[key] = parseElement.value;
            }
        }
        let replayMsg: string;
        try {
            let launchpadInfo = await LocalRemote.addLaunchpadInfo(payload);
            replayMsg = "添加launchpad信息成功,Id:" + launchpadInfo.launchpad.Id;
        } catch (e) {
            console.error("添加launchpad信息失败:", e);
            replayMsg = "添加launchpad信息失败:" + e.message;
        }
        // 给用户发送消息
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'user_id',
            },
            data: {
                receive_id: msg.instanceData.user_id,
                msg_type: 'text',
                content: JSON.stringify({
                    text: replayMsg
                }),
            },
        });
    }

    async doUpdateLaunchpadInfo(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17664713202350001": "launchpadId",
            "widget17665550229780001": "closeTime",
            // "widget17664716110760001": "tradeTime",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            if (["closeTime"].includes(key)) {
                payload[key] = new Date(parseElement.value).getTime();
            } else {
                payload[key] = parseElement.value;
            }
        }
        let replayMsg: string;
        try {
            let launchpadInfo = await LocalRemote.updateLaunchpadInfo(payload);
            replayMsg = "更新launchpad信息成功,Id:" + launchpadInfo.Id;
        } catch (e) {
            console.error("更新launchpad信息失败:", e);
            replayMsg = "更新launchpad信息失败:" + e.message;
        }
        // 给用户发送消息
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'user_id',
            },
            data: {
                receive_id: msg.instanceData.user_id,
                msg_type: 'text',
                content: JSON.stringify({
                    text: replayMsg
                }),
            },
        });
    }

    async doOutboundApply(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "applyId",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            payload[key] = parseElement.value;
        }
        let replayMsg: string;
        try {
            let outboundResult = await LocalRemote.doOutboundApply(payload);
            replayMsg = "出库申请成功,Id:" + outboundResult.Id;
        } catch (e) {
            console.error("出库申请失败:", e);
            replayMsg = "出库申请失败:" + e.message;
        }

        // 给用户发送消息
        await App.LarkClient.im.message.create({
            params: {
                receive_id_type: 'user_id',
            },
            data: {
                receive_id: msg.instanceData.user_id,
                msg_type: 'text',
                content: JSON.stringify({
                    text: replayMsg
                }),
            },
        });
    }

    async doExportDocument(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "title",
            "widget17679384529100001": "type",
            "widget17688881206730001": "subType",
            "widget17679384589320001": "docUrl",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            if (key == "type") {
                switch (parseElement.value) {
                    case "新闻":
                        payload[key] = 1;
                        break;
                    case "公告":
                        payload[key] = 2;
                        break;
                    default:
                        return;
                }
            } else if (key == "subType") {
                switch (parseElement.value) {
                    case "行业热点":
                        payload[key] = 100;
                        break;
                    case "新卡上线":
                        payload[key] = 200;
                        break;
                    case "卡片下线":
                        payload[key] = 201;
                        break;
                    default:
                        payload[key] = 0;
                }
            } else {
                payload[key] = parseElement.value;
            }
        }

        const urlObj = new URL(payload.docUrl);
        const pathParts = urlObj.pathname.split('/').filter(p => p);
        const wikiToken = pathParts[1]; // 文档token是路径的第二部分
        if (!wikiToken) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, "无法从链接中提取文档信息");
            return;
        }
        // 使用 wikiToken 获取文档内容
        const contentRes = await App.LarkClient.docs.v1.content.get({
            params: {
                doc_token: wikiToken,
                doc_type: "docx",
                content_type: "markdown"
            }
        });

        if (contentRes.code !== 0) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, `获取文档内容失败: ${contentRes.msg}`);
            return;
        }
        let mdContent = contentRes.data.content;
        await LocalRemote.addAnnouncement({
            title: payload.title,
            content: mdContent,
            fileName: "",
            docUrl: payload.docUrl,
            type: payload.type,
            subType: payload.subType,
            priority: 1,
        });

        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, `${payload.title} 添加成功, 文档链接: ${payload.docUrl}`);

    }

    async doUpdateDocument(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "id",
            "widget17679388819070001": "title",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            payload[key] = parseElement.value;
        }
        // 获取公告详情
        let announcementDetail = await LocalRemote.getAnnouncementDetail({
            announcementId: payload.id,
        });
        if (!announcementDetail) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, "文章不存在");
            return;
        }
        const urlObj = new URL(announcementDetail.DocUrl);
        const pathParts = urlObj.pathname.split('/').filter(p => p);
        const wikiToken = pathParts[1]; // 文档token是路径的第二部分
        if (!wikiToken) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, "无法从链接中提取文档信息");
            return;
        }
        // 使用 wikiToken 获取文档内容
        const contentRes = await App.LarkClient.docs.v1.content.get({
            params: {
                doc_token: wikiToken,
                doc_type: "docx",
                content_type: "markdown"
            }
        });

        if (contentRes.code !== 0) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, `获取文档内容失败: ${contentRes.msg}`);
            return;
        }
        let mdContent = contentRes.data.content;

        await LocalRemote.updateAnnouncement({
            announcementId: payload.id,
            title: payload.title,
            content: mdContent,
        });

        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, `${announcementDetail.Title} 更新成功, 文档链接: ${announcementDetail.DocUrl}`);

    }


    async doDeleteDocument(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "id",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            payload[key] = parseElement.value;
        }
        // 获取公告详情
        let announcementDetail = await LocalRemote.getAnnouncementDetail({
            announcementId: payload.id,
        });
        if (!announcementDetail) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, "文章不存在");
            return;
        }
        await LocalRemote.deleteAnnouncement({
            announcementId: payload.id,
        });
        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, `${announcementDetail.Title} 删除成功, 文档链接: ${announcementDetail.DocUrl}`);

    }

    async doRejectApply(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "applyId",
            "widget17684444012610001": "type",
        };
        let typeName: string;
        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            if (key === "type") {
                typeName = parseElement.value;
                switch (typeName) {
                    case "入库":
                        payload[key] = 1;
                        break;
                    case "上架":
                        payload[key] = 2;
                        break;
                    case "出库":
                        payload[key] = 3;
                        break;
                }
                continue;
            }
            payload[key] = parseElement.value;
        }

        if (!payload.type) {
            await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, "申请类型不存在");
            return;
        }

        let replayMsg: string;
        try {
            await LocalRemote.rejectApply(payload);
            replayMsg = `拒绝${typeName}申请成功, 申请ID: ${payload.applyId}`;
        } catch (e) {
            console.error(`拒绝${typeName}申请失败:`, e);
            replayMsg = `拒绝${typeName}申请失败, 申请ID: ${payload.applyId}`;
        }

        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, replayMsg);
    }

    async doSubscriptionApproval(msg: ApprovalEvent) {
        const id2KeyMap = {
            "widget17670829482210001": "launchpadId",
            "widget17684444012610001": "select",
            "widget17694998545920001": "exchangeVolume",
            "widget17695053839870001": "exchangeTotalPrice",
            "widget17694998561210001": "cardholderVolume",
            "widget17697619229720001": "tradeTime",
        };

        let payload: any = {};
        for (let parseElement of msg.instanceData.parseForm) {
            let key = id2KeyMap[parseElement.id];
            if (!key) {
                continue;
            }
            payload[key] = parseElement.value;
        }

        let replayMsg: string;
        try {
            await LocalRemote.subscriptionApproval(payload);
            replayMsg = "申购审批处理成功";
        } catch (e) {
            console.error("申购审批处理失败:", e);
            replayMsg = "申购审批处理失败:" + e.message;
        }

        // 给用户发送消息
        await App.BotUtil.sendPrivateMsg(msg.instanceData.user_id, replayMsg);
    }

    async handleExport(docUrl: string) {
        let res = {
            isSuccess: false,
            msg: "",
            file: "",
        }
        // "docx" | "pdf" | "xlsx" | "csv"
        const fileExtension = "docx";
        try {
            if (!docUrl) {
                res.msg = "文档链接不能为空";
                return res;
            }

            // 从文档链接中提取 wiki token 和 doc_type
            let wikiToken: string;
            let linkDocType: "doc" | "sheet" | "bitable" | "mindnote" | "file" | "wiki" | "docx" | "folder" | "synced_block" | "slides";

            const urlObj = new URL(docUrl);
            const pathParts = urlObj.pathname.split('/').filter(p => p);
            linkDocType = pathParts[0] as any; // 第一部分是 doc_type，如 wiki、doc、sheet 等
            wikiToken = pathParts[1]; // 最后一部分是 token
            if (!wikiToken || !linkDocType) {
                res.msg = "无法从链接中提取文档信息";
                return res;
            }
            console.log("从链接提取 wiki token:", wikiToken, "doc_type:", linkDocType);

            // 获取文档元数据以获取真正的 doc_token
            const metaRes = await App.LarkClient.drive.meta.batchQuery({
                data: {
                    request_docs: [
                        {
                            doc_token: wikiToken,
                            doc_type: linkDocType
                        }
                    ],
                    with_url: false
                }
            });

            if (metaRes.code !== 0 || !metaRes.data?.metas || metaRes.data.metas.length === 0) {
                res.msg = `获取文档元数据失败: ${metaRes.msg}`;
                return res;
            }

            const meta = metaRes.data.metas[0];
            const docToken = meta.doc_token;
            const docType = meta.doc_type;
            console.log("获取到文档 token:", docToken, "类型:", docType);

            // 创建导出任务
            const createRes = await App.LarkClient.drive.exportTask.create({
                data: {
                    token: docToken,
                    type: docType as any,
                    file_extension: fileExtension,
                }
            });

            if (createRes.code !== 0 || !createRes.data?.ticket) {
                res.msg = `创建导出任务失败: ${createRes.msg}`;
                return res;
            }

            const ticket = createRes.data.ticket;
            console.log("导出任务创建成功, ticket:", ticket);

            // 轮询查询导出任务状态
            let exportResult = null;
            const maxRetries = 30;
            const pollInterval = 4000;

            for (let i = 0; i < maxRetries; i++) {
                await TimeUtil.sleep(pollInterval);

                const getRes = await App.LarkClient.drive.exportTask.get({
                    path: {
                        ticket: ticket,
                    },
                    params: {
                        token: docToken,
                    }
                });

                if (getRes.code !== 0) {
                    res.msg = `查询导出任务失败: ${getRes.msg}`;
                    return res;
                }

                const result = getRes.data?.result;
                if (!result) {
                    res.msg = "导出任务结果为空";
                    return res;
                }
                console.log(`导出任务状态: ${result.job_status}, 尝试: ${i + 1}/${maxRetries}`);

                if (result.job_status === 0) {
                    exportResult = result;
                    break;
                } else if (result.job_status !== 1 && result.job_status !== 2) {
                    res.msg = `导出任务失败: ${result.job_error_msg}`;
                    return res;
                }
            }

            if (!exportResult) {
                res.msg = "导出任务超时，请稍后重试";
                return res;
            }
            console.log("导出任务完成, file_token:", exportResult.file_token);

            // 下载导出文件
            const downloadRes = await App.LarkClient.drive.exportTask.download({
                path: {
                    file_token: exportResult.file_token,
                }
            });

            // 创建导出文件夹
            const exportDir = path.join(process.cwd(), 'exports');
            if (!fs.existsSync(exportDir)) {
                fs.mkdirSync(exportDir, {recursive: true});
            }

            // 保存文件
            const fileName = `${exportResult.file_name}_${Date.now()}.${fileExtension}`;
            const filePath = path.join(exportDir, fileName);
            await downloadRes.writeFile(filePath);
            res.isSuccess = true;
            res.file = fileName;
            return res;

        } catch (e) {
            console.error("文档导出失败:", e);
            res.msg = "文档导出失败:" + e.message;
            return res;
        }
    }
}
