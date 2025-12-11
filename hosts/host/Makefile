TOPDIR=../../

all:
	gcc -Wall -Werror -pedantic -std=c99 -O2 -DUVM32_MEMORY_SIZE=65536 -I${TOPDIR}/uvm32 -I${TOPDIR}/common -o host ${TOPDIR}/uvm32/uvm32.c host.c

clean:
	rm -f host
