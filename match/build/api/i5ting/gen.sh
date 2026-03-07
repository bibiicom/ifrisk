#!/bin/bash

iconv -f GBK -t UTF-8 api.md -o api.u.md
i5ting_toc -f api.u.md -o
