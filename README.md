# Commodore Source Code

This repository collects original source code of various Commodore Business Machines (CBM) computers converted to a modern encoding (ASCII, LF, indentation).

## BASIC_C64

The Commodore 64 BASIC source (901226-01). Extracted from c64-basic.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $A000-$BF52
* $BF71-$BFFF
* $E000-$E4AB

The checksum byte at $BF52 is 0 and will have to be calculated in a later step.

## KERNAL_C64_01

The Commodore 64 KERNAL source, original version (901227-01). Extracted from c64kernal.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $E500-$FF5A: code
* $FF81-$FFF5: jump table
* $FFFA-$FFFF: vectors

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

## KERNAL_C64_03

The Commodore 64 KERNAL source, final version (901227-03). Based on KERNAL_C64_00 and manually patched to reflect differences in the [LST printout of the -03 KERNAL](http://pagetable.com/docs/C64_KERNAL_03_LST.pdf).

In the printout, the added patch function `PIOKEY` ends with `JMP CLKHI` ($EE85), but the -03 binary contains $EE8E, which is `JMP CLKLO`. The printout is probably older than the -03 binary, so the correct `JMP CLKLO` was put into the source.

The sources have been verified to build the correct -03 KERNAL binary, but no guarantees can be given that all changes, especially in comments and formatting, have been patched correctly.

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

## BASIC_TED, KERNAL_TED_0{4|5}

The Commodore Plus/4, C16 and C116 BASIC 3.5 and KERNAL source (1984). Source: [ted_kernal_basic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/plus4/index.html)

* KERNAL_TED_04 is 318004-04 (PAL) and 318005-04 (NTSC). $FF80 = $84/$04. (It was reconstructed from kernal/kernal.xrf in the dump.)
* KERNAL_TED_05 is 318004-05 (PAL) and 318005-05 (NTSC). $FF80 = $85/$05. (The sources were missing a patch, which was reconstructed from kernal/kernal.lst in the dump.)

##  BASIC_C128, KERNAL_C128_0{3|5|6}, EDITOR_C128[_DIN], MONITOR_C128

The Commodore 128 BASIC 7.0, KERNAL, EDITOR and MONITOR sources (1985/1986). Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

* KERNAL_C128_03 is 318020-03, $FF80 = 0. (It was reconstructed from listings/first_release/kernal.lis in the dump.)
* KERNAL_C128_05 is 318020-05, $FF80 = 1.
* KERNAL_C128_06 is 318020-06, $FF80 = 2. This version seems to have been unreleased.
* EDITOR_C128 is the US version (318020-05), EDITOR_C128_DIN the German version (315078-03).

## BASIC_PET2001

The PET 2001 BASIC source (V2, 1978).

* This is Microsoft's original source that can also build BASIC for other systems, like OSI and Apple.
* On the PET, this was called BASIC V2, Microsoft called it V1.1.
* It includes the "WAIT6502,1" easter egg (symbol "ZSTORDO").

## BASIC_CBM2&#x5f;{A|B}, KERNAL_CBM2&#x5f;{A|B}, EDITOR_CBM2

The CBM2 BASIC, EDITOR and KERNAL sources (1983/1984). The A version is from [David Viner](https://www.davidviner.com/cbm9.html), and the B version is from disks 67/68 of the [CBUG](http://www.zimmers.net/anonftp/pub/cbm/b/CBUG/) library. The files have been converted to LF line breaks and LST-style indenting.

* The B version of BASIC contains two patches.
* The B version of the KERNAL contains minor modifications, and comes with alternate versions of some files as well as additional (test) sources.
* Except for file naming, the editor sources A and B were identical.

## DOS_4040

The CBM 4040/2040 DOS V2.1 source (1980). Extracted from 4040-source.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks.

* Comments say this is for the 2040. The 2040 and the 4040 have identical hardware, the 4040 is just a 2040 with DOS V2.

## DOS_8250

The CBM 8250/8050/4040 DOS V2.7 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The same ROM binary will work on the 8250 (DS/QD), the 8050 (SS/QD) and the 4040 (SS/DD), the differences will be handled at runtime (symbols "NSIDES", "DOS").
* Comments in the source call this "SUPER DOS".

## DOS_8070

The CBM 8070 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The CBM 8070 is an unreleased device with one 8" hard drive (drive #0) and one 8" double-sided floppy drive (drive #1, like CBM 8062).
* The power-on message says "CBM DOS V3.0 8070".
* The file copy code is stored on the hard disk and loaded on demand ("ldcopy").

## DOS_D9065

The CBM D9065 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The D9065 is an unreleased 7.5 MB hard drive, probably very similar to the D9060/D9090.

## DOS_1540

The Commodore 1540 DOS V2.6 source (1980), 325302-01/325303-01. Extracted from 1541-2031-num1.d64 and 1541-2031-source-disk-copy-num2-side1.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The source contains both the IEEE-488 and the IEC serial interface code, so it represents both the CBM 2031 and the Commodore 1540. Different `master` source files build different configurations.
* The power-on message always says "`CBM DOS V2.6 V170`" though, which corresponds to the 1540.

## DOS_1541[C]_0{1|2|3|5|6}[_REC]

The Commodore 1541/1541C/1541-II DOS V2.6 source.

* Directories ending in "`_REC`" have been reconstructed from DOS_1540 and DOS_1571 to match the ROM images. They all match in case and spacing, so they can be used for comparing versions against each other.
* Directories not ending in "`_REC`" contain original source from [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz) and are lower case.

These are the respective directories:

* DOS_1541_01_REC is 325302-01/901229-01 (1981).
* DOS_1541_02_REC is 325302-01/901229-02.
* DOS_1541_03_REC is 325302-01/901229-03.
* DOS_1541_05[_REC] is 325302-01/901229-05 (1983, shipped with the short board 1541).
* DOS_1541_06_REC is 325302-01/901229-06.
* DOS_1541C_01[_REC] is 251968-01 (1984, shipped with the 1541C)
* DOS_1541C_02[_REC] is 251968-02 (1986, [release notes](http://www.zimmers.net/anonftp/pub/cbm/firmware/drives/new/1541/1541C.251968-02.txt))
* DOS_1541C_03[_REC] is 251968-03 (shipped with the 1541-II)

## DOS_1551

The Commodore 1551 DOS 2.6 TDISK source (1984), 318008-01. Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)

## DOS_1570

The Commodore 1570 DOS 3.0 source (1985), 315090-01. Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)

## DOS_1571{_03|_05|CR}

The Commodore 1571 DOS sources:

* DOS_1571_03 is DOS 3.0 (1985), original version (310654-03).
* DOS_1571_05 is DOS 3.0 (1986), updated version (310654-05).
* DOS_1571CR is DOS 3.1 (1986) of the cost-reduced 1571 (318047-01).

Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz); the -03 version was reconstructed from 1571/310654_03/serlib.lst in the dump.

## DOS_1581

The Commodore 1581 DOS source (10.0, 1987), original version (318045-01). Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)

## RAMDOS

The C64/C128 RAMDISK DOS V4.3 source (1987). Extracted from [ramdos.zip](http://www.zimmers.net/anonftp/pub/cbm/src/drives/ramdos.zip). This is a version of Commodore DOS running on the C64/C128 for using a REU (RAM Expansion Unit) as a RAM disk.

# Credits

This repository is maintained by Michael Steil, mist64@mac.com
