all: build/ts build/tc build/us build/uc

build/ts: tcp_server.c net.h | build
	gcc -o build/ts tcp_server.c

build/tc: tcp_client.c net.h | build
	gcc -o build/tc tcp_client.c

build/us: udp_server.c net.h | build
	gcc -o build/us udp_server.c

build/uc: udp_client.c net.h | build
	gcc -o build/uc udp_client.c

build:
	mkdir -p build

clean:
	rm -r build

.PHONY: all clean
