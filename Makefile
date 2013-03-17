# Makefile with basic concepts
 
# Define compiler and options
CC=clang
CFLAGS= -Wall -Wextra -std=c99 -O2 -lSDL -lSDL_image
SRC= basics.c inputs.c main.c
BIN = aerial

all:
	${CC} ${CFLAGS} ${SRC} -o ${BIN}
run:all
	./${BIN}
clean::
	rm -f *~ *.o ${BIN}
 
# END (do not delete)
