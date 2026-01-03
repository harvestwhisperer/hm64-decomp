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

LD_MAP := $(BASENAME).map

# Tools

KMC_PATH := $(TOOLS_DIR)/gcc-2.7.2/

CROSS := mips-linux-gnu-

CC := $(KMC_PATH)gcc
AS := $(CROSS)as
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
CPP := gcc -E -P -x c
MKLDSCRIPT := $(TOOLS_DIR)/build/mkldscript

# Common recipe command to create output directory
MKDIR = @mkdir -p $(dir $@)

SPEC := spec
SPEC_PROCESSED := $(BUILD_DIR)/spec
LD_SCRIPT := $(BUILD_DIR)/$(BASENAME).ld
BSS_LD_SCRIPT := config/$(REGION)/common_bss.ld

# Export BUILD_DIR for mkldscript's $(BUILD_DIR) expansion
export BUILD_DIR

# Extract object files from the spec file
# Preprocesses spec, replaces $(BUILD_DIR), extracts .o paths
BUILD_DIR_REPLACE := sed -e 's|$$(BUILD_DIR)|$(BUILD_DIR)|g'
SPEC_CPP_FLAGS := -I. -I include -I lib/nusys-1/include -I lib/libultra/include
SPEC_O_FILES := $(shell $(CPP) $(SPEC_CPP_FLAGS) $(SPEC) | $(BUILD_DIR_REPLACE) | sed -n -E 's/^[[:space:]]*include[[:space:]]+"([^"]+\.o)"/\1/p')
OBJECTS := $(SPEC_O_FILES)

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

LDFLAGS := -G 0 -T config/$(REGION)/undefined_funcs.txt -T config/$(REGION)/undefined_syms.txt -T config/$(REGION)/undefined_funcs_auto.txt -T config/$(REGION)/undefined_syms_auto.txt -T $(BSS_LD_SCRIPT) -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

# Binary asset matching (cutscenes, dialogues, texts)

ALL_CUTSCENES := farmBusiness farmVisits familyCutscenes cutsceneBank4 cutsceneBank5 \
	annCutscenes karenCutscenes1 cutsceneBank8 mariaCutscenes karenCutscenes2 \
	sowingFestival horseRace flowerFestival squareFireworks fireworksFestival \
	fireflyFestival seaFestival cowFestival harvestFestival eggFestival \
	dogRace spiritFestival newYearFestival funeralIntro evaluationEnding \
	demos howToPlay

ifeq ($(REGION),us)
# All US cutscenes are decompiled to DSL
DECOMPILED_CUTSCENES := $(ALL_CUTSCENES)
else
DECOMPILED_CUTSCENES :=
endif

# Directories
CUTSCENE_SRC_DIR := $(SRC_DIRS)/bytecode/cutscenes
CUTSCENE_ASSETS_DIR := assets/cutscenes
CUTSCENE_BUILD_DIR := $(BUILD_DIR)/bin/cutscenes

# Transpilers
CUTSCENE_TRANSPILER := python3 $(TOOLS_DIR)/assets/hm64_cutscene_transpiler.py
CUTSCENE_UTILITIES := python3 $(TOOLS_DIR)/assets/hm64_cutscene_utilities.py

# Decompiled cutscene files (DSL -> ASM)
# DSL source in src/bytecode/cutscenes/, ASM output to assets/cutscenes/
CUTSCENE_DSL := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_SRC_DIR)/$(bank).cutscene)
CUTSCENE_ASM := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_ASSETS_DIR)/$(bank).s)
CUTSCENE_OBJ := $(foreach bank,$(DECOMPILED_CUTSCENES),$(CUTSCENE_BUILD_DIR)/$(bank).bin.o)

# Non-decompiled cutscenes: extract ASM directly from ROM
TRANSPILED_CUTSCENES := $(filter-out $(DECOMPILED_CUTSCENES),$(ALL_CUTSCENES))
TRANSPILED_CUTSCENE_ASM := $(foreach bank,$(TRANSPILED_CUTSCENES),$(CUTSCENE_ASSETS_DIR)/$(bank).s)
TRANSPILED_CUTSCENE_OBJ := $(foreach bank,$(TRANSPILED_CUTSCENES),$(CUTSCENE_BUILD_DIR)/$(bank).bin.o)

ifeq ($(REGION),us)
DECOMPILED_DIALOGUES := \
	text1Dialogue \
	diaryDialogue \
	elliDialogue \
	kaiDialogue \
	karenDialogue \
	cliffDialogue \
	jeffDialogue \
	harrisDialogue \
	popuriDialogue \
	mariaDialogue \
	annDialogue \
	grayDialogue \
	ellenDialogue \
	gotzDialogue \
	sashaDialogue \
	kentDialogue \
	mayDialogue \
	stuDialogue \
	dougDialogue \
	basilDialogue \
	lilliaDialogue \
	saibaraDialogue \
	midwifeDialogue \
	dukeDialogue \
	shipperDialogue \
	harvestSprites1Dialogue \
	harvestSprites2Dialogue \
	harvestSprites3Dialogue \
	assistantCarpenters1Dialogue \
	assistantCarpenters2Dialogue \
	masterCarpenterDialogue \
	mayorDialogue \
	gregDialogue \
	rickDialogue \
	barleyDialogue \
	sydneyDialogue \
	mayorWifeDialogue \
	pastorDialogue \
	potionShopDealerDialogue \
	cutscenes1Dialogue \
	text48Dialogue \
	text49Dialogue \
	text50Dialogue \
	text51Dialogue \
	text52Dialogue \
	dogRaceDialogue \
	vegetableFestivalDialogue \
	text54Dialogue \
	text55Dialogue \
	text56Dialogue \
	horseRaceDialogue \
	karenCutscenes1Dialogue \
	text63Dialogue \
	farmVisitsDialogue \
	text65Dialogue \
	eggFestivalDialogue \
	marriedDialogue \
	text61Dialogue \
	namingScreenDialogue \
	newYearFestivalDialogue \
	karenCutscenes2Dialogue \
	shopDialogue \
	festivalOverlaySelectionsDialogue \
	babyDialogue \
	mrsManaAndJohn1Dialogue \
	mrsManaAndJohn2Dialogue \
	libraryDialogue \
	additionalNpcs1Dialogue \
	npcBabyDialogue \
	additionalNpcs2Dialogue
else
DECOMPILED_DIALOGUES :=
endif

DIALOGUE_SRC_DIR := $(SRC_DIRS)/bytecode/dialogues
DIALOGUE_ASSETS_DIR := assets/dialogues
DIALOGUE_BUILD_DIR := $(BUILD_DIR)/bin/dialogues/bytecode

DIALOGUE_TRANSPILER := python3 $(TOOLS_DIR)/assets/hm64_dialogue_transpiler.py

DIALOGUE_DSL := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_SRC_DIR)/$(bank).dialogue)
DIALOGUE_ASM := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_ASSETS_DIR)/$(bank).s $(DIALOGUE_ASSETS_DIR)/$(bank)Index.s)
DIALOGUE_OBJ := $(foreach bank,$(DECOMPILED_DIALOGUES),$(DIALOGUE_BUILD_DIR)/$(bank).bin.o $(DIALOGUE_BUILD_DIR)/$(bank)Index.bin.o)

ifeq ($(REGION),us)

DECOMPILED_TEXTS := text1 \
	library \
	diary \
	recipesJapanese \
	festivalOverlaySelections \
	letters \
	shop \
	animalInteractions \
	tv \
	text10 \
	namingScreen \
	elli \
	kai \
	karen \
	gotz \
	sasha \
	cliff \
	jeff \
	kent \
	harris \
	popuri \
	maria \
	may \
	ann \
	doug \
	gray \
	basil \
	lillia \
	duke \
	shipper \
	harvestSprites \
	assistantCarpenters \
	masterCarpenter \
	mayor \
	greg \
	rick \
	barley \
	sydney \
	potionShopDealer \
	mayorWife \
	ellen \
	stu \
	midwife \
	pastor \
	saibara \
	cutscenes1 \
	funeralIntro \
	text48 \
	text49 \
	text50 \
	text51 \
	text52 \
	dogRace \
	text54 \
	text55 \
	text56 \
	newYearFestival \
	spiritFestival \
	horseRace \
	marriedDialogues \
	text61 \
	karenCutscenes1 \
	text63 \
	farmVisits \
	text65 \
	eggFestival \
	karenCutscenes2 \
	vegetableFestival \
	baby \
	mrsManaAndJohn \
	additionalNpcs \
	howToPlay

else
DECOMPILED_TEXTS :=
endif

TEXT_ASSETS_DIR := assets/text
TEXT_BUILD_DIR := $(BUILD_DIR)/bin/text

TEXT_TRANSPILER := python3 $(TOOLS_DIR)/assets/hm64_text_transpiler.py

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

build/lib/nusys-1/src/nusys/nuboot.o: NU_OPTFLAGS := -O0
build/lib/nusys-1/src/sample/nunospak/nupakmenu.o: NU_OPTFLAGS += -g2
build/lib/nusys-1/src/sample/nunospak/nupakmenuloadfont.o: NU_OPTFLAGS += -g2
build/lib/libultra/src/io/aisetnextbuf.o: ULTRALIBVER := -DBUILD_VERSION=6

# Targets

all: check

clean:
	@rm -rf build
	@rm -rf asm
	@rm -rf bin
# don't remove spec files or text source files (for modding workflow)
	@find assets -type f ! -name "*.spec" ! -name "*.txt" -delete 2>/dev/null || true
# clean up empty directories
	@find assets -type d -empty -delete 2>/dev/null || true
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)
	@rm -f ${DIALOGUE_ASM}
	@rm -f ${TEXT_ASM}

# Explicitly clean extracted text source files (modding workflow)
clean-texts:
	@find $(TEXT_ASSETS_DIR) -name "text*.txt" -delete 2>/dev/null || true
	@find $(TEXT_ASSETS_DIR) -name "_metadata.txt" -delete 2>/dev/null || true
	@find $(TEXT_ASSETS_DIR) -type d -empty -delete 2>/dev/null || true
	@echo "Cleaned extracted text files from $(TEXT_ASSETS_DIR)"

clean-artifacts:
	@rm -rf build
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)
	@rm -f $(CUTSCENE_ASM) $(TRANSPILED_CUTSCENE_ASM)
	@rm -f ${DIALOGUE_ASM}
	@rm -f ${TEXT_ASM}

submodules:
	git submodule update

ifeq ($(REGION),jp)
split:
	$(V)python3 -m splat split ./config/$(REGION)/splat.$(REGION).yaml
else
split:
	$(V)python3 -m splat split ./config/$(REGION)/splat.$(REGION).yaml --modes code bin animationScripts
endif

setup: clean split

rebuild: clean-artifacts $(LD_SCRIPT) check

rerun: clean $(LD_SCRIPT) check

# Asset extraction

extract-sprites:
# don't delete spec files
	@find assets/sprites -type f ! -name "*.spec" -delete 2>/dev/null || true
# clean up empty directories
	@find assets/sprites -type d -empty -delete 2>/dev/null || true
	@cd tools/assets && python3 ./hm64_sprite_utilities.py write_all_textures    

extract-animation-metadata:
	@cd tools/assets && python3 ./hm64_animation_utilities.py extract_animation_metadata

extract-animation-scripts:
	@cd tools/assets && python3 ./extract_animation_scripts.py

extract-animation-sprites:
	@cd tools/assets && python3 ./hm64_animation_utilities.py fetch_sprites_for_animations
	
extract-animations:
	@cd tools/assets && python3 ./hm64_animation_utilities.py extract_all

extract-gifs:
	@cd tools/assets && python3 ./hm64_animation_utilities.py make_gifs_from_animations

extract-texts:
	@cd tools/assets && python3 ./hm64_text_utilities.py process_all write_files

extract-dialogues:
	@cd tools/assets && python3 ./hm64_dialogue_utilities.py decode_all

extract-map-sprites:
	@cd tools/assets && python3 ./hm64_map_utilities.py write_all_textures

extract-fonts:
	@cd tools/assets && python3 ./hm64_font_utilities.py extract_all
	@cd tools/assets && python3 ./hm64_font_utilities.py extract_all_palettes

extract-cutscenes:
	@cd tools/assets && python3 hm64_cutscene_utilities.py --all --labels --scan --verbose-scan --output-dir ../../$(CUTSCENE_ASSETS_DIR)
	@cd tools/assets && python3 hm64_cutscene_utilities.py --all --labels --graph --output-dir ../../$(CUTSCENE_ASSETS_DIR)
	@cd tools/assets && python3 hm64_cutscene_utilities.py --all --labels --asm --output-dir ../../$(CUTSCENE_ASSETS_DIR)

# Main code segment

# need to pass -B <dir> to gcc to prevent it from fetching system default cc1
$(BUILD_DIR)/src/mainproc.o: src/mainproc.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/mainLoop.o: src/mainLoop.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/game/%.o: src/game/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/system/%.o: src/system/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/buffers/%.o: src/buffers/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/data/%.o: src/data/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $< 

# Lib

$(BUILD_DIR)/lib/libmus/src/player.o: lib/libmus/src/player.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(LIBMUS_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBMUS_CPP_FLAGS) -c -o $@ $<

# nusys

$(BUILD_DIR)/lib/nusys-1/src/nusys/%.o: lib/nusys-1/src/nusys/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/nualstl/%.o: lib/nusys-1/src/nualstl/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUALSTL_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/sample/nunospak/%.o: lib/nusys-1/src/sample/nunospak/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

# libkmc

$(BUILD_DIR)/lib/libkmc/src/%.o: lib/libkmc/src/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(LIBKMC_OPTFLAGS) $(CFLAGS) $(LIBKMC_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libkmc/src/%.o: lib/libkmc/src/%.s
	$(MKDIR)
	$(CC) -B $(KMC_PATH) -c $(LIBKMC_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# libultra

$(BUILD_DIR)/lib/libultra/src/%.o: lib/libultra/src/%.c
	$(MKDIR)
	$(CC) -B $(KMC_PATH) $(LIBULTRA_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBULTRA_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libultra/src/gu/%.o: lib/libultra/src/gu/%.s
	$(MKDIR)
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/libc/%.o: lib/libultra/src/libc/%.s
	$(MKDIR)
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/os/%.o: lib/libultra/src/os/%.s
	$(MKDIR)
	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -o32 -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/os/unusedPadding.o: lib/libultra/src/os/unusedPadding.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/lib/libultra/src/rmon/%.o: lib/libultra/src/rmon/%.s
	$(MKDIR)
# 	$(CC) -B $(KMC_PATH) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<
	$(CC) -B $(KMC_PATH) -c -I lib/libultra/include $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# ucode

$(BUILD_DIR)/lib/ucode/%.o: lib/ucode/%.s
	$(MKDIR)
	gcc -E -x assembler-with-cpp -I include $(HASM_AS_DEFINES) $< | $(AS) $(ASFLAGS) -I lib/ucode -o $@ -
# 	$(CC) -B $(KMC_PATH) -E -I include $(HASM_AS_DEFINES) $< | $(AS) $(ASFLAGS) -o $@ -

# ROM header

$(BUILD_DIR)/config/$(REGION)/header.o: config/$(REGION)/header.s
	$(MKDIR)
	$(CC) -B $(KMC_PATH) -c $(NUSYS_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# Makerom segments

$(BUILD_DIR)/makerom/ipl3.o: bin/makerom/ipl3.bin
	$(MKDIR)
	$(OBJCOPY) -I binary -O elf32-tradbigmips -B mips $< $@

$(BUILD_DIR)/makerom/entry.o: asm/makerom/entry.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Asset building

# Decompiled cutscenes: transpile DSL to assembly (outputs to assets/cutscenes/)
$(CUTSCENE_ASSETS_DIR)/%.s: $(CUTSCENE_SRC_DIR)/%.cutscene
	@mkdir -p $(CUTSCENE_ASSETS_DIR)
	$(V)$(CPP) -I src/buffers -I src/game -I src/assetIndices $< | $(CUTSCENE_TRANSPILER) - -n $* -o $@

# Non-decompiled cutscenes: extract assembly from ROM (also outputs to assets/cutscenes/)
$(TRANSPILED_CUTSCENE_ASM): $(CUTSCENE_ASSETS_DIR)/%.s: $(BASEROM)
	@mkdir -p $(CUTSCENE_ASSETS_DIR)
	$(V)cd $(TOOLS_DIR)/assets && python3 hm64_cutscene_utilities.py --bank $* --asm --output-dir ../../$(CUTSCENE_ASSETS_DIR)

# All cutscenes: assemble from assets/cutscenes/
$(CUTSCENE_BUILD_DIR)/%.bin.o: $(CUTSCENE_ASSETS_DIR)/%.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Transpile dialogue DSL to assembly (outputs to assets/dialogues/)
# The transpiler produces both .s and Index.s
$(DIALOGUE_ASSETS_DIR)/%.s: $(DIALOGUE_SRC_DIR)/%.dialogue
	@mkdir -p $(DIALOGUE_ASSETS_DIR)
	$(V)$(DIALOGUE_TRANSPILER) transpile $< -n $* -o $(DIALOGUE_ASSETS_DIR)/

# Mark dependency
$(DIALOGUE_ASSETS_DIR)/%Index.s: $(DIALOGUE_ASSETS_DIR)/%.s
	@:

$(DIALOGUE_BUILD_DIR)/%.bin.o: $(DIALOGUE_ASSETS_DIR)/%.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Extract texts to assets directory and transpile to assembly (generates two files: bytecode and index)
# The transpiler produces both Text.s and TextIndex.s
$(TEXT_ASSETS_DIR)/%Text.s:
	$(V)cd $(TOOLS_DIR)/assets && python3 hm64_text_utilities.py extract_bank $* --literal
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

NUBOOT_OBJECTS := \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nuboot.o

CODE_OBJECTS := \
	$(BUILD_DIR)/src/mainproc.o \
	$(BUILD_DIR)/src/mainLoop.o \
	$(BUILD_DIR)/src/system/interpolation.o \
	$(BUILD_DIR)/src/system/math.o \
	$(BUILD_DIR)/src/system/graphic.o \
	$(BUILD_DIR)/src/system/spriteDMAManager.o \
	$(BUILD_DIR)/src/system/sceneGraph.o \
	$(BUILD_DIR)/src/system/sprite.o \
	$(BUILD_DIR)/src/system/globalSprites.o \
	$(BUILD_DIR)/src/system/entity.o \
	$(BUILD_DIR)/src/system/unknownData.o \
	$(BUILD_DIR)/src/system/unknownData2.o \
	$(BUILD_DIR)/src/system/staticGfx.o \
	$(BUILD_DIR)/src/system/paddingData.o \
	$(BUILD_DIR)/src/system/map.o \
	$(BUILD_DIR)/src/system/mapController.o \
	$(BUILD_DIR)/src/system/audio.o \
	$(BUILD_DIR)/src/system/message.o \
	$(BUILD_DIR)/src/system/dialogue.o \
	$(BUILD_DIR)/src/system/overlayScreenSprites.o \
	$(BUILD_DIR)/src/system/cutscene.o \
	$(BUILD_DIR)/src/system/controller.o \
	$(BUILD_DIR)/src/system/memory.o \
	$(BUILD_DIR)/src/system/flags.o \
	$(BUILD_DIR)/src/data/fieldTileMaps/fieldTiles.o \
	$(BUILD_DIR)/src/game/gameStart.o \
	$(BUILD_DIR)/src/game/initialize.o \
	$(BUILD_DIR)/src/game/transition.o \
	$(BUILD_DIR)/src/game/load.o \
	$(BUILD_DIR)/src/game/game.o \
	$(BUILD_DIR)/src/game/gameStatus.o \
	$(BUILD_DIR)/src/game/player.o \
	$(BUILD_DIR)/src/game/level.o \
	$(BUILD_DIR)/src/game/npc.o \
	$(BUILD_DIR)/src/game/animals.o \
	$(BUILD_DIR)/src/game/cutscenes.o \
	$(BUILD_DIR)/src/data/fieldTileMaps/cutsceneFieldTiles.o \
	$(BUILD_DIR)/src/game/evaluation.o \
	$(BUILD_DIR)/src/game/gameAudio.o \
	$(BUILD_DIR)/src/game/levelInteractions.o \
	$(BUILD_DIR)/src/game/overlayScreens.o \
	$(BUILD_DIR)/src/game/items.o \
	$(BUILD_DIR)/src/game/time.o \
	$(BUILD_DIR)/src/game/tv.o \
	$(BUILD_DIR)/src/game/fieldObjects.o \
	$(BUILD_DIR)/src/game/weather.o \
	$(BUILD_DIR)/src/game/shop.o \
	$(BUILD_DIR)/src/game/title.o \
	$(BUILD_DIR)/src/game/gameFile.o \
	$(BUILD_DIR)/src/data/animationScripts/animationScripts.o \
	$(BUILD_DIR)/src/data/audio/sfx.o \
	$(BUILD_DIR)/src/game/namingScreen.o

NUSYS_SAMPLE_OBJECTS := \
	$(BUILD_DIR)/lib/nusys-1/src/sample/nunospak/nupakmenu.o \
	$(BUILD_DIR)/lib/nusys-1/src/sample/nunospak/nupakmenuloadfont.o

NUALSTL_OBJECTS := \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlinit.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlbankset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlplayerinit.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlseqplayersetdata.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlseqplayerplay.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlsndplayerplay.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuauprenmifuncset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuauprenmiproc.o \
	$(BUILD_DIR)/lib/nusys-1/src/nualstl/nuaustlmgr.o

LIBMUS_OBJECTS := \
	$(BUILD_DIR)/lib/libmus/src/player.o

LIBNAUDIO_OBJECTS := \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_sl.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synaddplayer.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synallocvoice.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_syndelete.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synsetfxmix.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synsetpan.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synsetpitch.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synsetvol.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synstartvoice.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synstopvoice.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synthesizer.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_auxbus.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_drvrNew.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_env.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_load.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_resample.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_reverb.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_save.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_synallocfx.o \
	$(BUILD_DIR)/lib/libultra/src/libnaudio/n_mainbus.o

NUSYS_OBJECTS := \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nusched.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nurdpoutput.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nuyieldbuf.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nudramstack.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxthread.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxtaskmgr.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxfuncset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxprenmifuncset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxswapcfbfuncset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxswapcfb.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxtaskallendwait.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxretracewait.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxsetcfb.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxdisplayoff.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nugfxdisplayon.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontmgr.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontinit.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontdatalock.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontdatareadwait.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontdatareadstart.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontdatagetex.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontdatagetexall.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontqueryread.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakmgr.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakopen.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakgetfree.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakrepairid.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfileopenjis.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakcodeset.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfilereadwrite.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfiledelete.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfiledeletejis.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfilestate.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakfilenum.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontrmbmgr.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nupiinit.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nupireadrom.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nusimgr.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nusicallbackadd.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nusicallbackremove.o \
	$(BUILD_DIR)/lib/nusys-1/src/nusys/nucontpakjiston64.o

LIBULTRA_OBJECTS := \
	$(BUILD_DIR)/lib/libultra/src/gu/sqrtf.o \
	$(BUILD_DIR)/lib/libultra/src/gu/cosf.o \
	$(BUILD_DIR)/lib/libultra/src/gu/lookat.o \
	$(BUILD_DIR)/lib/libultra/src/gu/mtxutil.o \
	$(BUILD_DIR)/lib/libultra/src/gu/ortho.o \
	$(BUILD_DIR)/lib/libultra/src/gu/perspective.o \
	$(BUILD_DIR)/lib/libultra/src/gu/rotaterpy.o \
	$(BUILD_DIR)/lib/libultra/src/gu/scale.o \
	$(BUILD_DIR)/lib/libultra/src/gu/sinf.o \
	$(BUILD_DIR)/lib/libultra/src/gu/translate.o \
	$(BUILD_DIR)/lib/libultra/src/os/invaldcache.o \
	$(BUILD_DIR)/lib/libultra/src/os/unusedPadding.o \
	$(BUILD_DIR)/lib/libultra/src/os/setintmask.o \
	$(BUILD_DIR)/lib/libultra/src/os/writebackdcache.o \
	$(BUILD_DIR)/lib/libultra/src/os/writebackdcacheall.o \
	$(BUILD_DIR)/lib/libultra/src/os/createmesgqueue.o \
	$(BUILD_DIR)/lib/libultra/src/os/createthread.o \
	$(BUILD_DIR)/lib/libultra/src/os/recvmesg.o \
	$(BUILD_DIR)/lib/libultra/src/os/sendmesg.o \
	$(BUILD_DIR)/lib/libultra/src/os/setthreadpri.o \
	$(BUILD_DIR)/lib/libultra/src/os/startthread.o \
	$(BUILD_DIR)/lib/libultra/src/os/stopthread.o \
	$(BUILD_DIR)/lib/libultra/src/os/thread.o \
	$(BUILD_DIR)/lib/libultra/src/os/virtualtophysical.o \
	$(BUILD_DIR)/lib/libultra/src/debug/assert.o \
	$(BUILD_DIR)/lib/libultra/src/audio/sl.o \
	$(BUILD_DIR)/lib/libultra/src/audio/heapinit.o \
	$(BUILD_DIR)/lib/libultra/src/audio/heapalloc.o \
	$(BUILD_DIR)/lib/libultra/src/audio/copy.o \
	$(BUILD_DIR)/lib/libultra/src/audio/syndelete.o \
	$(BUILD_DIR)/lib/libultra/src/io/aigetlen.o \
	$(BUILD_DIR)/lib/libultra/src/io/aigetstat.o \
	$(BUILD_DIR)/lib/libultra/src/io/aisetnextbuf.o \
	$(BUILD_DIR)/lib/libultra/src/io/spsetpc.o \
	$(BUILD_DIR)/lib/libultra/src/io/sptask.o \
	$(BUILD_DIR)/lib/libultra/src/io/sptaskyield.o \
	$(BUILD_DIR)/lib/libultra/src/io/sptaskyielded.o \
	$(BUILD_DIR)/lib/libultra/src/io/vigetcurrframebuf.o \
	$(BUILD_DIR)/lib/libultra/src/io/vigetnextframebuf.o \
	$(BUILD_DIR)/lib/libultra/src/io/visetevent.o \
	$(BUILD_DIR)/lib/libultra/src/io/visetmode.o \
	$(BUILD_DIR)/lib/libultra/src/io/visetyscale.o \
	$(BUILD_DIR)/lib/libultra/src/io/viswapbuf.o \
	$(BUILD_DIR)/lib/libultra/src/io/viblack.o \
	$(BUILD_DIR)/lib/libultra/src/io/contquery.o \
	$(BUILD_DIR)/lib/libultra/src/io/controller.o \
	$(BUILD_DIR)/lib/libultra/src/io/epidma.o \
	$(BUILD_DIR)/lib/libultra/src/io/epilinkhandle.o \
	$(BUILD_DIR)/lib/libultra/src/libc/bcopy.o \
	$(BUILD_DIR)/lib/libultra/src/libc/bzero.o \
	$(BUILD_DIR)/lib/libultra/src/libc/syncPrintf.o \
	$(BUILD_DIR)/lib/libultra/src/audio/drvrnew.o \
	$(BUILD_DIR)/lib/libultra/src/audio/env.o \
	$(BUILD_DIR)/lib/libultra/src/audio/reverb.o \
	$(BUILD_DIR)/lib/libultra/src/audio/save.o \
	$(BUILD_DIR)/lib/libultra/src/audio/synthesizer.o \
	$(BUILD_DIR)/lib/libultra/src/io/aisetfreq.o \
	$(BUILD_DIR)/lib/libultra/src/io/cartrominit.o \
	$(BUILD_DIR)/lib/libultra/src/io/contreaddata.o \
	$(BUILD_DIR)/lib/libultra/src/io/motor.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsallocatefile.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsdeletefile.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsfilestate.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsfreeblocks.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsinitpak.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsnumfiles.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsreadwritefile.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsrepairid.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfssearchfile.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsselectbank.o \
	$(BUILD_DIR)/lib/libultra/src/io/pimgr.o \
	$(BUILD_DIR)/lib/libultra/src/io/pirawdma.o \
	$(BUILD_DIR)/lib/libultra/src/io/sirawdma.o \
	$(BUILD_DIR)/lib/libultra/src/io/sprawdma.o \
	$(BUILD_DIR)/lib/libultra/src/io/vimgr.o \
	$(BUILD_DIR)/lib/libultra/src/io/vitbl.o \
	$(BUILD_DIR)/lib/libultra/src/io/visetspecial.o \
	$(BUILD_DIR)/lib/libultra/src/io/viswapcontext.o \
	$(BUILD_DIR)/lib/libultra/src/os/exceptasm.o \
	$(BUILD_DIR)/lib/libultra/src/gu/libm_vals.o \
	$(BUILD_DIR)/lib/libultra/src/os/initialize.o \
	$(BUILD_DIR)/lib/libultra/src/os/interrupt.o \
	$(BUILD_DIR)/lib/libultra/src/os/seteventmesg.o \
	$(BUILD_DIR)/lib/libultra/src/os/getcause.o \
	$(BUILD_DIR)/lib/libultra/src/os/getcount.o \
	$(BUILD_DIR)/lib/libultra/src/os/getsr.o \
	$(BUILD_DIR)/lib/libultra/src/os/invalicache.o \
	$(BUILD_DIR)/lib/libultra/src/os/probetlb.o \
	$(BUILD_DIR)/lib/libultra/src/os/setfpcsr.o \
	$(BUILD_DIR)/lib/libultra/src/os/setsr.o \
	$(BUILD_DIR)/lib/libultra/src/os/unmaptlball.o \
	$(BUILD_DIR)/lib/libultra/src/os/maptlbrdb.o \
	$(BUILD_DIR)/lib/libultra/src/os/destroythread.o \
	$(BUILD_DIR)/lib/libultra/src/os/getthreadpri.o \
	$(BUILD_DIR)/lib/libultra/src/os/gettime.o \
	$(BUILD_DIR)/lib/libultra/src/os/jammesg.o \
	$(BUILD_DIR)/lib/libultra/src/debug/kdebugserver.o \
	$(BUILD_DIR)/lib/libultra/src/os/settimer.o \
	$(BUILD_DIR)/lib/libultra/src/os/timerintr.o \
	$(BUILD_DIR)/lib/libultra/src/os/initrdb.o \
	$(BUILD_DIR)/lib/libultra/src/os/rdbsend.o \
	$(BUILD_DIR)/lib/libultra/src/os/osthreadsave.o \
	$(BUILD_DIR)/lib/libultra/src/host/readhost.o \
	$(BUILD_DIR)/lib/libultra/src/audio/load.o \
	$(BUILD_DIR)/lib/libultra/src/audio/auxbus.o \
	$(BUILD_DIR)/lib/libultra/src/audio/filter.o \
	$(BUILD_DIR)/lib/libultra/src/audio/mainbus.o \
	$(BUILD_DIR)/lib/libultra/src/audio/resample.o \
	$(BUILD_DIR)/lib/libultra/src/audio/synallocfx.o \
	$(BUILD_DIR)/lib/libultra/src/io/ai.o \
	$(BUILD_DIR)/lib/libultra/src/io/sp.o \
	$(BUILD_DIR)/lib/libultra/src/io/spgetstat.o \
	$(BUILD_DIR)/lib/libultra/src/io/spsetstat.o \
	$(BUILD_DIR)/lib/libultra/src/io/vi.o \
	$(BUILD_DIR)/lib/libultra/src/io/vigetcurrcontext.o \
	$(BUILD_DIR)/lib/libultra/src/io/siacs.o \
	$(BUILD_DIR)/lib/libultra/src/vimodes/vimodentsclan1.o \
	$(BUILD_DIR)/lib/libultra/src/vimodes/vimodepallan1.o \
	$(BUILD_DIR)/lib/libultra/src/vimodes/vimodempallan1.o \
	$(BUILD_DIR)/lib/libultra/src/io/pigetcmdq.o \
	$(BUILD_DIR)/lib/libultra/src/io/piacs.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonsio.o \
	$(BUILD_DIR)/lib/libultra/src/libc/bcmp.o \
	$(BUILD_DIR)/lib/libultra/src/libc/xprintf.o \
	$(BUILD_DIR)/lib/libultra/src/io/contpfs.o \
	$(BUILD_DIR)/lib/libultra/src/io/contramread.o \
	$(BUILD_DIR)/lib/libultra/src/io/contramwrite.o \
	$(BUILD_DIR)/lib/libultra/src/io/crc.o \
	$(BUILD_DIR)/lib/libultra/src/io/devmgr.o \
	$(BUILD_DIR)/lib/libultra/src/io/epirawdma.o \
	$(BUILD_DIR)/lib/libultra/src/io/epirawread.o \
	$(BUILD_DIR)/lib/libultra/src/io/epirawwrite.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfschecker.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsgetstatus.o \
	$(BUILD_DIR)/lib/libultra/src/io/sirawread.o \
	$(BUILD_DIR)/lib/libultra/src/io/sirawwrite.o \
	$(BUILD_DIR)/lib/libultra/src/os/setcompare.o \
	$(BUILD_DIR)/lib/libultra/src/os/resetglobalintmask.o \
	$(BUILD_DIR)/lib/libultra/src/os/setglobalintmask.o \
	$(BUILD_DIR)/lib/libultra/src/os/yieldthread.o \
	$(BUILD_DIR)/lib/libultra/src/io/si.o \
	$(BUILD_DIR)/lib/libultra/src/io/pfsisplug.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmoncmds.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonmem.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonmisc.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonregs.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmontask.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonmain.o \
	$(BUILD_DIR)/lib/libultra/src/libc/string.o \
	$(BUILD_DIR)/lib/libultra/src/libc/xlitob.o \
	$(BUILD_DIR)/lib/libultra/src/libc/xldtob.o \
	$(BUILD_DIR)/lib/libultra/src/io/sprawread.o \
	$(BUILD_DIR)/lib/libultra/src/io/sprawwrite.o \
	$(BUILD_DIR)/lib/libultra/src/os/getactivequeue.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonrcp.o \
	$(BUILD_DIR)/lib/libultra/src/rmon/rmonbrk.o \
	$(BUILD_DIR)/lib/libultra/src/libc/ldiv.o

LIBKMC_OBJECTS := \
	$(BUILD_DIR)/lib/libkmc/src/rand.o \
	$(BUILD_DIR)/lib/libkmc/src/mmuldi3.o

ALL_CODE_OBJECTS := \
	$(NUBOOT_OBJECTS) \
	$(CODE_OBJECTS) \
	$(NUSYS_SAMPLE_OBJECTS) \
	$(NUALSTL_OBJECTS) \
	$(LIBMUS_OBJECTS) \
	$(LIBNAUDIO_OBJECTS) \
	$(NUSYS_OBJECTS) \
	$(LIBULTRA_OBJECTS) \
	$(LIBKMC_OBJECTS)

# Single combined codesegment.o - equivalent to static linking
$(BUILD_DIR)/codesegment.o: $(ALL_CODE_OBJECTS)
	$(MKDIR)
	$(V)$(LD) -r -G 0 -o $@ $^

# Linker script generation from spec file

$(SPEC_PROCESSED): $(SPEC)
	$(MKDIR)
	$(V)$(CPP) $(SPEC_CPP_FLAGS) $< > $@

$(LD_SCRIPT): $(SPEC_PROCESSED) $(MKLDSCRIPT)
	$(V)$(MKLDSCRIPT) $< $@

# Final binary

$(BASENAME).elf: $(OBJECTS) $(LD_SCRIPT)
	$(V)$(LD) $(LDFLAGS) -o $@

$(TARGET): $(BASENAME).elf
	$(V)$(OBJCOPY) -O binary --gap-fill=0xFF $< $@
	$(V)python3 $(TOOLS_DIR)/build/makemask.py $@ --pad

check: $(TARGET)
	$(V)diff $(TARGET) $(BASEROM) && echo "OK"


.PHONY: all clean clean-texts clean-artifacts setup split submodules rebuild rerun check codesegment
.PHONY: extract-sprites extract-animation-metadata extract-animation-scripts
.PHONY: extract-animation-sprites extract-animations extract-gifs
.PHONY: extract-texts extract-dialogues extract-map-sprites extract-cutscenes

# Prevent Make from deleting intermediate .s files
.SECONDARY: $(CUTSCENE_ASM) $(TRANSPILED_CUTSCENE_ASM) $(DIALOGUE_ASM) $(TEXT_ASM)

MAKEFLAGS += --no-builtin-rules