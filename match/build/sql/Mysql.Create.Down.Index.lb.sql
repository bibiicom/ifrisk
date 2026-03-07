
ALTER TABLE `t_D_AccountDetail_new` ADD INDEX idx_key_1(`MemberID`,`ProductGroup`,`AccountID`,`Source`,`CreateTime`);
ALTER TABLE `t_D_FinishOrder_new` ADD INDEX idx_key_1(`MemberID`,`ProductGroup`,`AccountID`,`CreateTime`);
ALTER TABLE `t_D_FinishOrder_new` ADD INDEX idx_key_2(`OrderStatus`,`CreateTime`,`MemberID`);
ALTER TABLE `t_d_trade_new` ADD INDEX idx_key_1(`MemberID`,`ProductGroup`,`AccountID`,`CreateTime`);
ALTER TABLE `t_d_finishtriggerorder_new` ADD INDEX idx_key_1(`MemberID`,`ProductGroup`,`AccountID`,`CreateTime`);
ALTER TABLE `t_D_FinishTriggerOrder_new` ADD INDEX idx_key_2(`MemberID`,`LocalID`);
ALTER TABLE `t_D_FinishTriggerOrder_new` ADD INDEX idx_key_3(`MemberID`,`OrderID`);

