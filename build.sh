# This script will build most of the source in this repository.
# The directory "build" will contain the LST and OBJ files for each project.

KERNALEMU=kernalemu
CBM6502ASM=asm
ASSEMBLER64=assembler64.prg

test_tools()
{
	if ! command -v $KERNALEMU &> /dev/null
	then
	    echo "Please get https://github.com/mist64/kernalemu and install it in your PATH."
	    exit
	fi

	if ! command -v $CBM6502ASM &> /dev/null
	then
	    echo "Please get https://github.com/mist64/cbm6502asm and install it in your PATH."
	    exit
	fi

	if [ ! -f $ASSEMBLER64 ]; then
	    echo "Please get assembler64.prg from https://github.com/mist64/kernalemu/tree/main/demo."
	    exit
	fi
}

build1()
{
	DIR=$1
	SRC=$2
	mkdir build/$DIR
	for i in $DIR/*; do cat $i | tr '\n' '\r' | tr '\t' ' ' > build/$i; done
	cd build/$DIR
	for A in *; do if [ ! -e ${A^^} ]; then ln -s $A ${A^^}; fi; done
	echo -e "_tmp_obj\n\n\n$SRC" | $KERNALEMU $ASSEMBLER64
	mv printer4.txt ../$1.lst
	tr '\r' '\n' < _tmp_obj > ../$1.obj
	cd ../..
	rm -rf build/$DIR
}

build2()
{
	DIR=$1
	SRC=$2
	cp -pr $DIR build/
	cd build/$DIR
	for A in *; do if [ ! -e ${A^^} ]; then ln -s $A ${A^^}; fi; done
	$CBM6502ASM _tmp_obj,_tmp_lst=$SRC
	mv _tmp_lst.lst ../$1.lst
	mv _tmp_obj.obj ../$1.obj
	cd ../..
	rm -rf build/$DIR
}

test_tools

rm -rf build
mkdir build

build1 MONITOR_KIM kim.asm
build1 MONITOR_AIM65 pa00-j001a
build1 TIM tim.asm
build1 KIMATH kimath.asm

build1 KERNAL_PET_1.0_REC ossrc
build1 KERNAL_PET_2.0_REC ossrc
build1 KERNAL_PET_4.0_1979-10-23 ossrc
build1 KERNAL_PET_4.0_REC ossrc

build1 KERNAL_VIC_04 kernal

build1 KERNAL_MAX_REC kernal
build1 KERNAL_C64_01 kernal
build1 KERNAL_C64_02 kernal
build1 KERNAL_C64_03 kernal
build2 KERNAL_C64_03_BSO kernal.src
build2 KERNAL_C64GS console.src
build2 KERNAL_C64_C65 kernel64.src

build2 KERNAL_TED_04 kernal.src
build2 KERNAL_TED_05 kernal.src

build2 KERNAL_C128_03 kernal.src
build2 KERNAL_C128_05 kernal.src
build2 KERNAL_C128_06 kernal.src

build2 KERNAL_C65 system.src

build1 KERNAL_CBM2_1982-11-19 newkernal
build1 KERNAL_CBM2_1983-05-31 kernal
build1 KERNAL_CBM2_1983-07-07 kernal

build2 EDITOR_C128 editor.src
build2 EDITOR_C128_DIN editor.src
build2 EDITOR_C128_NATIONAL german.src

build1 EDITOR_CBM2_1982-11-19 sced.lib
build1 EDITOR_CBM2_1983-05-31 sced.lib

build1 EDITOR_PET_EURO edtlist

build1 BASIC_PET_V1_REC bassrc
build1 BASIC_PET_V1R_REC bassrc
build1 BASIC_PET_V2_REC bassrc
build1 BASIC_PET_V4_1979-07-23 bassrc
build1 BASIC_PET_V4_REC bassrc
build1 BASIC_PET_V4R_REC bassrc

build1 BASIC_VIC basic

build1 BASIC_MAX_REC basic
build1 BASIC_MAX_MINI_REC basic
build1 BASIC_C64 basic
build2 BASIC_C64_BSO basic.src
build2 BASIC_C64GS basic.src
build2 BASIC_C64_C65 basic64.src

build2 BASIC_TED basic.src

build2 BASIC_C128_03 basic.src
build2 BASIC_C128_04 basic.src

build2 BASIC_C65 b65.src
build2 GRAPHICS_C65 graphics.src

build1 BASIC_CBM2_1982-11-19 bas256 # or bas64 bas128 bas192 bas256
build1 BASIC_CBM2_1983-05-31 cbm256 # or cbm128
build1 BASIC_CBM2_1984-01-04 cbm256 # cbm128 cbm192 cbm256 cbm64 pet128 pet192 pet256 pet64

build2 CHARSET_TED charrom.src
build2 CHARSET_C128_NATIONAL german.src

build1 MONITOR_PET_ADDON add-on-mon
build1 MONITOR_C64 micii4
build2 MONITOR_C128 monitor.src

build1 CONTROLLER_8250 lib
build1 CONTROLLER_1001 single.src

build1 DOS_4040 master
build1 DOS_1540 ser.lib
build1 DOS_1541_01_REC ser.lib
build1 DOS_1541_02_REC ser.lib
build1 DOS_1541_03_REC ser.lib
build1 DOS_1541_05_REC ser.lib
build1 DOS_1541_06_REC ser.lib
build1 DOS_1541C_01_REC ser.lib
build1 DOS_1541C_02_REC ser.lib
build1 DOS_1541C_03_REC ser.lib
build2 DOS_1570 serlib.src
build2 DOS_1571_03 serlib.src
build2 DOS_1571_05 serlib.src
build2 DOS_1571CR serlibcr.src
build2 DOS_1581_1987-03-12 serlib.src
build2 DOS_1581_1987-03-19 serlib.src
build2 DOS_1581 serlib.src

build1 DOS_8250 master
build1 DOS_8060_REC dos
build1 DOS_8070 dos
build1 DOS_D9065 master
build1 DOS_8280 master3.1

build2 DOS_1551 d1551.src
build2 DOS_1541_05 1541.src
build2 DOS_1541C_02 serlib.src
build2 DOS_1541C_03 serlib.src

build2 DOS_C65 dos.src

build1 PRINTER_8023P us.ptr.src

build1 ASSEMBLER_AIM65_REC assembler
build1 ASSEMBLER_PET_REC assembler
build1 ASSEMBLER_PET_V112779_REC assembler
build1 ASSEMBLER_PET_V121579_REC assembler4
build1 ASSEMBLER_PET_V090580_A_REC assembler
build1 ASSEMBLER_PET_V090580_B_REC assembler
build1 ASSEMBLER_PET_V102780_REC assembler
build1 ASSEMBLER_PET_V26MAR82RR_REC assembler
build1 ASSEMBLER_C64_REC assembler
build2 ASSEMBLER_TED assembler.src
build2 ASSEMBLER_C128 assembler.src
build1 LOADER_PET loload.4
build1 LOADER_C64 loload
build2 LOADER_C128 loader.src
build2 LOADER_C65 loader65.src
build2 LOADER_C65_RS232 l232.src

build1 DISK_MONITOR mon.lib
build1 MATHPACK mathpack

build1 KICKMAN kickman
build1 OMEGA omega
build1 WIZARD wizard
build1 FIG FOR1-1

build2 A2232 a2232_6502.src

# TODO cbm6502asm problems:
#   build2 RAMDOS a
#   build2 HCD65_3.5 c65
#   build2 HCD65_65CE02 c65
#   build2 HCD65_65CE02_0.1 c65
#   build2 HCD65_65CE02_0.2 c65
#   build2 EDT_C128 edt
#  expression error
#   build2 DOS_SHELL shell
