.PHONY : all clean buildimg debugcom debugimg

nop: buildimg

all:
	$(MAKE) -C boot all

clean:
	$(MAKE) -C boot clean

buildimg:
	$(MAKE) -C boot buildimg

debugcom:
	$(MAKE) -C boot debugcom

debugimg:
	$(MAKE) -C boot debugimg
