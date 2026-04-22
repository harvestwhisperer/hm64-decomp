.DEFAULT_GOAL := all

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

# Modding text pipeline: extract .txt per bank instead of decoded bytecode.
TEXT_EXTRACT_ARGS := --modding

# Modding wavetable pipeline: omit HM64-specific SDK padding from repacked .bin.
WAVETABLE_REPACKER_ARGS := --modding

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
	$(BUILD_DIR)/src/game/groundObjects.o \
	$(BUILD_DIR)/src/game/weather.o \
	$(BUILD_DIR)/src/game/shop.o \
	$(BUILD_DIR)/src/game/title.o \
	$(BUILD_DIR)/src/game/gameFile.o \
	$(BUILD_DIR)/src/data/animation/animationData.o \
	$(BUILD_DIR)/src/data/audio/sfx.o \
	$(BUILD_DIR)/src/game/namingScreen.o

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

include Makefile.common

# ==============================================================================
# Linker flags
# ==============================================================================

LDFLAGS := -G 0 -T config/$(REGION)/undefined_syms.txt -T $(LD_SCRIPT) -Map $(LD_MAP) --no-check-sections

# ==============================================================================
# Asset paths
# ==============================================================================

BOOT_BIN := bin/makerom/ipl3.bin

MAPS_DIR := bin/maps
SEQ_DIR := bin/audio

SPRITES_DIR := assets/sprites
TEXTS_DIR := assets/text

# ==============================================================================
# Texts (modding pipeline)
# ==============================================================================

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

define TEXT_BANK_RULE
$(TEXT_ASSETS_DIR)/$(1)Text.s: $$(wildcard $(TEXT_ASSETS_DIR)/$(1)/*.txt)
	@echo "Transpiling text bank: $(1)"
	$(V)$(TEXT_TRANSPILER) transpile $(TEXT_ASSETS_DIR)/$(1) -n $(1)Text -o $(TEXT_ASSETS_DIR)/ --modding

$(TEXT_ASSETS_DIR)/$(1)TextIndex.s: $(TEXT_ASSETS_DIR)/$(1)Text.s
	@:
endef

$(foreach bank,$(TEXT_BANKS),$(eval $(call TEXT_BANK_RULE,$(bank))))

$(TEXT_BUILD_DIR)/%Text.bin.o: $(TEXT_ASSETS_DIR)/%Text.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(TEXT_BUILD_DIR)/%TextIndex.bin.o: $(TEXT_ASSETS_DIR)/%TextIndex.s
	$(MKDIR)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# ==============================================================================
# Targets
# ==============================================================================

all: $(TARGET)

# Extract all assets required for a dev rebuild
extract: split extract-texts extract-sprites extract-fonts

# ==============================================================================
# Clean
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
	@find assets -type f ! -name "*.spec" -delete 2>/dev/null || true
	@find assets -type d -empty -delete 2>/dev/null || true

.PHONY: all extract split extract-texts extract-sprites extract-fonts
.PHONY: clean clean-extracted clean-all-dangerous

TEXT_ASM := $(foreach bank,$(TEXT_BANKS),$(TEXT_ASSETS_DIR)/$(bank)Text.s $(TEXT_ASSETS_DIR)/$(bank)TextIndex.s)

# Prevent Make from deleting intermediate .s files (scoped: only the
# transpiled asm files we care about, not every implicit .bin or .o).
.SECONDARY: $(CUTSCENE_ASM) $(DIALOGUE_ASM) $(TEXT_ASM)

MAKEFLAGS += --no-builtin-rules
