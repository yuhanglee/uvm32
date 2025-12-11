TOPDIR=../../

all:
	xxd -n mandel -i ${TOPDIR}/precompiled/mandel.bin | sed -e "s/unsigned char/const unsigned char/" > mandel.h
	gcc -Wall -DUVM32_MEMORY_SIZE=512 -O2 -I${TOPDIR}/uvm32 -I${TOPDIR}/common -o host-mini ${TOPDIR}/uvm32/uvm32.c host-mini.c

clean:
	rm -f host-mini
