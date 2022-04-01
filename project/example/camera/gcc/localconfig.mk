#
# project local config options, override the common config options
#

# ----------------------------------------------------------------------------
# board definition
# ----------------------------------------------------------------------------
__PRJ_CONFIG_BOARD := YuzukiXR32

# ----------------------------------------------------------------------------
# override global config options
# ----------------------------------------------------------------------------
# set chip type: xr871 or xr32
export __CONFIG_CHIP_TYPE := xr32
# set y to enable bootloader and disable some features, for bootloader only
# export __CONFIG_BOOTLOADER := y

# set n to disable dual core features, for bootloader only
# export __CONFIG_ARCH_DUAL_CORE := n

# ----------------------------------------------------------------------------
# override project common config options
# ----------------------------------------------------------------------------
# support ram extended in another address space, for xr32 only, default to n
__PRJ_CONFIG_RAM_EXT := y

# support xplayer, default to n
# __PRJ_CONFIG_XPLAYER := y

# enable XIP, default to n
# __PRJ_CONFIG_XIP := y

# enable OTA, default to n
# __PRJ_CONFIG_OTA := y
