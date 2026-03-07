const HttpRequest = require('../utils/httpRequest');
const timeUtil = require("../utils/timeUtil");
const TDKLine = require("../models/TDKLine");
const fs = require("node:fs");
const log4js = require('../config/log4js.config');
const RandomUtil = require("../utils/randomUtil");
const logger = log4js.getLogger();

class Alt {
    fileBasePath = '../run/marketmaker/';
    filePathName = `${this.fileBasePath}libOneMegaToStable.market`;
    filePathTmpName = `${this.fileBasePath}libOneMegaToStable.market.tmp`;

    tmpPrice = {};

    constructor() {
        // 检查文件夹是否存在
        if (!fs.existsSync(this.fileBasePath)) {
            fs.mkdirSync(this.fileBasePath, {recursive: true});
        }
    }

    headers = {
        'Origin': 'https://app.alt.xyz',
        'Referer': 'https://app.alt.xyz/',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
    };

    needData = [
        {
            "listingId": "c8c74f15-c6c8-44af-9637-1db8986b3039",
            "shortName": "STEPHEN/USDC",
            "gradeNumber": "10.0",
            "fName": "STEPHEN/USDC",
        },
        {
            "listingId": "85c37265-ff9a-4594-a074-9dcc46eb2f2f",
            "shortName": "SABRINA/USDC",
            "gradeNumber": "9.0",
            "fName": "SABRINA/USDC",
        },
        {
            "listingId": "3df08d15-8beb-438a-bb4b-c7a5eb242fbe",
            "shortName": "LaMELO/USDC",
            "gradeNumber": "10.0",
            "fName": "LaMELO/USDC",
        },
        {
            "listingId": "be56d4f0-39da-4bf4-998c-82c8ea205354",
            "shortName": "JULIO/USDC",
            "gradeNumber": "10.0",
            "fName": "JULIO/USDC",
        },
    ]

    async doGetKLine() {

        for (let needDatum of this.needData) {
            let asset = await this.getAssetId(needDatum.listingId);
            logger.info(needDatum.shortName + ' ' + asset.assetId);

            // let gNum = await this.getGradeNumber(asset.assetId);
            // logger.info(needDatum.shortName + ' ' + gNum);

            let marketT = await this.getMarketTransactions(asset.assetId);

            //处理数据入库
            await this.handleKLine(marketT, needDatum.gradeNumber, needDatum.shortName);
            logger.info(needDatum.shortName + ' down');

            await timeUtil.sleep(3000);
        }

        for (let needDatum of this.needData) {
            for (let type = 1; type <= 2; type++) {
                for (let day = 0; day < 300; day++) {
                    await this.handleBarKLine(needDatum.shortName, type, day);
                }

            }
        }
    }

    async doGetKLine2() {

        for (let needDatum of this.needData) {
            let asset = await this.getAssetId(needDatum.listingId);
            logger.info(needDatum.shortName + ' ' + asset.assetId);

            let marketT = await this.getMarketTransactions(asset.assetId);

            //处理数据入库
            await this.handleKLine2(marketT, needDatum.gradeNumber, needDatum.shortName);
            logger.info(needDatum.shortName + ' down');

            await timeUtil.sleep(3000);
        }

        for (let needDatum of this.needData) {
            await this.handleWeekKLine(needDatum.shortName, 3 * 12 * 4);
            await this.handleMonthKLine(needDatum.shortName, 3 * 12);
        }
    }

    async doGenerateFile() {

        await fs.unlink(this.filePathTmpName, err => {
        });
        let okxList = await this.getOkxKLine();

        for (let i = 0; i < this.needData.length; i++) {
            let needDatum = this.needData[i];
            try {
                //生成文件
                await this.generateFile(okxList[i], needDatum.fName);
                logger.info(needDatum.shortName + ' down');
            } catch (e) {
                logger.error(`Error generateFile for ${needDatum.shortName}:`, e);
            }

            await timeUtil.sleep(3000);
        }
        // await fs.unlink(this.filePathName, err => {
        // });
        await fs.rename(this.filePathTmpName, this.filePathName, err => {
            if (err) {
                logger.info("rename error, try to del -> rename");
                fs.unlinkSync(this.filePathName);
                fs.renameSync(this.filePathTmpName, this.filePathName);
            }
        });
        logger.info('OneMegaToStable.market updated');
    }

    async doUpdatePrice() {
        for (let needDatum of this.needData) {
            try {
                let asset = await this.getAssetId(needDatum.listingId);
                logger.info(needDatum.shortName + ' ' + asset.assetId);
                let marketT = await this.getMarketTransactions(asset.assetId);
                //生成文件
                await this.updatePrice(marketT, needDatum.gradeNumber, needDatum.fName);
                logger.info(needDatum.shortName + ' down');
            } catch (e) {
                logger.error(`Error updatePrice for ${needDatum.shortName}:`, e);
            }
            await timeUtil.sleep(3000);
        }
        logger.info('Price updated');
    }

    async getAssetId(listingId) {
        let res = await HttpRequest.post('https://alt-platform-server.production.internal.onlyalt.com/graphql/PublicListingsInSameCardGroup', {
            "operationName": "PublicListingsInSameCardGroup",
            "variables": {"listingId": listingId},
            "query": "query PublicListingsInSameCardGroup($listingId: ID!) {\n  publicListingsInSameCardGroup(listingId: $listingId) {\n    publicListings {\n      ...PublicListingBase\n      ...PublicListingItems\n      __typename\n    }\n    __typename\n  }\n}\n\nfragment PublicListingBase on PublicListing {\n  id\n  minOfferPrice\n  listPrice\n  state\n  createdAt\n  type\n  expiresAt\n  isEligibleForBidding\n  depositsWaived\n  disallowOffers\n  subtitle\n  description\n  requireIdVerification\n  listingPaymentState {\n    state\n    __typename\n  }\n  ...PublicListingLiveDetails\n  __typename\n}\n\nfragment PublicListingLiveDetails on PublicListing {\n  id\n  __typename\n}\n\nfragment PublicListingItems on PublicListing {\n  id\n  items {\n    ...PublicItemBase\n    ...PublicItemAsset\n    __typename\n  }\n  __typename\n}\n\nfragment PublicItemBase on PublicItem {\n  id\n  displayNames {\n    ...ItemDisplayNamesBase\n    __typename\n  }\n  images {\n    ...ImageBase\n    __typename\n  }\n  attributes {\n    ...ItemAttributesBase\n    __typename\n  }\n  itemVaultState {\n    ...ItemVaultStateBase\n    __typename\n  }\n  timeSeriesStats {\n    ...PublicTimeSeriesStatsBase\n    __typename\n  }\n  recommendedOfferPrice\n  __typename\n}\n\nfragment ItemDisplayNamesBase on ItemDisplayNames {\n  itemName\n  groupName\n  assetName\n  __typename\n}\n\nfragment ImageBase on Image {\n  position\n  url\n  __typename\n}\n\nfragment ItemAttributesBase on ItemAttributes {\n  gradeNumber\n  gradingCompany\n  serial\n  printRun\n  certNumber\n  autograph\n  centering\n  corners\n  edges\n  surfaces\n  qualifier\n  __typename\n}\n\nfragment ItemVaultStateBase on ItemVaultState {\n  createdAt\n  state\n  underAltCustody\n  __typename\n}\n\nfragment PublicTimeSeriesStatsBase on TimeSeriesStats {\n  latestValue\n  altValueConfidenceData {\n    currentConfidenceMetric\n    currentErrorUpperBound\n    currentErrorLowerBound\n    __typename\n  }\n  __typename\n}\n\nfragment PublicItemAsset on PublicItem {\n  asset {\n    ...AssetBase\n    __typename\n  }\n  __typename\n}\n\nfragment AssetBase on Asset {\n  id\n  name\n  year\n  subject\n  category\n  brand\n  variety\n  attributes {\n    cardNumber\n    printRun\n    __typename\n  }\n  __typename\n}"
        }, {
            headers: this.headers
        });
        let asset = res.data.data.publicListingsInSameCardGroup.publicListings[0].items[0].asset;
        let assetListPrice = res.data.data.publicListingsInSameCardGroup.publicListings[0].listPrice;
        let assetId = asset.id;
        let assetName = asset.name;
        return {assetId, assetName, assetListPrice};
    }

    async getGradeNumber(assetId) {
        let res2 = await HttpRequest.post('https://alt-platform-server.production.internal.onlyalt.com/graphql/AssetDetails', {
            "operationName": "AssetDetails",
            "variables": {
                "id": assetId, "tsFilter": {
                    "gradingCompany": "PSA", "gradeNumber": "10.0", "autograph": null
                }
            },
            "query": "query AssetDetails($id: ID!, $tsFilter: TimeSeriesFilter!) {\n  asset(id: $id) {\n    ...AssetDetails\n    ...AssetActiveListings\n    __typename\n  }\n}\n\nfragment AssetDetails on Asset {\n  ...AssetBase\n  images {\n    ...ImageBase\n    __typename\n  }\n  attributes {\n    cardNumber\n    printRun\n    __typename\n  }\n  predictedPrice(tsFilter: $tsFilter)\n  altValueInfo(tsFilter: $tsFilter) {\n    currentAltValue\n    confidenceData {\n      currentConfidenceMetric\n      currentErrorLowerBound\n      currentErrorUpperBound\n      __typename\n    }\n    __typename\n  }\n  cardPops {\n    ...CardPopBase\n    __typename\n  }\n  __typename\n}\n\nfragment AssetBase on Asset {\n  id\n  name\n  year\n  subject\n  category\n  brand\n  variety\n  attributes {\n    cardNumber\n    printRun\n    __typename\n  }\n  __typename\n}\n\nfragment ImageBase on Image {\n  position\n  url\n  __typename\n}\n\nfragment CardPopBase on CardPop {\n  gradingCompany\n  gradeNumber\n  count\n  __typename\n}\n\nfragment AssetActiveListings on Asset {\n  activeListings {\n    ...PublicListingBase\n    items {\n      ...PublicItemBase\n      ...PublicItemAsset\n      __typename\n    }\n    __typename\n  }\n  __typename\n}\n\nfragment PublicListingBase on PublicListing {\n  id\n  minOfferPrice\n  listPrice\n  state\n  createdAt\n  type\n  expiresAt\n  isEligibleForBidding\n  depositsWaived\n  disallowOffers\n  subtitle\n  description\n  requireIdVerification\n  listingPaymentState {\n    state\n    __typename\n  }\n  ...PublicListingLiveDetails\n  __typename\n}\n\nfragment PublicListingLiveDetails on PublicListing {\n  id\n  __typename\n}\n\nfragment PublicItemBase on PublicItem {\n  id\n  displayNames {\n    ...ItemDisplayNamesBase\n    __typename\n  }\n  images {\n    ...ImageBase\n    __typename\n  }\n  attributes {\n    ...ItemAttributesBase\n    __typename\n  }\n  itemVaultState {\n    ...ItemVaultStateBase\n    __typename\n  }\n  timeSeriesStats {\n    ...PublicTimeSeriesStatsBase\n    __typename\n  }\n  recommendedOfferPrice\n  __typename\n}\n\nfragment ItemDisplayNamesBase on ItemDisplayNames {\n  itemName\n  groupName\n  assetName\n  __typename\n}\n\nfragment ItemAttributesBase on ItemAttributes {\n  gradeNumber\n  gradingCompany\n  serial\n  printRun\n  certNumber\n  autograph\n  centering\n  corners\n  edges\n  surfaces\n  qualifier\n  __typename\n}\n\nfragment ItemVaultStateBase on ItemVaultState {\n  createdAt\n  state\n  underAltCustody\n  __typename\n}\n\nfragment PublicTimeSeriesStatsBase on TimeSeriesStats {\n  latestValue\n  altValueConfidenceData {\n    currentConfidenceMetric\n    currentErrorUpperBound\n    currentErrorLowerBound\n    __typename\n  }\n  __typename\n}\n\nfragment PublicItemAsset on PublicItem {\n  asset {\n    ...AssetBase\n    __typename\n  }\n  __typename\n}"
        }, {
            headers: this.headers
        });
        let cardPops = res2.data.data.asset.cardPops;

        const maxCountPSACard = cardPops
            .filter(card => card.gradingCompany === "PSA")
            .reduce((max, current) => current.count > max.count ? current : max);
        return maxCountPSACard.gradeNumber;
    }

    async getMarketTransactions(assetId) {
        let res3 = await HttpRequest.post('https://alt-platform-server.production.internal.onlyalt.com/graphql/AssetMarketTransactions', {
            "operationName": "AssetMarketTransactions",
            "variables": {
                "id": assetId, "marketTransactionFilter": {
                    "allGrades": true, "showSkipped": true
                }
            },
            "query": "query AssetMarketTransactions($id: ID!, $marketTransactionFilter: MarketTransactionFilter!) {\n  asset(id: $id) {\n    ...AssetMarketTransactions\n    __typename\n  }\n}\n\nfragment AssetMarketTransactions on Asset {\n  id\n  marketTransactions(marketTransactionFilter: $marketTransactionFilter) {\n    ...MarketTransactionBase\n    __typename\n  }\n  __typename\n}\n\nfragment MarketTransactionBase on MarketTransaction {\n  id\n  date\n  auctionHouse\n  auctionType\n  price\n  attributes {\n    ...MarketTransactionAttributesBase\n    __typename\n  }\n  subjectToChange\n  consolidatedSkippedReason\n  label\n  __typename\n}\n\nfragment MarketTransactionAttributesBase on MarketTransactionAttributes {\n  gradeNumber\n  gradingCompany\n  url\n  autograph\n  __typename\n}"
        }, {
            headers: this.headers
        });

        return res3.data.data.asset.marketTransactions;
    }

    async handleKLine(marketTransactions, gradeNumber, assetName) {
        let find = marketTransactions.filter(transaction => transaction.attributes.gradeNumber === gradeNumber && transaction.attributes.gradingCompany === "PSA");
        // 按日期分组
        const groupedByDate = find.reduce((acc, transaction) => {
            const date = transaction.date;
            if (!acc[date]) {
                acc[date] = [];
            }
            acc[date].push(transaction);
            return acc;
        }, {});

        let tDKLineList = [];

        for (const [date, transactions] of Object.entries(groupedByDate)) {
            logger.info(`日期: ${date}, 交易数量: ${transactions.length}`);
            let firstPrice = Number(transactions[0].price) * 10000;
            let openPrice = 0;
            let closePrice = 0;
            let randomElements = RandomUtil.randomElements(transactions);
            if (randomElements.length === 1) {
                openPrice = Number(randomElements[0].price) * 10000;
                closePrice = Number(randomElements[0].price) * 10000;
            } else {
                openPrice = Number(randomElements[0].price) * 10000;
                closePrice = Number(randomElements[1].price) * 10000;
            }

            let volume = 0;
            let turnover = 0;
            let maxPrice = firstPrice;
            let minPrice = firstPrice;
            for (let i = 0; i < transactions.length; i++) {
                const price = Number(transactions[i].price) * 10000;
                if (price > maxPrice) {
                    maxPrice = price;
                } else if (price < minPrice) {
                    minPrice = price;
                }
                volume += 10000;
                turnover += price;
            }
            // logger.info(`   交易时间: ${date}, max: ${maxPrice}, min: ${minPrice}`);
            let [y, m, d] = date.split('-');
            let tDKLine = {
                ExchangeID: 'SCARD',
                InstrumentID: assetName,
                Bar: '1d', // 1d 7d 1o=30d 1y=365d
                InsertTime: Date.UTC(Number(y), Number(m) - 1, Number(d)),
                OpenPrice: openPrice / 10000,
                HighestPrice: maxPrice / 10000,
                LowestPrice: minPrice / 10000,
                ClosePrice: closePrice / 10000,
                Volume: volume,
                Turnover: turnover / 10000,
                DBTime: new Date()
            };
            tDKLineList.push(tDKLine);
        }

        logger.info(assetName + ' ' + tDKLineList.length);
        for (let tDKLine of tDKLineList) {
            await TDKLine.createByObject(tDKLine);
        }
    }

    async handleBarKLine(assetName, barType, handleDay = 0) {
        let checkDay = 7;
        let bar = '7d'; // 24h 7d 1m=30d 1y=360d
        switch (barType) {
            case 1:
                checkDay = 7;
                bar = '1w';
                break;
            case 2:
                checkDay = 30;
                bar = '1o';
                break;
            case 3:
                checkDay = 360;
                bar = '1y';
                break;
            default:
                return;
        }

        let now = new Date();
        now.setDate(now.getDate() - handleDay);

        let start = Date.UTC(now.getUTCFullYear(), now.getUTCMonth(), now.getUTCDate() - checkDay);
        let end = Date.UTC(now.getUTCFullYear(), now.getUTCMonth(), now.getUTCDate() - 1);
        let res = await TDKLine.searchByDate(assetName, start, end);

        // logger.info(res);
        let openPrice = Number(res[0].OpenPrice) * 10000;
        let closePrice = Number(res[res.length - 1].ClosePrice) * 10000;
        let maxPrice = Number(res[0].HighestPrice) * 10000;
        let minPrice = Number(res[0].LowestPrice) * 10000;
        let volume = 0;
        let turnover = 0;
        for (let i = 0; i < res.length; i++) {
            let re = res[i];

            const hPrice = Number(re.HighestPrice) * 10000;
            const lPrice = Number(re.LowestPrice) * 10000;
            const turn = Number(re.Turnover) * 10000;
            if (hPrice > maxPrice) {
                maxPrice = hPrice;
            }
            if (lPrice < minPrice) {
                minPrice = lPrice;
            }
            volume += Number(re.Volume);
            turnover += turn;
        }

        let tDKLine = {
            ExchangeID: res[0].ExchangeID,
            InstrumentID: res[0].InstrumentID,
            Bar: bar,
            InsertTime: start,
            OpenPrice: openPrice / 10000,
            HighestPrice: maxPrice / 10000,
            LowestPrice: minPrice / 10000,
            ClosePrice: closePrice / 10000,
            Volume: volume,
            Turnover: turnover / 10000,
            DBTime: new Date()
        };
        // logger.info(tDKLine);
        // logger.info(bar + ' ' + new Date(tDKLine.InsertTime).toLocaleString() + ' ' + tDKLine.Turnover);
        await TDKLine.createByObject(tDKLine);
    }

    async updatePrice(marketTransactions, gradeNumber, fName) {
        let find = marketTransactions.filter(transaction => transaction.attributes.gradeNumber === gradeNumber && transaction.attributes.gradingCompany === "PSA");
        let marketOne = find[0];

        this.tmpPrice[fName] = marketOne.price;
    }

    async generateFile(okxData, fName) {
        let price = this.tmpPrice[fName];
        if (!price) {
            return;
        }
        let volume = 0.0001;
        if (okxData) {
            let open24h = Number(okxData.ticker.open24h);
            let last = Number(okxData.ticker.last);
            price = last / open24h * price;
            volume = volume + RandomUtil.randomIntBetween(1, 20);
        }
        // price 保留7位有效数字
        let ftPrice = price.toPrecision(7);
        let date = new Date();
        // 格式化为 20251217-10:05:09 格式
        let dateStr = `${date.getFullYear()}${(date.getMonth() + 1).toString().padStart(2, '0')}${date.getDate().toString().padStart(2, '0')}-${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}:${date.getSeconds().toString().padStart(2, '0')}`;

        let text = `${fName} ${ftPrice} ${volume.toFixed(4)} ${dateStr}\n`;
        fs.appendFileSync(this.filePathTmpName, text);
    }


    async handleKLine2(marketTransactions, gradeNumber, assetName) {
        let find = marketTransactions.filter(transaction => transaction.attributes.gradeNumber === gradeNumber && transaction.attributes.gradingCompany === "PSA");
        // 按日期分组
        for (let findElement of find) {
            let time = RandomUtil.randomIntBetween(0, 23 * 60 * 60 * 1000);
            findElement.time = new Date(findElement.date).getTime() + time;
        }
        // find 按 time 排序
        find.sort((a, b) => a.time - b.time);

        const groupedByDate = find.reduce((acc, transaction) => {
            const date = transaction.date;
            if (!acc[date]) {
                acc[date] = [];
            }
            acc[date].push(transaction);
            return acc;
        }, {});

        let tDKLineList = [];
        let nextOpenPrice = 0;

        for (const [date, transactions] of Object.entries(groupedByDate)) {
            logger.info(`日期: ${date}, 交易数量: ${transactions.length}`);
            let firstPrice = Number(transactions[0].price) * 10000;
            let openPrice = nextOpenPrice === 0 ? firstPrice : nextOpenPrice;
            let closePrice = Number(transactions[transactions.length - 1].price) * 10000;
            nextOpenPrice = closePrice;

            let volume = 0;
            let turnover = 0;
            let maxPrice = firstPrice;
            let minPrice = firstPrice;
            for (let i = 0; i < transactions.length; i++) {
                const price = Number(transactions[i].price) * 10000;
                if (price > maxPrice) {
                    maxPrice = price;
                } else if (price < minPrice) {
                    minPrice = price;
                }
                volume += 10000;
                turnover += price;
            }
            // logger.info(`   交易时间: ${date}, max: ${maxPrice}, min: ${minPrice}`);
            let [y, m, d] = date.split('-');
            let tDKLine = {
                ExchangeID: 'SCARD',
                InstrumentID: assetName,
                Bar: '1d', // 1d 7d 1o=30d 1y=365d
                InsertTime: Date.UTC(Number(y), Number(m) - 1, Number(d)),
                OpenPrice: openPrice / 10000,
                HighestPrice: maxPrice / 10000,
                LowestPrice: minPrice / 10000,
                ClosePrice: closePrice / 10000,
                Volume: volume,
                Turnover: turnover / 10000,
                DBTime: new Date()
            };
            tDKLineList.push(tDKLine);
        }

        logger.info(assetName + ' ' + tDKLineList.length);
        for (let tDKLine of tDKLineList) {
            await TDKLine.createByObject(tDKLine);
        }
    }

    async handleWeekKLine(assetName, handleCount = 1) {
        let bar = '1w'; // 1d 1w 1o
        // 获取当前UTC时间
        const weekTimeStamp = 7 * 24 * 60 * 60 * 1000;
        const now = new Date();
        // 设置为本周一（UTC时间）
        const weekStart = new Date(Date.UTC(now.getUTCFullYear(), now.getUTCMonth(), now.getUTCDate()));
        const day = weekStart.getUTCDay();
        const diff = weekStart.getUTCDate() - day + (day === 0 ? -6 : 1); // 调整到周一
        weekStart.setUTCDate(diff);
        weekStart.setUTCHours(0, 0, 0, 0); // 设置时间为00:00:00
        // 设置为本周日（UTC时间）
        const weekEnd = new Date(weekStart.getTime());
        weekEnd.setUTCDate(weekStart.getUTCDate() + 7);
        // 获取时间戳（毫秒）
        let startTimeStamp = weekStart.getTime();
        let endTimeStamp = weekEnd.getTime();
        for (let i = 0; i < handleCount; i++) {
            startTimeStamp -= weekTimeStamp;
            endTimeStamp -= weekTimeStamp;
            await this.createBarKLine(assetName, bar, startTimeStamp, endTimeStamp);
        }
    }

    async handleMonthKLine(assetName, handleCount = 1) {
        let bar = '1o'; // 1d 1w 1o
        // 获取当前UTC时间
        const now = new Date();
        // 设置为本月第一天（UTC时间）
        const monthStart = new Date(Date.UTC(now.getUTCFullYear(), now.getUTCMonth(), 1));
        monthStart.setUTCHours(0, 0, 0, 0); // 设置时间为00:00:00

        // 设置为本月最后一天（UTC时间）
        const monthEnd = new Date(Date.UTC(now.getUTCFullYear(), now.getUTCMonth() + 1, 1));
        monthEnd.setUTCHours(0, 0, 0, 0); // 设置时间为23:59:59.999

        // 获取时间戳（毫秒）
        let startTimeStamp = monthStart.getTime();
        let endTimeStamp = monthEnd.getTime();

        // 处理指定月份的数量
        for (let i = 0; i < handleCount; i++) {
            // 移动到上个月
            monthStart.setUTCMonth(monthStart.getUTCMonth() - 1);
            monthEnd.setUTCMonth(monthEnd.getUTCMonth() - 1);

            startTimeStamp = monthStart.getTime();
            endTimeStamp = monthEnd.getTime();
            // logger.info(monthStart.toISOString(), monthEnd.toISOString());
            await this.createBarKLine(assetName, bar, startTimeStamp, endTimeStamp);
        }
    }

    async createBarKLine(assetName, bar, startTimeStamp, endTimeStamp) {
        let res = await TDKLine.searchByDate(assetName, startTimeStamp, endTimeStamp);
        if (res.length === 0) {
            return;
        }

        let openPrice = Number(res[0].OpenPrice) * 10000;
        let closePrice = Number(res[res.length - 1].ClosePrice) * 10000;
        let maxPrice = Number(res[0].HighestPrice) * 10000;
        let minPrice = Number(res[0].LowestPrice) * 10000;
        let volume = 0;
        let turnover = 0;
        for (let i = 0; i < res.length; i++) {
            let re = res[i];

            const hPrice = Number(re.HighestPrice) * 10000;
            const lPrice = Number(re.LowestPrice) * 10000;
            const turn = Number(re.Turnover) * 10000;
            if (hPrice > maxPrice) {
                maxPrice = hPrice;
            }
            if (lPrice < minPrice) {
                minPrice = lPrice;
            }
            volume += Number(re.Volume);
            turnover += turn;
        }

        let tDKLine = {
            ExchangeID: res[0].ExchangeID,
            InstrumentID: res[0].InstrumentID,
            Bar: bar,
            InsertTime: startTimeStamp,
            OpenPrice: openPrice / 10000,
            HighestPrice: maxPrice / 10000,
            LowestPrice: minPrice / 10000,
            ClosePrice: closePrice / 10000,
            Volume: volume,
            Turnover: turnover / 10000,
            DBTime: new Date()
        };
        // logger.info(tDKLine.OpenPrice, tDKLine.ClosePrice);
        // logger.info(bar + ' ' + new Date(tDKLine.InsertTime).toLocaleString() + ' ' + tDKLine.Turnover);
        await TDKLine.createByObject(tDKLine);
    }


    async getOkxKLine() {
        const headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36',
            'Accept': 'application/json',
            'Origin': 'https://www.okx.com',
            'Referer': 'https://www.okx.com/zh-hans/markets/prices',
        };

        const options = {
            headers,
        };

        let res = await HttpRequest.post('https://www.okx.com/priapi/v5/rubik/discover2/market?t=' + Date.now(), {
            "instType": "COIN",
            "countryFilter": 1,
            "upDownTimeZone": 0,
            "pageNum": 1,
            "pageSize": 25,
            "groupId": 1,
            "filter": {}
        }, options);
        if (res.data.code !== 0) {
            logger.error('获取okx失败', res.data);
            return [];
        }
        return res.data.data.dataList;

    }


}

module.exports = Alt;