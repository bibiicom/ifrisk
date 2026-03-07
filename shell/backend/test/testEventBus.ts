import {App} from '../base/App';
import {EventType} from '../const/EventTypes';
import {CardLaunchedListener} from '../base/listener/CardLaunchedListener';
import {IEventListener} from '../base/event/EventBus';

/**
 * 事件系统测试脚本
 */
async function testEventBus() {
    console.log('开始测试事件系统...');
    
    // 测试1: 验证EventBus单例是否正常获取
    console.log('\n=== 测试1: 验证EventBus单例是否正常获取 ===');
    const eventBus = App.EventBus;
    console.log('EventBus实例获取成功:', eventBus);
    
    // 测试2: 验证事件监听器注册
    console.log('\n=== 测试2: 验证事件监听器注册 ===');
    // 创建自定义监听器
    class TestListener implements IEventListener {
        public handle(eventData: any): boolean {
            console.log('自定义监听器收到事件:', eventData);
            return false;
        }
    }
    
    const testListener = new TestListener();
    const cardLaunchedListener = new CardLaunchedListener();
    
    // 注册监听器
    App.EventBus.subscribe(EventType.CARD_LAUNCHED, testListener, 5);
    App.EventBus.subscribe(EventType.CARD_LAUNCHED, cardLaunchedListener, 10);
    
    // 检查监听器数量
    const listenerCount = App.EventBus.getListenerCount(EventType.CARD_LAUNCHED);
    console.log(`CARD_LAUNCHED事件监听器数量: ${listenerCount}`);
    
    // 测试3: 验证事件发布
    console.log('\n=== 测试3: 验证事件发布 ===');
    // 模拟卡片上架事件数据
    const eventData = {
        cardId: 1,
        cardName: '测试卡片',
        cardSymbol: 'TEST',
        launchpadId: 1,
        openTime: new Date(),
        closeTime: new Date(Date.now() + 3600000),
        tradeTime: new Date(Date.now() + 1800000),
        summary: '这是一个测试卡片',
        imgUrl: 'https://example.com/test.jpg'
    };
    
    // 发布事件
    const isHandled = await App.EventBus.publish(EventType.CARD_LAUNCHED, eventData);
    console.log(`事件发布结果: ${isHandled ? '已处理' : '未处理'}`);
    
    // 测试4: 验证事件监听器取消订阅
    console.log('\n=== 测试4: 验证事件监听器取消订阅 ===');
    App.EventBus.unsubscribe(EventType.CARD_LAUNCHED, testListener);
    const listenerCountAfterUnsubscribe = App.EventBus.getListenerCount(EventType.CARD_LAUNCHED);
    console.log(`取消订阅后监听器数量: ${listenerCountAfterUnsubscribe}`);
    
    // 再次发布事件
    const isHandledAfterUnsubscribe = await App.EventBus.publish(EventType.CARD_LAUNCHED, eventData);
    console.log(`取消订阅后事件发布结果: ${isHandledAfterUnsubscribe ? '已处理' : '未处理'}`);
    
    console.log('\n=== 事件系统测试完成 ===');
}

// 执行测试
setTimeout(() => {
    testEventBus().catch(console.error);
}, 1000);