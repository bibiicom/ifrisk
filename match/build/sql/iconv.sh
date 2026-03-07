#!/bin/bash

for file in *.sql; do
	echo $file
	iconv -f gbk -t utf-8 $file -o $file.utf-8
done

