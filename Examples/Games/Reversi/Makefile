########################################################################
#
# Makefile for Microsoft Visual C++ 4.x Compiler/Linker
#

TARGET=reversi

all: $(TARGET).dll


$(TARGET).dll: $(TARGET).c
	cl /nologo /O2 /W3 /Zi /DWIN32 $(TARGET).c /LD /link


clean:
	-@erase $(TARGET).obj
	-@erase $(TARGET).exp
	-@erase $(TARGET).lib
	-@erase *.pdb

