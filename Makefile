# Global definitions
platform   ?= unix
core_platform ?= $(platform)

CC        = $(CROSS_COMPILE)gcc
SYSROOT   = $(shell $(CC) --print-sysroot)

PROCS     = -j4

OBJS      = libpicofe/input.o libpicofe/in_sdl.o libpicofe/linux/in_evdev.o libpicofe/linux/plat.o libpicofe/fonts.o libpicofe/readpng.o libpicofe/config_file.o cheat.o config.o content.o core.o menu.o main.o options.o overrides.o patch.o scale.o unzip.o util.o

BIN       = picoarch

CFLAGS     += -Wall
CFLAGS     += -fdata-sections -ffunction-sections -DPICO_HOME_DIR='"/.picoarch/"' -flto
CFLAGS     += -I./ -I./libretro-common/include/ $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)

LDFLAGS    = -lc -ldl -lgcc -lm -lSDL -lasound -lpng -lz -Wl,--gc-sections -flto

# Unpolished or slow cores that build
# EXTRA_CORES += fbalpha2012
# EXTRA_CORES += mame2003_plus

CORES = beetle-pce-fast bluemsx fceumm fmsx gambatte gme gpsp mame2000 pcsx_rearmed picodrive quicknes smsplus-gx snes9x2002 snes9x2005 $(EXTRA_CORES)

ifeq ($(platform), funkey-s)
CORES := $(CORES) snes9x2005_plus
endif

beetle-pce-fast_REPO = https://github.com/libretro/beetle-pce-fast-libretro
beetle-pce-fast_CORE = mednafen_pce_fast_libretro.so
beetle-pce-fast_TYPES = pce,cue,ccd,chd,toc,m3u

bluemsx_REPO = https://github.com/libretro/blueMSX-libretro
bluemsx_TYPES = rom,ri,mx1,mx2,dsk,col,sg,sc,cas,m3u

fbalpha2012_BUILD_PATH = fbalpha2012/svn-current/trunk
fbalpha2012_MAKEFILE = makefile.libretro
fbalpha2012_TYPES = zip

fceumm_REPO = https://github.com/libretro/libretro-fceumm
fceumm_MAKEFILE = Makefile.libretro
fceumm_TYPES = fds,nes,unf,unif

fmsx_REPO = https://github.com/libretro/fmsx-libretro
fmsx_TYPES = rom,mx1,mx2,dsk,cas

gambatte_REPO = https://github.com/libretro/gambatte-libretro
gambatte_TYPES = gb,gbc,dmg,zip

gme_REPO = https://github.com/libretro/libretro-gme

gpsp_TYPES = gba,bin,zip

mame2000_REPO = https://github.com/libretro/mame2000-libretro
mame2003_plus_REPO = https://github.com/libretro/mame2003-plus-libretro
mame2000_TYPES = zip

mame2003_plus_TYPES = zip

pcsx_rearmed_MAKEFILE = Makefile.libretro
pcsx_rearmed_TYPES = bin,cue,img,mdf,pbp,toc,cbn,m3u,chd

picodrive_MAKEFILE = Makefile.libretro
picodrive_TYPES = bin,gen,smd,md,32x,cue,iso,chd,sms,gg,m3u,68k,sgd

quicknes_REPO = https://github.com/libretro/QuickNES_Core
quicknes_TYPES = nes

smsplus-gx_MAKEFILE = Makefile.libretro
smsplus-gx_CORE = smsplus_libretro.so
smsplus_TYPES = sms,bin,rom,col,gg,sg

snes9x2002_TYPES = smc,fig,sfc,gd3,gd7,dx2,bsx,swc,zip

snes9x2005_REPO = https://git.crowdedwood.com/snes9x2005
snes9x2005_REVISION = performance
snes9x2005_TYPES = smc,fig,sfc,gd3,gd7,dx2,bsx,swc,zip

snes9x2005_plus_REPO = https://git.crowdedwood.com/snes9x2005
snes9x2005_plus_REVISION = performance
snes9x2005_plus_FLAGS = USE_BLARGG_APU=1
snes9x2005_plus_TYPES = smc,fig,sfc,gd3,gd7,dx2,bsx,swc,zip

ifeq ($(platform), trimui)
	OBJS += plat_trimui.o
	CFLAGS += -mcpu=arm926ej-s -mtune=arm926ej-s -fno-PIC -DCONTENT_DIR='"/mnt/SDCARD/Roms"'
	LDFLAGS += -fno-PIC
else ifeq ($(platform), funkey-s)
	OBJS += plat_funkey.o funkey/fk_menu.o funkey/fk_instant_play.o
	CFLAGS += -DCONTENT_DIR='"/mnt"' -DFUNKEY_S
  LDFLAGS += -fPIC
	LDFLAGS += -lSDL_image -lSDL_ttf # For fk_menu
  core_platform = classic_armv7_a7
else ifeq ($(platform), unix)
	OBJS += plat_linux.o
	LDFLAGS += -fPIE
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -Og -g
	LDFLAGS += -g
else
	CFLAGS += -Ofast -DNDEBUG

ifneq ($(PROFILE), 1)
	LDFLAGS += -s
endif

endif

ifeq ($(PROFILE), 1)
	CFLAGS += -fno-omit-frame-pointer -pg -g
	LDFLAGS += -pg -g
else ifeq ($(PROFILE), GENERATE)
	CFLAGS	+= -fprofile-generate=./profile/picoarch
	LDFLAGS	+= -lgcov
else ifeq ($(PROFILE), APPLY)
	CFLAGS	+= -fprofile-use -fprofile-dir=./profile/picoarch -fbranch-probabilities
endif

ifeq ($(MINUI), 1)
	MMENU = 1
	CFLAGS += -DMINUI
endif

ifeq ($(MMENU), 1)
	CFLAGS += -DMMENU
	LDFLAGS += -lSDL_image -lSDL_ttf -ldl
endif

CFLAGS += $(EXTRA_CFLAGS)

SOFILES = $(foreach core,$(CORES),$(core)_libretro.so)

print-%:
	@echo '$*=$($*)'

all: $(BIN) cores

libpicofe/.patched:
	cd libpicofe && git apply -p1 < ../patches/libpicofe/0001-key-combos.patch && git apply -p1 < ../patches/libpicofe/0002-small-screen.patch && touch .patched

clean-libpicofe:
	test ! -f libpicofe/.patched || (cd libpicofe && git apply -p1 -R < ../patches/libpicofe/0002-small-screen.patch && git apply -p1 -R < ../patches/libpicofe/0001-key-combos.patch && rm .patched)

plat_trimui.o: plat_sdl.c
plat_funkey.o: plat_sdl.c
plat_linux.o: plat_sdl.c

$(BIN): libpicofe/.patched $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BIN)

define CORE_template =

$1_REPO ?= https://github.com/libretro/$(1)/

$1_BUILD_PATH ?= $(1)

$1_MAKE = make $(and $($1_MAKEFILE),-f $($1_MAKEFILE)) platform=$(core_platform) $(and $(DEBUG),DEBUG=$(DEBUG)) $(and $(PROFILE),PROFILE=$(PROFILE)) $($(1)_FLAGS)

$(1):
	git clone $(if $($1_REVISION),,--depth 1) --recursive $$($(1)_REPO) $(1)
	$(if $1_REVISION,cd $(1) && git checkout $($1_REVISION),)
	(test ! -d patches/$(1)) || (cd $(1) && $(foreach patch, $(sort $(wildcard patches/$(1)/*.patch)), git apply -p1 < ../$(patch) &&) true)

$(1)/$(1)_libretro.so: $(1)
	cd $$($1_BUILD_PATH) && $$($1_MAKE) $(PROCS)

$(1)_libretro.so: $(1)/$(1)_libretro.so
	cp $$($1_BUILD_PATH)/$(if $($(1)_CORE),$($(1)_CORE),$(1)_libretro.so) $(1)_libretro.so

clean-$(1):
	test ! -d $(1) || cd $$($1_BUILD_PATH) && $$($1_MAKE) clean
	rm -f $(1)_libretro.so
endef

$(foreach core,$(CORES),$(eval $(call CORE_template,$(core))))

cores: $(SOFILES)

clean: clean-libpicofe
	rm -f $(OBJS) $(BIN) $(SOFILES)
	rm -rf pkg

clean-all: $(foreach core,$(CORES),clean-$(core)) clean

force-clean: clean
	rm -rf $(CORES)

ifeq ($(platform), trimui)

beetle-pce-fast_NAME = pce_fast
beetle-pce-fast_ROM_DIR = PCE
beetle-pce-fast_PAK_NAME = TurboGrafx-16

bluemsx_NAME = blueMSX
bluemsx_ROM_DIR = MSX
bluemsx_PAK_NAME = MSX (blueMSX)

fbalpha2012_NAME = fba2012
fbalpha2012_ROM_DIR = ARCADE
fbalpha2012_PAK_NAME = Arcade (FBA)

fceumm_ROM_DIR = FC
fceumm_PAK_NAME = Nintendo (fceumm)

fmsx_NAME = fMSX
fmsx_ROM_DIR = MSX
fmsx_PAK_NAME = MSX

gambatte_ROM_DIR = GB
gambatte_PAK_NAME = Game Boy

gpsp_ROM_DIR = GBA
gpsp_PAK_NAME = Game Boy Advance
define gpsp_PAK_EXTRA

needs-swap

endef

gme_ROM_DIR = MUSIC
gme_PAK_NAME = Game Music

mame2000_ROM_DIR = ARCADE
mame2000_PAK_NAME = Arcade

mame2003_plus_NAME = mame2003+
mame2003_plus_ROM_DIR = ARCADE
mame2003_plus_PAK_NAME = Arcade (MAME 2003-plus)

picodrive_ROM_DIR = MD
picodrive_PAK_NAME = Genesis

pcsx_rearmed_ROM_DIR = PS
pcsx_rearmed_PAK_NAME = PlayStation
define pcsx_rearmed_PAK_EXTRA

needs-swap

endef

quicknes_ROM_DIR = FC
quicknes_PAK_NAME = Nintendo

smsplus_ROM_DIR = MS
smsplus_PAK_NAME = Game Gear

snes9x2002_ROM_DIR = SFC
snes9x2002_PAK_NAME = Super Nintendo

snes9x2005_ROM_DIR = SFC
snes9x2005_PAK_NAME = Super Nintendo (2005)

# -- gmenunx

dist-gmenu-section:
	mkdir -p pkg/gmenunx/Apps/picoarch
	mkdir -p pkg/gmenunx/Apps/gmenunx/sections/emulators
	mkdir -p pkg/gmenunx/Apps/gmenunx/sections/libretro
	touch pkg/gmenunx/Apps/gmenunx/sections/libretro/.section

dist-gmenu-picoarch: $(BIN) dist-gmenu-section
	cp $(BIN) pkg/gmenunx/Apps/picoarch
	$(file >pkg/gmenunx/Apps/gmenunx/sections/emulators/picoarch,$(picoarch_SHORTCUT))

define CORE_gmenushortcut =

$1_NAME ?= $1

define $1_SHORTCUT
title=$$($1_NAME)
exec=/mnt/SDCARD/Apps/picoarch/picoarch
params=/mnt/SDCARD/Apps/picoarch/$1_libretro.so
selectordir=/mnt/SDCARD/Roms/$($1_ROM_DIR)
selectorfilter=$($1_TYPES)
endef

dist-gmenu-$(1): $(BIN) $(1)_libretro.so dist-gmenu-picoarch dist-gmenu-section
	cp $1_libretro.so pkg/gmenunx/Apps/picoarch
	$$(file >pkg/gmenunx/Apps/gmenunx/sections/libretro/$(1),$$($(1)_SHORTCUT))

endef

$(foreach core, $(CORES),$(eval $(call CORE_gmenushortcut,$(core))))

define picoarch_SHORTCUT
title=$(BIN)
exec=/mnt/SDCARD/Apps/picoarch/picoarch
endef

dist-gmenu: $(foreach core, $(CORES), dist-gmenu-$(core)) dist-gmenu-picoarch
	cp README.trimui.md pkg/

# -- MinUI

ifeq ($(MINUI), 1)
define CORE_pak_template =

define $1_LAUNCH_SH
#!/bin/sh
# $($1_PAK_NAME).pak/launch.sh

EMU_EXE=picoarch
EMU_DIR=$$$$(dirname "$$$$0")
ROM_DIR=$$$${EMU_DIR/.pak/}
ROM_DIR=$$$${ROM_DIR/Emus/Roms}
EMU_NAME=$$$${ROM_DIR/\/mnt\/SDCARD\/Roms\//}
ROM=$$$${1}
$($1_PAK_EXTRA)
HOME="$$$$ROM_DIR"
cd "$$$$EMU_DIR"
"$$$$EMU_DIR/$$$$EMU_EXE" ./$1_libretro.so "$$$$ROM" &> "/mnt/SDCARD/.minui/logs/$$$$EMU_NAME.txt"
endef

dist-minui-$(1): $(BIN) $(1)_libretro.so
	mkdir -p "pkg/MinUI/Emus/$($1_PAK_NAME).pak"
	$$(file >$1_launch.sh,$$($1_LAUNCH_SH))
	mv $1_launch.sh "pkg/MinUI/Emus/$($1_PAK_NAME).pak/launch.sh"
	cp $(BIN) $1_libretro.so "pkg/MinUI/Emus/$($1_PAK_NAME).pak"

endef

define picoarch_LAUNCH_SH
#!/bin/sh
# picoarch.pak/launch.sh

EMU_EXE=picoarch
EMU_DIR=$$(dirname "$$0")
EMU_NAME=$$EMU_EXE

needs-swap

HOME="/mnt/SDCARD/Games/picoarch.pak/"
cd "$$EMU_DIR"
"$$EMU_DIR/$$EMU_EXE" &> "/mnt/SDCARD/.minui/logs/$$EMU_NAME.txt"
endef

dist-minui-picoarch: $(BIN) cores
	mkdir -p "pkg/MinUI/Games/picoarch.pak"
	$(file >picoarch_launch.sh,$(picoarch_LAUNCH_SH))
	mv picoarch_launch.sh "pkg/MinUI/Games/picoarch.pak/launch.sh"
	cp $(BIN) $(SOFILES) "pkg/MinUI/Games/picoarch.pak"

$(foreach core, $(CORES),$(eval $(call CORE_pak_template,$(core))))

dist-minui: $(foreach core, $(CORES), dist-minui-$(core)) dist-minui-picoarch
	cp README.trimui.md pkg/

endif # MINUI=1

picoarch.zip:
	make platform=trimui PROFILE=APPLY clean-all dist-gmenu
	rm -f $(OBJS) $(BIN)
	make platform=trimui PROFILE=APPLY EXTRA_CFLAGS=-Wno-error=coverage-mismatch MINUI=1 dist-minui
	cd pkg && zip -r ../picoarch.zip *

endif # platform=trimui

ifeq ($(platform), funkey-s)

beetle-pce-fast_NAME = pce_fast
beetle-pce-fast_ROM_DIR = /mnt/PCE-TurboGrafx

bluemsx_NAME = blueMSX
bluemsx_ROM_DIR = /mnt/MSX

fbalpha2012_NAME = fba2012
fbalpha2012_ROM_DIR = /mnt/Arcade

fceumm_ROM_DIR = /mnt/NES

fmsx_NAME = fMSX
fmsx_ROM_DIR = /mnt/MSX

gambatte_ROM_DIR = /mnt/Game Boy

gme_ROM_DIR = /mnt/Music
gme_TYPES = ay,gbs,gym,hes,kss,nsf,nsfe,sap,spc,vgm,vgz,zip

gpsp_ROM_DIR = /mnt/Game Boy Advance

mame2000_ROM_DIR = /mnt/Arcade

mame2003_plus_NAME = mame2003+
mame2003_plus_ROM_DIR = /mnt/Arcade

pcsx_rearmed_ROM_DIR = /mnt/PS1

picodrive_ROM_DIR = /mnt/Sega Genesis

quicknes_ROM_DIR = /mnt/NES

smsplus_ROM_DIR = /mnt/Game Gear

snes9x2002_ROM_DIR = /mnt/SNES

snes9x2005_ROM_DIR = /mnt/SNES

snes9x2005_plus_NAME = snes9x2005+
snes9x2005_plus_ROM_DIR = /mnt/SNES

define CORE_opk =

$1_NAME ?= $1

define $1_DESKTOP
[Desktop Entry]
Name=$$($1_NAME)
Comment=
Exec=picoarch ./$1_libretro.so %f
Icon=$$($1_ICON)
SelectorBrowser=true
SelectorDir=$($1_ROM_DIR)
SelectorFilter=$($1_TYPES)
Terminal=false
Type=Application
StartupNotify=true
Categories=emulators;
endef

picoarch-$(1).opk: $(BIN) $(1)_libretro.so
	mkdir -p .opkdata
	cp $1_libretro.so .opkdata
	$$(file >$$($(1)_NAME).funkey-s.desktop,$$($(1)_DESKTOP))
	mv $$($(1)_NAME).funkey-s.desktop .opkdata
	cp $(BIN) $(1)_libretro.so .opkdata
	cd .opkdata && mksquashfs * ../picoarch-$(1).opk -all-root -no-xattrs -noappend -no-exports
	rm -r .opkdata

endef

$(foreach core, $(CORES),$(eval $(call CORE_opk,$(core))))

define picoarch_DESKTOP
[Desktop Entry]
Name=picoarch
Comment=Small screen libretro frontend
Exec=picoarch
Icon=
Terminal=false
Type=Application
StartupNotify=true
Categories=emulators;
endef

picoarch.opk: $(BIN) cores
	mkdir -p .opkdata
	$(file >picoarch.funkey-s.desktop,$(picoarch_DESKTOP))
	mv picoarch.funkey-s.desktop .opkdata
	cp $(BIN) $(SOFILES) .opkdata
	cd .opkdata && mksquashfs * ../picoarch.opk -all-root -no-xattrs -noappend -no-exports
	rm -r .opkdata

picoarch-funkey-s.zip: picoarch.opk $(foreach core, $(CORES), picoarch-$(core).opk)
	rm -f picoarch-funkey-s.zip
	zip picoarch-funkey-s.zip README.funkey-s.md *.opk

endif # platform=funkey-s
