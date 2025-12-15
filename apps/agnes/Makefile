PROJECT=agnes
TOPDIR=../..

HEAP_SIZE=$(shell echo "1024 * 1024 * 1" | bc)
HOST_EXTRA=-e ${HEAP_SIZE} -i 99999999 -W 256 -H 240

all:
	@# zig's objcopy is broken, so use external tool
	@# https://ziggit.dev/t/addobjcopy-producing-zero-padding-at-start-of-binary/13384
	zig build -Dheapsize=${HEAP_SIZE} && ${PREFIX}objcopy zig-out/bin/${PROJECT} -O binary ${PROJECT}.bin

clean: clean_common
	rm -rf zig-out .zig-cache

test: all
	${TOPDIR}/hosts/host-sdl/host-sdl ${HOST_EXTRA} ${PROJECT}.bin

include ${TOPDIR}/apps/common/makefile.common
