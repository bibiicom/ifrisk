rem @echo off
call setenv.bat

set xmlpath=..\..\xmls

rem genxml.exe DBOracle Oracle.xml %xmlpath%\entity_tmp\OFP_table.xml %xmlpath%\entity_base\Entity.xml %xmlpath%\entity_base\DataType.xml
rem call gencode DBCreateInit.Oracle.sql DBCreateInit.Oracle.sql.tpl Oracle.xml %xmlpath%\entity_base\DataType.xml 1>nul
rem call gencode DBCreateDown.Oracle.sql DBCreateDown.Oracle.sql.tpl Oracle.xml %xmlpath%\entity_base\DataType.xml 1>nul
rem call gencode DBClear.sql DBClear.sql.tpl DB_Oracle.xml 1>nul

genxml.exe DBMysql Mysql.xml %xmlpath%\entity_tmp\OFP_table.xml %xmlpath%\entity_base\Entity.xml %xmlpath%\entity_base\DataType.xml
call gencode Mysql.Create.Snap.sql Mysql.Create.Snap.sql.tpl Mysql.xml %xmlpath%\entity_base\DataType.xml
call gencode Mysql.Create.Down.sql Mysql.Create.Down.sql.tpl Mysql.xml %xmlpath%\entity_base\DataType.xml
call gencode Mysql.PartitionByMonth.Create.Down.sql Mysql.PartitionByMonth.Create.Down.sql.tpl Mysql.xml %xmlpath%\entity_base\DataType.xml
call gencode Mysql.PartitionByMonth.Crontab.sql Mysql.PartitionByMonth.Crontab.sql.tpl Mysql.xml %xmlpath%\entity_base\DataType.xml

rem copy Mysql.Create.Snap.sql ..\..\build\api\sql\
rem copy Mysql.PartitionByMonth.Function.sql ..\..\build\api\sql\
rem copy Mysql.PartitionByMonth.Crontab.sql ..\..\build\

rem for %%i in (Mysql.Create.Down.sql Mysql.PartitionByMonth.Create.Down.sql) do (
rem 	grep -v RENAME ..\..\build\api\sql\%%i > %%i.grep.old
rem 	grep -v RENAME %%i > %%i.grep
rem 	diff %%i.grep.old %%i.grep > a
rem 
rem 	for %%a in (a) do (
rem 	 if not %%~za==0 (
rem 			echo copy %%i ..\..\build\api\sql\
rem 			copy %%i ..\..\build\api\sql\
rem 		)
rem 	)
rem 
rem 	del a
rem 	del %%i.grep.old
rem 	del %%i.grep
rem )


