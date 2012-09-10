#############################################################################
#
# file: bmptk/Makefile.inc
#
# Makefile for bare metal C++ projects
#
# (c) Wouter van Ooijen (www.voti.nl) 2012
# 
#############################################################################


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


#============================================================================
#
# the project source file is sure part of the project
# 
#============================================================================

SOURCES   += $(PROJECT).cpp


#============================================================================
#
# find the library's files
# 
#============================================================================

VPATH     += $(BMPTK)
VPATH     += $(BMPTK)/targets
VPATH     += $(BMPTK)/graphics

INCLUDES  += -I.
INCLUDES  += -I$(BMPTK)
INCLUDES  += -I$(BMPTK)/targets
INCLUDES  += -I$(BMPTK)/graphics


#============================================================================
#
# default port, baudrate, xtal for download and debug log
# 
#============================================================================

PORT      := COM4
BAUDRATE  := 115200
XTAL      := 12000


#============================================================================
#
# tool executables
# 
#============================================================================


CC        = "$(PREFIX)gcc"
CPP       = "$(PREFIX)g++"
LINKER    = "$(PREFIX)g++"
AS        = "$(PREFIX)gcc"
OBJCOPY   = "$(PREFIX)objcopy"
OBJDUMP   = "$(PREFIX)objdump"
SIZES     = "$(PREFIX)size"

NDSTOOL   := "$(DEVKITPRO)/devkitARM/bin/ndstool"
IMGTOCPP-TOOL = $(BMPTK)/tools/img_to_cpp.py
FFTTOCPP-TOOL = $(BMPTK)/tools/ttf_to_cpp.py
TXTTOCPP-TOOL = $(BMPTK)/tools/txt_to_cpp.py
IMGTOCPP  := $(PYTHON) $(IMGTOCPP-TOOL)
FFTTOCPP  := $(PYTHON) $(FFTTOCPP-TOOL)
TXTTOCPP  := $(PYTHON) $(TXTTOCPP-TOOL)


#============================================================================
#
# target-dependent parts
# 
#============================================================================

# will remain empty when no valid target is specified
PREFIX     := 

ifeq ($(TARGET),nds)
   CHIP        := ARMv7
   PREFIX      := $(DEVKITPRO)/devkitARM/bin/arm-none-eabi-
   CORE-FLAGS  := -march=armv5te -mtune=arm946e-s -fomit-frame-pointer 
   CORE-FLAGS  += -ffast-math -mthumb -mthumb-interwork -DARM9
   INCLUDES    += -I$(DEVKITPRO)/libnds/include 
   LIBRARIES   += -L$(DEVKITPRO)/libnds/lib -lnds9
   LDFLAGS	   += -g -mthumb -mthumb-interwork -specs=ds_arm9.specs
   RESULTS     = $(ELF) $(NDS)
   RUN         = "$(DEVKITPRO)/emulators/desmume/DeSmuME" $(NDS)
   LIBS        += bmptk_nds
endif

ifeq ($(TARGET),win)
   CHIP        := Intel
   PREFIX      := $(DEVCPP)/bin/
   CORE-FLAGS  :=    
   INCLUDES    += -I"$(DEVCPPLIB)/lib/gcc/mingw32/3.4.2/include"  
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2/backward" 
   INCLUDES    += -I"$(DEVCPP)/lib/gcc/mingw32"
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2"
   INCLUDES    += -I"$(DEVCPP)/include"
   LIBRARIES   += -L"$(DEVCPP)/lib" 
   LDFLAGS     += $(BMPTK)/targets/bmptk_win_libbgi.a 
   LDFLAGS     += -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
   RESULTS     = $(EXE)
   RUN         = ./$(EXE)
   LIBS        += bmptk_win
endif

ifeq ($(TARGET),olimex-mod-lcd43)
   XTAL        := 12000000
   CHIP        := lpc2478
endif

ifeq ($(CHIP),lpc2478)
   XTAL        := 12000000
   PREFIX      := $(ARMTOOLS)/bin/arm-none-eabi-
   # PREFIX      := $(ARMDEVENV)/bin/arm-thumb-elf-
   CORE-FLAGS  := -mcpu=arm7tdmi -mtune=arm7tdmi -nostartfiles -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-get-exception-ptr
   LDFLAGS	   += -T$(BMPTK)/targets/$(CHIP).ld -nostartfiles
   SOURCES     += $(CHIP).cpp $(CHIP)-startup.s
   HEADERS     += $(CHIP).h
   RESULTS     = $(ELF) $(BIN) $(MAP) $(HEX) $(DMP) $(LSS)
endif

ifeq ($(PREFIX),)
   $(error invalid or no target specified: "$(TARGET)")
endif
   
DEFINES   += -DTARGET=$(TARGET)
DEFINES   += -DTARGET_$(TARGET)
DEFINES   += -DCHIP=$(CHIP)
DEFINES   += -DCHIP_$(CHIP)
DEFINES   += -DXTAL=$(XTAL)


#============================================================================
#
# the default font, required by bmptk_graphics 
#
# The 04B_03__.ttf file (listed as freeware) was downloaded from
# http://www.eaglefonts.com/04b03-ttf-131597.htm
# 
#============================================================================

FONT-DEFAULT  := $(BMPTK)/graphics/04B_03__.TTF

ifeq (bmptk_graphics,$(filter bmptk_graphics,$(LIBS)))
   LIBS          += bmptk_font_default
   CLEAN         += bmptk_font_default.cpp bmptk_font_default.h
   DEFINES       += -DBMPTK_GRAPHICS
endif

bmptk_font_default.cpp bmptk_font_default.h: $(FONT-DEFAULT) $(FFTTOCPP-TOOL)
		$(FFTTOCPP) $(FONT-DEFAULT) bmptk_font_default font_default 8 "" bmptk

      
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
OBJ       := $(OBJECTS)
OBJ       += $(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(SOURCES)))
OBJ       += $(patsubst %,%.o,$(LIBS))
OBJ       += $(patsubst %.jpg,%.o,$(IMAGES))


#============================================================================
#
# Dependencies
# 
#============================================================================

HEADERS   += $(patsubst %,%.h,$(LIBS))

# Assume all sources depend on all headers, the linkerscript, and the Makefile
DEPEND    := $(HEADERS) $(LNK) Makefile
$(OBJ): $(DEPEND)
             
             
#============================================================================
#
# tool options
# 
#============================================================================

CPPFLAGS    += $(CORE-FLAGS) $(INCLUDES) $(DEFINES) -I$(BMPTK) 
CPPFLAGS    += -g -O2 -pipe -Wall  -Wno-multichar 
CPPFLAGS    += -fno-builtin -fno-rtti -fno-exceptions 
# not for devcpp
# CPPFLAGS    += -mstructure-size-boundary=32 -std=c++0x 

ASFLAGS     := $(CORE-FLAGS) $(INCLUDES) $(DEFINES) -I$(BMPTK) 
ASFLAGS     += -x assembler-with-cpp

# default LDFLAGS
LDFLAGS     +=  -Wl,-Map,$(MAP)              


#============================================================================
#
# how to make things
# 
#============================================================================

# use only the explcit rules below
.SUFFIXES:

# How to make object files?
%.o: %.cpp; $(CPP) $(CPPFLAGS) -c -o $@ $<
%.o: %.s; $(AS)  $(ASFLAGS)  -c -o $@ $<

# How to make a listing from a cpp file
%.list: %.cpp; $(CPP) $(CPPFLAGS) -c -S -o $@ $<

# make .elf by linking all objects
$(ELF): $(OBJ) $(LNK)
		$(LINKER) -o $@ $(OBJ) $(LDFLAGS) $(LIBRARIES)

# make .exe by linking all objects
$(EXE): $(OBJ) $(LNK)
		$(LINKER) -o $@ $(OBJ) $(LDFLAGS) $(LIBRARIES) 

# make .bin from .elf
$(BIN): $(ELF)
		$(OBJCOPY) -O binary $< $@

# make .hex from .elf
$(HEX):	$(ELF)
		$(OBJCOPY) --output-target ihex $< $@

# make .dmp from elf
$(DMP):	$(ELF)
		$(OBJDUMP) -x --syms $< > $@

# make .lss from elf
$(LSS):	$(ELF)
		$(OBJDUMP) -S $< > $@

# make .nds from .elf
$(NDS): $(ELF)
		$(NDSTOOL) -c $@ -9 $< -b $(DEVKITPRO)/libnds/icon.bmp "$(PROJECT)"		

# show object sizes
.PHONY: psizes
sizes: $(OBJ)
		$(SIZES) -B -t $(OBJ)


#============================================================================
#
# targets
# 
#============================================================================

.PHONY: $(PROJECT) all build new fresh run clean

# Build the project
$(PROJECT): $(RESULTS) sizes

# aliases for build the project
all: $(PROJECT)
build: $(PROJECT)

# aliases for clean+build
new: clean build
fresh: clean build

# Run (= download and start) the project
run: $(PROJECT)
		$(RUN)
   
# Cleanup
CLEAN += $(RESULTS)
CLEAN += $(OBJ)
CLEAN += $(MAP)
clean:
		-$(RM) $(CLEAN)

