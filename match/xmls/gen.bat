@echo off
setlocal enabledelayedexpansion
rd /s /q entity 1>nul 2>nul
mkdir entity

call setenv.bat

genxml.exe MERGEM2 entity_tmp\OFP.xml  entity_tmp\OFP_base.xml entity_tmp\OFP_table.xml

genxml.exe MDB entity\MDB.xml entity_tmp\MDB.xml entity_base\Entity.xml entity_base\DataType.xml
genxml.exe XTP entity\OFP.xml entity_tmp\OFP.xml entity_base\Entity.xml entity_base\DataType.xml
genxml.exe FTD entity\FTD.xml entity_tmp\OFP.xml entity_base\Entity.xml entity_base\DataType.xml
genxml.exe CHECK entity_base\DataType.xml entity_base\Entity.xml entity\MDB.xml entity\.xml entity\FTD.xml > warning.txt

copy entity_base\Entity.xml entity\
copy entity_base\DataType.xml entity\
copy entity_base\error.xml entity\
copy entity_base\example.xml entity\

rem copy entity_base\api_Mega_One.xml entity\api.xml
rem copy entity_base\api_Long_Bag.xml entity\api.xml
copy entity_base\api_Mega_One.xml entity\api.xml
