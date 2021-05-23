#define UNARYL2R 0
/* 1 BSO method       evaluate consecutive unary operators left to right */
/* 0 Normal method    evaluate consecutive unary operators right to left */
/* actually BSO evaluates right to left also, they have a documentation error */

#define RADIX2 '%'
#define RADIX8 '@'
#define RADIX16 '$'
#define MAXESTK 50          /* maximum size of evaluation stacks */

#define LX extern
#include "asm.h"
#include "template.h"

long stack[MAXESTK];         /* big enough for any expr that fits on a line */
char opstk[MAXESTK];         /* operator stack */           
int vsp,osp;                 /* stack pointers */
int lastval;                 /* was last token a value? */
char *lp;                    /* pointer to position in expression */

long evalsym(){
    char sym[32];            /* save symbol */
    int i=0;                 /* position of last character added to sym */

    sym[i++]=toupper(*(lp-1));
    while(isalpha(*lp) || isdigit(*lp) || *lp=='$' || *lp=='.' || *lp=='_'){
        sym[i]=toupper(*lp); /* gather the symbol name */
        lp++;
        if(i<31)
            i++;
    }
    sym[i]=0;                   /* terminate the symbol name */
    ptr=find_symbol(sym);       /* get a pointer to the symbol table entry */
    if(!ptr)
        insert_symbol(sym,0L,0,0,0);  /* insert an undefined symbol */
    else
        addref(&ptr->xref,0);   /* add a reference to symbol is it exists */
    if(ptr->section)            /* bad idea to use section names in expressions */
        display_error(error='S');
    if(ptr->multiple)           /* use of multiply defined symbol */
        display_error(error='S');
    if(ptr->known)              /* is the value of the symbol known? */
        return(ptr->value);     /* value of symbol */
    if(pass==2)
        display_error(error='U');
    expdef=FALSE;               /* symbol value is unknown */
    return(0L);
}

long evalcon(r)                 /* evaluate a constant in radix r */
int r;  
{
    char c;   
    long con=0;

    while(isxdigit(*lp)){
        c=toupper(*lp)-'0';
        if(c>9)
            c-=7;
        if(c<0 || c>=r)
            return(con);
        con*=r;
        con+=c;
        lp++;
    }
    return(con);
}
             
long evalocal(){                /* evaluate a local symbol */
    int loc=0;
    char *p,sym[8];

    for(p=lp;isdigit(*p);p++)
        loc=loc*10+*p-'0';      /* get the decimal # part of local symbol */
    if(*p == '$'){              /* yes it is a local symbol */
        lp= ++p;                /* point past the $ */
        if(loc<1 || loc>255){   /* it is a valid local symbol name */
            display_error(error='E');
            return(0L);
        }
        makelocal(loc,sym);
        ptr=find_symbol(sym);       /* get a pointer to the symbol table entry */
        if(!ptr)
            insert_symbol(sym,0L,0,0,1);/* insert an undefined symbol */
        if(ptr->section)            /* bad idea to use section names in expressions */
            display_error(error='S');
        if(ptr->multiple)           /* use of multiply defined symbol */
            display_error(error='S');
        if(ptr->known)              /* is the value of the symbol known? */
            return(ptr->value);     /* value of symbol */
        if(pass==2)
            display_error(error='U');
        expdef=FALSE;               /* symbol value is unknown */
        return(0L);
    }
/* wasn't local symbol - must be a constant */
    return(evalcon(radix));
}

long evalchar(t)
char t;
{
    long con=0;
    char c;
    while(c= *lp){                      /* test for end of field */
        lp++;
        if(t!=c)                        /* test for terminator */
            con=(con<<8)+c;
        else
            if(*lp==t){                 /* if double terminator use as char */
                con=(con<<8)+*lp;
                lp++;
            }else
                break;                  /* terminator found */
    }
    if(!con || con&0xffff0000)          /* error if more than 2 chars or */
        display_error(error='V');       /* no characters and no terminator */
    return(con);                        /* end of expression found */
}

int priority(c)
 char c;
{
    if(c=='$')                                          return(0);
    if(c=='(')                                          return(1);
    if(c=='+' || c=='-')                                return(2);
    if(c=='*' || c=='/' || c=='&' || c=='|' || c=='^')  return(3);
    display_error(error='C');       /* invalid operator on stack */
    return(9);
}

void pushval(l)
long l;
{ 
    int k=0;
    do_unary(&l,&k);            /* do unary operations before pushing value */
    stack[--vsp]=l;             /* push value */
    osp+=k;
    lastval=TRUE;               /* last item scanned was a value */
}

void do_unary(l,k)              /* perform unary operations */
long *l;
int *k;
{
    char c;

#if UNARYL2R

    c=opstk[*k+osp];    /* unary operations */
    if(c=='>' || c=='<' || c=='~' || c=='_'){
        *k+=1;
        do_unary(l,k);
    }else
        return;
    if(c=='>'){         *l=(*l>>8)&255;         return; } 
    if(c=='<'){         *l&=255;                return; }
    if(c=='~'){         *l= ~*l;                return; }
    if(c=='_'){         *l= -*l;                return; }

#else

    *k=0;
    while(osp<MAXESTK){
        c=opstk[osp];
        if(c=='>'){     *l=(*l>>8)&255; osp++;  continue; }
        if(c=='<'){     *l&=255;        osp++;  continue; }
        if(c=='~'){     *l= ~*l;        osp++;  continue; }
        if(c=='_'){     *l= -*l;        osp++;  continue; }
        break;
    }

#endif

}

void do_oper(){                 /* perform binary operations */

    long l=stack[vsp++];
    char c=opstk[osp++];
    if(vsp>MAXESTK){    vsp=MAXESTK;    display_error(error='C');}
    if(osp>MAXESTK){    osp=MAXESTK;    display_error(error='C');}   
    lastval=TRUE;
    switch(c){
        case '+':           stack[vsp]+=l;      break;
        case '-':           stack[vsp]-=l;      break;
        case '*':           stack[vsp]*=l;      break;
        case '^':           stack[vsp]^=l;      break;
        case '&':           stack[vsp]&=l;      break;
        case '|':           stack[vsp]|=l;      break;
        case '/':           if(l)     stack[vsp]/=l;    /* divide by 0? */
                            else      display_error(error='E');
                            break;
        default:            display_error(error='C');
    }
}   

void eval(op)
char op;
{
    int more=TRUE;
    if(op=='_' || op=='<' || op=='>' || op=='~' || op=='('){/* unary operators */
        opstk[--osp]=op;
        more=lastval=FALSE;
    }
    while(more)
        if(priority(opstk[osp])<priority(op)){
            opstk[--osp]=op;
            more=lastval=FALSE;
        }else
            do_oper();
}    

int evaluate(str,value)
char *str;
long *value;
{
    int i;
    char c;

    expdef=TRUE;                /* assume expression will be defined */
    lastval=FALSE;
    lp=str;
    osp=vsp=MAXESTK;
    opstk[--osp]='$';
    while(c= *lp){
        if(isspace(c)||c==';')
            break;
        if(osp<=0 || vsp<=0)
            error_msg("evaluation stack overflow ",str,4);
        if(osp>MAXESTK || vsp>MAXESTK)
            error_msg("evaluation stack underflow ",str,4);
        lp++;                              /* point to next character */

        if(isalpha(c) || c=='.' || c=='_'){/* it must be a symbol */
            pushval(evalsym());
            continue;
        }
        if(c=='\"'||c=='\''){              /* it is a character constant */
            pushval(evalchar(c));
            continue;
        }
        if(isdigit(c)){                    /* it is a decimal number */
            lp--;                          /* point to the digit again */
            pushval(evalocal());
            continue;
        }
        if(c==RADIX2){                     /* it is a binary constant */
            pushval(evalcon(2));
            continue;
        }
        if(c==RADIX8){                     /* it is an octal constant */
            pushval(evalcon(8));
            continue;
        }
        if(c==RADIX16){                    /* it is a hexidecimal constant */
            pushval(evalcon(16));
            continue;
        }

        if(c=='!'){                        /* start of 2 character operator */
            c=toupper(*lp);                /* get the rest of the operator */
            lp++;
            if(!(c=='N' || c=='.' || c=='+' || c=='X'))
                display_error(error='E');
            else{
                if(c=='N')
                    c='~';
                if(c=='.')
                    c='&';
                if(c=='+')
                    c='|';
                if(c=='X')
                    c='^';
                eval(c);
            }
            continue;
        }
        if(c=='*'){                          /* multiply or location counter */
/* if preceded by nothing or an operator then it is the location counter */
            if(!lastval)
                pushval(loc_cntr);
            else
/* if preceded by a constant or symbol then it is the multiply operator */
                eval(c);
            continue;
        }
        if(c=='>' || c=='<' || c=='/'){
            eval(c);
            continue;
        }
        if(c=='+'){                            /* unary or binary operator */
/* if preceded by nothing or an operator then it is a unary +, ignore it */
/* if preceded by a constant or symbol then it is a binary operator */
            if(lastval)
                eval(c);
            continue;
        }
        if(c=='-'){                            /* unary or binary operator */
/* if preceded by nothing or an operator then it is a unary operator */
            if(!lastval)
                c='_';
/* if preceded by a constant or symbol then it is a binary operator */
            eval(c);
            continue;
        }
        if(c=='('){
            eval(c);
            continue;
        }
        if(c==')'){
            int more=TRUE;
            while(more)
                if(opstk[osp]=='('){
                    more=FALSE;
                    osp++;
                    pushval(stack[vsp++]);
                }else
                    if(opstk[osp]=='$'){
                        display_error(error='B');
                        more=FALSE;
                    }else
                        do_oper();
            continue;
        }
        display_error(error='C');       /* no valid state if we get here */
    }
    while(opstk[osp]!='$')
        do_oper();
    for(i=osp;i<MAXESTK;i++)
        if(opstk[i]=='(')
           display_error(error='B');    /* too many ( in expression */
    if(vsp!=MAXESTK-1)
           display_error(error='E');    /* stack was messed up */
    if(error)
        stack[vsp]=0;
    *value=stack[vsp];
    return(expdef);
}

