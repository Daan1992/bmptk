#############################################################################
#
# file: bmptk/Makefile.inc
#
# Makefile for bare metal C /C++ / assembler projects using GCC
#
# Copyright (c) 2012 ... 2015 Wouter van Ooijen (wouter@voti.nl)
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
# 
#############################################################################


# This string must not contain spaces, that does
# (for some unclear reason) not work on Windows8
VERSION := V03.00_work_in_progress_2014_12_17


#============================================================================
#
# Invoke the Makefile.custom (if it exists) or else the Makefile.local,
# to get the locations of the external toolsets
# 
#============================================================================

ifneq ($(wildcard $(BMPTK)/Makefile.custom),)
   include $(BMPTK)/Makefile.custom
else   
   include $(BMPTK)/Makefile.local
endif

# for debugging
# $(error $(WPF) )


#============================================================================
#
# These windows executables are distributed with bmptk.
# For another platform you will have to compile them.
# 
#============================================================================

ifeq ($(OS),Windows_NT)
   MAKE         ?= "bmptk-make.exe"
   REMOVE       ?= "bmptk-rm.exe" -f
   IMAGE_SIZES  ?= "$(BMPTK)/tools/image_sizes/image_sizes.exe"
   CHECK_PORT   ?= "$(BMPTK)/tools/check_serial_port/check_serial_port.exe"
   LPC21ISP     ?= "$(BMPTK)/tools/lpc21isp_197/lpc21isp_hr.exe"
else
   $(error "you must build and define the executables" )
   REMOVE       ?= /bin/rm -f
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
# the C++ tools are used for ALL files.
# If not, the C tools are used.
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
# find the files
# 
#============================================================================

SEARCH    += $(BMPTK)
SEARCH    += $(BMPTK)/targets
INCLUDES  += $(foreach x, $(SEARCH), -I$(x))

   
#============================================================================
#
# default port, baudrate, xtal for download and debug log
# 
#============================================================================

SERIAL_PORT      ?= COM4
SERIAL_BAUDRATE  ?= 38400
XTAL             ?= 12000


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
SOURCESX  := $(patsubst %.asm,%.o,$(patsubst %.S,%.o,$(SOURCES)))
OBJ       += $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCESX)))
LST       += $(patsubst %.o,%.lst,$(OBJ))


#============================================================================
#
# boards
# 
#============================================================================

# the target can be either a chip, or a board
ifeq ($(TARGET),)
   TARGET = $(CHIP)
else
   ifeq ($(CHIP),)
      CHIP = $(TARGET)
   endif
endif   

ifeq ($(TARGET),db103)
   CHIP        := lpc1114fn28
endif

ifeq ($(TARGET),db104)
   CHIP        := lpc1114fn28
endif


#============================================================================
#
# target-specific parts
# 
#============================================================================

# will remain empty when no valid target is found
PREFIX     := 

define Hosted
   HOSTED         = 1
   HAS_FILESYSTEM = 1
endef

ifeq ($(TARGET),win)
   $(eval $(Hosted))
   CHIP           := Intel
   PREFIX         := $(MINGW)/bin/
   export PATH    := $PREFIX:$(PATH)
#   CPP       = "$(PREFIX)c++"
   CORE_FLAGS     := -mno-ms-bitfields   
   LDFLAGS        += -lgdi32
   RESULTS        = $(EXE)
   RUN            = ./$(EXE)
   LIBRARIES      += win
   SEARCH         += $(BMPTK)/targets/win
   LINKERSCRIPT   =
endif

define Embedded 
   EMBEDDED        = 1
   HOSTED          = 0
   HAS_FILESYSTEM  =  0
   CORE_FLAGS      += -nostartfiles 
   PREFIX          := $(GNU-ARM)/bin/arm-none-eabi-
   LINKERSCRIPT    = linkerscript.ld
endef

define Cortex
   $(eval $(Embedded))
   INCLUDES       += -I$(BMPTK)/targets/cortex/cmsis
   SOURCES        += bmptk_cortex.c
   RESULTS        += $(ELF) $(BIN) $(HEX) 
   RUN            = $(LPC21ISP) -control -verify -term $(HEX) $(SERIAL_PORT) $(SERIAL_BAUDRATE) 12000
   PORT_CHECK     = $(CHECK_PORT) $(SERIAL_PORT)
   LDFLAGS	      += -T linkerscript.ld -nostartfiles 
   DEFINES        += -DBMPTK_EMBEDDED -DBMPTK_EMBEDDED_HEAP -DBMPTK_EMBEDDED_IOSTREAM   
   RESULTS        += linkerscript.ld
   OBJ            += bmptk_cortex.o
endef

define Cortex-M0
   $(eval $(Cortex))
   CORE_FLAGS     += -mcpu=cortex-m0 -mthumb -fomit-frame-pointer -march=armv6-m
   SEARCH         += $(BMPTK)/targets/cortex
   INCLUDES       += -I$(BMPTK)/targets/cortex
   LD_CONFIG      = -DROM_START=0 -DRAM_START=0x10000000
endef

define LPC81X
   $(eval $(Cortex-M0))
   INCLUDES       += -I$(BMPTK)/targets/cortex/cmsis/lpc800/inc
endef

ifeq ($(CHIP),lpc810m021fn8)
   $(eval $(LPC81X))
   FNAME          = LPC810M021FN8
   STACK_SIZE     ?= 256
   LD_CONFIG      += -DROM_SIZE=8k -DRAM_SIZE=1k 
endif

ifeq ($(CHIP),lpc812m101)
   $(eval $(LPC81X))
   FNAME          = LPC810M021FN8
   STACK_SIZE     ?= 1k
   LD_CONFIG      += -DROM_SIZE=16k -DRAM_SIZE=4k 
endif

define LPC1114
   $(eval $(Cortex-M0))
   INCLUDES       += -I$(BMPTK)/targets/cortex/cmsis/11xx/inc 
endef

ifeq ($(CHIP),lpc1114fn28)
   $(eval $(LPC1114))
   FNAME          = LPC1114/102
   STACK_SIZE     ?= 2k
   LD_CONFIG      += -DROM_SIZE=32k -DRAM_SIZE=4k
   HWCPP_HEADERS  += lpc1114.hpp lpc1114fn28.hpp
endif

ifneq ($(NOPROJECT),1)
   ifeq ($(PREFIX),)
      $(error invalid or no target specified: "$(TARGET)")
   endif
endif   

   
#============================================================================
#
# hwcpp
# 
#============================================================================

ifeq ($(LANGUAGE),C++)
   include $(BMPTK)/hwcpp/Makefile.inc
   SEARCH    += $(foreach x, $(HWCPP_DIRECTORIES), $(BMPTK)/$(x))
   HEADERS   += $(HWCPP_HEADERS)
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


#============================================================================
#
# Dependencies
# 
#============================================================================

HEADERS   += $(patsubst %,%.h,$(LIBRARIES))

# Assume all sources depend on all headers, the linkerscript, and the Makefile
DEPEND    := $(HEADERS) $(LNK) Makefile
$(OBJ): $(DEPEND)
             
             
#============================================================================
#
# tool options
# 
#============================================================================

# show all warnings, treat most as errors
CORE_FLAGS  += -Wall -Werror
CORE_FLAGS  += -Wno-unused-local-typedefs
CORE_FLAGS  += -Wno-maybe-uninitialized

# use sections to save space
CORE_FLAGS  += -fdata-sections -ffunction-sections

# optimize for space
CORE_FLAGS  += -Os

# cpp: don't use features that are too expensive
CORE_CPP_FLAGS += -fno-rtti -fno-exceptions 
CORE_CPP_FLAGS += -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr

# cpp: use latest language standard
CORE_CPP_FLAGS    += -std=c++11

# assembler: enable the preprocessor
CORE_AS_FLAGS     += -x assembler-with-cpp

# linker specific
LDFLAGS     +=  -Wl,-Map,$(MAP)      
LDFLAGS     += -Wl,--gc-sections  

COMMON      := $(CORE_FLAGS) $(INCLUDES) $(DEFINES)
CPPFLAGS    += $(COMMON) $(CORE_CPP_FLAGS)
CFLAGS      += $(COMMON) $(CORE_CC_FLAGS) 
ASFLAGS     += $(COMMON) $(CORE_AS_FLAGS)
LDFLAGS     += $(COMMON) $(CORE_LD_FLAGS)


#============================================================================
#
# what to make
# 
#============================================================================

RESULTS += $(DMP) $(LSS) $(NMDUMP) $(MAP)
# RESULTS += $(LST)


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
vpath %.c    $(SEARCH)
vpath %.asm  $(SEARCH)
vpath %.cpp  $(SEARCH)

# use only the explcit rules below
.SUFFIXES:

# How to make assembler listings
%.lst: %.c
	@echo 'Compiling C file to asm listing: $<'
	$(CC) $(CFLAGS) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo ' ' 
%.lst: %.cpp
	@echo 'Compiling CPP file to asm listing: $<'
	$(CPP) $(CPPFLAGS) -c -S -o $(patsubst %.o,%.lst,$@) $<
	@echo '    

# How to make object files?
%.o: %.cpp
	@echo 'Compiling C++ file: $<'
	$(CPP) $(CPPFLAGS) -c -o $@ $< 
	@echo ' '	
	
%.o: %.c
	@echo 'Compiling C file: $<'
	$(CC) $(CFLAGS) -c -o $@ $< 
	@echo ' '	
	
%.o: %.S
	@echo 'Assembling file: $<'
	$(AS)  $(ASFLAGS)  -c -o $@ $<
	@echo ' '	

%.o: %.asm
	@echo 'Assembling file: $<'
	$(AS)  $(ASFLAGS)  -c -o $@ $<
	@echo ' '	

# make the linkerscipt  
LD_CONFIG += -DSTACK_SIZE=$(STACK_SIZE)
linkerscript.ld: $(BMPTK)/targets/cortex/linkerscript.c
	@echo 'Creating the linkerscript'
	$(CC) $(LD_CONFIG) -E -P -C $(BMPTK)/targets/cortex/linkerscript.c -o $@ 
	@echo ' '
   
# make .elf by linking all objects
# $(error $(addprefix ./,$(OBJ) ) )
$(ELF): $(addprefix ./,$(OBJ)) $(LIBA) $(LNK) ./$(LINKERSCRIPT)
	@echo 'Linking target: $(ELF)'
	$(LINKER) -Wl,-fatal-warnings -o $@ $(OBJ) $(LIBA) $(LDFLAGS) $(LIBS)
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
	$(RUN)
	$(TERMINAL)
    
doxygen: doxyfile
	Doxygen
   
# Cleanup
CLEAN += $(RESULTS)
CLEAN += $(OBJ)
CLEAN += $(MAP)
CLEAN += $(LST)
CLEAN += doxydocs
clean:
	$(REMOVE) $(CLEAN)

