use perpetual;
delete from t_D_KLine WHERE PeriodID="1m" and BeginTime < UNIX_TIMESTAMP()-10000*60 limit 1000;
delete from t_D_KLine WHERE PeriodID="5m" and BeginTime < UNIX_TIMESTAMP()-10000*5*60 limit 100;
delete from t_D_KLine WHERE PeriodID="1h" and BeginTime < UNIX_TIMESTAMP()-10000*60*60 limit 10;
optimize table t_D_KLine;