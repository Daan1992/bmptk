#============================================================================
#
# This file defines the directories and files that are part of hwcpp.
#
# THis file can be included in a Makefile.
# 
#============================================================================

HWCPP_DIRECTORIES   = hwcpp hwcpp/core hwcpp/targets hwcpp/chips

HWCPP_INCLUDES      = 
HWCPP_INCLUDES      += hwcpp.hpp
HWCPP_INCLUDES      += basics.hpp timing.hpp
HWCPP_INCLUDES      += string.hpp
HWCPP_INCLUDES      += numeric.hpp
HWCPP_INCLUDES      += pins.hpp channels.hpp
HWCPP_INCLUDES      += i2c.hpp spi.hpp one_wire.hpp
HWCPP_INCLUDES      += hc595.hpp pcf8574.hpp mcp23xxx.hpp pcf8591.hpp
HWCPP_INCLUDES      += hd44780.hpp
# HWCPP_INCLUDES      += graphics.hpp
HWCPP_INCLUDES      += pcd8544.hpp lcd_19.hpp
HWCPP_INCLUDES      += ds18x20.hpp
HWCPP_INCLUDES      += ads7843.hpp