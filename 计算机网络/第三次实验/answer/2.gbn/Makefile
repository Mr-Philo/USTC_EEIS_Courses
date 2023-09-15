all: build/rdt_gbn_receiver build/rdt_gbn_sender

build/%: %.c net.c net.h | build
	gcc -Wall -pthread -o $@ $< net.c

build:
	mkdir -p build

clean:
	rm -r build

.PHONY: all clean