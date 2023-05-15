CC=gcc
CFLAGS=-Wall -Werror
LFLAGS=-lbluetooth
OUTDIR=bin/blesca

.PHONY: build
build:
	${CC} ${CFLAGS} src/main.c ${LFLAGS} -o ${OUTDIR}

clean:
	rm -r ./build/*
	rm ./bin/*
