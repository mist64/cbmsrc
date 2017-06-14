# Commodore Source Code

This repository collects original source code of various Commodore Business Machines (CBM) computers.

## BASIC_C64

The Commodore 64 BASIC source (901226-01). Extracted from c64-basic.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

The source contains the ranges

* $A000-$BF52
* $BF71-$BFFF
* $E000-$E4AB

The checksum byte at $BF52 is 0 and will have to be calculated in a later step.

## KERNAL_C64

The Commodore 64 KERNAL source, original version (901227-01). Extracted from c64kernal.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

The source contains the ranges

* $E500-$FF5A: code
* $FF81-$FFF5: jump table
* $FFFA-$FFFF: vectors

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

## DOS_1540

The Commodore 1540 DOS source, 325302-01/325303-01. Extracted from 1541-2031-num1.d64 and 1541-2031-source-disk-copy-num2-side1.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks and tab indenting.

* The source contains both the IEEE-388 and the IEC serial interface code, so it represents both the CBM 2031 and the Commodor 1540. Different `master` source files build different configurations.
* The power-on message always says "`CBM DOS V2.6 V170`" though, which corresponds to the 1540.
* The Commodore 1541 is a minimally patched 1540, so this source is very close to the 1541 as well.

# Credits

This repository is maintained by Michael Steil, mist64@mac.com