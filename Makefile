BASENAME := hm64
TARGET := $(BASENAME).z64
BASEROM = baserom.us.z64

VERBOSE := 0

PERMUTER ?= 0

SRC_DIRS := src
BIN_DIRS := bin

BUILD_DIR := build
TOOLS_DIR := tools

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

MACROS := -D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -DSUPPORT_NAUDIO -DNU_SYSTEM -DF3DEX_GBI_2 -DN_MICRO -DLANG_JAPANESE=0 -DUSE_EPI -DNDEBUG -D_AUDIOVISIBLE -DN_AUDIO
CFLAGS_COMMON := -G0 -mips3 -mgp32 -mfp32 -Wa,-Iinclude -funsigned-char

CFLAGS := $(CFLAGS_COMMON) $(MACROS)
ASFLAGS := -G 0 -I include -mips3 -mabi=32
CPPFLAGS := -I. -I include -I src -I include/libmus -I include/PR -I include/gcc -I include/nusys

DEBUG_FLAGS := -g2
OPTFLAGS := -O2
LIBULTRA_OPTFLAGS := -O3 -g0
LIBKMC_OPTFLAGS := -O1
LIBMUS_OPTFLAGS := -O0 -g0
NU_OPTFLAGS := -O3

ULTRALIBVER := -DBUILD_VERSION=7

LD_MAP := $(BASENAME).map

LDFLAGS := -T undefined_funcs.txt -T undefined_syms.txt -T undefined_funcs_auto.txt -T undefined_syms_auto.txt -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

ifeq ($(VERBOSE),0)
V := @
endif

ifeq ($(PERMUTER),1)
MACROS += -D_PERMUTER=1
endif

dir_guard = @mkdir -p $(@D)

build/src/lib/nusys-1/nuboot.c.o: NU_OPTFLAGS := -O0
build/src/lib/nusys-1/nupakmenu.c.o: NU_OPTFLAGS += -g2
build/src/lib/nusys-1/nupakmenuloadfont.c.o: NU_OPTFLAGS += -g2
build/src/lib/os/libultra/io/aisetnextbuf.c.o: ULTRALIBVER := -DBUILD_VERSION=6

all: check

clean:
	@rm -rf build
	@rm -rf asm
	@rm -rf bin
	@rm -rf assets
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

extract-sprites:
	@rm -rf assets/sprites
	@cd tools && python3 ./hm64_sprite_utilities.py write_all_textures    

extract-animation-metadata:
	@cd tools && python3 ./hm64_animation_utilities.py extract_animation_metadata

extract-animation-scripts:
	@cd tools && python3 ./extract_animation_scripts.py

extract-animation-sprites:
	@cd tools && python3 ./hm64_animation_utilities.py fetch_sprites_for_animations
	
extract-animations:
	@cd tools && python3 ./hm64_animation_utilities.py extract_all

gifs:
	@cd tools && python3 ./hm64_animation_utilities.py make_gifs_from_animations

extract-texts:
	@cd tools && python3 ./hm64_text_utilities.py process_all write_files

extract-dialogues:
	@cd tools && python3 ./hm64_dialogue_utilities.py decode_all


build:
	@mkdir $@

# need to pass -B <dir> to gcc to prevent it from fetching system default cc1
$(BUILD_DIR)/src/%.c.o: src/%.c
	$(dir_guard) 
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/game/%.c.o: src/game/%.c
	$(dir_guard) 
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/system/%.c.o: src/system/%.c
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/data/animation/%.c.o: src/data/animation/%.c
	$(dir_guard) 
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<
	
$(BUILD_DIR)/src/overlays/%.c.o : src/overlays/%.c build
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 
	
$(BUILD_DIR)/src/lib/libmus/player.c.o : src/lib/libmus/player.c build
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(LIBMUS_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 

$(BUILD_DIR)/src/lib/nusys-1/%.c.o : src/lib/nusys-1/%.c build
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 
	
$(BUILD_DIR)/src/lib/os/libultra/%.c.o : src/lib/os/libultra/%.c build
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(LIBULTRA_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 

$(BUILD_DIR)/src/lib/libkmc/%.c.o : src/lib/libkmc/%.c build
	$(dir_guard)
	$(CC) -B $(KMC_PATH) $(LIBKMC_OPTFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< 

# for hasm OS functions
$(BUILD_DIR)/asm/lib/os/libultra/gu/%.s.o : src/lib/os/libultra/gu/%.s build
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

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