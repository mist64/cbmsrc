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

	if [ ! -f assembler64.prg ]; then
	    echo "Please get assembler64.prg from https://github.com/mist64/kernalemu/tree/main/demo."
	    exit
	fi
}

build1()
{
	DIR=$1
	SRC=$2
	mkdir build/$DIR
	for i in $DIR/*; do cat $i | sed -e "s/\.NLIST//" | tr '\n' '\r' | tr '\t' ' ' > build/$i; done
	cd build/$DIR
	echo "_tmp_obj\n\n\n$SRC" | $KERNALEMU ../../$ASSEMBLER64
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
	$CBM6502ASM _tmp_obj,_tmp_lst=$SRC
	mv _tmp_lst.lst ../$1.lst
	mv _tmp_obj.obj ../$1.obj
	cd ../..
	rm -rf build/$DIR
}

test_tools

rm -rf build
mkdir build

build1 KERNAL_VIC_04 kernal

build1 KERNAL_C64_01 kernal
build1 KERNAL_C64_02 kernal
build1 KERNAL_C64_03 kernal
build2 KERNAL_C64_03_VAX kernal
build2 KERNAL_C64GS console

build2 KERNAL_TED_04 kernal
build2 KERNAL_TED_05 kernal

build2 KERNAL_C128_03 kernal
build2 KERNAL_C128_05 kernal
build2 KERNAL_C128_06 kernal

build1 KERNAL_CBM2_A kernal
build1 KERNAL_CBM2_B kernal

build2 EDITOR_C128 editor
build2 EDITOR_C128_DIN editor

build1 EDITOR_CBM2 sced.lib

build1 BASIC_VIC basic

build1 BASIC_C64 basic
build2 BASIC_C64_VAX basic
build2 BASIC_C64GS basic

build2 BASIC_TED basic

build2 BASIC_C128 basic

build1 BASIC_CBM2_A cbm256 # or cbm128
build1 BASIC_CBM2_B cbm256 # cbm128 cbm192 cbm256 cbm64 pet128 pet192 pet256 pet64

build1 MONITOR_VIC micii4
build2 MONITOR_C128 monitor

build2 ASSEMBLER_TED assembler

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
build2 DOS_1570 serlib
build2 DOS_1571_03 serlib
build2 DOS_1571_05 serlib
build2 DOS_1571CR serlibcr
build2 DOS_1581 serlib

build1 DOS_8250 master
build1 DOS_8060 dos
build1 DOS_8070 dos
build1 DOS_D9065 master

build2 DOS_1551 d1551
build2 DOS_1541_05 1541
build2 DOS_1541C_02 serlib
build2 DOS_1541C_03 serlib

# TODO cbm6502asm problems:
#   build2 RAMDOS a

build1 PRINTER_8023P us.ptr.src
