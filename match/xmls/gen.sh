#!/bin/bash

rm -rf entity
mkdir entity

genxml MERGEM2 entity_tmp/OFP.xml  entity_tmp/OFP_base.xml entity_tmp/OFP_table.xml

genxml MDB entity/MDB.xml entity_tmp/MDB.xml entity_base/Entity.xml entity_base/DataType.xml
genxml XTP entity/OFP.xml entity_tmp/OFP.xml entity_base/Entity.xml entity_base/DataType.xml
genxml FTD entity/FTD.xml entity_tmp/OFP.xml entity_base/Entity.xml entity_base/DataType.xml
genxml CHECK entity_base/DataType.xml entity_base/Entity.xml entity/MDB.xml entity/OFP.xml entity/FTD.xml > warning.txt


cp -f entity_base/Entity.xml entity/
cp -f entity_base/DataType.xml entity/
cp -f entity_base/error.xml entity/
cp -f entity_base/example.xml entity/

cp -f entity_base/api_Mega_One.xml entity/api.xml
