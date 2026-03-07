#!/bin/bash

tail -f flow/httpfront_*.tlog | grep "us]" | awk -F \{  {'print $1'}
