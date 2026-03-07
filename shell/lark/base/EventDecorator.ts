import {App} from "./App";
import {EventHandler as EventHandlerType} from "./EventBus";
import * as fs from "fs";
import * as path from "path";

// 存储需要注册的事件处理函数信息
interface EventHandlerInfo {
    eventName: string;
    methodName: string;
    priority: number;
}

// 存储类的事件处理函数信息的映射
const classEventHandlersMap = new Map<Function, EventHandlerInfo[]>();

// 存储已创建的类实例，以便复用
const classInstanceMap = new Map<Function, any>();

/**
 * 事件处理装饰器，用于自动注册事件处理函数
 * @param eventName 事件名称
 * @param priority 事件优先级，数字越大优先级越高，默认 0
 * @returns 装饰器函数
 */
export function EventHandler(eventName: string, priority: number = 0) {
    return function (target: any, propertyKey: string, descriptor: PropertyDescriptor) {
        // 获取类的构造函数
        const constructor = target.constructor;

        // 如果该类还没有事件处理函数信息，创建一个空数组
        if (!classEventHandlersMap.has(constructor)) {
            classEventHandlersMap.set(constructor, []);
        }

        // 添加事件处理函数信息
        const eventHandlers = classEventHandlersMap.get(constructor)!;
        eventHandlers.push({
            eventName,
            methodName: propertyKey,
            priority
        });
    };
}

/**
 * 注册所有事件处理函数
 */
export function registerAllEventHandlers(): void {
    // 扫描 handler 文件夹，自动导入所有装饰器
    const handlerDir = path.join(__dirname, "../handler");

    // 检查 handler 文件夹是否存在
    if (fs.existsSync(handlerDir)) {
        // 读取 handler 文件夹中的所有文件
        const files = fs.readdirSync(handlerDir);

        // 导入所有 .ts 或 .js 文件（支持 TypeScript 和编译后的 JavaScript）
        for (const file of files) {
            if ((file.endsWith(".ts") || file.endsWith(".js"))) {
                // 构建文件路径
                const filePath = path.join(handlerDir, file);
                // 动态导入文件，添加异常处理
                try {
                    require(filePath);
                } catch (error) {
                    console.error(`加载事件处理文件失败: ${filePath}`, error);
                }
            }
        }
    }

    // 遍历所有类的事件处理函数信息
    for (const [constructor, handlers] of classEventHandlersMap) {
        // 检查是否已有实例，如果没有则创建新实例
        let instance = classInstanceMap.get(constructor);
        if (!instance) {
            // 创建类的实例，使用类型断言解决 TypeScript 编译错误
            instance = new (constructor as new () => any)();
            // 存储实例以便后续复用
            classInstanceMap.set(constructor, instance);
        }

        // 注册每个事件处理函数
        for (const {eventName, methodName, priority} of handlers) {
            // 绑定实例到方法，确保 this 指向正确
            // 即使复用实例，仍需要 bind 来确保事件回调中的 this 指向正确的实例
            const handler = instance[methodName].bind(instance);
            // 注册事件处理函数
            App.eventBus.subscribe(eventName, handler as EventHandlerType, priority);
        }
    }
    let eventNames = App.eventBus.getEventNames();
    for (let eventName of eventNames) {
        console.log(`[EventBus] 事件: ${eventName} 注册处理函数，当前处理器数量: ${App.eventBus.getHandlers(eventName)?.length}`);
    }
}