# This script will build most of the source in this repository.
# The directory "build" will contain the LST and OBJ files for each project.

[ -z "$KERNALEMU" ]   && KERNALEMU=kernalemu
[ -z "$CBM6502ASM" ]  && CBM6502ASM=asm
[ -z "$ASSEMBLER64" ] && ASSEMBLER64=`pwd`/assembler64.prg

test_tools()
{
	if ! command -v $KERNALEMU &> /dev/null
	then
	    echo "Please get https://github.com/mist64/kernalemu and install it in your PATH,"
	    echo "or export the \$KERNELEMU variable to point to the binary."
	    exit
	fi

	if ! command -v $CBM6502ASM &> /dev/null
	then
	    echo "Please get https://github.com/mist64/cbm6502asm and install it in your PATH,"
	    echo "or export the \$CBM6502ASM variable to point to the binary."
	    exit
	fi

	if [ ! -f $ASSEMBLER64 ]; then
	    echo "Please get assembler64.prg from https://github.com/mist64/kernalemu/tree/main/demo,"
	    echo "or export the \$ASSEMBLER64 variable to point to the file."
	    exit
	fi
}

build1()
{
	echo build1 $1 $2
	DIR=$1
	SRC=$2
	mkdir build/$DIR
	for i in $DIR/*; do cat $i | tr '\n' '\r' | tr '\t' ' ' > build/$i; done
	cd build/$DIR
	#for A in *; do if [ ! -e ${A^^} ]; then ln -s $A ${A^^}; fi; done
	[ -e $SRC ] || 	echo "$DIR/$SRC does not exist"
	echo -e "_tmp_obj\n\n\n$SRC" | $KERNALEMU $ASSEMBLER64
	mv printer4.txt ../$1.lst
	tr '\r' '\n' < _tmp_obj > ../$1.obj
	srec_cat ../$1.obj -MOS_Technologies -offset -0xe000 -o ../$1.bin -Binary
	cd ../..
	rm -rf build/$DIR
}

build2()
{
	echo build2 $1 $2
	DIR=$1
	SRC=$2
	cp -pr $DIR build/
	cd build/$DIR
	for A in *; do if [ ! -e ${A^^} ]; then ln -s $A ${A^^}; fi; done
	[ -e $SRC ] || 	echo "$DIR/$SRC does not exist"
	$CBM6502ASM _tmp_obj,_tmp_lst=$SRC
	mv _tmp_lst.lst ../$1.lst
	mv _tmp_obj.obj ../$1.obj
	cd ../..
	rm -rf build/$DIR
}

test_tools

rm -rf build
mkdir build

build1 ASSEMBLER_KIM_REC assembler

# TODO cbm6502asm problems:
#   build2 RAMDOS a
#   build2 HCD65_3.5 c65
#   build2 HCD65_65CE02 c65
#   build2 HCD65_65CE02_0.1 c65
#   build2 HCD65_65CE02_0.2 c65
#   build2 EDT_C128 edt
#  expression error
#   build2 DOS_SHELL shell
