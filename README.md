# XR32SDK
The SDK source code of XR32 SoC. [![Sync To Gitee](https://github.com/YuzukiHD/XR32SDK/actions/workflows/Gitee.yml/badge.svg)](https://github.com/YuzukiHD/XR32SDK/actions/workflows/Gitee.yml)

Based on [XradioTech/XR871SDK](https://github.com/XradioTech/XR871SDK)

## Toolchain
Before development, please setup toolchain.

```shell
./setup_toolchain.sh 
```

## Building

- Building commands
```
  $ cd ${prj_gcc_path}     # eg. cd project/wlan_demo/gcc
  $ make lib               # build libraries and copy them to "lib"
  $ make lib_clean         # remove files in "src" generated by `make lib`
  $ make lib_install_clean # Remove libraries in "lib" generated by `make lib`
  $ make                   # build the executable binary
  $ make clean             # remove files generated by `make`
  $ make image             # create the image file
  $ make image_clean       # remove files generated by `make image`
  $ make objdump           # generate the disassembled file
  $ make build             # same as `make lib && make && make image`
  $ make build_clean       # same as `make image_clean clean lib_clean`
```