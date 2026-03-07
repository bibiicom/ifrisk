export class BaseClass {
    private static _instance: any;

    public constructor() {

    }

    /**
     * 获取一个单例
     * @returns {any}
     */
    public static getInstance(...args: any[]): any {
        let Class: any = this;
        if (!this._instance) {
            this._instance = new Class(...args);
            this._instance?.afterInstance()
        }
        return this._instance;
    }

    public afterInstance() {

    }
}
