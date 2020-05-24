DESTDIR	= build

.PHONY : all clean buildimg debugcom debugimg createdir

nop: buildimg

all: createdir
	$(MAKE) -C boot all

clean:
	$(MAKE) -C boot clean

buildimg: createdir
	$(MAKE) -C boot buildimg

debugcom: createdir
	$(MAKE) -C boot debugcom

debugimg: createdir
	$(MAKE) -C boot debugimg

createdir:
ifeq (,$(wildcard $(DESTDIR)))
	mkdir -p $(DESTDIR)
endif