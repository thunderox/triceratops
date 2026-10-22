#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

PLUGIN=triceratops 	

include DPF/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

dgl:
ifeq ($(HAVE_OPENGL),true)
	$(MAKE) -C DPF/dgl opengl
endif

plugins: dgl
	$(MAKE) all -C plugins/$(PLUGIN)

ifneq ($(CROSS_COMPILING),true)
gen: plugins DPF/utils/lv2_ttl_generator
	@$(CURDIR)/DPF/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/DPF/utils/generate-vst-bundles.sh
endif

DPF/utils/lv2_ttl_generator:
	$(MAKE) -C DPF/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C DPF/dgl
	$(MAKE) clean -C DPF/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(PLUGIN)
	rm -rf bin build
	rm -f delirium_ui/*.d
	rm -f delirium_ui/*.o
	find -name "*~" -delete

# --------------------------------------------------------------
install:
	mkdir ~/.config/thunderox/triceratops -p
	chmod 777 ~/.config/thunderox/triceratops -R
	cp $(CURDIR)/bin/triceratops.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/triceratops/triceratops_presets.lv2 /usr/lib/lv2 -r

	cp $(CURDIR)/plugins/triceratops/logo.png /usr/lib/lv2/triceratops.lv2
	cp $(CURDIR)/plugins/triceratops/manifest.ttl /usr/lib/lv2/triceratops.lv2
	cp $(CURDIR)/bin/triceratops-dssi /usr/lib/dssi -r
	cp $(CURDIR)/bin/triceratops-dssi.so /usr/lib/dssi
	cp $(CURDIR)/bin/triceratops-vst.so /usr/lib/vst
	cp $(CURDIR)/bin/triceratops.clap /usr/lib/clap
	cp $(CURDIR)/bin/triceratops.vst3 /usr/lib/vst3 -r
	chmod +x $(CURDIR)/bin/triceratops
	cp $(CURDIR)/bin/triceratops /usr/bin

# --------------------------------------------------------------
uninstall:
	rm ~/.config/thunderox -R
	rm /usr/lib/lv2/triceratops.lv2 -rf
	rm /usr/lib/lv2/triceratops/triceratops_presets.lv2 -rf
	rm /usr/lib/dssi/triceratops.dssi -rf
	rm /usr/lib/dssi/triceratops-dssi.so -f
	rm /usr/lib/clap/triceratops.clap -f
	rm /usr/lib/vst/triceratops-vst.so -f
	rm /usr/lib/vst3/triceratops.vst3 -rf
	rm /usr/bin/triceratops


# --------------------------------------------------------------

.PHONY: plugins
