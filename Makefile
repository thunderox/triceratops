#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

PLUGIN=thunderox_triceratops 	

include dpf/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

dgl:
ifeq ($(HAVE_OPENGL),true)
	$(MAKE) -C dpf/dgl opengl
endif

plugins: dgl
	$(MAKE) all -C plugins/$(PLUGIN)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(PLUGIN)
	rm -rf bin build
	rm -f delirium_ui/*.d
	rm -f delirium_ui/*.o
	find -name "*~" -delete

# --------------------------------------------------------------
install:
	mkdir ~/.config/thunderox/triceratops -p
	chmod 777 ~/.config/triceratops -R
	cp $(CURDIR)/bin/thunderox_triceratops.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_bass.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_brass.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_electric_piano.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_experimental.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_house_and_rave /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_lead.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_pads.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_synth.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/presets/triceratops_presets_vocal.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/thunderox_triceratops/logo.png /usr/lib/lv2/thunderox_triceratops.lv2
	cp $(CURDIR)/plugins/thunderox_triceratops/manifest.ttl /usr/lib/lv2/thunderox_triceratops.lv2
	cp $(CURDIR)/bin/thunderox_triceratops-dssi /usr/lib/dssi -r
	cp $(CURDIR)/bin/thunderox_triceratops-dssi.so /usr/lib/dssi
	cp $(CURDIR)/bin/thunderox_triceratops-vst.so /usr/lib/vst
	chmod +x $(CURDIR)/bin/thunderox_triceratops
	cp $(CURDIR)/bin/thunderox_triceratops /usr/bin

# --------------------------------------------------------------
uninstall:
	rm ~/.config/thunderox -R
	rm /usr/lib/lv2/thunderox_triceratops.lv2 -rf
	rm /usr/lib/lv2/triceratops_presets_bass -rf
	rm /usr/lib/lv2/triceratops_presets_brass -rf
	rm /usr/lib/lv2/triceratops_presets_electric_piano -rf
	rm /usr/lib/lv2/triceratops_presets_experimental -rf
	rm /usr/lib/lv2/triceratops_presets_house_and_rave-rf
	rm /usr/lib/lv2/triceratops_presets_lead -rf
	rm /usr/lib/lv2/triceratops_presets_pads -rf
	rm /usr/lib/lv2/triceratops_presets_synth -rf	
	rm /usr/lib/lv2/triceratops_presets_vocal -rf	
	rm /usr/lib/dssi/thunderox_triceratops.dssi -rf
	rm /usr/lib/dssi/thunderox_triceratops-dssi.so -f
	rm /usr/lib/vst/thunderox_triceratops-vst.so -f
	rm /usr/bin/thunderox_triceratops


# --------------------------------------------------------------

.PHONY: plugins
