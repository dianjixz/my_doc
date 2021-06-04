# Makefile
TARGET = YTC100
 
# CFLAG = -Wall -Werror -std=c99
#CFLAG = -Wall -std=c99 -O2
CFLAG = -Wall -O2
LIB = -pthread 
 
${TARGET}: main.o camera.o queue.o display.o
	gcc main.o camera.o display.o queue.o ${LIB} ${CFLAG} -o ${TARGET}
 
main.o: main.c config.h
	gcc main.c ${CFLAG} -c -o main.o
 
camera.o: camera.c camera.h config.h
	gcc camera.c ${CFLAG} -c -o camera.o
 
# encode.o: encode.c encode.h config.h
#     gcc encode.c ${CFLAG} -c -o encode.o
 
queue.o: queue.c queue.h config.h
	gcc queue.c ${CFLAG} -c -o queue.o
 
display.o: display.c display.h config.h
	gcc display.c ${CFLAG} -c -o display.o
 
.PHONY: clean
 
clean:
	rm -f *.o ${TARGET}
