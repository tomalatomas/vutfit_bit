BASE = ../../
FCMAKE = c:/Program Files/FITkit/bin/fcmake.exe
FKFLASH = c:/Program Files/FITkit/bin/fkflash.exe
FKTERM = c:/Program Files/FITkit/bin/fkterm.exe
FKTEST = c:/Program Files/FITkit/bin/fktest.exe
PROJECT = project.xml
OUTPUTPREFIX = vigenere
FPGACHIP = xc3s50
FPGASPEEDGRADE = 4
FPGAPACKAGE = pq208

all: dependencycheck build/vigenere_f1xx.hex build/vigenere_f2xx.hex build/vigenere.bin

#MCU part
#=====================================================================
HEXFILE_F1XX = build/vigenere_f1xx.hex
HEXFILE_F2XX = build/vigenere_f2xx.hex

build/mcu/main_f1xx.o: mcu/main.c
	$(comp_tpl_f1xx)

build/mcu/main_f2xx.o: mcu/main.c
	$(comp_tpl_f2xx)

OBJFILES_F1XX = build/mcu/main_f1xx.o
OBJFILES_F2XX = build/mcu/main_f2xx.o

#FPGA part
#=====================================================================
FPGAUCF = fpga/vigenere.ucf
BINFILE = build/vigenere.bin
HDLFILES  = fpga/vigenere.vhd

build/vigenere.bin: build/fpga/vigenere.par.ncd build/fpga/vigenere.pcf

include $(BASE)/base/Makefile.inc
