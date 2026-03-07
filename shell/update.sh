#!/bin/bash
git config --global credential.helper store
git fetch origin
git reset --hard origin/main
chmod u+x *.sh
chmod u+x */*.sh
chmod u+x */*/*.sh
chmod u+x control/*