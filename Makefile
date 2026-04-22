BASENAME := hm64
TARGET := $(BASENAME).z64

# default to US
REGION ?= us
BASEROM := baserom.$(REGION).z64

# Options
VERBOSE := 0
MODERN_GCC ?= 0

# Directories
SRC_DIRS := src
BIN_DIRS := bin
BUILD_DIR := build
TOOLS_DIR := tools
ASSETS_DIR := assets

# Files
C_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
BIN_FILES=$(foreach dir, $(BIN_DIRS), $(wildcard $(dir)/*.bin))

LD_MAP := $(BASENAME).map

# Tools

PYTHON ?= python3

KMC_PATH := $(TOOLS_DIR)/gcc-2.7.2/

CROSS := mips-linux-gnu-

AS := $(CROSS)as
LD := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
CPP := gcc -E -P -x c
MKLDSCRIPT := $(TOOLS_DIR)/build/mkldscript

ifeq ($(MODERN_GCC),1)
  CC := $(CROSS)gcc
  CC_FLAG :=
else
  CC := $(KMC_PATH)gcc
  CC_FLAG := -B $(KMC_PATH)
endif

# Common recipe command to create output directory
MKDIR = @mkdir -p $(dir $@)

SPEC := spec
SPEC_PROCESSED := $(BUILD_DIR)/spec
LD_SCRIPT := $(BUILD_DIR)/$(BASENAME).ld

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

ifeq ($(TESTING),1)
  MACROS += -DTESTING=1
endif

# Common flags for both compilers
CFLAGS_COMMON := -G0 -mgp32 -mfp32 -funsigned-char

ifeq ($(MODERN_GCC),1)
  # Modern GCC flags for N64/VR4300
  CFLAGS_COMMON += -march=vr4300 -mtune=vr4300 -mfix4300
  CFLAGS_COMMON += -mabi=32 -mno-abicalls -fno-PIC -mno-shared
  CFLAGS_COMMON += -ffreestanding -fno-builtin
  CFLAGS_COMMON += -EB
  CFLAGS_COMMON += -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-pointer-sign -Wno-incompatible-pointer-types -Wno-int-conversion
  MACROS += -DMODERN_GCC
else
  # Original KMC GCC 2.7.2 flags
  CFLAGS_COMMON += -mips3
  CFLAGS_COMMON += -Wa,-Iinclude
endif

CFLAGS := $(CFLAGS_COMMON) $(MACROS)

# Assembly flags - note -mabi32 (old) vs -mabi=32 (modern)
ifeq ($(MODERN_GCC),1)
  HASM_ASFLAGS := -march=vr4300 -mabi=32 -mno-abicalls -fno-PIC -G 0 -mgp32 -mfp32 -nostdinc
  # Modern GCC doesn't need -o32 (covered by -mabi=32)
  OS_ASM_FLAG :=
else
  HASM_ASFLAGS := -mips3 -nostdinc -fno-PIC -mno-abicalls -G 0 -mgp32 -mfp32 -mabi32
  # Old GCC needs -o32 for OS assembly
  OS_ASM_FLAG := -o32
endif

ASFLAGS := -G 0 -I include -mips3
CPPFLAGS := -I. -I include -I src -I lib/nusys-1/include -I lib/libultra/include -I lib/libultra/include/PR -I lib/libmus/include/PR -I lib/gcc/include -I lib/yay0

HASM_AS_DEFINES := -D_LANGUAGE_ASSEMBLY -DMIPSEB -D_ULTRA64 -D_MIPS_SIM=1

ifeq ($(MODERN_GCC),1)
  HASM_AS_DEFINES += -DMODERN_GCC
endif

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

LDFLAGS := -G 0 -T config/$(REGION)/undefined_syms.txt -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

ifeq ($(VERBOSE),0)
V := @
endif

# Create all output directories upfront
$(shell mkdir -p $(sort $(dir $(OBJECTS))))

# ==============================================================================
# CUTSCENES
# ==============================================================================

CUTSCENE_SRC_DIR := $(SRC_DIRS)/bytecode/cutscenes
CUTSCENE_ASSETS_DIR := assets/cutscenes
CUTSCENE_BUILD_DIR := $(BUILD_DIR)/assets/cutscenes

CUTSCENE_TRANSPILER := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.cutscenes.transpiler

$(CUTSCENE_ASSETS_DIR)/%.s: $(CUTSCENE_SRC_DIR)/%.cutscene
	@mkdir -p $(CUTSCENE_ASSETS_DIR)
	$(V)$(CPP) -I src/buffers -I src/game -I src/assetIndices $< | $(CUTSCENE_TRANSPILER) - -n $* -o $@

$(CUTSCENE_BUILD_DIR)/%.bin.o: $(CUTSCENE_ASSETS_DIR)/%.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

CUTSCENE_OBJECTS := \
	$(CUTSCENE_BUILD_DIR)/farmBusiness.bin.o \
	$(CUTSCENE_BUILD_DIR)/farmVisits.bin.o \
	$(CUTSCENE_BUILD_DIR)/houseCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/roadCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/mountainCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/ranchCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/vineyardCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/village1Cutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/village2Cutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/beachCutscenes.bin.o \
	$(CUTSCENE_BUILD_DIR)/sowingFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/horseRace.bin.o \
	$(CUTSCENE_BUILD_DIR)/flowerFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/vegetableFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/fireworksFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/fireflyFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/seaFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/cowFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/harvestFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/eggFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/dogRace.bin.o \
	$(CUTSCENE_BUILD_DIR)/spiritFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/newYearFestival.bin.o \
	$(CUTSCENE_BUILD_DIR)/funeralIntro.bin.o \
	$(CUTSCENE_BUILD_DIR)/evaluationEnding.bin.o \
	$(CUTSCENE_BUILD_DIR)/demos.bin.o \
	$(CUTSCENE_BUILD_DIR)/howToPlay.bin.o

# ==============================================================================
# DIALOGUES
# ==============================================================================

DIALOGUE_SRC_DIR := $(SRC_DIRS)/bytecode/dialogues
DIALOGUE_ASSETS_DIR := assets/dialogues
DIALOGUE_BUILD_DIR := $(BUILD_DIR)/assets/dialogues/bytecode

DIALOGUE_TRANSPILER := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.dialogues.transpiler

$(DIALOGUE_ASSETS_DIR)/%.s: $(DIALOGUE_SRC_DIR)/%.dialogue
	@mkdir -p $(DIALOGUE_ASSETS_DIR)
	$(V)$(DIALOGUE_TRANSPILER) transpile $< -n $* -o $(DIALOGUE_ASSETS_DIR)/

# Mark dependency
$(DIALOGUE_ASSETS_DIR)/%Index.s: $(DIALOGUE_ASSETS_DIR)/%.s
	@:

$(DIALOGUE_BUILD_DIR)/%.bin.o: $(DIALOGUE_ASSETS_DIR)/%.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

DIALOGUE_OBJECTS := \
	$(DIALOGUE_BUILD_DIR)/text1Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/text1DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/diaryDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/diaryDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/elliDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/elliDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/kaiDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/kaiDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/karenDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/karenDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/cliffDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/cliffDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/jeffDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/jeffDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/harrisDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/harrisDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/popuriDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/popuriDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mariaDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mariaDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/annDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/annDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/grayDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/grayDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/ellenDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/ellenDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/gotzDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/gotzDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/sashaDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/sashaDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/kentDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/kentDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/stuDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/stuDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/dougDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/dougDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/basilDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/basilDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/lilliaDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/lilliaDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/saibaraDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/saibaraDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/midwifeDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/midwifeDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/dukeDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/dukeDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/shipperDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/shipperDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites1Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites1DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites2Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites2DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites3Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestSprites3DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/assistantCarpenters1Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/assistantCarpenters1DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/assistantCarpenters2Dialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/assistantCarpenters2DialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/masterCarpenterDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/masterCarpenterDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayorDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayorDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/gregDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/gregDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/rickDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/rickDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/barleyDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/barleyDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/sydneyDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/sydneyDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayorWifeDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mayorWifeDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/pastorDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/pastorDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/potionShopDealerDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/potionShopDealerDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/ranchCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/ranchCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/fireworksFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/fireworksFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/flowerFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/flowerFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/seaFestivalAndEvaluationDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/seaFestivalAndEvaluationDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/cowFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/cowFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/fireflyFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/fireflyFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/dogRaceDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/dogRaceDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/vegetableFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/vegetableFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mountainCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mountainCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/sowingFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/sowingFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/harvestFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/horseRaceDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/horseRaceDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/vineyardCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/vineyardCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/roadCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/roadCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/farmVisitsDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/farmVisitsDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/houseCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/houseCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/eggFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/eggFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/village1CutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/village1CutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/village2CutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/village2CutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/namingScreenDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/namingScreenDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/newYearFestivalDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/newYearFestivalDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/beachCutscenesDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/beachCutscenesDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/shopDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/shopDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/festivalOverlaySelectionsDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/festivalOverlaySelectionsDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/babyDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/babyDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/mrsManaDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/mrsManaDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/johnDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/johnDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/libraryDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/libraryDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/gourmetJudgeDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/gourmetJudgeDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/npcBabyDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/npcBabyDialogueIndex.bin.o \
	$(DIALOGUE_BUILD_DIR)/entomologistDialogue.bin.o \
	$(DIALOGUE_BUILD_DIR)/entomologistDialogueIndex.bin.o

# ==============================================================================
# Texts
# ==============================================================================

TEXT_TRANSPILER := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.text.transpiler
TEXT_EXTRACTOR := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.text.extractor

TEXT_ASSETS_DIR := assets/text
TEXT_BUILD_DIR := $(BUILD_DIR)/assets/text

TEXT_BANKS := text1 \
	library \
	diary \
	festivalOverlaySelections \
	letters \
	levelInteractions \
	animalInteractions \
	tv \
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
	ranchCutscenes \
	funeralIntro \
	fireworksFestival \
	flowerFestival \
	seaFestivalAndEvaluation \
	cowFestival \
	fireflyFestival \
	dogRace \
	mountainCutscenes \
	sowingFestival \
	harvestFestival \
	newYearFestival \
	spiritFestival \
	horseRace \
	village1Cutscenes \
	village2Cutscenes \
	vineyardCutscenes \
	roadCutscenes \
	farmVisits \
	houseCutscenes \
	eggFestival \
	beachCutscenes \
	vegetableFestival \
	baby \
	mrsManaAndJohn \
	additionalNPCs \
	howToPlay

# ==============================================================================
# Sprites
# ==============================================================================

SPRITE_ASSETS_DIR := assets/sprites
SPRITE_BUILD_DIR := $(BUILD_DIR)/assets/sprites
SPRITE_ASSEMBLER := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.sprites.assembler
SPRITE_EXTRACTOR := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.sprites.extractor

extract-sprites:
	@find $(SPRITE_ASSETS_DIR) -type f ! -name "*.spec" -delete 2>/dev/null || true
	@find $(SPRITE_ASSETS_DIR) -type d -empty -delete 2>/dev/null || true
	$(V)$(SPRITE_EXTRACTOR) extract_all

# Each sprite directory produces 3 bin files: <label>Texture.bin, <label>AssetsIndex.bin, <label>SpritesheetIndex.bin
# The Texture.bin rule produces all three; the other two are marked as depending on it.

$(SPRITE_BUILD_DIR)/%AssetsIndex.bin: $(SPRITE_BUILD_DIR)/%Texture.bin
	@:

$(SPRITE_BUILD_DIR)/%SpritesheetIndex.bin: $(SPRITE_BUILD_DIR)/%Texture.bin
	@:

# Sprite compression: transform the raw .bin files from the assembler into
# Yay0-compressed equivalents. Each sprite's 2-or-3 .bin files compress in a
# single tool invocation, producing .bin.yay0 siblings. Final wrap produces
# .bin.yay0.o, mirroring the map convention (.hm64map.yay0.o).
SPRITE_COMPRESSOR := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.yay0.compress_sprite

# Sprites that have a SpritesheetIndex on disk but are DMA'd as a whole blob at
# runtime (see globalSprites.c assetType-2 path) must be compressed whole-blob.
WHOLE_BLOB_SPRITES := festivalFlowers

# Preserve the stamp + .bin.yay0 intermediates across builds so Make
# doesn't recompress every sprite on every incremental build.
.PRECIOUS: $(SPRITE_BUILD_DIR)/%.sprite-yay0.stamp
.PRECIOUS: $(SPRITE_BUILD_DIR)/%Texture.bin.yay0
.PRECIOUS: $(SPRITE_BUILD_DIR)/%AssetsIndex.bin.yay0
.PRECIOUS: $(SPRITE_BUILD_DIR)/%SpritesheetIndex.bin.yay0

# Stamp file routes all 2-or-3 .bin.yay0 outputs through a single
# pattern-rule dependency. Avoids Make's implicit-chain "Circular dependency
# dropped" warnings that arise when each sibling points at Texture.bin.yay0.
$(SPRITE_BUILD_DIR)/%.sprite-yay0.stamp: $(SPRITE_BUILD_DIR)/%Texture.bin $(SPRITE_BUILD_DIR)/%AssetsIndex.bin
	@mode=auto; \
	 case " $(WHOLE_BLOB_SPRITES) " in *" $(notdir $*) "*) mode=whole-blob ;; esac; \
	 sht=$(SPRITE_BUILD_DIR)/$*SpritesheetIndex.bin; \
	 if [ -f "$$sht" ]; then \
	   $(SPRITE_COMPRESSOR) --mode $$mode \
	     --texture-in $(SPRITE_BUILD_DIR)/$*Texture.bin --assets-index-in $(SPRITE_BUILD_DIR)/$*AssetsIndex.bin --sheet-index-in $$sht \
	     --texture-out $(SPRITE_BUILD_DIR)/$*Texture.bin.yay0 --assets-index-out $(SPRITE_BUILD_DIR)/$*AssetsIndex.bin.yay0 \
	     --sheet-index-out $(SPRITE_BUILD_DIR)/$*SpritesheetIndex.bin.yay0; \
	 else \
	   $(SPRITE_COMPRESSOR) --mode $$mode \
	     --texture-in $(SPRITE_BUILD_DIR)/$*Texture.bin --assets-index-in $(SPRITE_BUILD_DIR)/$*AssetsIndex.bin \
	     --texture-out $(SPRITE_BUILD_DIR)/$*Texture.bin.yay0 --assets-index-out $(SPRITE_BUILD_DIR)/$*AssetsIndex.bin.yay0; \
	 fi
	@touch $@

$(SPRITE_BUILD_DIR)/%Texture.bin.yay0: $(SPRITE_BUILD_DIR)/%.sprite-yay0.stamp
	@:

$(SPRITE_BUILD_DIR)/%AssetsIndex.bin.yay0: $(SPRITE_BUILD_DIR)/%.sprite-yay0.stamp
	@:

$(SPRITE_BUILD_DIR)/%SpritesheetIndex.bin.yay0: $(SPRITE_BUILD_DIR)/%.sprite-yay0.stamp
	@:

# Sprite .bin.yay0.o wraps the Yay0-compressed bytes.
$(SPRITE_BUILD_DIR)/%.bin.yay0.o: $(SPRITE_BUILD_DIR)/%.bin.yay0
	$(V)$(LD) -r -b binary -o $@ $<

.SECONDEXPANSION:

$(SPRITE_BUILD_DIR)/%Texture.bin: $(SPRITE_ASSETS_DIR)/$$(dir $$*)manifest.json
	@mkdir -p $(dir $@)
	$(V)$(SPRITE_ASSEMBLER) assemble $(SPRITE_ASSETS_DIR)/$(dir $*) $(dir $@)

# ==============================================================================
# Fonts
# ==============================================================================

FONT_ASSETS_DIR := assets/font
FONT_BUILD_DIR := $(BUILD_DIR)/assets/font
FONT_ASSEMBLER := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.fonts.assembler
FONT_EXTRACTOR := PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.fonts.extractor

extract-fonts:
	@rm -f $(FONT_ASSETS_DIR)/*.ci2 $(FONT_ASSETS_DIR)/*.pal 2>/dev/null || true
	$(V)$(FONT_EXTRACTOR)

$(FONT_BUILD_DIR)/fontTexture.bin: $(FONT_ASSETS_DIR)/fontTexture.ci2 $(FONT_ASSETS_DIR)/fontPalette1.pal $(FONT_ASSETS_DIR)/fontPalette2.pal $(FONT_ASSETS_DIR)/fontPalette3.pal
	@mkdir -p $(dir $@)
	$(V)$(FONT_ASSEMBLER) --assets-dir $(FONT_ASSETS_DIR) --output-dir $(FONT_BUILD_DIR)

$(FONT_BUILD_DIR)/fontPalette1.bin: $(FONT_BUILD_DIR)/fontTexture.bin
	@:

$(FONT_BUILD_DIR)/fontPalette2.bin: $(FONT_BUILD_DIR)/fontTexture.bin
	@:

$(FONT_BUILD_DIR)/fontPalette3.bin: $(FONT_BUILD_DIR)/fontTexture.bin
	@:

$(FONT_BUILD_DIR)/%.bin.o: $(FONT_BUILD_DIR)/%.bin
	$(V)$(LD) -r -b binary -o $@ $<

# ==============================================================================
# ASSET PATHS
# ==============================================================================

BOOT_BIN := bin/makerom/ipl3.bin

MAPS_DIR := bin/maps
SEQ_DIR := bin/audio

SPRITES_DIR := assets/sprites
TEXTS_DIR := assets/text

# ==============================================================================
# COMPRESSION
# ==============================================================================

# Must be defined before rules that reference it (compressed_ranges.h prereq).
SPEC_INCLUDES := $(shell find assets -name '*.spec' 2>/dev/null)

# Yay0 compression pipeline (generic across asset types):
#   .bin.o --(objcopy -j .data)--> .bin --(yay0 encoder)--> .yay0 --(ld -r -b binary)--> .yay0.o
# NOTE: only works on asset .bin.o files with zero relocations. Cutscene
# banks carry relocations against game globals and cannot use this path
# directly; they need a two-pass build that extracts resolved bytes from
# the final ELF.
$(BUILD_DIR)/assets/%.bin: $(BUILD_DIR)/assets/%.bin.o
	$(V)$(OBJCOPY) -O binary -j .data $< $@

$(BUILD_DIR)/assets/%.yay0: $(BUILD_DIR)/assets/%.bin
	$(V)PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.yay0.encoder $< $@

$(BUILD_DIR)/assets/%.yay0.o: $(BUILD_DIR)/assets/%.yay0
	$(V)$(LD) -r -b binary -o $@ $<

# Parallel pipeline for maps (.hm64map.o extension).
$(BUILD_DIR)/assets/%.hm64map.raw: $(BUILD_DIR)/assets/%.hm64map.o
	$(V)$(OBJCOPY) -O binary -j .data $< $@

$(BUILD_DIR)/assets/%.hm64map.yay0: $(BUILD_DIR)/assets/%.hm64map.raw
	$(V)PYTHONPATH=$(TOOLS_DIR) $(PYTHON) -m libhm64.yay0.encoder $< $@

$(BUILD_DIR)/assets/%.hm64map.yay0.o: $(BUILD_DIR)/assets/%.hm64map.yay0
	$(V)$(LD) -r -b binary -o $@ $<

# Don't wipe these on every build — they're expensive to regenerate.
.SECONDARY:

# ==============================================================================
# TARGETS
# ==============================================================================

.DEFAULT_GOAL := all

.PHONY: all extract split extract-texts extract-sprites extract-fonts
.PHONY: clean clean-extracted clean-all-dangerous

all: $(TARGET)

# ==============================================================================
# ASSET EXTRACTION
# ==============================================================================

# Extract all assets required for a dev rebuild
extract: split extract-texts extract-sprites extract-fonts

split:
	$(V)$(PYTHON) -m splat split ./config/$(REGION)/splat.$(REGION).yaml --modes code animationScripts bin hm64map seq

extract-texts:
	$(V)$(TEXT_EXTRACTOR) extract_all --modding

# ==============================================================================
# TEXTS
# ==============================================================================

# Pattern rule: Text .txt files -> .s assembly files
# Rebuilds whenever any .txt file in the bank directory changes
define TEXT_BANK_RULE
$(TEXT_ASSETS_DIR)/$(1)Text.s: $$(wildcard $(TEXT_ASSETS_DIR)/$(1)/*.txt)
	@echo "Transpiling text bank: $(1)"
	$(V)$(TEXT_TRANSPILER) transpile $(TEXT_ASSETS_DIR)/$(1) -n $(1)Text -o $(TEXT_ASSETS_DIR)/ --modding

# Mark the index as depending on the main file
$(TEXT_ASSETS_DIR)/$(1)TextIndex.s: $(TEXT_ASSETS_DIR)/$(1)Text.s
	@:
endef

# Generate rules for all text banks
$(foreach bank,$(TEXT_BANKS),$(eval $(call TEXT_BANK_RULE,$(bank))))

$(TEXT_BUILD_DIR)/%Text.bin.o: $(TEXT_ASSETS_DIR)/%Text.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(TEXT_BUILD_DIR)/%TextIndex.bin.o: $(TEXT_ASSETS_DIR)/%TextIndex.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# ==============================================================================
# CODE
# ==============================================================================

build/lib/nusys-1/src/nusys/nuboot.o: NU_OPTFLAGS := -O0
build/lib/nusys-1/src/sample/nunospak/nupakmenu.o: NU_OPTFLAGS += -g2
build/lib/nusys-1/src/sample/nunospak/nupakmenuloadfont.o: NU_OPTFLAGS += -g2
build/lib/libultra/src/io/aisetnextbuf.o: ULTRALIBVER := -DBUILD_VERSION=6

$(BUILD_DIR)/src/mainproc.o: src/mainproc.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/mainLoop.o: src/mainLoop.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/game/%.o: src/game/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/system/%.o: src/system/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/buffers/%.o: src/buffers/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/data/%.o: src/data/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(CPPFLAGS) -c -o $@ $<

# Lib

$(BUILD_DIR)/lib/libmus/src/player.o: lib/libmus/src/player.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(LIBMUS_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBMUS_CPP_FLAGS) -c -o $@ $<

# yay0

$(BUILD_DIR)/lib/yay0/%.o: lib/yay0/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(OPTFLAGS) $(CFLAGS) $(DEBUG_FLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/yay0/%.o: lib/yay0/%.s
	$(MKDIR)
	$(AS) $(ASFLAGS) -o $@ $<

# nusys

$(BUILD_DIR)/lib/nusys-1/src/nusys/%.o: lib/nusys-1/src/nusys/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/nualstl/%.o: lib/nusys-1/src/nualstl/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUALSTL_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/nusys-1/src/sample/nunospak/%.o: lib/nusys-1/src/sample/nunospak/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(NU_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(NUSYS_CPP_FLAGS) -c -o $@ $<

# libkmc

$(BUILD_DIR)/lib/libkmc/src/%.o: lib/libkmc/src/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(LIBKMC_OPTFLAGS) $(CFLAGS) $(LIBKMC_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libkmc/src/%.o: lib/libkmc/src/%.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c $(LIBKMC_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# libultra

$(BUILD_DIR)/lib/libultra/src/%.o: lib/libultra/src/%.c
	$(MKDIR)
	$(CC) $(CC_FLAG) $(LIBULTRA_OPTFLAGS) $(CFLAGS) $(ULTRALIBVER) $(LIBULTRA_CPP_FLAGS) -c -o $@ $<

$(BUILD_DIR)/lib/libultra/src/gu/%.o: lib/libultra/src/gu/%.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/libc/%.o: lib/libultra/src/libc/%.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/os/%.o: lib/libultra/src/os/%.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c $(LIBULTRA_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) $(ULTRALIBVER) $(OS_ASM_FLAG) -x assembler-with-cpp -o $@ $<

$(BUILD_DIR)/lib/libultra/src/rmon/%.o: lib/libultra/src/rmon/%.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c -I lib/libultra/include $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# ucode

$(BUILD_DIR)/lib/ucode/%.o: lib/ucode/%.s
	$(MKDIR)
	gcc -E -x assembler-with-cpp -I include $(HASM_AS_DEFINES) $< | $(AS) $(ASFLAGS) -I lib/ucode -o $@ -

# ROM header

$(BUILD_DIR)/config/$(REGION)/header.o: config/$(REGION)/header.s
	$(MKDIR)
	$(CC) $(CC_FLAG) -c $(NUSYS_CPP_FLAGS) $(HASM_AS_DEFINES) $(HASM_ASFLAGS) -x assembler-with-cpp -o $@ $<

# Makerom segments

$(BUILD_DIR)/makerom/ipl3.o: bin/makerom/ipl3.bin
	$(MKDIR)
	$(OBJCOPY) -I binary -O elf32-tradbigmips -B mips $< $@

$(BUILD_DIR)/makerom/entry.o: makerom/entry.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.s.o: %.s
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.hm64map.o: %.hm64map
	$(V)$(LD) -r -b binary -o $@ $<

$(BUILD_DIR)/%.seq.o: %.seq
	$(V)$(LD) -r -b binary -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin
	$(V)$(LD) -r -b binary -o $@ $<


# ==============================================================================
# CODE SEGMENT
# ==============================================================================


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
	$(BUILD_DIR)/src/system/staticGfx.o \
	$(BUILD_DIR)/src/system/map.o \
	$(BUILD_DIR)/src/system/mapController.o \
	$(BUILD_DIR)/src/system/audio.o \
	$(BUILD_DIR)/src/system/message.o \
	$(BUILD_DIR)/src/system/dialogue.o \
	$(BUILD_DIR)/src/system/numberSprites.o \
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
	$(BUILD_DIR)/src/data/animation/animationData.o \
	$(BUILD_DIR)/src/data/audio/sfx.o \
	$(BUILD_DIR)/src/game/namingScreen.o
	
LIB_COMPRESSION_OBJECTS := \
	$(BUILD_DIR)/lib/yay0/yay0.o \
	$(BUILD_DIR)/lib/yay0/yay0Decode.o

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
	$(LIB_COMPRESSION_OBJECTS) \
	$(NUSYS_SAMPLE_OBJECTS) \
	$(NUALSTL_OBJECTS) \
	$(LIBMUS_OBJECTS) \
	$(LIBNAUDIO_OBJECTS) \
	$(NUSYS_OBJECTS) \
	$(LIBULTRA_OBJECTS) \
	$(LIBKMC_OBJECTS)

$(BUILD_DIR)/codesegment.o: $(ALL_CODE_OBJECTS)
	$(MKDIR)
	$(V)$(LD) -r -G 0 -o $@ $^

# ==============================================================================
# LINKER SCRIPT
# ==============================================================================

$(SPEC_PROCESSED): $(SPEC) $(SPEC_INCLUDES)
	$(MKDIR)
	$(V)$(CPP) $(SPEC_CPP_FLAGS) $< > $@

$(LD_SCRIPT) $(ENTRY_ASM): $(SPEC_PROCESSED) $(MKLDSCRIPT)
	$(V)$(MKLDSCRIPT) $< $@

# ==============================================================================
# TARGET
# ==============================================================================

$(BASENAME).elf: $(OBJECTS) $(LD_SCRIPT)
	$(V)$(LD) $(LDFLAGS) -o $@

$(TARGET): $(BASENAME).elf
	$(V)$(OBJCOPY) -O binary --gap-fill=0xFF $< $@
	$(V)python3 $(TOOLS_DIR)/build/makemask.py $@ --pad

# ==============================================================================
# CLEAN
# ==============================================================================

# Clean build artifacts only
clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(LD_SCRIPT)
	@rm -f $(BASENAME).elf
	@rm -f $(BASENAME).map
	@rm -f $(TARGET)
	@rm -rf asm
	@find assets -type f -name "*.s" -delete 2>/dev/null || true

# Clean only extracted bin files (not extracted texts)
clean-extracted:
	@rm -rf bin
	@rm lib/ucode/*.bin

# Clean everything including texts
clean-all-dangerous: clean
	@rm -rf bin
	@rm lib/ucode/*.bin
# Remove everything in assets except .spec files
	@find assets -type f ! -name "*.spec" -delete 2>/dev/null || true
	@find assets -type d -empty -delete 2>/dev/null || true


CUTSCENE_ASM := $(patsubst $(CUTSCENE_BUILD_DIR)/%.bin.o,$(CUTSCENE_ASSETS_DIR)/%.s,$(CUTSCENE_OBJECTS))
DIALOGUE_ASM := $(patsubst $(DIALOGUE_BUILD_DIR)/%.bin.o,$(DIALOGUE_ASSETS_DIR)/%.s,$(DIALOGUE_OBJECTS))
TEXT_ASM := $(foreach bank,$(TEXT_BANKS),$(TEXT_ASSETS_DIR)/$(bank)Text.s $(TEXT_ASSETS_DIR)/$(bank)TextIndex.s)

# Prevent Make from deleting intermediate .s files
.SECONDARY: $(CUTSCENE_ASM) $(DIALOGUE_ASM) $(TEXT_ASM)

MAKEFLAGS += --no-builtin-rules
