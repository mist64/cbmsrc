# Commodore Source Code

This repository collects original source code of various Commodore Business Machines (CBM) computers.

## KERNAL_C64

The Commodore 64 KERNAL source, original version (901227-01). Extracted from c64kernal.d64 from Steve Gray's <a href="http://6502.org/users/sjgray/dj/">Dennis Jarvis Page</a> and converted to LF line breaks.

The source contains the ranges

* $E500-$FF5A: code
* $FF81-$FFF5: jump table
* $FFFA-$FFFF: vectors

It does not contain the version byte at $FF80 (which is $AA in the binary) or the "RRBY" signature at $FFF6.

# Credits

This repository is maintained by Michael Steil, mist64@mac.com