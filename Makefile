# Entry point of kernel
# It must have the same value with 'KernelEntryPointPhyAddr' in load.inc!
export ENTRYPOINT	= 0x30400

DESTDIR		= build
FDFILE		= boot.img
FDDEBUG		= debug.img
BOOTBINFILE = boot.bin
BOOTCOMFILE = boot.com
LOADERFILE	= loader.bin
KERNELFILE 	= kernel.bin

.PHONY : all clean buildimg debugimg createdir bootbin bootcom kernel

nop: buildimg

all: createdir
	$(MAKE) -C boot all

clean:
	$(MAKE) -C boot clean
	$(MAKE) -C kernel clean
	$(RM) $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(FDDEBUG)

buildimg: bootbin kernel createdir
ifeq (,$(wildcard $(FDFILE)))
	dd if=/dev/zero of=$(DESTDIR)/$(FDFILE) count=1440 bs=1k
endif
	dd if=$(DESTDIR)/$(BOOTBINFILE) of=$(DESTDIR)/$(FDFILE) bs=512 count=1 conv=notrunc
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(LOADERFILE) ::/
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(KERNELFILE) ::/

debugimg: bootcom kernel createdir
ifeq (,$(wildcard $(FDDEBUG)))
	dd if=/dev/zero of=$(DESTDIR)/$(FDDEBUG) count=1440 bs=1k
	/sbin/mkfs.msdos $(DESTDIR)/$(FDDEBUG)
endif
	mcopy -D o -i $(DESTDIR)/$(FDDEBUG) $(DESTDIR)/$(BOOTCOMFILE) ::/
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(LOADERFILE) ::/
	mcopy -D o -i $(DESTDIR)/$(FDFILE) $(DESTDIR)/$(KERNELFILE) ::/

bootbin:
	$(MAKE) -C boot

bootcom:
	$(MAKE) -C boot bootcom

kernel:
	$(MAKE) -C kernel

createdir:
ifeq (,$(wildcard $(DESTDIR)))
	mkdir -p $(DESTDIR)
endif