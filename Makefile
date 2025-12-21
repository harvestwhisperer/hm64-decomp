BASENAME := hm64
TARGET := $(BASENAME).z64

# default to US
REGION ?= us
BASEROM := baserom.$(REGION).z64

# Options

VERBOSE := 0
PERMUTER ?= 0

# Directories

SRC_DIRS := src
BIN_DIRS := bin

BUILD_DIR := build
TOOLS_DIR := tools

# Files

C_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
BIN_FILES=$(foreach dir, $(BIN_DIRS), $(wildcard $(dir)/*.bin))

ifeq ($(REGION), jp)
LD_SCRIPT := $(BASENAME)-$(REGION).ld
else
LD_SCRIPT := $(BASENAME).ld
endif

LD_MAP := $(BASENAME).map
	
OBJECTS := $(shell grep -E 'build.+\.o' $(LD_SCRIPT) -o)

# Tools

KMC_PATH := $(TOOLS_DIR)/gcc-2.7.2/

CROSS := mips-linux-gnu-

CC := $(KMC_PATH)gcc
AS := $(CROSS)as
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy

# Flags

MACROS := -D_LANGUAGE_C -D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -DSUPPORT_NAUDIO -DNU_SYSTEM -DF3DEX_GBI_2 -DN_MICRO -DLANG_JAPANESE=0 -DUSE_EPI -DNDEBUG -D_AUDIOVISIBLE -DN_AUDIO
CFLAGS_COMMON := -G0 -mips3 -mgp32 -mfp32 -Wa,-Iinclude -funsigned-char

CFLAGS := $(CFLAGS_COMMON) $(MACROS)
HASM_ASFLAGS := -mips3 -nostdinc -fno-PIC -mno-abicalls -G 0 -mgp32 -mfp32 -mabi32
ASFLAGS := -G 0 -I include -mips3
CPPFLAGS := -I. -I include -I src -I lib/nusys-1/include -I lib/libultra/include -I lib/libmus/include/PR -I lib/gcc/include

HASM_AS_DEFINES := -D_LANGUAGE_ASSEMBLY -DMIPSEB -D_ULTRA64 -D_MIPS_SIM=1

LIBULTRA_CPP_FLAGS := -I lib/libultra/include -I lib/libultra/include/PR -I lib/gcc/include
LIBMUS_CPP_FLAGS := -I lib/libmus/include/PR -I lib/nusys-1/include -I lib/libultra/include -I lib/libultra/include/PR -I lib/libultra/src/libnaudio
NUSYS_CPP_FLAGS := -I lib/nusys-1/include -I lib/libultra/include
NUALSTL_CPP_FLAGS := -I lib/nusys-1/include -I lib/libultra/include -I lib/libultra/include/PR -I lib/libultra/src/libnaudio -I lib/libmus/include/PR
LIBKMC_CPP_FLAGS := -I include -I lib/libkmc/include -I lib/gcc/include -I lib/nusys-1/include -I lib/libultra/include -I lib/libmus/include/PR

DEBUG_FLAGS := -g2
OPTFLAGS := -O2
LIBULTRA_OPTFLAGS := -O3 -g0
LIBKMC_OPTFLAGS := -O1
LIBMUS_OPTFLAGS := -O0 -g0
NU_OPTFLAGS := -O3

ULTRALIBVER := -DBUILD_VERSION=7

LDFLAGS := -T config/$(REGION)/undefined_funcs.txt -T config/$(REGION)/undefined_syms.txt -T config/$(REGION)/undefined_funcs_auto.txt -T config/$(REGION)/undefined_syms_auto.txt -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

# Binary asset matching (cutscenes, dialogues, texts)

# List of matching cutscene bytecode segments

ifeq ($(REGION),us)
DECOMPILED_CUTSCENES := fireworksFestival
else
DECOMPILED_CUTSCENES := 
endif

CUTSCENE_SRC_DIR := $(SRC_DIRS)/bytecode/cutscenes
CUTSCENE_BUILD_DIR := $(BUILD_DIR)/bin/cutscenes

CUTSCENE_TRANSPILER := python3 $(TOOLS_DIR)/hm64_cutscene_transpiler.py

CUTSCENE_DSL := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_SRC_DIR)/$(bank).cutscene)
CUTSCENE_ASM := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_SRC_DIR)/$(bank).s)
CUTSCENE_OBJ := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_BUILD_DIR)/$(bank).bin.o)

DECOMPILED_DIALOGUES := mariaDialogueBytecode

ifeq ($(REGION),us)
DECOMPILED_DIALOGUES := mariaDialogueBytecode
else
DECOMPILED_DIALOGUES :=
endif

DIALOGUE_SRC_DIR := $(SRC_DIRS)/bytecode/dialogues
DIALOGUE_BUILD_DIR := $(BUILD_DIR)/bin/dialogues/bytecode

DIALOGUE_TRANSPILER := python3 $(TOOLS_DIR)/hm64_dialogue_transpiler.py

DIALOGUE_DSL := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_SRC_DIR)/$(bank).dialogue)
DIALOGUE_ASM := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_SRC_DIR)/$(bank).s $(DIALOGUE_SRC_DIR)/$(bank)Index.s)
DIALOGUE_OBJ := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_BUILD_DIR)/$(bank).bin.o $(DIALOGUE_BUILD_DIR)/$(bank)Index.bin.o)

ifeq ($(REGION),us)
DECOMPILED_TEXTS := text1 library diary animalInteractions namingScreen kai gotz sasha cliff funeralIntro howToPlay
else
DECOMPILED_TEXTS :=
endif

TEXT_ASSETS_DIR := assets/text
TEXT_BUILD_DIR := $(BUILD_DIR)/bin/text

TEXT_TRANSPILER := python3 $(TOOLS_DIR)/hm64_text_transpiler.py

TEXT_ASM := $(foreach bank,$(DECOMPILED_TEXTS),$(TEXT_ASSETS_DIR)/$(bank)Text.s $(TEXT_ASSETS_DIR)/$(bank)TextIndex.s)
TEXT_OBJ := $(foreach bank,$(DECOMPILED_TEXTS),$(TEXT_BUILD_DIR)/$(bank)Text.bin.o $(TEXT_BUILD_DIR)/$(bank)TextIndex.bin.o)

ifeq ($(VERBOSE),0)
V := @
endif

ifeq ($(PERMUTER),1)
MACROS += -D_PERMUTER=1
endif

ifeq ($(REGION),jp)
MACROS += -D_JP=1
endif

# Create all output directories upfront
$(shell mkdir -p $(sort $(dir $(OBJECTS))))

# Flag overrides

build/lib/nusys-1/src/nusys/nuboot.c.o: NU_OPTFLAGS := -O0
build/lib/nusys-1/src/sample/nunospak/nupakmenu.c.o: NU_OPTFLAGS += -g2
build/lib/nusys-1/src/sample/nunospak/nupakmenuloadfont.c.o: NU_OPTFLAGS += -g2
build/lib/libultra/src/io/aisetnextbuf.c.o: ULTRALIBVER := -DBUILD_VERSION=6

# Targets

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

split: 
	$(V)python3 -m splat split ./config/$(REGION)/splat.$(REGION).yaml

setup: clean split

rebuild: clean-artifacts $(LD_SCRIPT) check

rerun: clean $(LD_SCRIPT) check

# Asset extraction

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

extract-gifs:
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

# Main code segment

# need to pass -B <dir> to gcc to prevent it from fetching system default cc1
$(BUILD_DIR)/src/mainproc.c.o: src/mainproc.c
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/mainLoop.c.o: src/mainLoop.c
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/game/%.c.o: src/game/%.c
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/system/%.c.o: src/system/%.c
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/buffers/%.c.o: src/buffers/%.c
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/data/%.c.o: src/data/%.c
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 

# Lib

$(BUILD_DIR)/lib/libmus/src/player.c.o: lib/libmus/src/player.c
	$(CC) -B $(KMC_PATH) $(LIBMUS_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBMUS_CPP_FLAGS) -c -o $@ $<

# nusys

$(BUILD_DIR)/lib/nusys-1/src/nusys/%.c.o: lib/nusys-1/src/nusys/%.c
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/nualstl/%.c.o: lib/nusys-1/src/nualstl/%.c
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUALSTL_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/sample/nunospak/%.c.o: lib/nusys-1/src/sample/nunospak/%.c
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

# libkmc

$(BUILD_DIR)/lib/libkmc/src/%.c.o: lib/libkmc/src/%.c 
	$(CC) -B $(KMC_PATH) $(LIBKMC_OPTFLAGS) $(CFLAGS) $(LIBKMC_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libkmc/src/%.s.o: lib/libkmc/src/%.s 
	$(CC) -B $(KMC_PATH) -c $(LIBKMC_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# libultra

$(BUILD_DIR)/lib/libultra/src/%.c.o: lib/libultra/src/%.c
	$(CC) -B $(KMC_PATH) $(LIBULTRA_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBULTRA_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libultra/src/gu/%.s.o: lib/libultra/src/gu/%.s
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/libc/%.s.o: lib/libultra/src/libc/%.s
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/os/%.s.o: lib/libultra/src/os/%.s
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -o32 -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/os/unusedPadding.s.o: lib/libultra/src/os/unusedPadding.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<
	
$(BUILD_DIR)/lib/libultra/src/rmon/%.s.o: lib/libultra/src/rmon/%.s
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

# Asset building

# transpile cutscene DSL to assembly
$(CUTSCENE_SRC_DIR)/%.s: $(CUTSCENE_SRC_DIR)/%.cutscene
	$(V)$(CUTSCENE_TRANSPILER) $< -o $@

$(CUTSCENE_BUILD_DIR)/%.bin.o: $(CUTSCENE_SRC_DIR)/%.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# transpile dialogue DSL to assembly (generates two files: bytecode and index)
# The transpiler produces both .s and Index.s
$(DIALOGUE_SRC_DIR)/%.s: $(DIALOGUE_SRC_DIR)/%.dialogue
	$(V)$(DIALOGUE_TRANSPILER) transpile $< -n $* -o $(DIALOGUE_SRC_DIR)/

# Mark dependency
$(DIALOGUE_SRC_DIR)/%Index.s: $(DIALOGUE_SRC_DIR)/%.s
	@:

$(DIALOGUE_BUILD_DIR)/%.bin.o: $(DIALOGUE_SRC_DIR)/%.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Extract texts to assets directory and transpile to assembly (generates two files: bytecode and index)
# The transpiler produces both Text.s and TextIndex.s
$(TEXT_ASSETS_DIR)/%Text.s:
	$(V)cd $(TOOLS_DIR) && python3 hm64_text_utilities.py extract_bank $* --literal
	$(V)$(TEXT_TRANSPILER) transpile $(TEXT_ASSETS_DIR)/$* -n $*Text -o $(TEXT_ASSETS_DIR)/ --literal

# Mark dependency
$(TEXT_ASSETS_DIR)/%TextIndex.s: $(TEXT_ASSETS_DIR)/%Text.s
	@:

$(TEXT_BUILD_DIR)/%Text.bin.o: $(TEXT_ASSETS_DIR)/%Text.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(TEXT_BUILD_DIR)/%TextIndex.bin.o: $(TEXT_ASSETS_DIR)/%TextIndex.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# $(TEXT_ASSETS_DIR)/%Text.s:
# 	$(V)$(TEXT_TRANSPILER) transpile $(TEXT_ASSETS_DIR)/$* -n $*Text -o $(TEXT_ASSETS_DIR)/ --literal

# Rest of code and extracted binary files

$(BUILD_DIR)/%.s.o: %.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin
	$(V)$(LD) -r -b binary -o $@ $<
	
# Final binary

$(BASENAME).elf: $(OBJECTS)
	$(V)$(LD) $(LDFLAGS) -o $@

$(TARGET): $(BASENAME).elf
	$(V)$(OBJCOPY) -O binary $< $@

check: $(TARGET)
	$(V)diff $(TARGET) $(BASEROM) && echo "OK"


.PHONY: all clean clean-artifacts setup split submodules rebuild rerun check
.PHONY: extract-sprites extract-animation-metadata extract-animation-scripts
.PHONY: extract-animation-sprites extract-animations extract-gifs
.PHONY: extract-texts extract-dialogues extract-map-sprites extract-cutscenes

# If needed, prevent Make from deleting intermediate .s files
# .SECONDARY: $(CUTSCENE_ASM) $(DIALOGUE_ASM) $(TEXT_ASM)

MAKEFLAGS += --no-builtin-rules