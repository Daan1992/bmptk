#############################################################################
#
# file: bmptk/Makefile.inc
#
# Makefile for bare metal C++ projects
#
# (c) Wouter van Ooijen (www.voti.nl) 2013
#
# LICENSE (MIT expat license, copy of bmptk/license.txt)
#
# Copyright (c) 2012, 2013 Wouter van Ooijen (wouter@voti.nl)
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
#############################################################################


# This string must not contain spaces, bacuse that does
# (for some unclear reason) not work on Windows8
VERSION := V02.00-work-in-progress-2013-08-30


#============================================================================
#
# find the external tools
# 
#============================================================================

ifneq ($(wildcard $(BMPTK)/Makefile.custom),)
   include $(BMPTK)/Makefile.custom
else   
   include $(BMPTK)/Makefile.local
endif

# $(error $(WPF) )


#============================================================================
#
# the project source file is sure part of the project, but
# it can either be a .c or a .cpp file
#
# the default project name is 'main'
# 
#============================================================================

ifeq ($(PROJECT),)
   PROJECT = main
endif   

ifeq ($(wildcard $(PROJECT)),)
   ifeq ($(wildcard $(PROJECT).cpp),)
      ifeq ($(wildcard $(PROJECT).c),)
         ifeq ($(wildcard $(PROJECT).asm),)
            $(error PROJECT ($(PROJECT)) is not a .cpp, .c or .asm file)
         else	  
            SOURCES += $(PROJECT).asm
         endif	  
      else	  
         SOURCES += $(PROJECT).c
      endif	  
   else	     
      SOURCES += $(PROJECT).cpp
   endif   
else
   SOURCES += $(PROJECT)
endif


#============================================================================
#
# decide whether we have a C or C++ project
# 
#============================================================================

SOURCES := $(strip $(SOURCES))
ifeq ($(patsubst %.cpp,,$(SOURCES)),$(SOURCES))
   LANGUAGE := C
else
   LANGUAGE := C++
endif
# $(error $(LANGUAGE) '$(SOURCES)' '$(patsubst %.cpp,,$(SOURCES))' )


#============================================================================
#
# find the library's files
# 
#============================================================================

VPATH     += $(BMPTK)
VPATH     += $(BMPTK)/core
VPATH     += $(BMPTK)/targets
VPATH     += $(BMPTK)/graphics
VPATH     += $(BMPTK)/rtos
VPATH     += $(BMPTK)/chips
VPATH     += $(SEARCH)

INCLUDES  += -I.
INCLUDES  += -I$(BMPTK)
INCLUDES  += -I$(BMPTK)/targets
INCLUDES  += -I$(BMPTK)/graphics
INCLUDES  += -I$(BMPTK)/rtos
INCLUDES  += -I$(BMPTK)/chips
INCLUDES  += $(foreach x, $(SEARCH), -I$(x))

bmptk_graphics.h:
	# You specified an absolute path ($(BMPTK)) for the bmptk location.
	# The 'make' tool you use has a bug in handling absolute paths that
	# prevents the bmptk makefile from finding some source files.
	# Two solutions are: 
	# 1) Use a make that does not have this bug.
	#    The gnu make available from www.gnu.org/software/make/ does
	#    not have this bug, nor does the cs-make that is part of the 
	#    CodesSourcery distribution. 
	# 2) Use a relative path to specify the bmptk location. 
	#    This almost forces you to locate your project inside the bmptk 
	#    tree, (a subdirectory of) the examples directory is a 
	#    reasonable choice.

#============================================================================
#
# default port, baudrate, xtal for download and debug log
# 
#============================================================================

SERIAL_PORT := COM4
BAUDRATE    := 38400
XTAL        := 12000


#============================================================================
#
# tool executables
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

NDSTOOL        := "$(DEVKITPRO)/devkitARM/bin/ndstool"
IMGTOCPP-TOOL  := $(BMPTK)/tools/img_to_cpp.py
FFTTOCPP-TOOL  := $(BMPTK)/tools/ttf_to_cpp.py
TXTTOCPP-TOOL  := $(BMPTK)/tools/txt_to_cpp.py
IMGTOCPP       := $(PYTHON) $(IMGTOCPP-TOOL)
FFTTOCPP       := $(PYTHON) $(FFTTOCPP-TOOL)
TXTTOCPP       := $(PYTHON) $(TXTTOCPP-TOOL)
#RM             := CMD.EXE /C del 
RM             := "$(BMPTK)/tools/cs-rm" -f 

#needed for DevCpp
export PATH:=$(PATH);/bin;$(DEVCPP)/bin


#============================================================================
#
# Intermediate and result files
# 
#============================================================================

BIN       := $(PROJECT).bin
ELF       := $(PROJECT).elf
MAP       := $(PROJECT).map
HEX       := $(PROJECT).hex
DMP       := $(PROJECT).dmp
LSS       := $(PROJECT).lss
NDS       := $(PROJECT).nds
EXE       := $(PROJECT).exe
NMDUMP    := $(PROJECT).nmdump
OBJ       += $(OBJECTS)
ifeq ($(LANGUAGE),C++)
   OBJ       += bmptk.o
   # OBJ       += memory.o context.o rtos.o  # does not yet work for nds and win
endif   
OBJ       += $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(SOURCES))))
OBJ       += $(patsubst %,%.o,$(LIBS))
OBJ       += $(patsubst %.jpg,%.o,$(IMAGES))
LST       += $(patsubst %.o,%.lst,$(OBJ))


#============================================================================
#
# boards
# 
#============================================================================

# the target can be either a chip, or a board
ifeq ($(TARGET),)
   TARGET = $(CHIP)
endif
ifeq ($(CHIP),)
   CHIP = $(TARGET)
endif

ifeq ($(TARGET),db103)
   CHIP        := lpc1114fn28
endif


#============================================================================
#
# target-dependent parts
# 
#============================================================================

# will remain empty when no valid target is found
PREFIX     := 

HAS_FILESYSTEM = 0

ifeq ($(TARGET),nds)
   CHIP        := ARMv7
   PREFIX      := $(DEVKITPRO)/devkitARM/bin/arm-none-eabi-
   CORE_FLAGS  := -march=armv5te -mtune=arm946e-s -fomit-frame-pointer 
   CORE_FLAGS  += -ffast-math -mthumb -mthumb-interwork -DARM9
   INCLUDES    += -I$(DEVKITPRO)/ -I$(DEVKITPRO)/libnds/include 
   LIBRARIES   += -L$(DEVKITPRO)/libnds/lib -lnds9
   LDFLAGS	   += -g -mthumb -mthumb-interwork -specs=ds_arm9.specs
   RESULTS     = $(ELF) $(NDS) $(NMDUMP)
   RUN         = "$(DEVKITPRO)/emulators/desmume/DeSmuME" $(NDS)
   LIBS        += nds
   VPATH       += $(BMPTK)/stdlib $(BMPTK)/targets/nds
   INCLUDES    += -I$(BMPTK)/stdlib -I$(BMPTK)/targets/nds
   TICKS_PER_US = 67
   LINKERSCRIPT =
endif

ifeq ($(TARGET),win)
   CHIP        := Intel
   PREFIX      := $(DEVCPP)/bin/
   CORE_FLAGS  := -mno-ms-bitfields   
   INCLUDES    += -I"$(DEVCPPLIB)/lib/gcc/mingw32/3.4.2/include"  
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2/backward" 
   INCLUDES    += -I"$(DEVCPP)/lib/gcc/mingw32"
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2"
   INCLUDES    += -I"$(DEVCPP)/include"
   LIBRARIES   += -L"$(DEVCPP)/lib" 
   LDFLAGS     += -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
   RESULTS     = $(EXE)
   RUN         = ./$(EXE)
   LIBS        += win
   VPATH       += $(BMPTK)/targets/win
   INCLUDES    += -I$(BMPTK)/targets/win
   TICKS_PER_US = 1
   HAS_FILESYSTEM = 1
   LINKERSCRIPT =
endif

ifeq ($(TARGET),olimex-mod-lcd43)
   XTAL        := 12000000
   CHIP        := lpc2478
endif

ifeq ($(CHIP),lpc2148)
   XTAL        := 12000000
   PREFIX      := $(ARMDEVENV)/bin/arm-thumb-elf-
   CORE_FLAGS  := -mcpu=arm7tdmi -mtune=arm7tdmi -nostartfiles -fno-exceptions 
   CORE_FLAGS  += -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr
   LDFLAGS	   += -T$(BMPTK)/targets/bmptk_$(CHIP).ld -nostartfiles
   SOURCES     += bmptk_$(CHIP).cpp bmptk_$(CHIP)_startup.s
   HEADERS     += bmptk_$(CHIP).h
   RESULTS     = $(ELF) $(BIN) $(MAP) $(HEX) $(DMP) $(LSS)
endif

ifeq ($(CHIP),lpc2478)
   XTAL        := 12000000
   PREFIX      := $(ARMTOOLS)/bin/arm-none-eabi-
   # PREFIX      := $(ARMDEVENV)/bin/arm-thumb-elf-
   CORE_FLAGS  := -mcpu=arm7tdmi -mtune=arm7tdmi -nostartfiles -fno-exceptions 
   CORE_FLAGS  += -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr
   LDFLAGS	   += -T$(BMPTK)/targets/$(CHIP).ld -nostartfiles
   SOURCES     += $(CHIP).cpp $(CHIP)_startup.s
   HEADERS     += $(CHIP).h
   RESULTS     = $(ELF) $(BIN) $(MAP) $(HEX) $(DMP) $(LSS)
endif

define Embedded
   ifeq ($(LANGUAGE),C++)
      OBJ         += iostream.o cxa_pure_virtual.o new.o
      #SOURCES     += $(CHIP).cpp    
   endif	  
   CORE_FLAGS  += -nostartfiles -fno-exceptions 
   CORE_CPP_FLAGS  += -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr
   VPATH       += $(BMPTK)/stdlib
   INCLUDES    += -I$(BMPTK)/stdlib
   OBJ         += memcpy.o delete.o
   PREFIX      := $(ARMTOOLS)/bin/arm-none-eabi-
   #HEADERS     += $(CHIP).h   
   DEFINES     += -DBMPTK_EMBEDDED_HEAP
   RESULTS     += $(NMDUMP) sizes 
endef

define Cortex
   $(eval $(Embedded))
   INCLUDES    += -I$(BMPTK)/targets/cortex/cmsis
   XTAL        := 12000000
   SOURCES     += cortex_startup.c
   RESULTS     += $(ELF) $(BIN) $(MAP) $(HEX) $(DMP) $(LSS) $(NMDUMP)
   FMCMD       = COM($(patsubst COM%,%,$(SERIAL_PORT)),$(BAUDRATE)) 
   FMCMD       += DEVICE($(FNAME), 12000000, 0)
   FMCMD       += HARDWARE(BOOTEXEC, 50, 100)
   FMCMD       += ERASE(DEVICE, PROTECTISP)
   FMCMD       += HEXFILE($(HEX), NOCHECKSUMS, NOFILL, PROTECTISP)
   RUN         = $(LPC21ISP) -control -verify -term $(HEX) $(SERIAL_PORT) $(BAUDRATE) 12000
   # TERMINAL    = lpc21isp -termonly x $(SERIAL_PORT) 19200 y
   # RUN         = $(FLASHMAGIC) '$(FMCMD)'
   LINKERSCRIPT = linkerscript.ld
   LDFLAGS	   += -T linkerscript.ld -nostartfiles -nodefaultlibs
   RESULTS     += linkerscript.ld
   TICKS_PER_US = 6
endef

define Cortex-M0
   $(eval $(Cortex))
   VPATH       += $(BMPTK)/targets/cortex/libaeabi-cortexm0
   CORE_FLAGS  += -mcpu=cortex-m0 -mthumb 
   VPATH       += $(BMPTK)/targets/cortex
   INCLUDES    += -I$(BMPTK)/targets/cortex
   LIBA        += libaeabi-cortexm0.a
   OBJ         += cortex_m0_resume.o
endef

define LPC1114
   $(eval $(Cortex-M0))
   INCLUDES    += -I$(BMPTK)/targets/cortex/cmsis/11xx/inc
   SOURCES     += lpc1114.cpp   
endef

define LPC1227
   $(eval $(Cortex-M0))
   INCLUDES    += -I$(BMPTK)/targets/cortex/cmsis/12xx/inc
   SOURCES     += lpc1227.cpp   
endef

ifeq ($(CHIP),lpc1114fn28)
   $(eval $(LPC1114))
   HEADERS     += lpc1114fn28.h
   FNAME       = LPC1114/102
   LD_CONFIG   = -DROM_START=0 -DROM_SIZE=32k -DRAM_START=0x10000000 -DRAM_SIZE=4k -DSTACK_SIZE=1k
endif

ifeq ($(CHIP),lpc810m021)
   $(eval $(Cortex-M0))
   FNAME       = LPC810M021FN8
   LD_CONFIG   = -DROM_START=0 -DROM_SIZE=8k -DRAM_START=0x10000000 -DRAM_SIZE=1k -DSTACK_SIZE=256
   INCLUDES    += -I$(BMPTK)/targets/cmsis/lpc800/inc
endif

ifeq ($(CHIP),lpc1227fbd301)
   $(eval $(LPC1227))
   FNAME       = LPC1227/301
   LD_CONFIG   = -DROM_START=0 -DROM_SIZE=96k -DRAM_START=0x10000000 -DRAM_SIZE=8k -DSTACK_SIZE=2k
endif

ifeq ($(PREFIX),)
   $(error invalid or no target specified: "$(TARGET)")
endif
   
DEFINES   += -DBMPTK_TARGET=$(TARGET)
DEFINES   += -DBMPTK_TARGET_$(TARGET)
DEFINES   += -DBMPTK_CHIP=$(CHIP)
DEFINES   += -DBMPTK_CHIP_$(CHIP)
DEFINES   += -DBMPTK_XTAL=$(XTAL)
DEFINES   += -DBMPTK_BAUDRATE=$(BAUDRATE)
DEFINES   += -DBMPTK_VERSION=$(VERSION)
DEFINES   += -DBMPTK_TICKS_PER_US=$(TICKS_PER_US)

ifeq ($(HAS_FILESYSTEM),1)
   DEFINES   += -DBMPTK_HAS_FILESYSTEM
endif   


#============================================================================
#
# the default font, required by bmptk_graphics 
#
# The 04B_03__.ttf file (listed as freeware) was downloaded from
# http://www.eaglefonts.com/04b03-ttf-131597.htm
# 
#============================================================================

FONT-DEFAULT  := $(BMPTK)/graphics/fonts/04B_03__.TTF

#ifeq (bmptk_graphics,$(filter bmptk_graphics,$(LIBS)))
   LIBS          += font_default graphics
   DEFINES       += -DBMPTK_GRAPHICS
#endif

#font_default.cpp font_default.h: $(FONT-DEFAULT) $(FFTTOCPP-TOOL)
#		$(FFTTOCPP) $(FONT-DEFAULT) font_default font_default 8 "" bmptk::graphics

ifeq (bmptk_rtos,$(filter bmptk_rtos,$(LIBS)))
   LIBS          += bmptk_coroutines
   DEFINES       += -DBMPTK_RTOS
endif


#============================================================================
#
# Dependencies
# 
#============================================================================

HEADERS   += $(patsubst %,%.h,$(LIBS))

HEADERS   += core/basics.h core/time.h core/memory.h 
HEADERS   += core/context.h core/rtos.h
HEADERS   += hardware/pins.h hardware/ports.h hardware/demos.h 
HEADERS   += hardware/i2c.h hardware/spi.h
HEADERS   += chips/pcf8574.h chips/pcd8544.h chips/hc595.h
# HEADERS   += hardware/rfm73.h hardware/nrf24l01.h
HEADERS   += chips/sr04.h

# Assume all sources depend on all headers, the linkerscript, and the Makefile
DEPEND    := $(HEADERS) $(LNK) Makefile
$(OBJ): $(DEPEND)
             
             
#============================================================================
#
# tool options
# 
#============================================================================

CPPFLAGS    += $(CORE_FLAGS) $(CORE_CPP_FLAGS) $(INCLUDES) $(DEFINES) -I$(BMPTK) 
CFLAGS      += $(CORE_FLAGS) $(CORE_CC_FLAGS)  $(INCLUDES) $(DEFINES) -I$(BMPTK) 

# CPPFLAGS    += -g -O2 -pipe -Wall  -Wno-multichar 
CPPFLAGS    += -fno-builtin  -Werror 


CPPFLAGS    += -fno-rtti -fno-exceptions 

#CPPFLAGS    += -nostartfiles -nodefaultlibs
#CFLAGS    += -nostartfiles -nodefaultlibs 
# not for devcpp
# CPPFLAGS    += -mstructure-size-boundary=32 
CPPFLAGS    += -std=c++11 -Wall 

# CPPFLAGS  +=  -mapcs -mapcs-frame -mapcs-stack-check
# CPPFLAGS  +=  -fstack-limit-symbol=_stack_start
# CPPFLAGS  +=   -fstack-check=generic -fstack-limit-register=r6
# -fstack-limit-symbol=current_stack_limit 
# 

ASFLAGS     := $(CORE_FLAGS) $(INCLUDES) $(DEFINES) -I$(BMPTK) 
ASFLAGS     += -x assembler-with-cpp

# default LDFLAGS
LDFLAGS     +=  -Wl,-Map,$(MAP)      
LDFLAGS     += -Wl,--gc-sections       
#LDFLAGS     += -nostartfiles -nodefaultlibs
// LDFLAGS     += -lstdc++

# sections
CPPFLAGS    += -fdata-sections -ffunction-sections
CFLAGS      += -fdata-sections -ffunction-sections
LDFLAGS     += -fdata-sections  -ffunction-sections


# notyet :(
# CPPFLAGS    += -fdiagnostics-show-option

#============================================================================
#
# how to make things
# 
#============================================================================

#OBJ := blink.o lpc1114.o
#LDFLAGS	   := -Tlpc1114.ld -nostartfiles -Wl,-Map,$(MAP)  
# CPPFLAGS    +=  -MD -MP -MF
# CPPFLAGS    +=  -no-exceptions -fverbose-asm 
# CPPFLAGS += -Wa,-ahlms=$(patsubst,%.cpp,%.lst,$<)
CPPFLAGS   += -Os 

# seems to have no effect
# CPPFLAGS += -finline-functions

CFLAGS += -Os

# use only the explcit rules below
.SUFFIXES:

# How to make object files?
%.o: %.cpp
	@echo 'Compiling C++ file: $<'
	$(CPP) $(CPPFLAGS) -c -o $@ $< 
	$(CPP) $(CPPFLAGS) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo ' '	
	
%.o: %.c
	@echo 'Compiling C file: $<'
	$(CC) $(CFLAGS) -c -o $@ $< 
	$(CC) $(CFLAGS) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo ' '	
	
%.o: %.S
	@echo 'Assembling file: $<'
	$(AS)  $(ASFLAGS)  -c -o $@ $<
	@echo ' '	

# make the libeabi	
LIBEABI-FILES  = crt.S divdi3.S idiv.S idivmod.S lasr.S ldivmod.S llsl.S llsr.S   
LIBEABI-FILES += lmul.S moddi3.S modsi3.S uidivmod.S uldivmod.S umoddi3.S umodsi3.S
CLEAN += $(LIBEABI-FILES:%.S=%.o) libaeabi-cortexm0.a
# libaeabi-cortexm0.a: $(patsubst %.S,%.o,$(LIBEABI-FILES))
libaeabi-cortexm0.a: $(LIBEABI-FILES)
	@echo 'Creating library libeabi-cortexm0.a: $<'
	make $(patsubst %.S,%.o,$(LIBEABI-FILES))
	"$(PREFIX)ar" rcs libaeabi-cortexm0.a $(LIBEABI-FILES:%.S=%.o)
	$(RM) '$(LIBEABI-FILES:%.S=%.o)'
	@echo ' '		

# How to make a listing from a cpp file
%.list: %.cpp
	$(CPP) $(CPPFLAGS) -c -S -o $@ $<

# make the linkerscipt   
linkerscript.ld: $(BMPTK)/targets/cortex/linkerscript.c
	@echo 'creating the linkerscript'
	$(CC) $(LD_CONFIG) -E -P -C $(BMPTK)/targets/cortex/linkerscript.c -o $@ 
	@echo ' '
   
# make .elf by linking all objects
$(ELF): $(OBJ) $(LIBA) $(LNK) $(LINKERSCRIPT)
	@echo 'Linking target: $(ELF)'
	$(LINKER) -o $@ $(OBJ) $(LIBA) $(LDFLAGS) $(LIBRARIES)
	@echo ' '	
   
# make .exe by linking all objects
$(EXE): $(OBJ) $(LNK) 
	@echo 'Linking target: $(EXE)'
	$(LINKER) -o $@ $(OBJ) $(LDFLAGS) $(LIBRARIES) 
	@echo ' '	
	
# make sizes listing from the elf file
$(NMDUMP): $(ELF)
	$(NM) -S --size-sort --radix=d $(ELF) >$(NMDUMP)

# make .bin from .elf
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# make .hex from .elf
$(HEX): $(ELF)
	$(OBJCOPY) --output-target ihex $< $@
	
# make .dmp from elf
$(DMP): $(ELF)
	$(OBJDUMP) -x --syms $< > $@

# make .lss from elf
$(LSS): $(ELF)
	$(OBJDUMP) -S $< > $@

# make .nds from .elf
$(NDS): $(ELF)
	$(NDSTOOL) -c $@ -9 $< -b $(DEVKITPRO)/libnds/icon.bmp "$(PROJECT)"		

# show object sizes
.PHONY: sizes
sizes: $(OBJ)
	$(SIZES) -B -t $(OBJ)
	"$(BMPTK)/tools/image_sizes/image_sizes.exe" $(MAP)


#============================================================================
#
# targets
# 
#============================================================================

.PHONY: $(PROJECT) all build new fresh run clean

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
	$(RUN)
	$(TERMINAL)
   
# Cleanup
CLEAN += $(RESULTS)
CLEAN += $(OBJ)
CLEAN += $(MAP)
CLEAN += $(LST)
clean:
	$(RM) $(CLEAN)

