
#
# Toolchain configuration
#
CONFIG_TOOLCHAIN_PATH="/opt/kendryte-toolchain/bin"
CONFIG_TOOLCHAIN_PREFIX="riscv64-unknown-elf-"
# end of Toolchain configuration

#
# Components configuration
#

#
# Board config
#
CONFIG_BOARD_MAIX=y
# CONFIG_BOARD_M5STICK is not set
# end of Board config

CONFIG_COMPONENT_DRIVERS_ENABLE=y
CONFIG_COMPONENT_KENDRYTE_SDK_ENABLE=y
CONFIG_SDK_LOG_LEVEL=5
CONFIG_COMPONENT_MICROPYTHON_ENABLE=y

#
# Micropython configurations
#
CONFIG_COMPONENT_SPIFFS_ENABLE=y
CONFIG_COMPONENT_UTILS_ENABLE=y
# end of Components configuration


