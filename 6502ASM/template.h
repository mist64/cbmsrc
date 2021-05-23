/*
        function templates for each function in the source files
*/
/*
        asm.c
*/
void help();                            /* print help screen */
void open_files(int,char **);           /* open files, scan com line, init stuff */
void display_error(char);               /* add an error to error field */
void error_msg(char *,char *,int);      /* print serious errors and may die */
void appendef(char *,char *);           /* append default extention */
/*
        line.c
*/
void parselin();                        /* extract fields, lab op/dir operand */
void parses3(int);                      /* process operand if necessary */
void macsubst(char *,char *,char *);    /* do macro substitutions */
int get_line(char *,char **,char *);    /* ret line from macro exp or src file */
/*
        outline.c
*/
void clean_list();                               /* output final list file */
void outline1(FILE *,char,char,char *,char,char);/* output to tmp or lst file */
void outhex(unsigned int,char);                  /* output hex to obj file */
void outline(char,char,char *,char,char);        /* output to tmp or lst file */
/*
        pass1.c
*/
void pass1(char *,char **,char *);      /* pass1 processor */
void get_label(char *,int);             /* enter a label in symbol table */
int good_label(char *);                 /* verify that label is valid */
int good_symbol(char *);                /* verify that symbol is valid */
/*
        pass2.c
*/
void pass2();                           /* assembler pass2 */
/*
        direct1.c
*/
int direct1(char *,char **,char *);     /* scan line for assembler directive */
int pushcond(int);                      /* save current conditional status */
/*
        direct2.c
*/
int direct2();                          /* calls dir2dot and dir2equ */
int dir2equ();                          /* handles equate directives */
int dir2dot();                          /* handles pass2 directives with . */
int isstring(char *);                   /* returns true for simple string */
/*
        opcode.c
*/
void opcode();                          /* scan line for legal operations */
int searchstr(char *,char *,int);       /* search for opcode match */
char parseopr();                        /* parse the operand */
/*
        macrodef.c
*/
void macro_def(char *,char **,char *,int);/* add macro definition to macro table */
void macro_tab();                       /* print the macro table */
/*
        express.c
*/
long evalsym();                         /* get a symbols value */
long evalcon(int);                      /* evaluate a constant - parm is radix */
long evalocal();                        /* evaluate a local symbol */
long evalchar(char);                    /* evaluate a character constant */
int priority(char);                     /* return priority of operator */
void pushval(long);                     /* push a value on evaluation stack */
void do_oper();                         /* perform binary operation */
void do_unary(long *,int*);             /* perform unary operations */
void eval(char);                        /* decide what to do with an operator */
int evaluate(char *,long *);            /* evaluate an expression */
/*
        symbol.c
*/
char *allocsym(int);                    /* allocate memory for a new symbol */
void head(int);                         /* link symbol in table */
void xchg(int,int);                     /* exchange to symbols in table */
void savepath(struct smbl *,int);       /* path to find symbol in binary tree */
void move();                            /* move symbol to new tree location */
void addref(struct xrfr **,int);        /* add an xref entry */
void makelocal(int,char *);             /* make a unique name for local sym */
void insert_symbol(char *,long,int,int,int);/* insert a new symbol in table */
struct smbl *find_symbol(char *);       /* return a pntr to symbol table entry */
void output_symbol(struct smbl *);      /* output a symbol table entry */
void list_sym();                        /* print the symbol table */
void prnref(struct xrfr *);             /* output a single reference */
void clear_local();                     /* increment local symbol block number */
/*
        help.c
*/
void help();                            /* about command line and switches */


