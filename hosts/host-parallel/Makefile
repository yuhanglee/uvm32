TOPDIR = ../../
all:
	xxd -n fib -i ${TOPDIR}/precompiled/fib.bin | sed -e "s/unsigned char/const unsigned char/" > fib.h
	gcc -Wall -DUVM32_MEMORY_SIZE=16386 -I${TOPDIR}/uvm32 -I${TOPDIR}/common -o host-parallel ${TOPDIR}/uvm32/uvm32.c host-parallel.c

clean:
	rm -f host-parallel
