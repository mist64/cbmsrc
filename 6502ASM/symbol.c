#define XREFALLOC 512        /* how many xref entries to alloc at a time */ 
#define SYMBLKSZ 4096        /* how much memory for symbol table entries to alloc at once */ 
#define LEFT 0 
#define RIGHT 1 
#define BALANCED 2 
#define LX extern 
#include "asm.h" 
#include "template.h" 

/* this source file contains the functions for symbol table insertion, look up
   and listing. The symbol table format is a balanced binary tree. Each entry 
   in the symbol table consists of two pointers to the symbols in the next level
   of the tree, the value of the symbol, a flag indicating the balance at this
   node and some other attributes, a pointer to the head of the cross reference
   list, and the symbol name. The pointers are 4 bytes each, the value is 4
   bytes, the flag 2 bytes, and the symbol is from 2 to 32 bytes long including
   the terminating null byte. A one character symbol requires a 20 byte entry.
   A 31 character symbol requires a 50 byte entry. Symbols longer than 31 
   characters are truncated to 31 characters.
*/

/* acknowledgement: Introduction to Computer Organization and Data Structures
                    by Stone Siewiorek, sect 11.2.3 Height-balanced Trees   */

struct xrfr *xrefheap;       /* block of memory allocated for xref entries */
int xrefcnt=0;               /* entries remaining in currect xrefheap */
struct smbl *path[30];       /* the path followed through the tree to reach node */
int direction[30];           /* left or right branch taken */
int level;                   /* the depth of the tree search */
int refknt;                  /* number of entries in cross reference table */
int symknt;                  /* number of permanent symbol table entries */
int locknt;                  /* number of local symbol table entries */
int symblk= -1;              /* number of blocks of memory allocated for symbols */
int symbol_ptr=SYMBLKSZ;     /* how much memory of current buffer is used */
char *symbol_buf;            /* the current buffer for symbol table entries */

char *allocsym(symlen)
int symlen;
{
/* returns a pointer for use as a symbol table entry */

    int symptr=symbol_ptr;
    symbol_ptr+=sizeof(struct smbl)-31+symlen;
    if(symbol_ptr>=SYMBLKSZ){
        symbol_buf=(char *)calloc(SYMBLKSZ,1);
        if(!symbol_buf)
            error_msg("Available memory exceeded storing symbol","",4);
        symptr=0;
        symbol_ptr=sizeof(struct smbl)-31+symlen;
        symblk++;
    }
    return(symbol_buf+symptr);
}

void head(i)
int i;
{
/* link sub-tree rooted at critical node  */

    if(level)
        path[level-1]->link[direction[level-1]]=path[i];
    else
        root_symtab=path[i];            /* first entry */
}

void xchg(a,b)
int a,b;
{
/* exchange a pair of nodes in the symbol table tree */

    path[a]->link[ path[a]->weight]=path[b]->link[!path[a]->weight];
    path[b]->link[!path[a]->weight]=path[a];
}

void savepath(ptr,dir)
struct smbl *ptr;
int dir;
{
/* keep track of the path used to reach point of insertion */

    direction[++level]=dir;
    path[level]=ptr;
}

void move(){
    if(direction[level]==direction[level+1]){
        head(level+1);
        xchg(level,level+1);
        path[level]->weight=path[level+1]->weight=BALANCED;
    }else{
        head(level+2);
        xchg(level,level+2);
        xchg(level+1,level+2);
        path[level+2]->weight=BALANCED;
        if(direction[level+1]==direction[level+2]){
            path[level]->weight=BALANCED;
            path[level+1]->weight=!direction[level+1];
        }else
            if(direction[level+2]!=BALANCED){
                path[level]->weight=!direction[level];
                path[level+1]->weight=BALANCED;
            }else
                path[level]->weight=path[level+1]->weight=BALANCED;
    }
}

void addref(xref,def)
int def;
struct xrfr **xref;
{
    struct xrfr *new;
    if(pass>1)
        return;
    if(--xrefcnt<=0){
        xrefcnt=XREFALLOC;
        if(!(xrefheap=(struct xrfr *)calloc(XREFALLOC,sizeof(struct xrfr))))
            error_msg("Available memory exceeded adding reference","",4);
    }
    new=xrefheap++;             /* pointer to memory to be used for xref */
    new->linenumb=linenum;      /* line number of reference */
    new->def=def;               /* is symbol defined on this line? */
    new->wrt=wrt;               /* is it a write operation to this symbol? */
    new->known=ptr->known;      /* is symbol defined at this point? */
    new->next= *xref;           /* pointer to next reference */
    *xref=new;                  /* add the reference to the list */
}

void makelocal(i,s)
int i;
char *s;
{    
/*
        local symbols are stored in the permanent symbol table. The only
        difference between permanent and local symbols is that cross
        references to local symbols are not kept and that local symbols
        are not printed in the symbol table listing.

        local symbol names consist of a - follow by a 4 character
        base 26 local symbol block number AAAA BAAA CAAA ... YZZZ ZZZZ,
        followed by the 2 hex digit local symbol number. this function
        creates the local symbol name.
*/
    int j=i>>4&15;
    i&=15;
    s[0]='-';               /* the first character indicating a local symbol */
    s[1]=lbn[0];            /* the local symbol block number */
    s[2]=lbn[1];
    s[3]=lbn[2];
    s[4]=lbn[3];
    s[5]=j+(j>9?'7':'0');   /* the local symbol number in hex */
    s[6]=i+(i>9?'7':'0');
    s[7]=0;                 /* null to terminate string */
}

void insert_symbol( sym, value,def,known,local)
char *sym;
long value;
int def,known,local;
{
int i,k;

/* see if symbol already exists and find parent of new entry if not */

    level= -1;
    sym[31]=0;          /* truncate symbol if longer than 31 chars */
    ptr=root_symtab;
    while(ptr){
        i=strcmp(ptr->name,sym);
        savepath(ptr,i<0);
        if(i)
            ptr=ptr->link[i<0];
        else{
            if(!local)
                addref(&ptr->xref,def);
            if(def && ptr->defined && (!equ || !ptr->equate) && pass==1)
                ptr->multiple=1;                /* symbol is multiply defined */
            ptr->defined|=def;
            if(def && known && !ptr->multiple){
                ptr->value=value;
                ptr->known=1;
            }
            return;
        }
    }

/* create a new symbol table entry */

    k=strlen(sym);
    if(k>31)                                                 
        k=31;   
    ptr=(struct smbl *)allocsym(k);
    for(i=0;(i<k)&&(ptr->name[i]=sym[i]);i++);  /* copy symbol to table entry */
    ptr->name[k]=0;                             /* terminating null character */
    ptr->value=value;                           /* value of symbol */
    ptr->link[0]=ptr->link[1]=NULL;             /* no child nodes yet */
    ptr->xref=NULL;                             /* set up for xref list */
    ptr->weight=BALANCED;
    ptr->defined=def;
    ptr->known=known;
    ptr->label=ptr->equate=ptr->section=
    ptr->relocate=ptr->multiple=ptr->external=0;
    if(!local)
        addref(&ptr->xref,def);
    savepath(ptr,BALANCED);
    head(level);                /* link the new symbol entry into the tree */

/* balance the binary tree */

    while(level--){
        i=path[level]->weight;
        if(i==direction[level]){
            move();
            break;
        }else{
            path[level]->weight=direction[level];
            if(i!=BALANCED){
                path[level]->weight=BALANCED;
                break;
            }
        }
    }
}

struct smbl *find_symbol(sym)
char *sym;
{                               /* find the address of a symbol's entry */
struct smbl *pntr;
int i;
    pntr=root_symtab;
    while(pntr)                 /* while not a terminal node */
        if(i=strcmp(pntr->name,sym))    /* if symbol name not equal */
            pntr=pntr->link[i<0];       /* try the appropriate sub-node */
        else
            return(pntr);       /* found the symbol in table */
    return(NULL);               /* symbol not found */
}

void output_symbol(pntr)
struct smbl *pntr;
{
    char c;
    symknt++;
    if(pntr->link[0])
        output_symbol(pntr->link[0]);
    if(pntr->name[0]=='-')
        locknt++;
    else{
        c=' ';
        if( pntr->equate)       c='=';          /* equated symbol */
        if( pntr->multiple)     c='M';          /* multiply defined symbol */
        if(!pntr->defined)      c='?';          /* undefined symbol */
        if( pntr->section)      c='S';          /* program section name */
        fprintf(file_lst,"\n%c %04lX %-31s ",c,pntr->value&0xffffL,pntr->name);
        if(prnxref)
            prnref(pntr->xref);
    }
    if(pntr->link[1])
        output_symbol(pntr->link[1]);
}

void list_sym(){
    refknt=locknt=symknt=0;
    fprintf(file_lst,"\nErrors detected in %d line",errcount);
    if(errcount!=1)
        fprintf(file_lst,"s");
    fprintf(file_lst,".\n\f\n");
    fprintf(file_lst," value name                            ");
    if(prnxref)
        fprintf(file_lst,"cross reference");
    fprintf(file_lst,"\n");
    if(root_symtab)
        output_symbol(root_symtab);
    fprintf(file_lst,"\n\nMemory used to store %d local symbols: %d. bytes."
        ,locknt,26*locknt);
    fprintf(file_lst,"\n\nMemory used to store %d permanent symbols: %d. bytes."
        ,symknt-locknt,SYMBLKSZ*symblk+symbol_ptr-locknt*26);
    if(!prnxref)
        return;
    fprintf(file_lst,"\n\nMemory used for cross reference table: %ld. bytes."
        ,(long)refknt*(long)sizeof(struct xrfr));
} 

void prnref(tmp)
struct xrfr *tmp;
{
    int i=0;
    char c;
    while(tmp){
        c=' ';                  /* nothing interesting about this reference */
        if(tmp->def)
            c='#';              /* symbol defined on this line */
        else
            if(tmp->wrt)
                c='$';          /* symbol is operand of write instruction */
            else
                if(!tmp->known)
                    c='?';      /* symbol is undefined here */
        fprintf(file_lst,"%6d%c",tmp->linenumb,c);
        tmp=tmp->next;
        refknt++;
        i++;
        if(i>(wide?12:4)){
            i=0;
            fprintf(file_lst,"\n       %31s "," ");
        }
    }
}

void clear_local(){             /* increment the local symbol block number */
    int i;
    for(i=0;i<4;i++){
        if(++lbn[i]<='Z')
            return;
        lbn[i]='A';
    }
}
