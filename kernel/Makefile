ASM			= nasm
CC			= gcc
LD			= ld
CFLAGS		= -m32 -I ../include/ -c -fno-builtin -fno-stack-protector
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
ASMFLAGS	= -I ../include/ -f elf
DESTDIR		= ../build
KERNELFILE	= kernel.bin
OBJS		= $(DESTDIR)/kernel.o $(DESTDIR)/start.o $(DESTDIR)/i8259.o $(DESTDIR)/global.o $(DESTDIR)/protect.o $(DESTDIR)/klib.o $(DESTDIR)/kliba.o $(DESTDIR)/string.o

$(DESTDIR)/$(KERNELFILE) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(DESTDIR)/$(KERNELFILE) $(OBJS)

.PHONY: clean

clean:
	$(RM) $(DESTDIR)/$(KERNELFILE) $(OBJS)

$(DESTDIR)/kernel.o : kernel.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(DESTDIR)/start.o: start.c
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/i8259.o : i8259.c 
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/global.o : global.c
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/protect.o : protect.c
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/klib.o : ../lib/klib.c
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/kliba.o : ../lib/kliba.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(DESTDIR)/string.o : ../lib/string.asm
	$(ASM) $(ASMFLAGS) -o $@ $<