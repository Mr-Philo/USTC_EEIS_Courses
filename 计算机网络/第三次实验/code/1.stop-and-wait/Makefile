all: build/rdt_stopwait_receiver build/rdt_stopwait_sender

build/%: %.c net.c net.h | build
	gcc -Wall -o $@ $< net.c

build:
	mkdir -p build

clean:
	rm -r build

.PHONY: all clean
