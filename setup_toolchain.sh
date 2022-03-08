#!/bin/bash

#
# project toolchain setup
# Copyright (C) 2022 YuzukiTsuru <gloomyghost@gloomyghost.com>. All rights reserved.
#

echo '=======================Setup Toolchain======================='
wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q2-update/+download/gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2 -O gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2
echo '=======================Unarchive Toolchain======================='
tar jxf gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2
rm -rf tools/gcc-arm-none-eabi-4_9-2015q2
rm -rf gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2
echo '=======================Add Toolchain======================='
mv gcc-arm-none-eabi-4_9-2015q2/ tools/
echo '=======================Add Toolchain path======================='
cp gcc.mk.in gcc.mk
sed -i 's/~\/tools\/gcc-arm-none-eabi-4_9-2015q2\/bin/#REPLACE-WITH-PATH\/tools\/gcc-arm-none-eabi-4_9-2015q2\/bin/g' gcc.mk
sed -i 's?#REPLACE-WITH-PATH?'`pwd`'?' gcc.mk
echo '=======================Set Tools======================='
chmod 777 tools/mkimage
chmod 777 tools/phoenixMC
echo '=======================ALL Done!======================='