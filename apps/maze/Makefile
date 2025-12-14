TOPDIR=../..
PROJECT:=$(shell basename ${PWD})
SRCS=${PROJECT}.c ${TOPDIR}/apps/common/crt0.S
OPT=-O2
all: all_common
test: test_common
clean: clean_common
include ${TOPDIR}/apps/common/makefile.common
