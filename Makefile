BASENAME := hm64
TARGET := $(BASENAME).z64
BASEROM = baserom.us.z64

VERBOSE := 0

PERMUTER ?= 0

SRC_DIRS := src
ASM_DIRS := asm asm/lib asm/data asm/nonmatchings $(wildcard asm/nonmatchings/**) $(wildcard asm/lib/**)
BIN_DIRS := bin

BUILD_DIR := build
TOOLS_DIR := tools

S_FILES = $(foreach dir, $(ASM_DIRS), $(wildcard $(dir)/*.s))
C_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
BIN_FILES=$(foreach dir, $(BIN_DIRS), $(wildcard $(dir)/*.bin))

LD_SCRIPT := $(BASENAME).ld
	
OBJECTS := $(shell grep -E 'build.+\.o' $(LD_SCRIPT) -o)

KMC_PATH := tools/gcc-2.7.2/

CROSS := mips-linux-gnu-

CC = $(KMC_PATH)gcc
AS := $(CROSS)as
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
STRIP := $(CROSS)strip

MACROS := -D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -D_FINALROM -DSUPPORT_NAUDIO -DNU_SYSTEM -DF3DEX_GBI_2
CFLAGS_COMMON := -G0 -mips3 -mgp32 -mfp32 -Wa,-Iinclude

CFLAGS := $(CFLAGS_COMMON) $(MACROS)
ASFLAGS := -G 0 -I include -mips3 -mabi=32
CPPFLAGS := -I. -I include -I src -I src/system -I include/libmus

DEBUG_FLAGS := -g2
OPTFLAGS := -O2
LIBULTRA_OPTFLAGS := -O1
NU_OPTFLAGS := -O2

LD_MAP := $(BASENAME).map

LDFLAGS := -T symbol_addrs.txt -T undefined_funcs.txt -T undefined_syms.txt -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

ifeq ($(VERBOSE),0)
V := @
endif

ifeq ($(PERMUTER),1)
MACROS += -D_PERMUTER=1
endif

dir_guard = @mkdir -p $(@D)

build/src/lib/nusys-1/nuboot.c.o: NU_OPTFLAGS := -O0

build/src/lib/os/libultra/io/contreaddata.c.o: LIBULTRA_OPTFLAGS := -O2
build/src/lib/os/libultra/destroythread.c.o: LIBULTRA_OPTFLAGS := -O3

all: check

clean:
	@rm -rf build
	@rm -rf asm
	@rm -rf bin
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)
	
clean-artifacts:
	@rm -rf build
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)

submodules:
	git submodule update

split:
	$(V)python3 tools/splat/split.py ./tools/splat.yaml

setup: clean split

rebuild: clean-artifacts check

rerun: clean split check

build:
	@mkdir $@

# $(LD_SCRIPT): split

# need to pass -B <dir> to gcc to prevent it from fetching system default cc1
$(BUILD_DIR)/src/%.c.o: src/%.c
	$(dir_guard) 
	$(V)export COMPILER_PATH=$(KMC_PATH) && $(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/lib/nusys-1/%.c.o : src/lib/nusys-1/%.c build
	$(dir_guard)
	$(V)export COMPILER_PATH=$(KMC_PATH) && $(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< 
	
$(BUILD_DIR)/src/lib/os/libultra/%.c.o : src/lib/os/libultra/%.c build
	$(dir_guard)
	$(V)export COMPILER_PATH=$(KMC_PATH) && $(CC) -B $(KMC_PATH) $(LIBULTRA_OPTFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< 

$(BUILD_DIR)/%.s.o : %.s build
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin build
	$(dir_guard)
	$(V)$(LD) -r -b binary -o $@ $<

$(BASENAME).elf: $(OBJECTS)
	$(V)$(LD) $(LDFLAGS) -o $@

$(TARGET): $(BASENAME).elf
	$(V)$(OBJCOPY) -O binary $< $@

check: $(TARGET)
	$(V)diff $(TARGET) $(BASEROM) && echo "OK"


.PHONY: all clean setup split submodules

MAKEFLAGS += --no-builtin-rules