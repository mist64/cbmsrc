# Commodore Source Code

This repository collects original source code of various Commodore Business Machines (CBM) computers converted to a modern encoding (ASCII, LF, indentation).

## BASIC_C64

The Commodore 64 BASIC source (901226-01). Extracted from c64-basic.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $A000-$BF52
* $BF71-$BFFF
* $E000-$E4AB

The checksum byte at $BF52 is 0 and will have to be calculated in a later step.

## KERNAL_C64

The Commodore 64 KERNAL source, original version (901227-01). Extracted from c64kernal.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

The source contains the ranges

* $E500-$FF5A: code
* $FF81-$FFF5: jump table
* $FFFA-$FFFF: vectors

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

## BASIC_CBM2_[AB], KERNAL_CBM2_[AB], EDITOR_CBM2

The CBM2 BASIC, EDITOR and KERNAL sources (1983/1984). The A version is from <a href="https://www.davidviner.com/cbm9.html">David Viner</a>, and the B version is from disks 67/68 of the <a href="http://www.zimmers.net/anonftp/pub/cbm/b/CBUG/">CBUG</a> library. The files have been converted to LF line breaks and LST-style indenting.

* The B version of BASIC contains two patches.
* The B version of the KERNAL contains minor modifications, and comes with alternate versions of some files as well as additional (test) sources.
* Except for file naming, the editor sources A and B were identical.

## DOS_4040

The CBM 4040/2040 DOS V2.1 source (1980). Extracted from 4040-source.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

* Comments say this is for the 2040. The 2040 and the 4040 have identical hardware, the 4040 is just a 4040 with DOS V2.

## DOS_8250

The CBM 8250/8050/4040 DOS V2.7 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

* The same ROM binary will work on the 8250 (DS/QD), the 8050 (SS/QD) and the 4040 (SS/DD), the differences will be handled at runtime (symbols "NSIDES", "DOS").
* Comments in the source call this "SUPER DOS".

## DOS_8070

The CBM 8070 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

* The CBM 8070 is an unreleased device with one 8" hard drive (drive #0) and one 8" double-sided floppy drive (drive #1, like CBM 8062).
* The power-on message says "CBM DOS V3.0 8070".
* The file copy code is stored on the hard disk and loaded on demand ("ldcopy").

## DOS_D9065

The CBM D9065 DOS V3.0 source (1981). Extracted from old-dos-sources.d81 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

* The D9065 is an unreleased 7.5 MB hard drive, probably very similar to the D9060/D9090.

## DOS_1540

The Commodore 1540 DOS V2.6 source (1980), 325302-01/325303-01. Extracted from 1541-2031-num1.d64 and 1541-2031-source-disk-copy-num2-side1.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and LST-style indenting.

* The source contains both the IEEE-488 and the IEC serial interface code, so it represents both the CBM 2031 and the Commodore 1540. Different `master` source files build different configurations.
* The power-on message always says "`CBM DOS V2.6 V170`" though, which corresponds to the 1540.
* The Commodore 1541 is a minimally patched 1540, so this source is very close to the 1541 as well.

## DOS_1571

The Commodore 1571 DOS source (3.0, 1986), updated version (310654-05). Extracted from 1571-source.d81 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

## DOS_1581

The Commodore 1581 DOS source (10.0, 1987), original version (318045-01). Extracted from 1581-source.d81 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

# Credits

This repository is maintained by Michael Steil, mist64@mac.com