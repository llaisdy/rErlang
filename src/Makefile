.SUFFIXES: .erl .beam

EBINDIR :=../ebin

.erl.beam:
	erlc -W -o${EBINDIR} erc.erl eri.erl rErlang.erl

MODS = eri
RM := rm

all:	${MODS:%=%.beam}

clean:
	${RM} -rf ${EBINDIR}/*.beam *~
