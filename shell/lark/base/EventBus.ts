// 事件处理函数类型
export interface EventHandler<T = any> {
    (data: T): Promise<void> | void;
}

// 带有优先级的事件处理器
interface PriorityEventHandler<T = any> {
    handler: EventHandler<T>;
    priority: number;
}

// 事件总线类
export class EventBus {
    // 事件映射，键为事件名称，值为事件处理器数组
    private eventHandlers: Map<string, PriorityEventHandler[]> = new Map();

    /**
     * 订阅事件
     * @param eventName 事件名称
     * @param handler 事件处理函数
     * @param priority 优先级，数字越大优先级越高，默认 0
     */
    subscribe<T = any>(eventName: string, handler: EventHandler<T>, priority: number = 0): void {
        if (!this.eventHandlers.has(eventName)) {
            this.eventHandlers.set(eventName, []);
        }

        const handlers = this.eventHandlers.get(eventName)!;
        handlers.push({ handler, priority });

        // 按优先级排序，优先级高的先执行
        handlers.sort((a, b) => b.priority - a.priority);
    }

    /**
     * 取消订阅事件
     * @param eventName 事件名称
     * @param handler 事件处理函数
     */
    unsubscribe<T = any>(eventName: string, handler: EventHandler<T>): void {
        if (!this.eventHandlers.has(eventName)) {
            return;
        }

        const handlers = this.eventHandlers.get(eventName)!;
        const index = handlers.findIndex(h => h.handler === handler);
        if (index !== -1) {
            handlers.splice(index, 1);
        }

        // 如果事件没有处理器了，删除该事件
        if (handlers.length === 0) {
            this.eventHandlers.delete(eventName);
        }
    }

    /**
     * 发布事件
     * @param eventName 事件名称
     * @param data 事件数据
     */
    async publish<T = any>(eventName: string, data: T): Promise<void> {
        if (!this.eventHandlers.has(eventName)) {
            console.log(`EventBus: No handlers for event ${eventName}`);
            return;
        }

        const handlers = this.eventHandlers.get(eventName)!;
        
        // 逐个执行事件处理器，单个处理器失败不影响其他处理器
        for (const { handler } of handlers) {
            try {
                await handler(data);
            } catch (error) {
                console.error(`EventBus: Error handling event ${eventName}:`, error);
                // 可以在这里添加更多的错误处理逻辑，比如发送告警等
            }
        }
    }

    /**
     * 获取事件的所有处理器
     * @param eventName 事件名称
     * @returns 事件处理器数组
     */
    getHandlers<T = any>(eventName: string): EventHandler<T>[] {
        if (!this.eventHandlers.has(eventName)) {
            return [];
        }
        return this.eventHandlers.get(eventName)!.map(h => h.handler);
    }

    /**
     * 清除所有事件处理器
     */
    clear(): void {
        this.eventHandlers.clear();
    }

    /**
     * 清除指定事件的所有处理器
     * @param eventName 事件名称
     */
    clearEvent(eventName: string): void {
        this.eventHandlers.delete(eventName);
    }

    /**
     * 获取所有注册的事件名称
     * @returns 事件名称数组
     */
    getEventNames(): string[] {
        return Array.from(this.eventHandlers.keys());
    }

    /**
     * 批量订阅事件
     * @param events 事件映射，键为事件名称，值为事件处理函数
     */
    batchSubscribe(events: Record<string, EventHandler>): void {
        for (const [eventName, handler] of Object.entries(events)) {
            this.subscribe(eventName, handler);
        }
    }

    /**
     * 批量取消订阅事件
     * @param events 事件映射，键为事件名称，值为事件处理函数
     */
    batchUnsubscribe(events: Record<string, EventHandler>): void {
        for (const [eventName, handler] of Object.entries(events)) {
            this.unsubscribe(eventName, handler);
        }
    }
}
