import {BaseClass} from '../core/BaseClass';

/**
 * 事件监听器接口
 */
export interface IEventListener {
    /**
     * 事件处理方法
     * @param eventData 事件数据
     * @returns 是否取消后续事件处理
     */
    handle(eventData: any): boolean | Promise<boolean>;
}

/**
 * 事件监听器包装类，包含监听器和优先级
 */
export class EventListenerWrapper {
    constructor(
        public listener: IEventListener,
        public priority: number = 0
    ) {}
}

/**
 * 事件总线类，用于事件的发布和订阅
 */
export class EventBus extends BaseClass {
    private eventListeners: Map<string, EventListenerWrapper[]> = new Map();
    
    /**
     * 订阅事件
     * @param eventType 事件类型
     * @param listener 事件监听器
     * @param priority 事件优先级，数字越大优先级越高
     */
    public subscribe(eventType: string, listener: IEventListener, priority: number = 0): void {
        if (!this.eventListeners.has(eventType)) {
            this.eventListeners.set(eventType, []);
        }
        
        const listeners = this.eventListeners.get(eventType)!;
        listeners.push(new EventListenerWrapper(listener, priority));
        
        // 按优先级排序
        listeners.sort((a, b) => b.priority - a.priority);
    }
    
    /**
     * 取消订阅事件
     * @param eventType 事件类型
     * @param listener 事件监听器
     */
    public unsubscribe(eventType: string, listener: IEventListener): void {
        if (!this.eventListeners.has(eventType)) {
            return;
        }
        
        const listeners = this.eventListeners.get(eventType)!;
        const index = listeners.findIndex(wrapper => wrapper.listener === listener);
        if (index !== -1) {
            listeners.splice(index, 1);
        }
        
        // 如果事件类型没有监听器了，移除该事件类型
        if (listeners.length === 0) {
            this.eventListeners.delete(eventType);
        }
    }
    
    /**
     * 发布事件
     * @param eventType 事件类型
     * @param eventData 事件数据
     * @returns 是否有监听器处理了事件
     */
    public async publish(eventType: string, eventData: any): Promise<boolean> {
        if (!this.eventListeners.has(eventType)) {
            return false;
        }
        
        const listeners = this.eventListeners.get(eventType)!;
        let isHandled = false;
        
        for (const wrapper of listeners) {
            try {
                // 调用监听器的handle方法
                const result = await wrapper.listener.handle(eventData);
                isHandled = true;
                
                // 如果监听器返回true，表示取消后续事件处理
                if (result) {
                    break;
                }
            } catch (error) {
                console.error(`Event listener error for event ${eventType}:`, error);
            }
        }
        
        return isHandled;
    }
    
    /**
     * 发布同步事件
     * @param eventType 事件类型
     * @param eventData 事件数据
     * @returns 是否有监听器处理了事件
     */
    public publishSync(eventType: string, eventData: any): boolean {
        if (!this.eventListeners.has(eventType)) {
            return false;
        }
        
        const listeners = this.eventListeners.get(eventType)!;
        let isHandled = false;
        
        for (const wrapper of listeners) {
            try {
                // 调用监听器的handle方法
                const result = wrapper.listener.handle(eventData);
                isHandled = true;
                
                // 如果监听器返回true，表示取消后续事件处理
                if (result) {
                    break;
                }
            } catch (error) {
                console.error(`Event listener error for event ${eventType}:`, error);
            }
        }
        
        return isHandled;
    }
    
    /**
     * 获取事件类型的监听器数量
     * @param eventType 事件类型
     * @returns 监听器数量
     */
    public getListenerCount(eventType: string): number {
        if (!this.eventListeners.has(eventType)) {
            return 0;
        }
        return this.eventListeners.get(eventType)!.length;
    }
    
    /**
     * 清除所有事件监听器
     */
    public clearAllListeners(): void {
        this.eventListeners.clear();
    }
    
    /**
     * 清除指定事件类型的所有监听器
     * @param eventType 事件类型
     */
    public clearListeners(eventType: string): void {
        this.eventListeners.delete(eventType);
    }
}