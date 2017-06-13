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

# Credits

This repository is maintained by Michael Steil, mist64@mac.com