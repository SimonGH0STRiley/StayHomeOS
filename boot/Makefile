ASM			= nasm
ASMBFLAGS	= -I include/
DASMFLAGS	= -D
DESTDIR		= ../build/boot
BINFILE     = ../build/boot.bin
COMFILE     = ../build/boot.com
LOADERFILE 	= ../build/loader.bin

FILES = $(COMFILE) $(BINFILE) $(LOADERFILE)

.PHONY : all clean bootbin bootcom loader

nop: bootbin loader

clean:
	$(RM) $(FILES)

bootbin: $(BINFILE) $(LOADERFILE)

bootcom: $(COMFILE) $(LOADERFILE)

$(BINFILE): boot.asm include/load.inc include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(COMFILE): boot.asm include/load.inc include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -DBOOT_DEBUG -o $@ $<

$(LOADERFILE): loader.asm include/load.inc include/fat12hdr.inc include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<