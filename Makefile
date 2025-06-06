#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

PLUGIN=triceratops 	

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
	chmod 777 ~/.config/thunderox/triceratops -R
	cp $(CURDIR)/bin/triceratops.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/triceratops/triceratops_presets.lv2 /usr/lib/lv2 -r

	cp $(CURDIR)/plugins/triceratops/logo.png /usr/lib/lv2/triceratops.lv2
	cp $(CURDIR)/plugins/triceratops/manifest.ttl /usr/lib/lv2/triceratops.lv2
	cp $(CURDIR)/bin/triceratops-dssi /usr/lib/dssi -r
	cp $(CURDIR)/bin/triceratops-dssi.so /usr/lib/dssi
	cp $(CURDIR)/bin/triceratops-vst.so /usr/lib/vst
	chmod +x $(CURDIR)/bin/triceratops
	cp $(CURDIR)/bin/triceratops /usr/bin

# --------------------------------------------------------------
uninstall:
	rm ~/.config/thunderox -R
	rm /usr/lib/lv2/triceratops.lv2 -rf
	rm /usr/lib/lv2/plugins/triceratops/triceratops_presets.lv2 -rf
	rm /usr/lib/dssi/triceratops.dssi -rf
	rm /usr/lib/dssi/triceratops-dssi.so -f
	rm /usr/lib/vst/triceratops-vst.so -f
	rm /usr/bin/triceratops


# --------------------------------------------------------------

.PHONY: plugins
