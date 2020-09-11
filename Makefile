# Entry point of kernel
# It must have the same value with 'KernelEntryPointPhyAddr' in load.inc!
export ENTRYPOINT	= 0x1000

FDFILE		= boot.img
BOOTBINFILE = boot.bin
LOADERFILE	= loader.bin
KERNELFILE 	= kernel.bin
LIB		= build/stayhomecrt.a
ASM		= nasm
CC		= gcc
LD		= ld
DESTDIR		= build
KDESTDIR	= build
ASMKFLAGS	= -I include/ -I include/sys/ -f elf
CFLAGS		= -m32 -I include/ -I include/sys/ -c -fno-builtin -fno-stack-protector -g -Wall
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT) -Map build/krnl.map
# kernel.o must be the first one
OBJS		= kernel/kernel.o kernel/start.o kernel/main.o\
			kernel/clock.o kernel/keyboard.o kernel/tty.o kernel/console.o\
			kernel/i8259.o kernel/global.o kernel/protect.o kernel/proc.o\
			kernel/systask.o kernel/hd.o\
			kernel/kliba.o kernel/klib.o\
			lib/syslog.o\
			mm/main.o mm/forkexit.o mm/exec.o\
			fs/main.o fs/open.o fs/misc.o fs/read_write.o\
			fs/link.o\
			fs/disklog.o
DESTOBJS = $(addprefix $(KDESTDIR)/, $(OBJS))

nop: buildimg

clean:
	$(MAKE) -C boot clean
	$(MAKE) -C lib clean
	$(RM) $(DESTDIR)/$(KERNELFILE) $(DESTOBJS) build/krnl.map
	# $(RM) $(DESTDIR)/kernel $(DESTDIR)/fs $(DESTDIR)/mm

$(DESTDIR)/$(KERNELFILE) : $(DESTOBJS) $(LIB)
	$(LD) $(LDFLAGS) -o $(DESTDIR)/$(KERNELFILE) $^

$(KDESTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(KDESTDIR)/%.o: %.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

$(LIB): 
	$(MAKE) -C lib

createdir:
	mkdir -p $(DESTDIR)
	mkdir -p $(DESTDIR)/kernel
	mkdir -p $(DESTDIR)/fs
	mkdir -p $(DESTDIR)/mm

buildimg: createdir bootbin kernel
ifeq (,$(wildcard $(FDFILE)))
	dd if=/dev/zero of=$(DESTDIR)/$(FDFILE) count=1440 bs=1k
endif
	dd if=$(DESTDIR)/$(BOOTBINFILE) of=$(DESTDIR)/$(FDFILE) bs=512 count=1 conv=notrunc
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(LOADERFILE) ::/
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(KERNELFILE) ::/

bootbin:
	$(MAKE) -C boot

kernel: createdir $(DESTDIR)/$(KERNELFILE)
	