all: c e

c:
	cd c_src/; make

e:
	cd src/; make

clean:
	cd ./c_src; make clean
	cd ./src; make clean
