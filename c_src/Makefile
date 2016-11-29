BINDIR :=../priv/bin
R_INCLUDE_DIR := /usr/share/R/include
ERL_BASE ?= $(shell erl -noshell -s init stop -eval "io:format(\"~s\", [code:lib_dir(erl_interface)]).")
R_HOME := /usr/lib/R

CSRC=$(shell ls *.c)
HED=$(shell ls *.h)
CC := gcc
RM := rm
VERSION := 0.1
PRODUCT_ERI := ERI-${VERSION}
PRODUCT_RE := rErlang-${VERSION}
NODE_SHELL := *.sh
R_FILE := rErlang.R
CFLAGS := -Wall -I${R_INCLUDE_DIR} -I${ERL_BASE}/include -I${ERL_BASE}
LFLAGS := -L${R_HOME}/lib -L${ERL_BASE}/lib -lR -lerl_interface_st -lei_st -lerl_interface -lei

all:	eri

eri:	${HED} ${CSRC}
	${CC} ${CFLAGS} -o ${BINDIR}/${PRODUCT_ERI} ${HED} ${CSRC} ${LFLAGS}
	${CC} -std=gnu99 ${CFLAGS} -fpic  -g -O2 -c rErlang.c -o ${BINDIR}/${PRODUCT_RE}.o
	${CC} -std=gnu99 -shared  -o ${BINDIR}/${PRODUCT_RE}.so ${BINDIR}/${PRODUCT_RE}.o ${LFLAGS}
	cp ${NODE_SHELL} ${R_FILE} ${BINDIR}

clean:
	${RM} -rf ${BINDIR}/${R_FILE} ${BINDIR}/${PRODUCT_ERI} ${BINDIR}/${PRODUCT_RE} ${BINDIR}/*.sh ${BINDIR}/*.o ${BINDIR}/*.so *~ *# *.o *.so
