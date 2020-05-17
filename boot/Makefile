ASM			= nasm
ASMBFLAGS	= 
DASMFLAGS	= -D
DESTDIR		= ../build
FDFILE		= boot.img
FDDEBUG		= debug.img
BINFILE     = boot.bin
COMFILE     = boot.com

FILES = $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(FDDEBUG) $(DESTDIR)/$(COMFILE) $(DESTDIR)/$(BINFILE)

.PHONY : all clean buildimg debugcom debugimg

nop: buildimg

all: buildimg debugimg

clean:
	rm -f $(FILES)

buildimg: $(DESTDIR)/$(BINFILE)
	dd if=$(DESTDIR)/$(BINFILE) of=$(DESTDIR)/$(FDFILE) bs=512 count=1 conv=notrunc

debugcom: $(DESTDIR)/$(COMFILE)

debugimg: $(DESTDIR)/$(COMFILE)
ifeq (,$(wildcard $(FDDEBUG)))
	dd if=/dev/zero of=$(DESTDIR)/$(FDDEBUG) count=1440 bs=1k
	/sbin/mkfs.msdos $(DESTDIR)/$(FDDEBUG)
endif
	mcopy -D o -i $(DESTDIR)/$(FDDEBUG) $(DESTDIR)/$(COMFILE) ::/

$(DESTDIR)/$(BINFILE): boot.asm
ifeq (,$(wildcard $(DESTDIR)))
	mkdir -p $(DESTDIR)
endif
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(DESTDIR)/$(COMFILE): boot.asm
ifeq (,$(wildcard $(DESTDIR)))
	mkdir -p $(DESTDIR)
endif
	$(ASM) $(ASMBFLAGS) -DBOOT_DEBUG -o $@ $<

