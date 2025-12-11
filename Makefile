.PHONY: test

all:
	make -C test
	(cd hosts && make)
	(cd apps && make)

clean:
	make -C test clean
	(cd hosts && make clean)
	(cd apps && make clean)

test:
	make -C test

distrib: all
	cp apps/*/*.bin precompiled/

dockerbuild:
	docker build -t uvm32 . --no-cache

dockershell:
	docker run -v `pwd`:/data -w /data --rm -ti uvm32 /bin/bash

docker:
	docker run -v `pwd`:/data -w /data --rm uvm32 make


