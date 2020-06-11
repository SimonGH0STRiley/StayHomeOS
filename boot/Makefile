ASM			= nasm
ASMBFLAGS	= -I include/
DASMFLAGS	= -D
DESTDIR		= ../build
BINFILE     = boot.bin
COMFILE     = boot.com
LOADERFILE 	= loader.bin

FILES = $(DESTDIR)/$(COMFILE) $(DESTDIR)/$(BINFILE) $(DESTDIR)/$(LOADERFILE)

.PHONY : all clean bootbin bootcom loader

nop: bootbin loader

clean:
	$(RM) $(FILES)

bootbin: $(DESTDIR)/$(BINFILE) $(DESTDIR)/$(LOADERFILE)

bootcom: $(DESTDIR)/$(COMFILE) $(DESTDIR)/$(LOADERFILE)

$(DESTDIR)/$(BINFILE): boot.asm include/load.inc include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(DESTDIR)/$(COMFILE): boot.asm include/load.inc include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -DBOOT_DEBUG -o $@ $<

$(DESTDIR)/$(LOADERFILE): loader.asm include/load.inc include/fat12hdr.inc include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<