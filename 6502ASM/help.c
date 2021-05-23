#include <stdio.h>
#include <stdlib.h>

void help(){

printf("\nthe assembler command line consists of the file names and switches\n");
printf("\nC65> [object],[listing]=source[,source]...[,source][/switch]...[/switch]\n");
printf("\nItems enclosed in brackets [...] are optional.\n");
printf("\nThe default file extentions are .OBJ , .LST and .SRC\n");
printf("\nSwitches  (either upper or lower case)\n");
printf("/A absolute assembly (default)\n");
printf("/Cn cpu instruction set /C0 for NMOS 6502   /C1 for CMOS 6502\n");
printf("     /C2 for CMOS 6502 w/bit instructions   /C3 for Commodore 4502 (default)\n");
printf("/Dpath specify path for intermediate file ( usually RAM disk on PCs).\n");
printf("/H help - prints this message\n");
printf("/L assume long branches on pass1. (default= assume short branches)\n");
printf("/Mnnn maximum macro nesting depth (default=50, limits=2-999).\n");
printf("/Pnn maximum number of passes to try (default=15, limits=2-99).\n");
printf("/N don't print errors to console during assembly\n");
printf("/R relocatable assembly - illegal since this is an absolute assembler only\n");
printf("/S narrow list format\n");
printf("/T don't print symbol table\n");
printf("/V don't print cross reference\n");
printf("/X print cross reference (default)\n");
}
