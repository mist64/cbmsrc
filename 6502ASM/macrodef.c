#define LX extern
#include "asm.h"
#include "template.h"

#define CHUNK 512       /* increment of memory to allocate for macro def */

void macro_def(macprmbgn,maclinptr,macargbgn,mactype)
char *macprmbgn,**maclinptr,*macargbgn;
int mactype;                    /* 1=.MACRO 2=.REPT 3=.IRP 4=.IRPC */
{
    unsigned int i,l,m,macrocnt=1;
    
    spechar='D';
    print=1;
    outline((char)0,(char)0,(char *)0,(char)0,(char)1);
    newmac=(struct macdef *)malloc(l=CHUNK); /* initial size of structure */
/*
  save macro name 
*/
    for(m=i=0;s1[i];i++)
        newmac->name[m++]=s1[i];                /* copy name */
    newmac->name[31]=newmac->name[m++]=0;       /* terminate name string */
    m=m>32?32:m;                                /* fix up if name was > 31 chars */
    if(!strlen(newmac->name))                   /* check for missing name */
        display_error(error='D');       /* missing macro name */
/*
  get macro's parameter list
*/
    parses3(1);                                 /* separate fields in operand */
    while(*s3 || s3[1])                         /* copy parms to macro definition */
        newmac->name[m++]= *s3++;
    newmac->name[m++]=0;
    newmac->name[m++]=0;
/*
  store the macro definition lines
*/
    i=1;
    while(i){
        get_line(macprmbgn,maclinptr,macargbgn);/* get a source or macro exp line */
        print=1;
 
        if(strcmp(s2,".MACRO")==0 ||        /* check for nested macro defs */
           strcmp(s2,".IRP"  )==0 || 
           strcmp(s2,".IRPC" )==0 ||
           strcmp(s2,".REPT" )==0)
                macrocnt++;
        if(strcmp(s2,".ENDM" )==0 ||
           strcmp(s2,".ENDR" )==0)
                macrocnt--;

        if(!strcmp(s2,".END"))
            error_msg(".END encountered during definition of macro: "
                       ,newmac->name,4);

        spechar='D';
                                        /* check for end of macro definition */
        if(((!strcmp(s2,".ENDM") && mactype==1)   ||
            (!strcmp(s2,".ENDR") && mactype>1 ))  && !macrocnt){            
            newmac->name[m++]=0;        /* add a null indicating macro end */
            newmac->size=m+8;
            if(error){
                free((char *)newmac);   /* don't save macro if error */
                return;
            }
            newmac->nextmac=macro_head; /* link macro definition into list */
                                        /* truncated memory block */
            tmpmac=(struct macdef *)realloc((char *)newmac,m+8);
            if(!(long)(macro_head=tmpmac))
                error_msg("Available memory exceeded",newmac->name,4);
            return;                     /* finished */
        }
        outline((char)0,(char)0,(char *)0,(char)0,(char)1);
                                        /* check for macro def in macro */
        if(m+8+LINELENG>l){             /* alloc more memory if needed */
            tmpmac=(struct macdef *)realloc((char *)newmac,l+=CHUNK);
            if(!(long)tmpmac)           /* check if allocation sucessful */
                error_msg("Available memory exceeded",newmac->name,4);
            newmac=tmpmac;              /* allocation was successful */
        }
        if(strlen(line)){
            strcpy(&newmac->name[m],line);  /* copy the line */
            m+=1+strlen(line);              /* add length of line and null */
        }else{
            newmac->name[m++]=' ';          /* blank line */
            newmac->name[m++]=0;
        }
    }
}

void macro_tab(){
    long s=0;
    int i=0;
    char *p;
    if(tmpmac=macro_head)
        fprintf(file_lst,"\n\n size  name                            parameters\n");
    while(tmpmac){
        i++;
        s+=tmpmac->size;
        fprintf(file_lst,"%5u. %-31s",tmpmac->size,tmpmac->name);
        p=tmpmac->name;
        while(*p++);
        while(*p){
            fprintf(file_lst," %s",p);
            while(*p++);
        }
        fprintf(file_lst,"\n");
        tmpmac=tmpmac->nextmac;
    }
    if(macro_head)
        fprintf(file_lst,"\nMemory used to store %d macro definitions: %ld. bytes.\n",i,s);
    else
        fprintf(file_lst,"\n\nNo macros defined.\n");
}
