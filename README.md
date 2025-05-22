# Commodore Source Code

This repository collects the *original* source code of various Commodore Business Machines (CBM) computers converted to a modern encoding (ASCII, LF, indentation).

Using [kernalemu](https://github.com/mist64/kernalemu) and [cbm6502asm](https://github.com/mist64/cbm6502asm), almost all source in this repo can be built from the UNIX command line. To build everything, run `build.sh` from the Unix command line. The script depends on the [srecord](https://srecord.sourceforge.net) package to convert the .hex files into binary.

## KIM-1/AIM-65

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [MONITOR_KIM](MONITOR_KIM)                                   | 1975 | KIM-1 ROM (6530-002, 6530-003) |
| [MONITOR_AIM65](MONITOR_AIM65)                               | 1978 | AIM-65 monitor |
| [TIM](TIM)                                                   | 1976 | TIM ROM (6530-004) |

## PET

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_PET_V1_REC](BASIC_PET_V1_REC)                         | 1977 |
| [BASIC_PET_V1R_REC](BASIC_PET_V1R_REC)                       | 1977 |
| [BASIC_PET_V2_MICROSOFT](BASIC_PET_V2_MICROSOFT)             | 1978 | Microsoft BASIC V2 (MACRO-11) |
| [BASIC_PET_V2_REC](BASIC_PET_V2_REC)                         | 1978 |
| [BASIC_PET_V4_1979-07-23](BASIC_PET_V4_1979-07-23)           | 1979 |
| [BASIC_PET_V4_REC](BASIC_PET_V4_REC)                         | 1980 |
| [BASIC_PET_V4R_REC](BASIC_PET_V4R_REC)                       | 1980 |
| [BASIC_PET_V5](BASIC_PET_V5)                                 | 1980 |
| [KERNAL_PET_1.0_REC](KERNAL_PET_1.0_REC)                     | 1977 |
| [KERNAL_PET_2.0_REC](KERNAL_PET_2.0_REC)                     | 1978 |
| [KERNAL_PET_4.0_1979-10-23](KERNAL_PET_4.0_1979-10-23)       | 1979 |
| [KERNAL_PET_4.0_REC](KERNAL_PET_4.0_REC)                     | 1980 |
| [EDITOR_PET_EURO](EDITOR_PET_EURO)                           | 1981 | European editor for CBM 8032 |

## CBM2

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_CBM2_1982-11-19](BASIC_CBM2_1982-11-19)               | 1982 |
| [BASIC_CBM2_1983-05-31](BASIC_CBM2_1983-05-31)               | 1983 |
| [BASIC_CBM2_1984-01-04](BASIC_CBM2_1984-01-04)               | 1984 |
| [EDITOR_CBM2_1982-11-19](EDITOR_CBM2_1982-11-19)             | 1982 |
| [EDITOR_CBM2_1983-05-31](EDITOR_CBM2_1983-05-31)             | 1983 |
| [KERNAL_CBM2_1982-11-19](KERNAL_CBM2_1982-11-19)             | 1982 |
| [KERNAL_CBM2_1983-05-31](KERNAL_CBM2_1983-05-31)             | 1983 |
| [KERNAL_CBM2_1983-07-07](KERNAL_CBM2_1983-07-07)             | 1983 |

## VIC-20

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_VIC](BASIC_VIC)                                       | 1980 | 901486-01 |
| [KERNAL_VIC_04](KERNAL_VIC_04)                               | 1980 | 901486-04 |

## C64

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_C64](BASIC_C64)                                       | 1982 | 901226-01 |
| [BASIC_C64_BSO](BASIC_C64_BSO)                               | 1982 | 901226-01, BSO syntax |
| [BASIC_C64GS](BASIC_C64GS)                                   | 1990 | 390852-01 |
| [KERNAL_C64_01](KERNAL_C64_01)                               | 1982 | 901227-01 |
| [KERNAL_C64_02](KERNAL_C64_02)                               | 1982 | 901227-02 |
| [KERNAL_C64_03](KERNAL_C64_03)                               | 1983 | 901227-03 |
| [KERNAL_C64_03_BSO](KERNAL_C64_03_BSO)                       | 1983 | 901227-03, BSO syntax |
| [KERNAL_C64GS](KERNAL_C64GS)                                 | 1990 | 390852-01 |

## Ultimax

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_MAX_REC](BASIC_MAX_REC)                               | 1982 | 901230-01 |
| [KERNAL_MAX_REC](KERNAL_MAX_REC)                             | 1982 | 901231-01 |
| [BASIC_MAX_MINI_REC](BASIC_MAX_MINI_REC)                     | 1982 | 901233-01 (MAX3101) |

## TED (C16, C116, Plus/4)

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_TED](BASIC_TED)                                       | 1984 | 318006-01 |
| [KERNAL_TED_04](KERNAL_TED_04)                               | 1984 | 318004-04 (PAL), 318005-04 (NTSC) |
| [KERNAL_TED_05](KERNAL_TED_05)                               | 1984 | 318004-05 (PAL), 318005-05 (NTSC) |
| [CHARSET_TED](CHARSET_TED)                                   | 1984 |  |

## C128

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_C128_03](BASIC_C128_03)                               | 1985 | 318018-03, 318019-03 |
| [BASIC_C128_04](BASIC_C128_04)                               | 1986 | 318018-04, 318019-04 |
| [EDITOR_C128](EDITOR_C128)                                   | 1985 | 318020-05 |
| [EDITOR_C128_DIN](EDITOR_C128_DIN)                           | 1985 | 315078-03, German |
| [EDITOR_C128_NATIONAL](EDITOR_C128_NATIONAL)                 | 1987 |             |
| [KERNAL_C128_03](KERNAL_C128_03)                             | 1985 | 318020-03 |
| [KERNAL_C128_05](KERNAL_C128_05)                             | 1985 | 318020-05 |
| [KERNAL_C128_06](KERNAL_C128_06)                             | 1988 | 318020-06 |
| [MONITOR_C128](MONITOR_C128)                                 | 1985 | |
| [CHARSET_C128_NATIONAL](CHARSET_C128_NATIONAL)               | 1985 | |

## C65

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [BASIC_C65](BASIC_C65)                                       | 1991 | C65 BASIC (911115) |
| [GRAPHICS_C65](GRAPHICS_C65)                                 | 1991 | C65 BASIC Graphics lib (911119) |
| [KERNAL_C65](KERNAL_C65)                                     | 1991 | C65 KERNAL, EDITOR, MONITOR (911119) |
| [DOS_C65](DOS_C65)                                           | 1991 | C65 DOS (911119) |
| [BASIC_C64_C65](BASIC_C64_C65)                               | 1991 | C64 mode BASIC V2 (900531) |
| [KERNAL_C64_C65](KERNAL_C64_C65)                             | 1991 | C64 mode KERNAL (910117) |


## Disk Drives

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [DOS_4040](DOS_4040)                                         | 1980 | DOS V2.0; IEEE-488; 2x SS DD 5.25" |
| [DOS_8060_REC](DOS_8060_REC)                                 | 1980 | DOS V7.0; IEEE-488; 2x SS/DS DD 8" |
| [DOS_8070](DOS_8070)                                         | 1980 | DOS V3.0; IEEE-488; SS/DS DD 8" + HD |
| [DOS_8250](DOS_8250)                                         | 1981 | DOS V2.7; IEEE-488; 2x SS/DS QD 5.25" |
| [DOS_8280](DOS_8280)                                         | 1981 | DOS V3.0; IEEE-488; 2x DS QD 8" |
| [DOS_D9065](DOS_D9065)                                       | 1981 | DOS V3.0; IEEE-488; HD |
| [DOS_1540](DOS_1540)                                         | 1981 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_01_REC](DOS_1541_01_REC)                           | 1982 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_02_REC](DOS_1541_02_REC)                           | 198? | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_03_REC](DOS_1541_03_REC)                           | 198? | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_05](DOS_1541_05)                                   | 1984 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_05_REC](DOS_1541_05_REC)                           | 198? | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541_06_REC](DOS_1541_06_REC)                           | 198? | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541C_01_REC](DOS_1541C_01_REC)                         | 198? | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541C_02](DOS_1541C_02)                                 | 1985 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541C_02_REC](DOS_1541C_02_REC)                         | 1985 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541C_03](DOS_1541C_03)                                 | 1987 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1541C_03_REC](DOS_1541C_03_REC)                         | 1987 | DOS V2.6; Serial; SS DD 5.25" |
| [DOS_1551](DOS_1551)                                         | 1984 | DOS V2.6; TCBM; SS DD 5.25" |
| [DOS_1570](DOS_1570)                                         | 1985 | DOS V3.0; Serial; SS DD 5.25" |
| [DOS_1571_03](DOS_1571_03)                                   | 1986 | DOS V3.0; Serial; DS DD 5.25" |
| [DOS_1571_05](DOS_1571_05)                                   | 1986 | DOS V3.0; Serial; DS DD 5.25" |
| [DOS_1571CR](DOS_1571CR)                                     | 1986 | DOS V3.1; Serial; DS DD 5.25" |
| [DOS_1581](DOS_1581)                                         | 1987 | DOS V10; Serial; DS DD 3.5" |
| [DOS_1581_1987-03-12](DOS_1581_1987-03-12)                   | 1987 | DOS V10; Serial; DS DD 3.5" |
| [DOS_1581_1987-03-19](DOS_1581_1987-03-19)                   | 1987 | DOS V10; Serial; DS DD 3.5" |
| [RAMDOS](RAMDOS)                                             | 1986 | REU 1764/1750 RAM disk |
| [CONTROLLER_1001](CONTROLLER_1001)                           | 1983 | Controller; DS QD 5.25" |
| [CONTROLLER_8250](CONTROLLER_8250)                           | 1981 | Controller; 2x DS QD 5.25" |

## Printer ROM

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [PRINTER_8023P](PRINTER_8023P)                               | 1980 | IEEE-488; based on Brother M82

## Software

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [KIMATH](KIMATH)                                             | 1976 | KIM-1 Math Package |
| [MATHPACK](MATHPACK)                                         | 1979 | CBM2 Math Package |
| [MONITOR_PET_ADDON](MONITOR_PET_ADDON)                       | 1981 | RAM-based monitor for PET 8x92 |
| [MONITOR_C64](MONITOR_C64)                                   | 1982 | RAM-based monitor for C64 |
| [DISK_MONITOR](DISK_MONITOR)                                 | 1980 | Monitor extension for PET |
| [DOS_SHELL](DOS_SHELL)                                       | 1985 | DOS SHELL tool (1571 Test/Demo disk) |
| [FIG](FIG)                                                   | 1980 | fig-FORTH |

## Software: Resident Assembler & HCD65

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [ASSEMBLER_AIM65_REC](ASSEMBLER_AIM65_REC)                   | 1978 | 4 KB, heavily optimized |
| [ASSEMBLER_PET_REC](ASSEMBLER_PET_REC)                       | 197? | ROM 2 only (undated) |
| [ASSEMBLER_PET_V112779_REC](ASSEMBLER_PET_V112779_REC)       | 1979 | ROM 2 only, adds mul/div, error messages |
| [ASSEMBLER_PET_V121579_REC](ASSEMBLER_PET_V121579_REC)       | 1979 | ROM 2/4, adds `.IFN`/`.IFE` |
| [ASSEMBLER_PET_V090580_A_REC](ASSEMBLER_PET_V090580_A_REC)   | 1980 | adds XREF support |
| [ASSEMBLER_PET_V090580_B_REC](ASSEMBLER_PET_V090580_B_REC)   | 1980 | optimizations |
| [ASSEMBLER_PET_V102780_REC](ASSEMBLER_PET_V102780_REC)       | 1980 | bugfixes, ROM auto-detect |
| [ASSEMBLER_PET_V26MAR82RR_REC](ASSEMBLER_PET_V26MAR82RR_REC) | 1982 | based on V090580_B, LST output changes, non-mainline |
| [ASSEMBLER_C64_REC](ASSEMBLER_C64_REC/)                      | 1982 | "C64 Macro Assembler", adds `.MAC` |
| [ASSEMBLER_TED](ASSEMBLER_TED)                               | 1984 | TED version, adds `.OPT LON`, `.OPT MLI` |
| [ASSEMBLER_C128](ASSEMBLER_C128)                             | 1986 | C128 version, heavily commented |
| [HCD65_3.5](HCD65_3.5)                                       | 1986 | BSO-compatible rewrite |
| [HCD65_65CE02_0.1](HCD65_65CE02_0.1)                         | 1988 | 65CE02/4510 opcode support |
| [HCD65_65CE02_0.2](HCD65_65CE02_0.2)                         | 1990 | update |
| [EDT_C128](EDT_C128)                                         | 1986 | |
| [LOADER_PET](LOADER_PET)                                     | 1979 | OBJ Loader |
| [LOADER_C64](LOADER_C64)                                     | 1986 | OBJ Loader |
| [LOADER_C128](LOADER_C128)                                   | 1986 | rewrite |
| [LOADER_C65](LOADER_C65)                                     | 1990 | |
| [LOADER_C65_RS232](LOADER_C65_RS232)                         | 1990 | RS-232 |

## Software: Games

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [KICKMAN](KICKMAN)                                           | 1982 | Kickman Ultimax Game |
| [OMEGA](OMEGA)                                               | 1982 | Omega Race Ultimax Game |
| [WIZARD](WIZARD)                                             | 1982 | Wizard of Wor Ultimax Game |

## Other

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [A2232](A2232)                                               | 1991 | Amiga A2232 Serial Port Card |

## Cross-Platform

| Directory                                                    | Year | Comments    |
|--------------------------------------------------------------|------|-------------|
| [6502ASM](6502ASM)                                           | 1989 | VAX/MS-DOS C 65CE02 cross assembler|


## Descriptions

### BASIC_PET&#x5f;&#x2a;, KERNAL_PET&#x5f;&#x2a;

The PET BASIC and KERNAL sources in different versions (197-1980). Reconstructed by Martin Hoffmann-Vetter.

* BASIC_PET_V4_1979-07-23 and KERNAL_PET_4.0_1979-10-23 were OCRed and reconstructed from the [LST printouts](https://www.retro-commodore.eu/pet-tech/) of a pre-V4 version.
* The other versions were reconstructed from this to match the corresponding binaries:
	* BASIC_PET_V1_REC: 901439-01, 901439-05, 901439-02, 901439-06, 901439-03
	* BASIC_PET_V1R_REC: 901439-09, 901439-05, 901439-02, 901439-06, 901439-03
	* BASIC_PET_V2_REC: 901465-01, 901465-02, 901447-24
	* BASIC_PET_V4_REC: 901465-19, 901465-20, 901465-21
	* BASIC_PET_V4R_REC: 901465-23, 901465-20, 901465-21
	* KERNAL_PET_1.0_REC: 901439-03, 901439-04, 901439-07
	* KERNAL_PET_2.0_REC: 901447-24, 901465-03
	* KERNAL_PET_4.0_REC: 901465-21, 901447-29, 901465-22
* BASIC_PET_V5: Unreleased, from FH disk basic50.d80. Does not build with C64 Resident Assembler (SYMBOL TABLE OVERFLOW).

Note that BASIC V1, V1R and V2 were built by Microsoft (see BASIC_PET_V2_MICROSOFT), and the binary was sent to Commodore. After that, Commodore moved BASIC development in-house and converted Microsoft's V2 source into MDT650 Resident Assembler format. These reconstructed versions of V1 and V1R in "Resident Assembler" format therefore never existed and have only been produced to allow for comparisons against BASIC V2, V4 and V4R (which were built by Commodore with the Resident Assembler).

### BASIC_VIC

The Commodore VIC-20 BASIC source (901486-01). Extracted from basic.src from [vic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/vic20/vic_src.tar.gz) and formatted the same as the C64 BASIC source.

### KERNAL_VIC_04

The Commodore VIC-20 KERNAL source, unreleased version (901486-04). Extracted from kernal.src from [vic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/vic20/vic_src.tar.gz) and formatted the same as the C64 KERNAL source.

### BASIC_C64

The Commodore 64 BASIC source (901226-01). Extracted from c64-basic.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $A000-$BF52
* $BF71-$BFFF
* $E000-$E4AB

The checksum byte at $BF52 is 0 and will have to be calculated in a later step.

### BASIC_MAX_REC, KERNAL_MAX_REC

The Ultimax BASIC cartridge (MAX3102, 901230-01/901231-01). Contains an adapted version of the C64 KERNAL (-01) and a slightly patched version of BASIC. Reconstructed from BASIC_C64 and KERNAL_C64_01.

### BASIC_MAX_MINI_REC

The Ultimax MINI BASIC cartridge (MAX3101, 901233-01). Contains a stripped-down version of the C64 BASIC (no arrays, no trigonometry etc.) and a tiny 1.5 KB KERNAL-like OS. Reconstructed from BASIC_C64 and KERNAL_C64_01.

### KERNAL_C64_01

The Commodore 64 KERNAL source, original version (901227-01). Extracted from c64kernal.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $E500-$FF5A: code
* $FF81-$FFF5: jump table
* $FFFA-$FFFF: vectors

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

### KERNAL_C64_02

The Commodore 64 KERNAL source, universal (PAL/NTSC) version (901227-02).

It contains the version byte (0) at $FF80, but it does not contain the "RRBY" signature at $FFF6.

### KERNAL_C64_03

The Commodore 64 KERNAL source, final version (901227-03). Based on KERNAL_C64_02 and manually patched using

* the [LST printout of the -03 KERNAL](http://pagetable.com/docs/C64_KERNAL_03_LST.pdf)
* the [BSO-format SRC from 1987](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz)

There are two differences though:

* In both sources, the added patch function `PIOKEY` ends with `JMP CLKHI` ($EE85), but the -03 binary contains $EE8E, which is `JMP CLKLO`. The change was probably done in the binary and not added back to the source. The version in this repository contains the correct `JMP CLKLO` though.
* The printout LST contains a version byte ($FF80) of 0, the BSO SRC contains the correct version byte of 3. The version in this repository contains a 3.

The sources have been verified to build the correct -03 KERNAL binary.

It contains the version byte (3) at $FF80, but it does not contain the "RRBY" signature at $FFF6.

### BASIC_C64_BSO, KERNAL_C64_BSO

This is the C64 BASIC and KERNAL source, converted from PET Resident Assembler syntax to the BSO CR6502/11 assembler by Fred Bowen in 1987, and with updated comments. Source: [c64_kernal_bas_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz).

This produces the same binaries as BASIC_C64 and KERNAL_C64_03, except for the pre-03 `PIOKEY` patch (see above).

### BASIC_C64GS, KERNAL_C64GS

The Commodore 64GS BASIC and KERNAL (390852-01) source (1990). Source: [c64_kernal_bas_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz).

The source files are in BSO assembler format and derived from BASIC_C64_BSO and KERNAL_C64_BSO.

* BASIC_C64GS: The source has updated comments, and the binary is identical with the regular C64 BASIC, except for:
	* The BASIC vector table is initialized with a literal $F72E as the main loop address. This causes BASIC to jump to the C64GS power-on message in the KERNAL.
	* The checksum is hardcoded with $EC.
* KERNAL_C64GS: The source has updated comments, and contains the following changes:
	* Tape support has been replaced by the power-on message code.
	* Memory test has been sped up by only testing one byte per block.
	* The `PIOKEY` patch has been fixed in the source.

The KERNAL source contains the version byte (3) at $FF80 as well as the "RRBY" signature at $FFF6.

### BASIC_TED, KERNAL_TED_0{4|5}

The Commodore Plus/4, C16 and C116 BASIC 3.5 and KERNAL source (1984). Source: [ted_kernal_basic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/plus4/index.html)

* KERNAL_TED_04 is 318004-04 (PAL) and 318005-04 (NTSC). $FF80 = $84/$04. (It was reconstructed from kernal/kernal.xrf in the dump.)
* KERNAL_TED_05 is 318004-05 (PAL) and 318005-05 (NTSC). $FF80 = $85/$05. (The sources were missing a patch, which was reconstructed from kernal/kernal.lst in the dump.)

###  BASIC_C128_0{3|4}, KERNAL_C128_0{3|5|6}, EDITOR_C128[_DIN], MONITOR_C128

The Commodore 128 BASIC 7.0, KERNAL, EDITOR and MONITOR sources (1985/1986). Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

* BASIC_C128_03 is 318018-03/318019-03.
* BASIC_C128_04 is 318018-04/318019-04.
* KERNAL_C128_03 is 318020-03, $FF80 = 0. (It was reconstructed from listings/first_release/kernal.lis in the dump.)
* KERNAL_C128_05 is 318020-05, $FF80 = 1.
* KERNAL_C128_06 is 318020-06, $FF80 = 2. This version seems to have been unreleased.
* EDITOR_C128 is the US version (318020-05)
* EDITOR_C128_DIN is the German version (315078-03). The code is the same as in EDITOR_C128_NATIONAL, but the comments are slightly different.
* EDITOR_C128_NATIONAL contains Denmark (325174-03), Finland/Sweden (325180-03), France/Belgium (325166-03), Germany/Austria (315078-03), Italy (325168-03), Norway (325177-03), Spain (325170-03), Swiss (325172-03).

### BASIC_PET_V2_MICROSOFT

The PET BASIC V2 source (1978) as built by Microsoft.

* This is Microsoft's original source that can also build BASIC for other systems, like OSI and Apple.
* Microsoft built all versions up this one with its own tools and sent the binary (together with the LST) to Commodore.
* On the PET, this was called BASIC V2, Microsoft called it V1.1.
* It includes the "WAIT6502,1" easter egg (symbol `ZSTORDO`).

### BASIC_C65

The final version of C65 BASIC V10 (911115, 1991-11-15). Source: folder system/9 in [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)

### GRAPHICS_C65

The final version of the C65 GRAPHICS BASIC (911119, 1991-11-19), the graphics library that is part of BASIC 10. Source: folder system/9 in [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)

### KERNAL_C65

The final version of the C65 KERNAL (called "KERNEL"; 911119, 1991-11-19). Source: folder system/9 in [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)

### DOS_C65

The final version of the C65 CBDOS (Computer-Based DOS; 911119, 1991-11-19). Source: folder system/9 in [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)


### BASIC_CBM2&#x5f;&#x2a;, KERNAL_CBM2&#x5f;&#x2a;, EDITOR_CBM2&#x5f;&#x2a;

The CBM2 BASIC, EDITOR and KERNAL sources in different versions (1982-1984).

* The *_1982-11-19 version is from [Ruud Baltissen's ICPUG disks](http://www.pagetable.com/docs/CBM2_Sources.zip).
* The *_1983-05-31 version is from [David Viner](https://www.davidviner.com/cbm9.html).
* KERNAL_1983-07-07 and BASIC_1984-01-04 are from disks 67/68 of the [CBUG](http://www.zimmers.net/anonftp/pub/cbm/b/CBUG/) library.

The files have been converted to LF line breaks and LST-style indenting.

### EDITOR_PET_EURO

The "CBM 8032 EURO SCREEN EDITOR 4V2E (E-901474-03)" ROM. Extracted from UNKN008.D80 from [Ruud Baltissen's ICPUG disks](http://www.pagetable.com/docs/CBM2_Sources.zip) and converted to LF line breaks and LST-style indenting.

### CHARSET_TED

The Character ROM for the TED series (Plus/4, C16 and C116; 1984). Extracted from [ted_kernal_basic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/plus4/ted_kernal_basic_src.tar.gz).

### CHARSET_C128_NATIONAL

The set of national Character ROMs for the C128 (1985): Denmark (325175-01), Finland/Sweden (325181-01), France/Belgium/Italy (325167-01), Germany/Belgium (315079-01), Norway (325178-01), Spain (325171-01), Switzerland (325173-01).

### MONITOR_PET_ADDON

The PET 8096/80196 memory expansion addon monitor (1981). Corrected and fixed-up from the [OCRed version](http://www.zimmers.net/anonftp/pub/cbm/src/pet/ADD-ON-MON.txt), which is based on the [LST printout](ftp://www.zimmers.net/pub/cbm/manuals/pet/MemExpansion/CBM_Expansion_Memory_Board_Add-on-Mon.pdf)

### MONITOR_C64

The C64 monitor program. Extracted from ml-monitor-source.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting. The symbol `WALKON` in the file `micii4` was changed from `1` to `0` so the source builds.

### DOS_4040

The CBM 4040/2040 DOS V2.1 source (1980). Extracted from 4040-source.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks.

* Comments say this is for the 2040. The 2040 and the 4040 have identical hardware, the 4040 is just a 2040 with DOS V2.

### DOS_8250

The CBM 8250/8050/1001/4040 DOS V2.7 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The same ROM binary will work on the 8250 (DS/QD), the 8050 (SS/QD), the SFD-1001 (DS/QD, single drive) and the 4040 (SS/DD), the differences will be handled at runtime (symbols "NSIDES", "DOS").
* Comments in the source call this "SUPER DOS".
* This only contains the DOS and IEEE code. The controller is in CONTROLLER_8250/CONTROLLER_1001.

### DOS_8280

The CBM 8280 V3.1 source (1981). Extracted from [Ruud Baltissen's ICPUG disks](http://www.pagetable.com/docs/CBM2_Sources.zip) and converted to LF line breaks and LST-style indenting.

### DOS_8060_REC

The reconstructed source for the CBM 8061/8062 (1980). The source is based on DOS_8070 and modified by Martin Hoffmann-Vetter to build into the DOS portion of the 901488-01/-02 ROMs.

The CBM 8061 (single side) and 8062 (dual side) are a pair of dual 8" disk drives. They were announced, but only a few were made and never sold publicly, but instead used internally at Commodore.

* The source only contains the bus controller portion of the ROM ($CC78-$FFFF), not the disk controller part.
* The power-on message says "CBM DOS V7.0 8060".

### DOS_8070

The CBM 8070 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

The CBM 8070 is an unreleased device with one 8" hard drive (#0) and one 8" double-sided floppy drive (#1). It is derived from the dual 8" drive CBM 8061/8062 (1980).

* The source only contains the bus controller portion of the ROM ($CE50-$FFFF), not the disk controller part.
* The power-on message says "CBM DOS V3.0 8070".
* The file copy code is stored on the hard disk and loaded on demand ("ldcopy").

### DOS_D9065

The CBM D9065 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The D9065 is an unreleased 7.5 MB hard drive, probably very similar to the D9060/D9090.

### DOS_1540

The Commodore 1540 DOS V2.6 source (1980), 325302-01/325303-01. Extracted from 1541-2031-copy-1-original-side1.d64/1541-2031-num1.d64 and 1541-2031-source-disk-copy-num2-side1.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* The source contains both the IEEE-488 and the IEC serial interface code, so it represents both the CBM 2031 and the Commodore 1540. Different `master` source files build different configurations.
* The power-on message always says "`CBM DOS V2.6 V170`" though, which corresponds to the 1540.

### DOS_1541[C]_0{1|2|3|5|6}[_REC]

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

(Commodore used the PET Resident Assembler to build the 1541 DOS until "&#x5f;03", and switched to the BSO assembler afterwards. All original source that has been preserved is in BSO format. To allow for better comparisons, all "&#x5f;REC" source has been formatted in the PET Resident Assembler format. So note that the "&#x5f;REC" source after "&#x5f;03" never existed in this format.)

### DOS_1551

The Commodore 1551 DOS 2.6 TDISK source (1984), 318008-01. Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)

### DOS_1570

The Commodore 1570 DOS 3.0 source (1985), 315090-01. Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)

### DOS_1571{_03|_05|CR}

The Commodore 1571 DOS sources:

* DOS_1571_03 is DOS 3.0 (1985), original version (310654-03).
* DOS_1571_05 is DOS 3.0 (1986), updated version (310654-05).
* DOS_1571CR is DOS 3.1 (1986) of the cost-reduced 1571 (318047-01).

Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz); the -03 version was reconstructed from 1571/310654_03/serlib.lst in the dump.

### DOS_1581
### DOS_1581_1987-03-12

* DOS_1581: The Commodore 1581 DOS source (10.0, 1987-03-20), original version (318045-01). Source: [all_new_dos.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/drives/all_new_dos.tar.gz)
* DOS_1581_1987-03-19: work revision, serlib_977e.lst from same archive.
* DOS_1581_1987-03-12: work revision, serlib_a9f5.lst from same archive; ".subttl" typo fixed in sieee.src.

### RAMDOS

The C64/C128 RAMDISK DOS V4.3 source (1987). Extracted from [ramdos.zip](http://www.zimmers.net/anonftp/pub/cbm/src/drives/ramdos.zip). This is a version of Commodore DOS running on the C64/C128 for using a REU (RAM Expansion Unit) as a RAM disk.

### CONTROLLER_8250

The CBM 8250/8050/4040 controller source (1981). Extracted from 4040-8050-8250-controller-code-tandon.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* This only contains the controller. The DOS and IEEE code is in DOS_8250.

### CONTROLLER_1001

The SFD-1001 controller source (1981). Extracted from old-dos-sources.d81 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting.

* This only contains the controller. The DOS and IEEE code is in DOS_8250.

### PRINTER_8023P

The CBM 8023P printer source code (1980). Extracted from 8023p-printer-source.d64 from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/) and converted to LF line breaks and LST-style indenting. The `.NOLIST` directive was commented out in `us.ptr.src`, so building would create a useful LST file.

The [CBM 8023P](http://www.zimmers.net/anonftp/pub/cbm/firmware/printers/8023P-MPP1361/index.html) is a dot matrix printer with an IEEE-488 connection, a 6502/6532/6522-based controller, and a Brother M82 mechanism. The source contains references to Epson Model 3110/3210 mechanisms which seem to have been used in earlier printers.

The source contains a 90 degree rotated 6x8 pixel variant of the PETSCII character/graphics set.

### DISK_MONITOR

"DISK MONITOR 2040 V1.0", an extension to the PET monitor for disk commands (see mon.cmd). Extracted from UNKN008.D80 from [Ruud Baltissen's ICPUG disks](http://www.pagetable.com/docs/CBM2_Sources.zip) and converted to LF line breaks and LST-style indenting.

### DOS_SHELL

The DOS Shell tool from the 1571 Test/Demo disk, autoboot for C128. Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

### MATHPACK

A variant of the "BCD Math Package" printed in the [Commodore 700 Reference Manual](http://www.zimmers.net/anonftp/pub/cbm/programming/b/Commodore_700_Reference_Manual.pdf). Extracted from UNKN008.D80 from [Ruud Baltissen's ICPUG disks](http://www.pagetable.com/docs/CBM2_Sources.zip) and converted to LF line breaks and LST-style indenting.

### ASSEMBLER_AIM65_REC

The AIM-65 version of the Resident Assembler (ROM R3224; 1978). Reconstructed from ASSEMBLER_TED.

This is a reconstruction of the oldest known version of the Resident Assembler. It's a 4 KB ROM that interfaces with the AIM-65 monitor. It is heavily optimized and outputs to RAM instead of OBJ files. It is probably based on the (lost) 6 KB KIM-1 resident assembler ("KIM-5"). The later PET versions are not based on this (optimizations did not carry over), but the original (also lost) MDT650 version.

### ASSEMBLER_PET_&#x2a;_REC

Different versions of the PET Resident Assembler. Reconstructed from ASSEMBLER_TED, based on work by Martin Hoffmann-Vetter.

* ASSEMBLER_PET_REC (undated, [6696 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_Vxxxxxx_pet.prg)); for BASIC 2
* ASSEMBLER_PET_V112779_REC (1979-11-27, [7426 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V112779_pet.prg)); for BASIC 2
* ASSEMBLER_PET_V121579_REC (1979-12-15, [7546 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V121579_BASIC4_pet.prg) for BASIC 4; and with `BASIC4=0`: 1979-12-15, [7546 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V121579_BASIC2_pet.prg) for BASIC 2)
* ASSEMBLER_PET_V090580_A_REC (1980-09-05, [7858 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V090580_a_pet.prg))
* ASSEMBLER_PET_V090580_B_REC (1980-09-05, [7938 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V090580_b1_pet.prg); and with `LINES=$40`: 1980-09-05, [7822 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V090580_b2_pet.prg))
* ASSEMBLER_PET_V102780_REC (1980-10-27, [8049 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V102780_pet.prg))
* ASSEMBLER_PET_V26MAR82RR_REC (1982-03-26, [7982 bytes](https://www.pagetable.com/docs/cbmasm/resident_assembler_V26MAR82RR_pet.prg)); hacked V090580_B, non-mainline

### ASSEMBLER_C64_REC

The C64 Macro Assembler (a version of the Commodore Resident Assembler). Reconstructed from ASSEMBLER_TED to build a binary that is identical with the V080282 version, modulo the leftover garage in the original binary between $081A and $0990, which is filled with $AA.

### ASSEMBLER_TED

A version of the Commodore Resident Assembler for the TED series (Plus/4, C16 and C116; 1984). Extracted from [ted_kernal_basic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/plus4/ted_kernal_basic_src.tar.gz).

### ASSEMBLER_C128

The final version of the Commodore Resident Assembler, for the C128 (V022086; 1986).

### HCD65_3.5

The release version of the C128 HCD65 assembler. Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

### HCD65_65CE02&#x5f;&#x2a;

The internal updated version of the C128 HCD65 assembler with 65CE02/4510 support. Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

The 0.1 version was reconstructed from an LST file.

### LOADER_PET

The PET OBJ Loader, version V121379. Extracted from [ted_kernal_basic_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/plus4/ted_kernal_basic_src.tar.gz) and converted to uppercase and LST-style indenting. File loload.4 added.

### LOADER_C64

The OBJ Loader of the C64 Macro Assembler, version V072882. Extracted from [c64_kernal_bas_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz) and converted to uppercase and LST-style indenting.

### LOADER_C128, LOADER_C65, LOADER_C65_RS232

The OBJ Loader of the HCD65 assembler.

* LOADER_C128, LOADER_C65: for C128 and C65 (disk). Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)
* LOADER_C65_RS232: for C65 (RS-232). Source: [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)

### EDT_C128

The editor for the HCD65 assembler for the C128 (1986). Source: [c128_dev_pack.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c128/index.html)

### MONITOR_KIM

The KIM-1 "monitor" ROM (1975), i.e. 6530-002 and 6530-003. From the [Hans Otten reconstructed source from the KIM-1 users manual](http://retro.hansotten.nl/uploads/6502docs/usrman.htm).

### MONITOR_AIM65

The AIM-65 "monitor" ROM (1978), i.e. R3222 and R3223-11. From the [reconstructed source](http://retro.hansotten.nl/uploads/files/aimmon_tasm.zip) from the [monitor listing printout](http://retro.hansotten.nl/uploads/files/aim65_monitor.pdf). Corrected and fixed up to match the LST.

### KIMATH

The KIM-1 "KIMATH" math package (1976). Generated from the [original LST printout](http://retro.hansotten.nl/uploads/files/kimath.ZIP), and the .asm reconstruction by C. Bond from the same package.

### TIM

The TIM (Terminal Interface Monitor) ROM (6530-004). Reconstructed from the [manual](http://archive.6502.org/books/mos_tim_terminal_interface_monitor_manual_mar_1976.pdf) by Martin Hoffmann-Vetter.

## FIG

fig-FORTH for 6502, Rel 1.1 (1980) for various platforms. Converted from the [LST](https://ksquiggle.neocities.org/ff6502.htm).

This is not MOS/Commodore source code, but it was developed with some version of the Resident Assembler.

## KICKMAN

The game "Kickman" (1982) for the Ultimax. Reconstructed from the [BSO-format source](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz).

## OMEGA

The game "Omega Race" (1982) for the Ultimax. Reconstructed from the [BSO-format source](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz).

## WIZARD

The game "Wizard of Wor" (1982) for the Ultimax. Reconstructed from the [BSO-format source](http://www.zimmers.net/anonftp/pub/cbm/src/c64/c64_kernal_bas_src.tar.gz).

## A2232

The 65CE02 source of the firmware of the Amiga A2232 7-port serial card.

* `old.src`: 1988 version. Source: [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz)
* `a2232_6502.src`: Final version (1991).

## 6502ASM

The "6502ASM" cross-assembler, V1.0 (1989-08-23) for VAX and MS-DOS. Reconstructed from the V1.0 LIS/LST files in [c65_src.tar.gz](http://www.zimmers.net/anonftp/pub/cbm/src/c65/c65_src.tar.gz) and  the B0.0 C files in 4502-asm-for-pc.img from Steve Gray's [Dennis Jarvis Page](http://6502.org/users/sjgray/dj/).



# Credits

This repository is maintained by Michael Steil, mist64@mac.com
