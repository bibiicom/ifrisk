#!/bin/bash

capiname=`ls cplusplus/*Api.so`
cp -p ../../libs/ofserviceapi/1.0/omqcapi/linux_x64/libomqcapi.so ${capiname/.so}X.so

mkdir cplusplus/include 2>/dev/null
cp ../../src_api/include/*.h cplusplus/include/
rm -rf cplusplus/include/*CApi.h
