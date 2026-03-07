import {LocalRemote} from "../remote/LocalRemote";

const TASK_TAG = "LarkAutoTask";
const INTERVAL_MS = 5 * 60 * 1000; // 5 minutes
const APPROVAL_INTERVAL_MS = 15 * 1000; // 15 seconds

/**
 * 自动同意申购定时任务
 * 处理已到期且有申购记录的 Launchpad，自动执行同意操作
 */
async function autoApproveSubscriptions() {
    console.log(`[AutoApproval] Checking subscription approvals at ${new Date().toISOString()}`);
    
    try {
        const result = await LocalRemote.getSubscriptionExpiryList({
            page: 1,
            pageSize: 100,
        });
        
        if (!result || !result.rows || result.rows.length === 0) {
            console.log(`[AutoApproval] No expired launchpads found`);
            return;
        }
        
        for (const launchpad of result.rows) {
            const subscribeVolume = launchpad.SubscribeVolume || 0;
            
            if (subscribeVolume <= 0) {
                console.log(`[AutoApproval] Launchpad ${launchpad.Id} has no subscriptions, skipping`);
                continue;
            }
            
            const roundVolume = Number(launchpad.RoundVolume) || 0;
            const remainingVolume = roundVolume - subscribeVolume;

            if (remainingVolume > 0) {
                // 有剩余量，分配所有量
                console.log(`[AutoApproval] Launchpad ${launchpad.Id} has remaining volume, approving all`);

                const halfVolume = Math.floor(remainingVolume / 2);
                const exchangeVolume = String(halfVolume);
                const cardholderVolume = String(remainingVolume - halfVolume);
                const exchangeTotalPrice = "0";
                const tradeTime = Date.now();

                console.log(`[AutoApproval] Auto-approving launchpad ${launchpad.Id}, ` +
                    `subscribeVolume: ${subscribeVolume}, remainingVolume: ${remainingVolume}, ` +
                    `exchangeVolume: ${exchangeVolume}, cardholderVolume: ${cardholderVolume}`);

                await LocalRemote.subscriptionApproval({
                    launchpadId: String(launchpad.Id),
                    select: "同意",
                    exchangeVolume,
                    exchangeTotalPrice,
                    cardholderVolume,
                    tradeTime,
                });
                continue;
            } else {
                // 无剩余量 直接同意
                await LocalRemote.subscriptionApproval({
                    launchpadId: String(launchpad.Id),
                    select: "同意",
                    tradeTime: Date.now(),
                });
            }

            
            console.log(`[AutoApproval] Successfully approved launchpad ${launchpad.Id}`);
        }
    } catch (e) {
        console.error("[AutoApproval] Task failed:", e);
    }
}

export function startSchedule() {
    console.log("[Schedule] Starting auto Launchpad task...");

    const runTask = async () => {
        try {
            console.log(`[Schedule] Executing task at ${new Date().toISOString()}`);

            // 1. Cleanup old empty launchpads
            // This will also reset the associated card's status to 1
            const cleanupRes = await LocalRemote.deleteEmptyLaunchpads();
            console.log(`[Schedule] Cleanup result:`, cleanupRes);

            // 2. Get Available Card (Filtered by Tag)
            // We only look for cards created by this task
            let cards = await LocalRemote.getLarkCardList();
            let cardId: number;
            let certNumber: string;

            if (cards.length > 0) {
                cardId = cards[0].Id;
                certNumber = cards[0].CertNumber;
                console.log(`[Schedule] Found existing reusable card: ${cardId}`);
            } else {
                console.log(`[Schedule] No available card found. Creating new one.`);
                let imgList = [
                    "https://d1htnxwo4o0jhw.cloudfront.net/cert/127544049/small/358072255.jpg",
                    "https://d1htnxwo4o0jhw.cloudfront.net/cert/127544049/small/358072258.jpg",
                ];
                // 打乱图片顺序
                // imgList.sort(() => Math.random() - 0.5);
                const newCard = await LocalRemote.addCard({
                    accountId: "0x2056bc10b0ae9a92ab28dc676053023dcd0d7ecc", // Dummy Owner
                    valuation: "10",
                    name: "2020 PANINI PRIZM DRAFT PICKS #98 LaMELO BALL",
                    img: imgList[0],
                    imgGroup: imgList.join("\n"),
                    description: TASK_TAG, // IMPORTANT: Tagging for isolation
                    certNumber: "64408819",
                    itemGrade: "GEM MT 10",
                    year: "2020",
                    brandTitle: "PANINI PRIZM DRAFT PICKS",
                    subject: "LaMELO BALL",
                    cardNumber: "98",
                    varietyPedigree: "AutoVariety",
                    status: 1, // IMPORTANT: Ready for Launchpad immediately
                });
                cardId = newCard.Id;
                certNumber = newCard.CertNumber;
                console.log(`[Schedule] Created new card: ${cardId}`);
            }

            // 3. Create Launchpad
            const now = new Date();
            const closeTime = new Date(now.getTime() + INTERVAL_MS);
            // 生成5位随机字符串大写
            const randomStr = Math.random().toString(36).substring(2, 7).toUpperCase();
            await LocalRemote.addLaunchpadInfo({
                cardId: String(cardId),
                currency: `ATO${randomStr}`,
                name: `Auto 2020 PANINI PRIZM DRAFT PICKS #98 LaMELO BALL`,
                volume: "100",
                price: "1",
                tradeStandardPct: "50",
                haltStandardPct: "70",
                feePct: "10",
                // minPrice: "0",
                // maxPrice: "0",
                openTime: now.getTime(),
                closeTime: closeTime.getTime(),
                summary: TASK_TAG // IMPORTANT: Tagging for cleanup identification
            });
            console.log(`[Schedule] Launchpad created successfully.`);

        } catch (e) {
            console.error("[Schedule] Task failed:", e);
        }
    };

    // Run immediately once, then schedule
    runTask();
    setInterval(runTask, INTERVAL_MS);

    // Start subscription auto-approval task
    console.log("[Schedule] Starting subscription auto-approval task...");
    setInterval(autoApproveSubscriptions, APPROVAL_INTERVAL_MS);
}
