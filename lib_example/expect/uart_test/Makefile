VERSION = 1.00
CC   =  /home/nihao/work/my_space/tina-d1-open/prebuilt/gcc/linux-x86/riscv/toolchain-thead-glibc/riscv64-glibc-gcc-thead_20200702/bin/riscv64-unknown-linux-gnu-gcc
# DEBUG   =-DUSE_DEBUG
CFLAGS  =-Wall
SOURCES   =$(wildcard ./*.c)
INCLUDES   =-I./include
# LIB_NAMES  =-lfun_a -lfun_so
# LIB_PATH  =-L./lib
OBJ   =$(patsubst %.c, %.o, $(SOURCES))
TARGET = uart

#links
$(TARGET):$(OBJ)
	@mkdir -p output
	$(CC) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o output/$(TARGET)-$(VERSION)
	@rm -rf $(OBJ)
 
#compile
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) output 