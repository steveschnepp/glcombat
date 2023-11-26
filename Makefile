SUBDIRS := gl
TOPTARGETS := all clean cleaner

all: $(SUBDIRS) $(EXES)

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)

cleaner: $(SUBDIRS) clean

.PHONY: all clean cleaner

.PHONY: $(TOPTARGETS) $(SUBDIRS)
