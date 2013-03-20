# Makefile with basic concepts
 
# Define compiler and options
CC=clang
CFLAGS= -Wall -Wextra -std=c99 -O2 -lSDL -lSDL_image
DIR=src/
SRC= ${DIR}basics.c ${DIR}inputs.c ${DIR}main.c
BIN = aerial

all:
	${CC} ${SRC} -o ${BIN} ${CFLAGS}
run:all
	./${BIN}
clean::
	rm -f *~ *.o ${BIN}
	@cd ${DIR}
	@rm -f *~ *.o
 
# END (do not delete)
