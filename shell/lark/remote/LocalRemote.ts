import {HttpRequest} from "../tool/HttpRequest";
import {BusinessError} from "../const/BusinessError";
import {App} from "../base/App";

export class LocalRemote {

    static getUrl(path: string) {
        const serverConfig = App.configManager.getServer();
        return serverConfig.localHost + path;
    }

    // 获取申请卡片列表
    static async getApplyCardList(): Promise<{
        Id: number,
        Contract: string,
        TokenID: string,
        Name: string,
        Owner: string,
        createdAt: string,
        updatedAt: string,
    }[]> {
        let url = this.getUrl("/backend/v1/lark/getApplyCardList");
        const payload = {};
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return [];
        }
        return res.data;
    }

    // 添加launchpad信息
    static async addLaunchpadInfo(payload: {
        currency: string,
        name: string,
        cardId: string,
        volume: string,
        price: string,
        tradeStandardPct: string,
        haltStandardPct: string,
        feePct: string,
        minPrice?: string,
        maxPrice?: string,
        openTime: number,
        closeTime: number,
        // tradeTime: string,
        summary: string,
    }): Promise<{ launchpad: any, cardModel: any }> {
        let url = this.getUrl("/backend/v1/lark/addLaunchpadInfo");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 更新launchpad信息
    static async updateLaunchpadInfo(payload: {
        launchpadId: string,
        closeTime: string,
        // tradeTime: string,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/updateLaunchpadInfo");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 添加卡片
    static async addCard(payload: {
        accountId: string,
        valuation: string,
        name: string,
        img: string,
        imgGroup?: string,
        description?: string,
        certNumber?: string,
        itemGrade?: string,
        year?: string,
        brandTitle?: string,
        subject?: string,
        cardNumber?: string,
        varietyPedigree?: string,
        status?: number,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/addCard");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    static async getStorageApplyCardList() {
        let url = this.getUrl("/backend/v1/lark/getStorageApplyCardList");
        const payload = {};
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return [];
        }
        return res.data;
    }

    // 获取出库申请卡片列表
    static async getOutboundApplyCardList() {
        let url = this.getUrl("/backend/v1/lark/getOutboundApplyCardList");
        const payload = {};
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return [];
        }
        return res.data;
    }

    // 处理出库申请
    static async doOutboundApply(payload: {
        applyId: string,
        status?: number,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/doOutboundApply");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 新增公告
    static async addAnnouncement(payload: {
        title: string,
        content: string,
        fileName: string,
        docUrl: string,
        type: number,
        subType: number,
        priority: number,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/addAnnouncement");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 更新公告
    static async updateAnnouncement(payload: {
        announcementId: string,
        title?: string,
        content?: string,
        fileName?: string,
        docUrl?: string,
        type?: number,
        subType?: number,
        status?: string,
        priority?: number,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/updateAnnouncement");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 删除公告
    static async deleteAnnouncement(payload: {
        announcementId: string,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/deleteAnnouncement");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("填写有误");
        }
        return res.data;
    }

    // 获取公告列表
    static async getAnnouncementListAndCount(payload: {
        type?: string,
        subType?: string,
        page?: number,
        pageSize?: number,
    }): Promise<{
        count: number,
        rows: any[],
    }> {
        let url = this.getUrl("/backend/v1/lark/getAnnouncementListAndCount");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return {
                count: 0,
                rows: [],
            };
        }
        return res.data;
    }

    // 获取公告详情
    static async getAnnouncementDetail(payload: {
        announcementId: string,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/getAnnouncementDetail");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return;
        }
        return res.data;
    }

    // 拒绝申请
    static async rejectApply(payload: {
        applyId: string,
        type: number,
    }): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/rejectApply");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("拒绝失败");
        }
        return res.data;
    }

    // 获取申购到期列表
    static async getSubscriptionExpiryList(payload: {
        page?: number,
        pageSize?: number,
    }): Promise<{
        count: number,
        rows: any[],
    }> {
        let url = this.getUrl("/backend/v1/lark/getSubscriptionExpiryList");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return {
                count: 0,
                rows: [],
            };
        }
        return res.data;
    }

    // 申购审批
    static async subscriptionApproval(payload: any): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/subscriptionApproval");
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            throw BusinessError.newMsg("申购审批失败");
        }
        return res.data;
    }

    // 自动任务：删除空 Launchpad
    static async deleteEmptyLaunchpads(): Promise<any> {
        let url = this.getUrl("/backend/v1/lark/deleteEmptyLaunchpads");
        const res = await HttpRequest.post(url, {});
        if (!res || res.code !== 0) {
            // throw BusinessError.newMsg("清理失败");
            return { deletedCount: 0 };
        }
        return res.data;
    }
    // 获取申请卡片列表
    static async getLarkCardList(): Promise<any[]> {
        let url = this.getUrl("/backend/v1/lark/getLarkCardList");
        const payload = {};
        const res = await HttpRequest.post(url, payload);
        if (!res || res.code !== 0) {
            return [];
        }
        return res.data;
    }
}
