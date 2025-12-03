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

# Binary asset matching (cutscenes, dialogues, texts)

# List of matching cutscene bytecode segments
DECOMPILED_CUTSCENES := fireworksFestival

CUTSCENE_SRC_DIR := $(SRC_DIRS)/bytecode/cutscenes
CUTSCENE_BUILD_DIR := $(BUILD_DIR)/bin/cutscenes

CUTSCENE_TRANSPILER := python3 tools/hm64_cutscene_transpiler.py

CUTSCENE_DSL := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_SRC_DIR)/$(bank).cutscene)
CUTSCENE_ASM := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_SRC_DIR)/$(bank).s)
CUTSCENE_OBJ := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_BUILD_DIR)/$(bank).s.o)

DECOMPILED_DIALOGUES := mariaDialogueBytecode

DIALOGUE_SRC_DIR := $(SRC_DIRS)/bytecode/dialogues
DIALOGUE_BUILD_DIR := $(BUILD_DIR)/bin/dialogues/bytecode

DIALOGUE_TRANSPILER := python3 tools/hm64_dialogue_transpiler.py

DIALOGUE_DSL := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_SRC_DIR)/$(bank).dialogue)
DIALOGUE_ASM := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_SRC_DIR)/$(bank).s $(DIALOGUE_SRC_DIR)/$(bank)Index.s)
DIALOGUE_OBJ := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_BUILD_DIR)/$(bank).s.o $(DIALOGUE_BUILD_DIR)/$(bank)Index.s.o)

DECOMPILED_TEXTS := text1 library diary animalInteractions namingScreen kai gotz sasha cliff funeralIntro howToPlay

TEXT_ASSETS_DIR := assets/text
TEXT_BUILD_DIR := $(BUILD_DIR)/bin/text

TEXT_TRANSPILER := python3 tools/hm64_text_transpiler.py

TEXT_ASM := $(foreach bank,$(DECOMPILED_TEXTS),$(TEXT_ASSETS_DIR)/$(bank)Text.s $(TEXT_ASSETS_DIR)/$(bank)TextIndex.s)
TEXT_OBJ := $(foreach bank,$(DECOMPILED_TEXTS),$(TEXT_BUILD_DIR)/$(bank)Text.s.o $(TEXT_BUILD_DIR)/$(bank)TextIndex.s.o)

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
	@rm -f $(CUTSCENE_ASM)
	@rm -f ${DIALOGUE_ASM}
	@rm -f ${TEXT_ASM}

clean-artifacts:
	@rm -rf build
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)
	@rm -f $(CUTSCENE_ASM)
	@rm -f ${DIALOGUE_ASM}
	@rm -f ${TEXT_ASM}

submodules:
	git submodule update

split: $(LD_SCRIPT)

setup: clean split

rebuild: clean-artifacts $(LD_SCRIPT) check

rerun: clean $(LD_SCRIPT) check

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

extract-map-sprites:
	@cd tools && python3 ./hm64_map_utilities.py write_all_textures

extract-cutscenes:
	@cd tools && python3 hm64_cutscene_utilities.py --all --labels --scan --verbose-scan
	@cd tools && python3 hm64_cutscene_utilities.py --all --labels --graph
	@cd tools && python3 hm64_cutscene_utilities.py --all --labels --asm

build:
	@mkdir $@

# Generate linker script via splat after assembling bytecode DSL and texts
# Then patch linker script to use decompiled segments instead of extracted binaries
# 
# For cutscenes: cutsceneBankN.bin.o -> cutsceneBankN.s.o
# For dialogue bytecode: <bank>.bin.o -> <bank>.s.o, <bank>Index.bin.o -> <bank>Index.s.o  
# For dialogue text: <bank>.bin.o -> <bank>.s.o, <bank>Index.bin.o -> <bank>Index.s.o
$(LD_SCRIPT): $(CUTSCENE_ASM) $(DIALOGUE_ASM) $(TEXT_ASM)
	$(V)python3 -m splat split ./tools/splat.yaml
	$(V)$(foreach bank,$(DECOMPILED_CUTSCENES),\
		sed -i 's|$(bank).bin.o|$(bank).s.o|g' $(LD_SCRIPT);)
	$(V)$(foreach bank,$(DECOMPILED_DIALOGUES),\
		sed -i 's|$(bank).bin.o|$(bank).s.o|g' $(LD_SCRIPT);\
		sed -i 's|$(bank)Index.bin.o|$(bank)Index.s.o|g' $(LD_SCRIPT);)
	$(V)$(foreach bank,$(DECOMPILED_TEXTS),\
    	sed -i 's|$(bank)Text.bin.o|$(bank)Text.s.o|g' $(LD_SCRIPT);\
    	sed -i 's|$(bank)TextIndex.bin.o|$(bank)TextIndex.s.o|g' $(LD_SCRIPT);)

# Main code segment

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

# Lib

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

$(BUILD_DIR)/asm/lib/os/libultra/gu/%.s.o : src/lib/os/libultra/gu/%.s build
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Asset building

# transpile cutscene DSL to assembly
$(CUTSCENE_SRC_DIR)/%.s: $(CUTSCENE_SRC_DIR)/%.cutscene
	$(dir_guard)
	$(V)$(CUTSCENE_TRANSPILER) $< -o $@

$(CUTSCENE_BUILD_DIR)/%.s.o: $(CUTSCENE_SRC_DIR)/%.s
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# transpile dialogue DSL to assembly (generates two files: bytecode and index)
$(DIALOGUE_SRC_DIR)/%.s $(DIALOGUE_SRC_DIR)/%Index.s: $(DIALOGUE_SRC_DIR)/%.dialogue
	$(V)$(DIALOGUE_TRANSPILER) transpile $< -n $* -o $(DIALOGUE_SRC_DIR)/

$(DIALOGUE_BUILD_DIR)/%.s.o: $(DIALOGUE_SRC_DIR)/%.s
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Extract texts to assets directory and transpile to assembly (generates two files: bytecode and index)
$(TEXT_ASSETS_DIR)/%Text.s $(TEXT_ASSETS_DIR)/%TextIndex.s: $(BASEROM)
	$(dir_guard)
	$(V)cd $(TOOLS_DIR) && python3 hm64_text_utilities.py extract_bank $* --literal
	$(V)$(TEXT_TRANSPILER) transpile $(TEXT_ASSETS_DIR)/$* -n $*Text -o $(TEXT_ASSETS_DIR)/ --literal

$(TEXT_BUILD_DIR)/%Text.s.o: $(TEXT_ASSETS_DIR)/%Text.s
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(TEXT_BUILD_DIR)/%TextIndex.s.o: $(TEXT_ASSETS_DIR)/%TextIndex.s
	$(dir_guard)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Rest of code and extracted binary files

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