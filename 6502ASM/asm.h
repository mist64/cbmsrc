#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STATLINE "                                " /* spacer for status */
#define VERSION "V1.0"                  /* version number */
#define DEFCPUTYPE 3                    /* default to Commodore 4502 assembly */
#define LINELENG 134                    /* maximum line length */
#define FIRSTCHAR 32                    /* pos of source statement in list file */
#define MAXCNDLEV 128                   /* conditionals may be nested 16x this */
#define MAXINCL 6                       /* maximum include nesting depth */
#define TRUE 1
#define FALSE 0

/* LX must be defined as null in the main module and as extern in all others */

struct macdef{
        struct macdef *nextmac;         /* pointer to next macro definition */
        unsigned int size;              /* size of macro definition structure */
        char name[32];                  /* macro name */
/* a variable number of null terminated parameters followed by another null */
/* a variable number of null ternimated definition lines followed by another null */
};

struct xrfr{
        struct xrfr *next;              /* pointer to next reference */
        unsigned def      :1;           /* was symbol defined on this line? */
        unsigned wrt      :1;           /* is symbol an operand of a write op? */
        unsigned known    :1;           /* is symbol known at this line #? */
        unsigned int linenumb;          /* the line number for output */
        };

struct smbl{
        struct smbl *link[2];           /* pointers to subnodes of binary tree */
        struct xrfr *xref;              /* head of x-reference list */
        long value;                     /* symbol value */
        unsigned weight     :2;         /* balance state of binary tree node */
        unsigned defined    :1;         /* is symbol defined? */
        unsigned multiple   :1;         /* has symbol been multiply defined? */
        unsigned section    :1;         /* is symbol a section name? */
        unsigned equate     :1;         /* was symbol defined by =? */
        unsigned relocate   :1;         /* is it a relocatable symbol? */
        unsigned external   :1;         /* is symbol external? */
        unsigned known      :1;         /* symbol value known? */
        unsigned label      :1;         /* is a label */
        char name[32];                  /* symbol name */
        };

LX struct smbl *ptr;                    /* temp pointer to a symbol */
LX struct smbl *root_symtab;            /* root of symbol table tree */
LX struct macdef *tmpmac;               /* temp pointer for macro definitions */
LX struct macdef *newmac;               /* pointer used for new macro defs */
LX struct macdef *macro_head;           /* pointer to head of macro def list */

LX char lbn[4];                         /* local symbol block number */

LX int chksum;                          /* check sum for hex file lines */
LX int binptr;                          /* hex file line index */
LX int binlin;                          /* number of lines in obj file */
LX unsigned int binadr;                 /* expected address of next byte */
LX char binbuf[50];                     /* hex file line buffer */
LX long codebytes;                      /* number of bytes of code generated */

LX assumeL;                             /* assume long if true else assume short */
LX char maclev;                         /* macro level character code */
LX char spechar;                        /* special code in temp file */
LX int word;                            /* flag used to output .word data */
LX int dbyt;                            /* flag used to output .dbyte data */
LX int byte;                            /* flag used to output .byte data */
LX int pass;                            /* which pass (1 or 2) */
LX int curpass;                         /* current pass number (1 to n) */
LX int maxpass;                         /* maximum pass number */
LX int condit;                          /* conditional flag */
LX short condstk[MAXCNDLEV];    /* conditional nesting max 128*16=2048 levels */
LX int condlevel;               /* conditional assembly nesting depth */
LX int macrolevel;         /* 0=normal processing, else macro nesting depth */
LX int maxmaclev;          /* maximum macro nesting depth */
LX int cputype;            /* 0=org 6502, 1=CMOS 6502, 2=Rockwell, 3=4502 */
LX int forcelong;          /* require a 16 bit operand */
LX int errcount;           /* count of lines with errors */
LX char cur_sect[32];      /* the current section name - defaults to A */
LX int radix;              /* the default radix */
LX long loc_cntr;          /* the location counter */
LX long old_loc;           /* location counter at beginning of line */
LX int endflag;            /* has the .END statement been read? */
LX int expdef;             /* expression defined? */
LX int wrt;                /* is it a write opcode? */
LX int angleb;             /* angle bracket state */
LX int dquote;             /* double quote state */
LX int squote;             /* single quote state */
LX long tmpcounter;        /* number to append to temporary symbols */

/* list control switches and forms control options */

LX int print;              /* flag to tell whether or not to print line */
LX int list;               /* state of list control */
LX int mlist;              /* state of macro list control */
LX int clist;              /* conditional assembly listing state */
LX int wide;               /* default is wide 132 column listing */
LX int gen;                /* default is display all generated code */
LX int linenum;            /* line number */
LX int formln;             /* initial lines/page */
LX int prnxref;            /* list the cross reference ? */
LX int prnsymtab;          /* list the symbol table ? */
LX int absasm;             /* absolute or relocatable switch */
LX char error;             /* error flag */
LX char error_field[6];    /* error field for listing */
LX char name[LINELENG];    /* title */
LX char subttl[LINELENG];  /* subtitle */
LX char datetime[32];      /* date and time of assembly */

/* file stuff */

LX int prnerrcon;          /* print errors on console ? */
LX int emitobj;            /* print an object file ? */
LX int emitlst;            /* emit a list file ? */
LX char *src;              /* pointer to start src files in command line */
LX char srcfile[LINELENG]; /* the current source file name */
LX char objfile[LINELENG]; /* the name of the object file */
LX char defipath[LINELENG];/* default include path */
LX FILE *file_src;
LX FILE *file_lst;
LX FILE *file_tmp;
LX FILE *file_obj;
LX FILE *incl_stack[MAXINCL];/* open include file pointers */
LX char tmpdev[LINELENG];    /* device to put temporary file on */
LX char filename[MAXINCL*50];/* save file names for when returning from includes */
LX int filnamptr[MAXINCL];   /* pointer to file name in filename buffer */
LX int fnptr;                /* pointer to source filname in filename string */
LX int incl_sp;              /* nesting level of included files */

LX char comline[LINELENG];   /* the command line */
LX char p1[LINELENG],p2[LINELENG],p3[LINELENG],*s1,*s2,*s3;
LX char line[2*LINELENG];    /* extra space for macro expansion to extend line */

LX char d[256],addr,data,equ,known;

