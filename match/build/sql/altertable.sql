use perpetual;

ALTER TABLE `t_D_FinishOrder` ADD COLUMN CopyMemberID char(42) default NULL COMMENT '带单员代码';
ALTER TABLE `t_D_FinishOrder` ADD COLUMN CopyOrderID char(20)  default NULL COMMENT '带单员报单号';
ALTER TABLE `t_D_FinishOrder` ADD COLUMN CopyProfitRate numeric(26,10) default NULL COMMENT '带单分配比例';
ALTER TABLE `t_D_FinishOrder` ADD COLUMN CopyProfit numeric(38,10)  default NULL COMMENT '带单盈利分配';
ALTER TABLE `t_D_FinishOrder` ADD COLUMN Position numeric(38,10)  default NULL COMMENT '成交时的持仓量';	

ALTER TABLE `t_D_Trade` MODIFY COLUMN ReserveMargin numeric(38,10) default NULL COMMENT '保证金中保留资金的比例(待删除)';
ALTER TABLE `t_D_Trade` ADD COLUMN APPID char(8) default NULL COMMENT '应用编号';
ALTER TABLE `t_D_Trade` ADD COLUMN CopyMemberID char(42) default NULL COMMENT '带单员代码';
ALTER TABLE `t_D_Trade` ADD COLUMN CopyOrderID char(20) default NULL COMMENT '带单员报单号';
ALTER TABLE `t_D_Trade` ADD COLUMN CopyProfit numeric(38,10) default NULL COMMENT '带单盈利分配';
ALTER TABLE `t_D_Trade` ADD COLUMN Position numeric(38,10) default NULL COMMENT '成交时的持仓量';	

ALTER TABLE `t_D_AccountDetail` ADD COLUMN RelatedID char(20) default NULL COMMENT '内外对账ID';

ALTER TABLE `t_D_FinishTriggerOrder` ADD COLUMN CopyMemberID char(42) default NULL COMMENT '带单员代码';
ALTER TABLE `t_D_FinishTriggerOrder` ADD COLUMN CopyOrderID char(20) default NULL COMMENT '带单员报单号';
ALTER TABLE `t_D_FinishTriggerOrder` ADD COLUMN CopyProfitRate numeric(38,10) default NULL COMMENT '带单盈利分配';







