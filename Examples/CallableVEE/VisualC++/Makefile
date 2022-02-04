
all: CallVEE.exe


CallVEE.exe: CallVEE.cpp
	cl -Zi -W3 -EHsc -D_CRT_SECURE_NO_DEPRECATE CallVEE.cpp /link ole32.lib

clean:
    -@erase CallVEE.obj
    -@erase CallVEE.ilk
    -@erase callserv.tlh
    -@erase callserv.tli
    -@erase *.pdb

