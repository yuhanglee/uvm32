PROJECT:=lissajous

DOCKER_IMAGE=riscv-dev
DOCKER_CMD:=docker run --rm -v ${PWD}../../../:/data -w /data/apps/${PROJECT} ${DOCKER_IMAGE}
PREFIX:=${DOCKER_CMD} riscv64-unknown-elf-
CFLAGS+=-I../../common
CFLAGS+=-fno-stack-protector
CFLAGS+=-static-libgcc -fdata-sections -ffunction-sections
CFLAGS+=-g -O6 -march=rv32ima_zicsr -mabi=ilp32 -static
LDFLAGS:= -T ../linker.ld -nostdlib -Wl,--gc-sections
LIBS:= -lgcc # needed for softfp

SRCS=${PROJECT}.c ../crt0.S

all:
	${PREFIX}gcc -o ${PROJECT}.elf ${CFLAGS} ${LDFLAGS} ${SRCS} ${LIBS}
	$(PREFIX)objcopy ${PROJECT}.elf -O binary ${PROJECT}.bin

disasm: all
	$(PREFIX)objdump -S -d -f ${PROJECT}.elf

test: all
	../../host/host ${PWD}/${PROJECT}.bin

clean:
	rm -f ${PROJECT}.o ${PROJECT}.elf ${PROJECT}.bin

