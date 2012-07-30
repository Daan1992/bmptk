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

PF        := C:/Program Files
ARMTOOLS  := $(PF)/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI
ARMDEVENV := $(PF)/ARMDevEnv
DEVKITPRO := C:/devkitPro
DEVCPP    := C:/Dev-Cpp

CC        = "$(PREFIX)gcc"
CPP       = "$(PREFIX)g++"
LINKER    = "$(PREFIX)g++"
AS        = "$(PREFIX)gcc"
OBJCOPY   = "$(PREFIX)objcopy"
OBJDUMP   = "$(PREFIX)objdump"
SIZES     = "$(PREFIX)size"

NDSTOOL   := "$(DEVKITPRO)/devkitARM/bin/ndstool"


#============================================================================
#
# target-dependent parts
# 
#============================================================================

# will remain empty when no valid target is specified
PREFIX     := 

ifeq ($(TARGET),nds)
   CHIP        := ARMv7
   PREFIX      := $(DEVKITPRO)/devkitARM/bin/arm-eabi-
   CORE-FLAGS  := -march=armv5te -mtune=arm946e-s -fomit-frame-pointer 
   CORE-FLAGS  += -ffast-math -mthumb -mthumb-interwork -DARM9
   INCLUDES    += -I$(DEVKITPRO)/libnds/include 
   LIBRARIES   += -L$(DEVKITPRO)/libnds/lib -lnds9
   LDFLAGS	   += -g -mthumb -mthumb-interwork -specs=ds_arm9.specs
   RESULTS     = $(ELF) $(NDS)
   RUN         = "$(DEVKITPRO)/emulators/desmume/DeSmuME" $(NDS)
   LIBS        += bmptk-nds
endif

ifeq ($(TARGET),console)
   CHIP        := Intel
   PREFIX      := $(DEVCPP)/bin/
   CORE-FLAGS  :=    
   INCLUDES    += -I"$(DEVCPPLIB)/lib/gcc/mingw32/3.4.2/include"  
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2/backward" 
   INCLUDES    += -I"$(DEVCPP)/lib/gcc/mingw32"
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2"
   INCLUDES    += -I"$(DEVCPP)/include"
   LIBRARIES   += -L"$(DEVCPP)/lib" 
   RESULTS     = $(EXE)
   RUN         = ./$(EXE)
endif

ifeq ($(TARGET),olimex-mod-lcd43)
   DEFINES     += -DTARGET_OLIMEX_MOD_43
   XTAL        += 
   CHIP        := lpc2478
   PREFIX      := $(DEVCPP)/bin/
   CORE-FLAGS  :=    
   INCLUDES    += -I"$(DEVCPPLIB)/lib/gcc/mingw32/3.4.2/include"  
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2/backward" 
   INCLUDES    += -I"$(DEVCPP)/lib/gcc/mingw32"
   INCLUDES    += -I"$(DEVCPP)/include/c++/3.4.2"
   INCLUDES    += -I"$(DEVCPP)/include"
   LIBRARIES   += -L"$(DEVCPP)/lib" 
   RESULTS     = $(EXE)
   RUN         = ./$(EXE)
endif

ifeq ($(CHIP),lpc2478)
   DEFINES     += -DCHIP_LPC2478
   XTAL        := 12000000
   DEFINES     += -DCLCK=$(XTAL)
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


#============================================================================
#
# Dependencies
# 
#============================================================================

HEADERS   += $(patsubst %,%.h,$(LIBS))

# Assume all sources depend on all headers, the linkerscript, and the Makefile
DEPEND    := $(HEADERS) $(LNK) Makefile
%.cpp::   $(DEPEND)
%.s::     $(DEPEND)


#============================================================================
#
# Result files
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
OBJ       := $(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(SOURCES)))
OBJ       += $(patsubst %,%.o,$(LIBS))
             
             
#============================================================================
#
# tool options
# 
#============================================================================

CPPFLAGS    := $(CORE-FLAGS) $(INCLUDES) $(DEFINES) -I$(BMPTK) 
CPPFLAGS    += -g -O2 -pipe -Wall -mstructure-size-boundary=32 -Wno-multichar 
CPPFLAGS    += -fno-builtin -fno-rtti -fno-exceptions -std=c++0x 

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
%.o: %.s;   $(AS)  $(ASFLAGS)  -c -o $@ $<

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

