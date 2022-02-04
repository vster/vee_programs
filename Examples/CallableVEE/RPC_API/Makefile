########################################################################
#
# Makefile for Microsoft Visual C++ 4.x
#

TARGET=callVEE

all: $(TARGET).exe


$(TARGET).exe: $(TARGET).c
	cl /nologo /O2 /W3 /Zi /DWIN32 -D_CRT_SECURE_NO_DEPRECATE $(TARGET).c -I..\..\..\include ..\..\..\lib\libvapi.lib

clean:
	-@erase $(TARGET).obj
	-@erase *.pdb

