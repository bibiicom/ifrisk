import DocsDeploy from "./docs/deploy";
import Market from "./market/Market";
import Exchange from "./exchange";
import HistoryOrder from "./historyOrders2";
import Assets from "./assets/Assets";
import FinancialRecord from "./financialrecord/FinancialRecord";
import FinancialRecord2 from "./financialRecord2";
import Faucet from "./faucet/faucet";
import SettlementRecord from "./settlementrecord/SettlementRecord";
import BlockchainTransfer from "./blockchaintransfer";

import Leader from "./account";
import Stats from "./stats";
import Reserve from "./account/reserve";
// import Launchpad from "@/pages/launchpad/Launchpad";
// import SubscriptionDetail from "@/pages/launchpad/components/ProjectCard/SubscriptionDetail";
// import SubscriptionRecords from "@/pages/launchpad/components/ProjectCard/SubscriptionDetail/SubscriptionRecords";
import NftEntry from "./nftentry";
import Buyback from "./buyback";
import WalletAssets2 from "./walletassets2";
import MyInfo from "./myinfo";
import Home from "./home/index";
import Inbound from "./inbound";
import Outbound from "./outbound";
import News from "./information/News";
import Announcement from "./information/Announcement";
import ArticleDetail from "./information/ArticleDetail";
import Launchpad from "./launchpad2";
import LaunchpadDetail from "./launchpad2/detail";
import Vault from "./vault";
import VaultDetail from "./vault/detail";
import Ranking from "./ranking";
import HelpCenter from "./helpcenter";

const FORGET_PASSWORD = "/reset_password";

const DOCS_DEPLOY = "/docs/deploy/";

const MARKET = "/market";

const FAUCET = "/faucet";

const ORDERS = "/orders";

const ASSETS = "/assets";

const MY_INFO = "/myinfo";

// const FINANCIAL = "/financial";

const FINANCIAL2 = "/financial";

const SETTLEMENT = "/info/settlement";

const FUNDING = "/info/funding";

const INDEX = "/index";

const LEADER = "/leader";

const STATS = "/stats";

const RESERVE = "/reserve";

// const LAUNCHPAD = "/launchpad";

// const SUBSCRIPTION_DETAIL = "/launchpad/subscription/:projectId";

// const SUBSCRIPTION_RECORDS = "/launchpad/subscription/:projectId/records";

const EXCHANGE = "/exchange";

const BLOCKCHAIN_TRANSFER = "/blockchaintransfer";



const NFT_ENTRY = "/nftentry";

const BUYBACK = "/buyback";

const WALLET_ASSETS2 = "/walletassets";

const INBOUND = "/inbound";

const OUTBOUND = "/outbound";

const NEWS = "/news";

const ANNOUNCEMENT = "/announcement";

const ANNOUNCEMENT_DETAIL = "/announcement/:id";

const LAUNCHPAD = "/launchpad";

const LAUNCHPAD_DETAIL = "/launchpad/:id";

const VAULT = "/vault";

const VAULT_DETAIL = "/vault/:id";

const RANKING = "/ranking";

const HELP_CENTER = "/helpcenter";

const HOME = "/home";

const HOME_PATH = HOME;

const REDIRECT_PATH = HOME_PATH;

function routes() {
  const baseRouters = [
    { path: HOME, component: Home },
    { path: EXCHANGE, component: Exchange },
    { path: MARKET, component: Market },
    { path: FAUCET, component: Faucet },
    { path: SETTLEMENT, component: SettlementRecord },
    { path: ASSETS, component: Assets },
    { path: RESERVE, component: Reserve },
    { path: LEADER, component: Leader },
    { path: STATS, component: Stats },
    // { path: SUBSCRIPTION_RECORDS, component: SubscriptionRecords },
    // { path: SUBSCRIPTION_DETAIL, component: SubscriptionDetail },
    // { path: LAUNCHPAD, component: Launchpad },
    { path: ORDERS, component: HistoryOrder },
    { path: MY_INFO, component: MyInfo },
    // { path: FINANCIAL, component: FinancialRecord },
    { path: FINANCIAL2, component: FinancialRecord2 },
    { path: `${DOCS_DEPLOY}:key`, component: DocsDeploy },
    { path: BLOCKCHAIN_TRANSFER, component: BlockchainTransfer },
    { path: NFT_ENTRY, component: NftEntry },
    { path: BUYBACK, component: Buyback },
    { path: WALLET_ASSETS2, component: WalletAssets2 },
    { path: INBOUND, component: Inbound },
    { path: OUTBOUND, component: Outbound },
    { path: ANNOUNCEMENT_DETAIL, component: ArticleDetail },
    { path: NEWS, component: News },
    { path: ANNOUNCEMENT, component: Announcement },
    { path: LAUNCHPAD_DETAIL, component: LaunchpadDetail },
    { path: LAUNCHPAD, component: Launchpad },
    { path: VAULT_DETAIL, component: VaultDetail },
    { path: VAULT, component: Vault },
    { path: RANKING, component: Ranking },
    { path: HELP_CENTER, component: HelpCenter },
  ];

  return baseRouters;


}
const baseUrl = "https://tobetrader.com/";

const Urls = {
  KNOWLEDGE: `${baseUrl}knowledge`,
  VIEWPOINT: `${baseUrl}viewpoint`,
  NEWS: `${baseUrl}news`,
  BIG_CARD: `${baseUrl}bigcard`,
  INDUSTRY_NAV: `${baseUrl}links`,
  DAILY: `${baseUrl}daily`,
};

export {
  routes,
  REDIRECT_PATH,
  HOME_PATH,
  HOME,
  FORGET_PASSWORD,
  FAUCET,
  MARKET,
  EXCHANGE,
  INDEX,
  ASSETS,
  LEADER,
  STATS,
  RESERVE,
  MY_INFO,
  ORDERS,
  // FINANCIAL,
  FINANCIAL2,
  SETTLEMENT,
  FUNDING,
  BLOCKCHAIN_TRANSFER,
  Urls,
  NFT_ENTRY,
  // LAUNCHPAD,
  // SUBSCRIPTION_DETAIL,
  // SUBSCRIPTION_RECORDS,
  BUYBACK,
  WALLET_ASSETS2,
  INBOUND,
  OUTBOUND,
  NEWS,
  ANNOUNCEMENT,
  ANNOUNCEMENT_DETAIL,
  LAUNCHPAD,
  LAUNCHPAD_DETAIL,
  VAULT,
  VAULT_DETAIL,
  RANKING,
  HELP_CENTER,
};
