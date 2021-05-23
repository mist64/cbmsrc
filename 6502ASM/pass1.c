#define LX extern
#include "asm.h"
#include "template.h"

int good_symbol(p)
char *p;
{
    int i=1;
    if(isalpha(*p) || *p=='.' || *p=='_')         /* global symbol */
        for(;i<32&&p[i];i++)
            if(!isalpha(p[i])&&!isdigit(p[i])&&p[i]!='.'&&p[i]!='_'&p[i]!='$'){
                display_error(error='C');
                return(0);
            }
    return(1);
}

int good_label(p)
char *p;
{
    int i;

    if(!*p)
        return(1);                      /* empty labels are okay */
    if(isdigit(*p)){                    /* must be a local symbol */
        for(i=error=0;isdigit(*p)&&*p;p++)
            i=i*10+*p-'0';
        if(*p++!='$')
            display_error(error='F');
        if(*p)
            display_error(error='F');
        if(i>255||i<1)
            display_error(error='F');
        return(!error);
    }
    return(good_symbol(p));
}

void get_label(p,endlocal)
char *p;
int endlocal;
{
    if(!*p || isspace(*p))
        return;                         /* no label */
    addr=TRUE;
    if(isdigit(*p)){                    /* must be a local symbol */
        int i;
        char s[32];
        for(i=0;isdigit(*p)&&*p;p++)
            i=i*10+*p-'0';
        makelocal(i,s);
        insert_symbol(s,loc_cntr,1,1,1);
    }else{
        insert_symbol(p,loc_cntr,1,1,0);
        if(endlocal)
            clear_local();
    }
    ptr->label=TRUE;
    if(ptr->multiple)
        display_error(error='M');
}
                                    
void pass1(macprmbgn,maclinptr,macargbgn)
char *macprmbgn,**maclinptr,*macargbgn;
{
    int done,macro,savprint;

    incl_sp=MAXINCL;
    while(incl_sp<=MAXINCL){            /* consider nested includes */

      while(get_line(macprmbgn,maclinptr,macargbgn)!=EOF){/* read to end of file or macro */
        old_loc=loc_cntr;               /* save location couter for listing */
        macro=wrt=addr=data=equ=0;
        print=known=1;                  /* unless otherwise determined */

        if(done = (*line==';'))         /* don't process comments */
            spechar= *line;

        if(!*line)
            done=spechar='1';

        if((!*s2 || *s2==';') && condit && !done && !strchr(s1,'=')){
            if(good_label(s1))          /* only label on line */
                get_label(s1,1);
            done=1;
        }

        s2[31]=0;
        tmpmac=macro_head;
        while(tmpmac&&!done&&condit){   /* check if this is a macro expansion */
            if(!strcmp(s2,tmpmac->name)){       /* is this a macro? */
                char *p;        /* temporary use to copy argument list */
                char *prmbgn;   /* pointer to beginning of parameter list */
                char *defbgn;   /* pointer to beginning of macro definition */
                char *argbgn;   /* pointer to beginning of argument list */
    /* use heap for argument list - don't fill up stack */
                if(*s1)
                    if(good_label(s1))
                        get_label(s1,1);
                p=argbgn=(char *)malloc(strlen(s3)+50);
                if(!p)
                    error_msg("Memory allocation failure during macro expansion.","",4);
                prmbgn=tmpmac->name;
                while(*prmbgn++);       /* point to start of parameter list */
                defbgn=prmbgn+2;
                while(*(defbgn-1)||*(defbgn-2))
                    defbgn++;           /* starts just after the double null */
                parses3(1);             /* argument list */
                while(*s3 || *(s3+1))   /* copy arg list until double null */
                    *p++= *s3++;
                *p++=0;
                *p=0;
                if(angleb|squote|dquote)
                    display_error(error='B');   /* unbalanced ",', or <> */
                print=macro=1;
                spechar='M';
                outline((char)1,(char)0,(char *)0,(char)0,(char)1);
                macrolevel++;           /* increment macro depth */
                pass1(prmbgn,&defbgn,argbgn);
                macrolevel--;           /* finished with macro */
                free(argbgn);           /* free memory used for arguments */
                break;                  /* macro found - stop searching */
            }
            tmpmac=tmpmac->nextmac;     /* not this macro - try next if any */
        }
        if(macro)
            continue;
        if(!done){
            savprint=print;         /* save print flag - directive changes it */
            if(!direct1(macprmbgn,maclinptr,macargbgn)) /* asm directive */
                if(!direct2()){
                    print=savprint; /* restore print flag if not directive */
                    s1=p1;
                    s2=p2;
                    s3=p3;
                    if(*p1)
                        if(good_label(p1))
                            get_label(p1,1);
                    opcode();       /* better be an instruction */
                }
        }

        if(!condit)
            print&=clist;
        outline(addr,data,d,equ,known);
      }
    if(macrolevel)
        return;
    fclose(file_src);
    if(incl_sp<MAXINCL){
        strcpy(srcfile,&filename[fnptr=filnamptr[incl_sp]]);
        fprintf(file_tmp,"%32.32sF%s\n",STATLINE,srcfile);
    }
    file_src=incl_stack[incl_sp++];
  }
}
