#注意，crontab文件格式一定要是unix格式，要不然会造成不可预知的后果

00 * * * * /home/ec2-user/run/cleanlog.sh >> /home/ec2-user/a
00 23 * * * /home/ec2-user/run/backup.sh >> /home/ec2-user/a
00 11 * * * /home/ec2-user/run/backup.sh >> /home/ec2-user/a


00 * * * * /home/trade/run/cleanlog.sh >> /home/trade/b
*/10 * * * * /home/trade/run/backup_remote.sh spot@10.10.12.222 >> /home/trade/a
00 * * * * /home/exchange/run/mysql.sh /home/exchange/run/cleankline.sql

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export LANG=en_US
export PATH=$PATH:$HOME/run:$HOME/control


00 * * * * /home/exchange/run/cleanlog.sh >> /home/exchange/a
00 * * * * /home/exchange/run/backup.sh >> /home/exchange/a
#05 * * * * rm -rf /home/exchange/backup/202*
05 * * * * /home/exchange/backup/gzip_dir.sh
00 * * * * /home/exchange/run/mysql.sh /home/exchange/run/cleankline.sql
*/5 * * * * /home/exchange/run/trade/snapgen.sh >> /home/exchange/run/trade/snapgen.log
*/2 * * * * /home/exchange/run/trade/snapdiff.sh
*/20 * * * * /home/exchange/run/trade/snapcheck.sh >> /home/exchange/run/trade/snapcheck.log
