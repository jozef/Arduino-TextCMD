all: test

01_basic.t: 01_basic.cpp libtextcmd.so
	g++ -I.. -L. -pthread -ltextcmd 01_basic.cpp -o 01_basic.t

libtextcmd.so: ../TextCMD.cpp ../TextCMD.h
	g++ -fPIC ../TextCMD.cpp -shared -o libtextcmd.so -Wl,--whole-archive -Wl,--no-whole-archive

.PHONY: test clean

test: 01_basic.t
	LD_LIBRARY_PATH=. ./01_basic.t

clean:
	rm -f libtextcmd.so 01_basic.t
