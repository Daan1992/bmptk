#############################################################################
#
# file: bmptk/Makefile.inc
#
# Makefile 'body' for bare metal C /C++ / assembler projects using GCC
#
# This file is meant to be included by a project-specific Makefile.
#
# Copyright (c) 2012 .. 2015 Wouter van Ooijen (wouter@voti.nl)
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
# 
#############################################################################


# This string must contain neither spaces (problems on windows8)
# nor periods (problems on xtensa)
VERSION := V03_00_work_in_progress_2015_02_11


#============================================================================
#
# Invoke the Makefile.custom (if it exists) or else the Makefile.local,
# to get the locations of the external toolsets
# 
#============================================================================

ifeq ($(OS),Windows_NT)
   # WPF is the windows prefix for 32-bit applications
   ifeq ($(wildcard C:/Program\ Files\ (x86)/* ),)
      WPF := C:/Program Files
   else
      WPF := C:/Program Files (x86)
   endif
endif

ifneq ($(wildcard $(BMPTK)/Makefile.custom),)
   include $(BMPTK)/Makefile.custom
else   
   include $(BMPTK)/Makefile.local
endif


#============================================================================
#
# These windows executables are distributed with bmptk,
# or must be installed separately.
#
# For another platform you will have to compile them or find a suitable 
# replacement.
# 
#============================================================================

ifeq ($(OS),Windows_NT)

   # seems to be required in some circustances
   SHELL=C:/Windows/System32/cmd.exe

   # distributed with bmptk 
   MAKE          ?= "bmptk-make.exe"
   REMOVE        ?= "bmptk-rm.exe" -f
   COPY          ?= "bmptk-cp.exe" -f
   IMAGE_SIZES   ?= "$(BMPTK)/tools/image_sizes/image_sizes.exe"
   CHECK_PORT    ?= "$(BMPTK)/tools/check_serial_port/check_serial_port.exe"
   CHECK_DRIVE   ?= "$(BMPTK)/tools/check_drive_name/check_drive_name.exe"
   COPY_TO_DRIVE ?= "$(BMPTK)/tools/copy_to_drive/copy_to_drive.exe"
   LPC21ISP      ?= "$(BMPTK)/tools/lpc21isp_197/lpc21isp_hr.exe" 
   AVRDUDE       ?= "$(BMPTK)/tools/avrdude/avrdude.exe"
   AVRDUDEC      ?= "$(BMPTK)/tools/avrdude/avrdude.conf"
   TERMINAL      ?= "$(BMPTK)/tools/Terminal.exe"
   BOSSAC        ?= "$(BMPTK)/tools/bmptk-bossac.exe"
   DUE_BOOTMODE  ?= cmd "/C due_bootmode.bat"
   
   # installed separately
   ATPROGRAM     ?= "$(AtmelToolchain)/../Atmel Studio 6.2/atbackend/atprogram.exe"
   STM32_CLI     ?= "$(STM32LU)/ST-LINK Utility/ST-LINK_CLI.exe"     
   
   # windows
   PAUSE         ?= cmd /c pause     
   
else

   # linux
   MAKE          ?= make
   REMOVE        ?= rm -f
   COPY          ?= cp
   
   # distributed with bmptk
   IMAGE_SIZES   ?= $(BMPTK)/tools/image_sizes/image_sizes
   CHECK_PORT    ?= $(BMPTK)/tools/check_serial_port/check_serial_port
   CHECK_DRIVE   ?= $(BMPTK)/tools/check_drive_name/check_drive_name
   # COPY_TO_DRIVE ?= $(BMPTK)/tools/copy_to_drive/copy_to_drive
   LPC21ISP      ?= $(BMPTK)/tools/lpc21isp_197/lpc21isp_hr
   
   # installed separately
   AVRDUDE       ?= avrdude
   
endif


#============================================================================
#
# The main project source file is sure part of the project.
# By default it is the name of the project, but we must find out what
# the extension is: .asm, .c or .cpp
#
# the default project name is 'main'
# 
#============================================================================

ifeq ($(MAKECMDGOALS),clean)
   NOPROJECT = 1
endif
ifeq ($(MAKECMDGOALS),doxygen)
   NOPROJECT = 1
endif

PROJECT ?= main

ifeq ($(wildcard $(PROJECT).cpp),)
   ifeq ($(wildcard $(PROJECT).c),)
      ifeq ($(wildcard $(PROJECT).asm),)
         ifneq ($(NOPROJECT),1)
            $(error PROJECT ($(PROJECT)) is not a .cpp, .c or .asm file)
         endif   
      else	  
         SOURCES += $(PROJECT).asm
      endif	  
   else	  
      SOURCES += $(PROJECT).c
   endif	  
else	     
   SOURCES += $(PROJECT).cpp
endif   


#============================================================================
#
# decide whether we have a C++ project or a C/Assembler project
#
# When a project uses at least one C++ file it is a C++ project, and
# the C++ tools are used for ALL files. If not, the C tools are used.
# 
#============================================================================

SOURCES := $(strip $(SOURCES))
ifeq ($(patsubst %.cpp,,$(SOURCES)),$(SOURCES))
   LANGUAGE ?= C
else
   LANGUAGE ?= C++
endif
# $(error $(LANGUAGE) '$(SOURCES)' '$(patsubst %.cpp,,$(SOURCES))' )

   
#============================================================================
#
# GCC executables
#
# = is used because the PREFIX is set by the target-specific parts
# 
#============================================================================

CC        = "$(PREFIX)gcc"
CPP       = "$(PREFIX)g++"
AS        = "$(PREFIX)gcc"
OBJCOPY   = "$(PREFIX)objcopy"
OBJDUMP   = "$(PREFIX)objdump"
SIZES     = "$(PREFIX)size"
NM        = "$(PREFIX)nm"

ifeq ($(LANGUAGE),C)
   LINKER    = "$(PREFIX)gcc"
else
   LINKER    = "$(PREFIX)g++"
endif

#============================================================================
#
# Result files
# 
#============================================================================

BIN       := $(PROJECT).bin
ELF       := $(PROJECT).elf
MAP       := $(PROJECT).map
HEX       := $(PROJECT).hex
SREC      := $(PROJECT).srec
DMP       := $(PROJECT).dmp
LSS       := $(PROJECT).lss
NDS       := $(PROJECT).nds
EXE       := $(PROJECT).exe
OUT       := $(PROJECT).out
BIN1      := $(PROJECT).bin1
BIN2      := $(PROJECT).bin2
NMDUMP    := $(PROJECT).nmdump


#============================================================================
#
# boards
# 
#============================================================================

ifeq ($(TARGET),esp-01)
   CHIP        ?= esp8266
   # by default the chip runs at 80 MHz
   XTAL        ?= 80000   
endif

ifeq ($(TARGET),lpc800_mini_kit)
   CHIP        ?= lpc810m021fn8
   XTAL        ?= 12000   
endif

ifeq ($(TARGET),lpc800_max)
   CHIP        ?= lpc812m101jdh16
   XTAL        ?= 12000   
#   PORT_CHECK  ?=
#   DOWNLOAD_DRIVE ?= MBED
#   RUN         ?= $(COPY_TO_DRIVE) $(BIN) $(DOWNLOAD_DRIVE) application.bin   
endif

ifeq ($(TARGET),tiny_11c14)
   CHIP        ?= lpc11c14fbd48
   XTAL        ?= 12000   
endif

ifeq ($(TARGET),arduino_uno)
   CHIP        ?= atmega328p
   XTAL        ?= 12000   
endif

ifeq ($(TARGET),arduino_due)
   CHIP        ?= atsam3x8e
   XTAL        ?= 12000   
endif

ifeq ($(TARGET),msp_exp430g2)
   CHIP        ?= msp430g2553
   XTAL        ?= 16000   
endif

ifeq ($(TARGET),msp_exp430fr4133)
   CHIP        ?= msp430fr4133
   XTAL        ?= 16000   
endif

ifeq ($(TARGET),db103)
   CHIP        ?= lpc1114fn28
endif
ifeq ($(TARGET),db104)
   CHIP        ?= lpc1114fn28
endif
ifeq ($(TARGET),db105)
   CHIP        ?= lpc810m021fn8
endif

ifeq ($(TARGET),stm32f0discovery)
   CHIP        ?= stm32f051r8
endif
ifeq ($(TARGET),stm32l1discovery)
   CHIP        ?= stm32l152rc
endif
ifeq ($(TARGET),stm32f411nucleo)
   CHIP        ?= stm32f411re
endif

ifeq ($(TARGET),frdm_kl25z)
   CHIP        ?= kl25z128vlk4
endif

ifeq ($(TARGET),xmc_2go_1100)
   CHIP        ?= xmc1100
endif

# the target can be either a chip, or a board
ifeq ($(TARGET),)
   TARGET = $(CHIP)
else
   ifeq ($(CHIP),)
      CHIP = $(TARGET)
   endif
endif  


#============================================================================
#
# target-specific parts
# 
#============================================================================

define All_Targets
   # make sure the bmptk and hwcpp files are found
   SEARCH            += $(BMPTK)
   SEARCH            += $(BMPTK)/targets
   SEARCH            += $(BMPTK)/hwcpp   
   
   # the as-yet empty interface header
   DEFINES           += -DBMPTK_INCLUDE_BMPTK=bmptk_interface.h    
   HEADERS           += bmptk_interface.h
   
   # show all warnings, treat most as errors
   # CORE_FLAGS        += -Wall -Werror
   #CORE_FLAGS        += -Wno-unused-local-typedefs not on arduino
   #CORE_FLAGS        += -Wno-maybe-uninitialized
   
   # cpp: use latest language standard
   CORE_CPP_FLAGS    += -std=c++11
   
   # assembler: enable the preprocessor
   CORE_AS_FLAGS     += -x assembler-with-cpp   
   
   # will remain empty when no valid target is found
   PREFIX            := 
endef

define Hosted
   $(eval $(All_Targets))
   HOSTED          ?= 1
   EMBEDDED        ?= 1
   HAS_FILESYSTEM  ?= 1
   BMPTK_HEAP      ?= 0
endef

define Embedded 
   $(eval $(All_Targets))
   EMBEDDED        ?= 1
   HOSTED          ?= 0
   HAS_FILESYSTEM  ?= 0
   BMPTK_HEAP      ?= 1
   CORE_FLAGS      += -Os
   CORE_FLAGS      += -fdata-sections -ffunction-sections
   LDFLAGS         += -Wl,-Map,$(MAP)      
   LDFLAGS         += -Wl,--gc-sections  
   LDFLAGS         += -fdata-sections -ffunction-sections 
   LDFLAGS         += -Wl,-fatal-warnings 
   
   RESULTS += $(DMP) $(LSS) $(NMDUMP) 
   # RESULTS += $(MAP)
   # RESULTS += $(LST)
   
   # cpp: don't use features that are too expensive
   CORE_CPP_FLAGS += -fno-rtti -fno-exceptions 
   CORE_CPP_FLAGS += -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr
   
   # create a 'fake' iostream in hwcpp
   DEFINES         += -DHWCPP_FAKE_OSTREAM 
endef

ifeq ($(TARGET),native)
   $(eval $(Hosted))
   
   ifeq ($(OS),Windows_NT)   
   
      # running native on Windows  
      CHIP            := Intel
      PREFIX          := $(GCC-WIN)/bin/
      SEARCH          += $(BMPTK)/targets/win
      SOURCES         += bmptk_win.c
      # HEADERS         += bmptk_win.h does not exist
      CORE_FLAGS      := -mno-ms-bitfields   
      LDFLAGS         += -lgdi32
      RESULTS         += $(EXE)
      export PATH     := $PREFIX:$(PATH)
      RUN             ?= ./$(EXE)
      RUN_PAUSE       ?= $(PAUSE)
   else
   
      # assume running native on some Linux

      CHIP            := LinuxNative
      PREFIX          := 
      RESULTS         += $(EXE)
      RUN             ?= ./$(EXE)
      
   endif   
endif

   #=========================================================================
   #
   # 8-bit chips
   # 
   #=========================================================================
   
define Atmega
   $(eval $(Embedded))
   PREFIX          := $(GCC-AVR)/bin/avr-
   SEARCH          += $(BMPTK)/targets/avr $(BMPTK)/targets/avr/include
   DEFINES         += -DBMPTK_INCLUDE_CHIP=io.h   
   RESULTS         += $(ELF) $(BIN) $(HEX) 
endef   

ifeq ($(CHIP),atmega328p)
   $(eval $(Atmega))
   ROM_START      ?= 0x0000
   RAM_START      ?= 0x0100
   ROM_SIZE       ?= 32k
   RAM_SIZE       ?= 2k
   XTAL           ?= 20000   
   DEFINES        += -D__AVR_ATmega328P__ 
   CORE_FLAGS     += -mmcu=atmega328p
   PORT_CHECK     ?= $(CHECK_PORT) $(SERIAL_PORT)  
   RUN            ?= $(AVRDUDE) -C$(AVRDUDEC) -carduino -patmega328p  -P$(SERIAL_PORT) -b115200 -Uflash:w:$(HEX)
   # RUN            ?= $(ATPROGRAM) -t avrispmk2 -d atmega328p erase program -f $(HEX)
endif

   #=========================================================================
   #
   # 16-bit chips
   # 
   #=========================================================================
   
define MSP430
   $(eval $(Embedded))
   ROM_START      ?= 0x00000000   
   ifeq ($(OS),Windows_NT)
      PREFIX         := $(GCC-MSP430)/bin/msp430-elf-
   else
      PREFIX         := $(GCC-MSP430)/bin/msp430-
   endif   
   # SEARCH         += c:/ti/ccsv6/tools/compiler/gcc_msp430_4.9.14r1_98/msp430-elf/include
   # SEARCH         += c:/ti/ccsv6/ccs_base/msp430/include_gcc
   SEARCH         += $(BMPTK)/targets/msp430/include
   DEFINES        += -DBMPTK_INCLUDE_CHIP=msp430.h 
   #RUN_PAUSE       ?= $(PAUSE)
   RESULTS         += $(ELF) $(BIN) $(HEX) 
   # linkerscript.ld
endef

ifeq ($(CHIP),msp430g2553)
   $(eval $(MSP430))   
   ROM_SIZE       ?= 16k
   RAM_SIZE       ?= 512
   DEFINES        += -mmcu=msp430g2553 
   LDFLAGS	      += -T$(BMPTK)/targets/msp430/linkerscripts/$(CHIP).ld
   RUN            ?= $(MSP430FLASHER) -n MSP430G2553 -w $(HEX) -v -z [VCC] -m SBW2 -g
   RUN_PAUSE      := $(PAUSE)
endif

ifeq ($(CHIP),msp430fr4133)
   $(eval $(MSP430))   
   ROM_SIZE       ?= 15k
   RAM_SIZE       ?= 2k
   DEFINES        += -mmcu=msp430fr4133 
   LDFLAGS	      += -T$(BMPTK)/targets/msp430/linkerscripts/$(CHIP).ld
   RUN            ?= $(MSP430FLASHER) -n MSP430FR4133 -w $(HEX) -v -z [VCC] -m SBW2 -g
   RUN_PAUSE      := $(PAUSE)
endif   
   
   #=========================================================================
   #
   # ARM v7 chips
   # 
   #=========================================================================

define ARM_V7
   $(eval $(Embedded))
   PREFIX          := $(GCC-ARM)/bin/arm-none-eabi-
#   CORE_FLAGS      += -mcpu=cortex-m0 -fomit-frame-pointer -march=armv6-m
   SEARCH          += $(BMPTK)/targets/armv7 $(BMPTK)/targets/armv7/include
   SOURCES         += bmptk_arm_startup.asm
   COREFLAGS    += -nostdlib
   COREFLAGS    += -nodefaultlibs  
   LDFLAGS    += -nostdlib
   LDFLAGS    += -nodefaultlibs  
   CORE_FLAGS      += -nostartfiles 
   LDFLAGS         += -nostartfiles   
   RESULTS         += $(ELF) $(BIN) $(HEX) linkerscript.ld
endef

define LPC_ARM_V7
   $(eval $(ARM_V7))
   $(eval $(LPC))
   LDFLAGS        += -T linkerscript.ld 
   LINKERSCRIPT    ?= linkerscript.ld
   ROM_START      ?= 0x00000000
   RAM_START      ?= 0x40000000
   INCLUDES       += -I$(BMPTK)/targets/armv7/include
   XTAL           ?= 12000   
   PORT_CHECK     ?= $(CHECK_PORT) $(SERIAL_PORT)  
   RUN            ?= $(LPC21ISP) -control -verify -term $(HEX) $(SERIAL_PORT) $(SERIAL_BAUDRATE) 12000
endef

ifeq ($(CHIP),lpc2148fbd64)
   $(eval $(LPC_ARM_V7))
   ROM_SIZE       ?= 512k
   RAM_SIZE       ?= 32k
   DEFINES        += -DBMPTK_INCLUDE_CHIP=lpc2148.h 
endif
  
   
   #=========================================================================
   #
   # Cortex chips
   # 
   #=========================================================================

define Cortex
   $(eval $(Embedded))
   PREFIX          := $(GCC-ARM)/bin/arm-none-eabi-
   ifeq ($(wildcard $(PREFIX)* ),)
      PREFIX          := $(GCC-ARM)/bin/arm-eabi-
   endif
   SEARCH          += $(BMPTK)/targets/cortex $(BMPTK)/targets/cortex/cmsis
   SOURCES         += bmptk_cortex.c
   #COREFLAGS    += -nostdlib
   #COREFLAGS    += -nodefaultlibs  
   #LDFLAGS    += -nostdlib
   #LDFLAGS    += -nodefaultlibs  
   CORE_FLAGS      += -nostartfiles 
   LDFLAGS         += -nostartfiles   
   LINKERSCRIPT    ?= linkerscript.ld
   # should be $(LINKERSCRIPT)
   LDFLAGS	       += -T linkerscript.ld
   RESULTS         += $(ELF) $(BIN) $(HEX) linkerscript.ld
endef   
   
define Cortex-M0
   $(eval $(Cortex)) 
   CORE_FLAGS      += -mcpu=cortex-m0 -mthumb -fomit-frame-pointer -march=armv6-m
endef

define Cortex-M0+
   $(eval $(Cortex))
   CORE_FLAGS     += -mcpu=cortex-m0 -mthumb -fomit-frame-pointer -march=armv6-m
endef

define Cortex-M3
   $(eval $(Cortex))
   CORE_FLAGS     += -mcpu=cortex-m3 -mthumb -fomit-frame-pointer -march=armv7-m
endef

define Cortex-M4F
   $(eval $(Cortex))
   CORE_FLAGS     += -mcpu=cortex-m4 -mthumb -fomit-frame-pointer 
   # -march=armv7-m
endef
   
      #======================================================================
      #
      # NXP : LPC chips
      # 
      #======================================================================

define LPC_Cortex
   $(eval $(LPC))
   ROM_START      ?= 0x00000000
   RAM_START      ?= 0x10000000
   INCLUDES       += -I$(BMPTK)/targets/cortex/lpc/cmsis   
   XTAL           ?= 12000   
   PORT_CHECK     ?= $(CHECK_PORT) $(SERIAL_PORT)  
   RUN            ?= $(LPC21ISP) -control -verify -term $(HEX) $(SERIAL_PORT) $(SERIAL_BAUDRATE) 12000
endef

define LPC81X
   $(eval $(Cortex-M0+))
   $(eval $(LPC_Cortex))
   INCLUDES       += -I$(BMPTK)/targets/cortex/lpc/cmsis/lpc800/inc
   DEFINES        += -DBMPTK_INCLUDE_CHIP=LPC8xx.h 
endef

ifeq ($(CHIP),lpc810m021fn8)
   $(eval $(LPC81X))   
   ROM_SIZE       ?= 8k
   RAM_SIZE       ?= 1k
endif

ifeq ($(CHIP),lpc812m101jdh16)
   $(eval $(LPC81X))
   ROM_SIZE       ?= 16k
   RAM_SIZE       ?= 4k
endif

define LPC1114
   $(eval $(Cortex-M0))
   $(eval $(LPC_Cortex))
   INCLUDES       += -I$(BMPTK)/targets/cortex/lpc/cmsis/11xx/inc 
   DEFINES        += -DBMPTK_INCLUDE_CHIP=LPC11xx.h 
endef

ifeq ($(CHIP),lpc1114fn28)
   $(eval $(LPC1114))
   ROM_SIZE       ?= 32k
   RAM_SIZE       ?= 4k
   HWCPP_CHIP     ?= lpc1114fn28.hpp
endif

ifeq ($(CHIP),lpc11c14fbd48)
   $(eval $(LPC1114))
   ROM_SIZE       ?= 32k
   RAM_SIZE       ?= 8k
   HWCPP_CHIP     ?= lpc11c14fbd48.hpp
endif

      #======================================================================
      #
      # STM chips
      # 
      #======================================================================

define STM32
   ROM_START      ?= 0x08000000
   RAM_START      ?= 0x20000000     
   INCLUDES       += -I$(BMPTK)/targets/cortex/stm32/cmsis  
   RUN            ?= $(STM32_CLI) -P $(HEX) -Rst
   RUN_PAUSE      ?= $(PAUSE)
endef

ifeq ($(CHIP),stm32f051r8)
   $(eval $(Cortex-M0))
   $(eval $(STM32))
   ROM_SIZE       ?= 64k
   RAM_SIZE       ?= 8k
   DEFINES        += -DBMPTK_INCLUDE_CHIP=stm32f051x8.h 
endif

ifeq ($(CHIP),stm32l152rc)
   $(eval $(Cortex-M3))
   $(eval $(STM32))
   ROM_SIZE       ?= 256k
   RAM_SIZE       ?= 32k  
   DEFINES        += -DBMPTK_INCLUDE_CHIP=stm32l1xx.h 
endif

ifeq ($(CHIP),stm32f411re)
   $(eval $(Cortex-M4F))
   $(eval $(STM32))
   ROM_SIZE       ?= 512k
   RAM_SIZE       ?= 128k   
   DEFINES        += -DBMPTK_INCLUDE_CHIP=stm32f411xe.h 
endif

      #======================================================================
      #
      # Freescale chips
      # 
      #======================================================================

define Freescale
   ROM_START      ?= 0x00000000   
   INCLUDES       += -I$(BMPTK)/targets/cortex/freescale/cmsis 
   RUN            ?= $(STM32_CLI) -P $(HEX) -Rst
   RUN_PAUSE      ?= $(PAUSE)
endef

ifeq ($(CHIP),kl25z128vlk4)
   $(eval $(Cortex-M0))
   $(eval $(Freescale))   
   ROM_SIZE       ?= 128k
   RAM_START      ?= 0x20000000
   RAM_SIZE       ?= 8k
   # 16k   
   DOWNLOAD_DRIVE ?= FRDM-KL25Z
   DEFINES        += -DBMPTK_INCLUDE_CHIP=MKL25Z4.h 
   RUN            = $(COPY_TO_DRIVE) $(BIN) $(DOWNLOAD_DRIVE) application.bin
endif

      #======================================================================
      #
      # Infineon chips
      # 
      #======================================================================

define Infineon
   ROM_START      ?= 0x00000000   
   INCLUDES       += -I$(BMPTK)/targets/cortex/infineon/cmsis 
   RUN            ?= $(STM32_CLI) -P $(HEX) -Rst
   RUN_PAUSE      ?= $(PAUSE)
endef

ifeq ($(CHIP),xmc1100)
   $(eval $(Cortex-M0))
   $(eval $(Infineon))   
   ROM_SIZE       ?= 64k
   RAM_START      ?= 0x20000000
   RAM_SIZE       ?= 16k
   DOWNLOAD_DRIVE ?= FRDM-KL25Z
   DEFINES        += -DBMPTK_INCLUDE_CHIP=XMC1100.h 
   RUN            = $(COPY_TO_DRIVE) $(BIN) $(DOWNLOAD_DRIVE) application.bin
endif


      #======================================================================
      #
      # Atmel chips
      # 
      #======================================================================
      
define Atmel_SAM3
   $(eval $(Cortex-M3))
   ROM_START      ?= 0x00000000   
   RAM_START      ?= 0x20000000     
   INCLUDES       += -I$(BMPTK)/targets/cortex/atmel
   PORT_CHECK     ?= $(CHECK_PORT) $(SERIAL_PORT)  
   RUN_PRE        := $(DUE_BOOTMODE)
   # was al gezet?
   RUN            := $(BOSSAC) -p $(SERIAL_PORT) -U false -e -w -v -b $(BIN) -R
   # RUN            := $(BOSSAC) -e -w -v -b $(BIN)   
   DEFINES        += -DDONT_USE_CMSIS_INIT 
   DEFINES        += -DBMPTK_INCLUDE_CHIP=sam.h 
   LDFLAGS        += -I$(BMPTK)/targets/cortex/atmel/sam3xa/source/as_gcc
endef      

ifeq ($(CHIP),atsam3x8e)
   $(eval $(Atmel_SAM3))
   ROM_SIZE       ?= 512k
   RAM_SIZE       ?= 96k
   DEFINES        += -D__SAM3X8E__ 
endif

      #======================================================================
      #
      # Tensilica xtensa chips
      #
      # Tensilica Xtensa LX3
      # 
      #======================================================================

define Xtensa-106
   $(eval $(Embedded)) 
   PREFIX         := $(GCC-XTENSA)/bin/xtensa-lx106-elf-
endef

ifeq ($(CHIP),esp8266)
   $(eval $(Xtensa-106))   
   ROM_START      ?= 0x00000000   
   ROM_SIZE       ?= 64k
   RAM_START      ?= 0x20000000
   RAM_SIZE       ?= 16k
   INCLUDES       += -I$(ESP-SDK)/include -I$(ESP-SDK)/include/json
   INCLUDES       += -I$(BMPTK)
   INCLUDES       += -I$(BMPTK)/targets/esp8266 
   DEFINES        += -DBMPTK_INCLUDE_CHIP=esp8266.h 
   
   CORE_FLAGS     += -Os -g -O2 -Wl,-EL 
   CORE_FLAGS     += -fno-inline-functions -nostdlib -mlongcalls 
   CORE_FLAGS     += -mtext-section-literals -D__ets__ -DICACHE_FLASH 
   CORE_FLAGS     += 
   
   LD_FLAGS       += -T$(ESP-SDK)\ld\eagle.app.v6.ld 
   LD_FLAGS       += -L$(ESP-SDK)\lib
   LD_FLAGS       += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
   ESP_LIBS       := -lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lmain
   LD_FLAGS       += -Wl,--start-group $(ESP_LIBS) -Wl,--end-group -o $@
   
   OUT_1_ARGS	= -bo $@ -bs .text -bs .data -bs .rodata -bc -ec
   OUT_2_ARGS	= -es .irom0.text $@ -ec
   
   RESULTS        += $(ELF) $(BIN1) $(BIN2)
   
   PORT_CHECK     ?= $(CHECK_PORT) $(SERIAL_PORT)    
   RUN            = $(ESP-LOADER) -p $(SERIAL_PORT) -b $(SERIAL_BAUDRATE) write_flash 0x00000 $(BIN1) 0x40000 $(BIN2)
   RUN_PAUSE      ?= $(PAUSE)
   
   BMPTK_HEAP     := 0
   
endif

      #======================================================================
   
$(BIN1): $(ELF)
	$(ESP-TOOL) -eo $(ELF) $(OUT_1_ARGS)

$(BIN2): $(ELF)
	$(ESP-TOOL) -eo $(ELF) $(OUT_2_ARGS)        

ifneq ($(NOPROJECT),1)
   ifeq ($(HOSTED),)
      $(error invalid or no target specified: "$(TARGET)")
   endif
endif   


#============================================================================
#
# where to find the files
# 
#============================================================================

INCLUDES  += $(foreach x, $(SEARCH), -I$(x))

   
#============================================================================
#
# hwcpp
# 
#============================================================================

ifeq ($(LANGUAGE),C++)
   include $(BMPTK)/hwcpp/Makefile.inc
   SEARCH    += $(foreach x, $(HWCPP_DIRECTORIES), $(BMPTK)/$(x))
   HEADERS   += $(HWCPP_DEPENDENCIES)
endif


#============================================================================
#
# Pass information to the code 
# 
#============================================================================

DEFINES   += -DBMPTK_TARGET=$(TARGET) 
DEFINES   += -DBMPTK_TARGET_$(TARGET) 
DEFINES   += -DBMPTK_CHIP=$(CHIP) 
DEFINES   += -DBMPTK_CHIP_$(CHIP) 
DEFINES   += -DBMPTK_XTAL=$(XTAL) 
DEFINES   += -DBMPTK_BAUDRATE=$(SERIAL_BAUDRATE) 
DEFINES   += -DBMPTK_VERSION=$(VERSION) 

ifeq ($(HOSTED),1)
   DEFINES   += -DBMPTK_HOSTED 
endif   
ifeq ($(EMBEDDED),1)
   DEFINES   += -DBMPTK_EMBEDDED 
endif   
ifeq ($(HAS_FILESYSTEM),1)
   DEFINES   += -DBMPTK_HAS_FILESYSTEM 
endif   
ifeq ($(BMPTK_HEAP),1)
   DEFINES   += -DBMPTK_HEAP   
endif   

SOURCES += bmptk_interface.c


#============================================================================
#
# Object and list files
# 
#============================================================================

OBJ       += $(OBJECTS)
SOURCES1  := $(patsubst %.asm,%.o,$(patsubst %.S,%.o,$(SOURCES)))
OBJ       += $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES1)))
LST       += $(patsubst %.o,%.lst,$(OBJ))

ifneq ($(strip $(findstring $(OPTIONS), LISTINGS)),)
   RESULTS += $(LST)
endif


#============================================================================
#
# Dependencies
# 
#============================================================================

# Assume compilation depends on 
# all headers, the linkerscript, and the Makefile
DEPEND    := $(HEADERS) $(LNK) Makefile
$(OBJ): $(DEPEND)
             
             
#============================================================================
#
# combine the tool options
# 
#============================================================================

COMMON      := $(CORE_FLAGS) $(INCLUDES) $(DEFINES)
CPPFLAGS    += $(COMMON) $(CORE_CPP_FLAGS)
CFLAGS      += $(COMMON) $(CORE_CC_FLAGS) 
ASFLAGS     += $(COMMON) $(CORE_AS_FLAGS)
LDFLAGS     += $(COMMON) $(CORE_LD_FLAGS)

STACK_SIZE   ?= 1024
LD_CONFIG    += -DROM_START=$(ROM_START) 
LD_CONFIG    += -DROM_SIZE=$(ROM_SIZE) 
LD_CONFIG    += -DRAM_START=$(RAM_START) 
LD_CONFIG    += -DRAM_SIZE=$(RAM_SIZE) 
LD_CONFIG    += -DSTACK_SIZE=$(STACK_SIZE) 

CPPFLAGS     += $(LD_CONFIG)
CLAGS        += $(LD_CONFIG)
ASFLAGS      += $(LD_CONFIG)
LDFLAGS      += $(LD_CONFIG) $(LD_FLAGS) -lgcc


#============================================================================
#
# how to make things
# 
#============================================================================

# find non-local source files
# VPATH is not used, beacause it would also 'find' .ld and .o files,
# which should be created locally
vpath %.h    $(SEARCH)
vpath %.hpp  $(SEARCH)
vpath %.asm  $(SEARCH)
vpath %.c    $(SEARCH)
vpath %.cpp  $(SEARCH)

# use only the explcit rules below
.SUFFIXES:

ifeq ($(LANGUAGE),C)
   CC      := $(CC)  $(CFLAGS)
   CPP     := 
   LINKER  := $(CC)
else
   CC      := $(CC)  $(CFLAGS)
   CPP     := $(CPP) $(CPPFLAGS)
   LINKER  := $(CPP)
endif   

# show version
.PHONY: version
build: version
version:
	$(CC) --version   

# How to make assembler listing from asm
%.lst: %.asm
	@echo 'Compiling C file to asm listing: $<'
	$(AS)  $(ASFLAGS) -c -S > $(patsubst %.o,%.lst,$@) $<
	@echo ' '   
   
# How to make assembler listing from C
%.lst: %.c
	@echo 'Compiling C file to asm listing: $<'
	$(CC) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo ' '   
   
# How to make assembler listing from C++   
%.lst: %.cpp
	@echo 'Compiling CPP file to asm listing: $<'
	$(CPP) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo '  '

# How to make object files?
%.o: %.cpp 
	@echo '#Compiling C++ file: $<'
	$(CPP) -c -o $@ $< 
	@echo ' '	
    
%.o: %.c 
	@echo 'Compiling C file: $<'
	$(CC) -c -o $@ $< 
	@echo ' '	 
	
%.o: %.asm
	@echo 'Assembling file: $<'
	$(AS)  $(ASFLAGS)  -c -o $@ $<
	@echo ' '	

# make the linkerscipt  
linkerscript.ld: linkerscript.c
	@echo 'Creating the linkerscript'
	$(CC) $(LD_CONFIG) -E -P -C $< -o $@ 
	@echo ' '
   
# $(LINKER) -Os -Wl,--gc-sections -mmcu=atmega328p -o $@ $(OBJ) 
  
# make .elf by linking all objects
# $(error $(addprefix ./,$(OBJ) ) )
$(ELF): $(addprefix ./,$(OBJ)) $(LIBA) $(LNK) $(LINKERSCRIPT)
	@echo 'Linking target: $(ELF)'
	$(LINKER) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)
	@echo ' '	
   
# make .exe by linking all objects
$(EXE): $(OBJ) $(LNK) 
	@echo 'Linking target: $(EXE)'
	$(LINKER) -o $@ $(OBJ) $(LDFLAGS) $(LIBS) 
	@echo ' '	
	
# make sizes listing from the elf file
$(NMDUMP): $(ELF)
	$(NM) -S --size-sort --radix=d $(ELF) >$(NMDUMP)
	$(SIZES) -B -t $(OBJ)
	$(IMAGE_SIZES) $(MAP)

# make .bin from .elf
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# make .hex from .elf
$(HEX): $(ELF)
	$(OBJCOPY) --output-target ihex $< $@
	
# make .hex from .elf
$(SREC): $(ELF)
	$(OBJCOPY) --output-target srec $< $@
	
# make .dmp from elf
$(DMP): $(ELF)
	$(OBJDUMP) -x --syms $< > $@

# make .lss from elf
$(LSS): $(ELF)
	$(OBJDUMP) -S $< > $@	


#============================================================================
#
# targets
# 
#============================================================================

.PHONY: $(PROJECT) all build new fresh run clean doxygen

.DEFAULT_GOAL := build

# Build the project
$(PROJECT): $(RESULTS)

# aliases for build the project
all: $(PROJECT)
build: $(PROJECT)

# aliases for clean+build
new: clean build
fresh: clean build

# Run (= download and start) the project
run: $(PROJECT)
	$(PORT_CHECK)
	$(RUN_PRE)   
	$(RUN)
	$(RUN_TERMINAL)
	$(RUN_PAUSE)
    
doxygen: doxyfile
	Doxygen
   
# Cleanup
CLEAN += $(RESULTS)
CLEAN += $(OBJ)
CLEAN += $(MAP)
CLEAN += $(LST)
CLEAN += -rf doxygen_docs
clean:
	$(REMOVE) $(CLEAN)

