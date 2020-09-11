ASM		= nasm
CC		= gcc
LD		= ld
DESTDIR		= ../build/lib
ASMKFLAGS	= -I ../include/ -I ../include/sys/ -f elf
CFLAGS		= -m32 -I ../include/ -I ../include/sys/ -c -fno-builtin -fno-stack-protector -g -Wall
ARFLAGS		= rcs

LOBJS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.asm, %.o, $(wildcard *.asm))
DESTLOBJS = $(addprefix $(DESTDIR)/, $(LOBJS))
LIB		= ../build/stayhomecrt.a

nop: createdir $(LIB)

clean:
	$(RM) $(LIB) $(DESTLOBJS)

$(LIB) : $(DESTLOBJS)
	echo $^
	$(AR) $(ARFLAGS) $@ $^

$(DESTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(DESTDIR)/%.o: %.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

createdir:
ifeq (,$(wildcard $(DESTDIR)))
	mkdir -p $(DESTDIR)
endif