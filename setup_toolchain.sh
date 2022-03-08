#!/bin/bash

#
# project toolchain setup
# Copyright (C) 2022 YuzukiTsuru <gloomyghost@gloomyghost.com>. All rights reserved.
#

echo 'Setup Toolchain...'
echo 'Unarchive Toolchain...'
tar jxf prebuilt/gcc-arm-none-eabi-4_9-2015q2-20150609-linux.tar.bz2
rm -rf tools/gcc-arm-none-eabi-4_9-2015q2
echo 'Add Toolchain...'
mv gcc-arm-none-eabi-4_9-2015q2/ tools/
echo 'Add Toolchain path...'
sed -i 's/~\/tools\/gcc-arm-none-eabi-4_9-2015q2\/bin/#REPLACE-WITH-PATH\/tools\/gcc-arm-none-eabi-4_9-2015q2\/bin/g' gcc.mk
sed -i 's?#REPLACE-WITH-PATH?'`pwd`'?' gcc.mk
