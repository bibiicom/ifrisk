#!/bin/bash

xmlpath=../../xmls

#genxml.exe DBOracle Oracle.xml $xmlpath/entity_tmp/OFP_table.xml $xmlpath/entity_base/Entity.xml $xmlpath/entity_base/DataType.xml
#gencode DBCreateInit.Oracle.sql DBCreateInit.Oracle.sql.tpl Oracle.xml $xmlpath/entity_base/DataType.xml 1>nul
#gencode DBCreateDown.Oracle.sql DBCreateDown.Oracle.sql.tpl Oracle.xml $xmlpath/entity_base/DataType.xml 1>nul
#gencode DBClear.sql DBClear.sql.tpl DB_Oracle.xml 1>nul

genxml DBMysql Mysql.xml $xmlpath/entity_tmp/OFP_table.xml $xmlpath/entity_base/Entity.xml $xmlpath/entity_base/DataType.xml
gencode Mysql.Create.Snap.sql Mysql.Create.Snap.sql.tpl Mysql.xml $xmlpath/entity_base/DataType.xml
gencode Mysql.Create.Down.sql Mysql.Create.Down.sql.tpl Mysql.xml $xmlpath/entity_base/DataType.xml
gencode Mysql.PartitionByMonth.Create.Down.sql Mysql.PartitionByMonth.Create.Down.sql.tpl Mysql.xml $xmlpath/entity_base/DataType.xml
gencode Mysql.PartitionByMonth.Crontab.sql Mysql.PartitionByMonth.Crontab.sql.tpl Mysql.xml $xmlpath/entity_base/DataType.xml

#copy Mysql.Create.Snap.sql ../../build/api/sql/
#copy Mysql.PartitionByMonth.Function.sql ../../build/api/sql/
#copy Mysql.PartitionByMonth.Crontab.sql ../../build/
cp cleankline.sql ../../build/