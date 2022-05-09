.PHONY: all 
CC ?= gcc
CORSS-CC = /opt/v831/toolchain-sunxi-musl/toolchain/bin/arm-openwrt-linux-gcc


all:ini_test ccmd scan_f pthread_create clean_build

ini_test: main.o ini.o
	$(CC) main.o ini.o -o ini_test

main.o: main.c
	$(CC) -c main.c

ini.o: ini.c
	$(CC) -c ini.c


ccmd: c_syscmd.o
	$(CC) c_syscmd.o -o ccmd

c_syscmd.o:c_syscmd.c
	$(CC) -c c_syscmd.c


scan_f:scan_f.c
	$(CC) scan_f.c -o scan_f


pthread_create: pthread_create.c
	$(CC) pthread_create.c -o pthread_create



clean_build:
	rm ./*.o

clean:
	# make clean_build
	rm ini_test ccmd scan_f pthread_create event

event: event.c
	$(CORSS-CC) event.c -o event
	adb push event /home