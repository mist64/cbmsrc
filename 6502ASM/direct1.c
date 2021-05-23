#define LX extern
#include "asm.h"
#include "template.h"

void memallocerr(){
        printf("\n\n memory allocation failure!\n");
}

int pushcond(cond)
int cond;
{
    int i=1<<(++condlevel&15);
    if(cond)
        condstk[condlevel>>4]  |=  i;
    else
        condstk[condlevel>>4]  &= ~i;
    if(condlevel>=16*MAXCNDLEV-1)
        error_msg("conditional nesting depth exceeds maximum","",4);
    condit&=cond;
    return 1;
}

int direct1(macprmbgn,maclinptr,macargbgn)
char *macprmbgn,**maclinptr,*macargbgn;
{
    int i,j;
    long l;

    print=clist;
    spechar='1';

/* conditional assembly directives */

    if(!strcmp(s2,".ELSE")){
        if(!isspace(line[0]))
            display_error(error='F');
        if(condlevel){
            condstk[condlevel>>4]^=1<<(condlevel&15);
            for(condit=TRUE,i=1;condit&&i<=condlevel;i++)
                condit&=condstk[i>>4]>>(i&15)&1;
        }else
            display_error(error='N');       /* no if */
        return 1;
    }

    if(!strcmp(s2,".ENDIF")){
        if(!isspace(line[0]))
            display_error(error='F');
        if(condlevel){
            condlevel--;
            for(condit=TRUE,i=1;condit&&i<=condlevel;i++)
                condit&=condstk[i>>4]>>(i&15)&1;
        }else
            display_error(error='N');       /* no if */
        return 1;
    }

    if(!strcmp(s2,".IFB"  ) || !strcmp(s2,".IFNB"  ) ||
       !strcmp(s2,".IFIDN") || !strcmp(s2,".IFNIDN") ||
       !strcmp(s2,".IFDEF") || !strcmp(s2,".IFNDEF") ||
       !strcmp(s2,".IFN")   || !strcmp(s2,".IFE"   ) ||
       !strcmp(s2,".IFGT")  || !strcmp(s2,".IFGE"  ) ||
       !strcmp(s2,".IFLT")  || !strcmp(s2,".IFLE"  ) ){
        if(!isspace(line[0]))
            display_error(error='F');
        if(!condit)     /* not interest in type if condit is false */
            return(pushcond(condit));
        if(!strcmp(s2,".IFB")){
            i=strlen(s3)-1;
            if(*s3!='<' || s3[i]!='>' || i<1){
                display_error(error='F');
                return 1;
            }
            return(pushcond(i==1));
        }
        if(!strcmp(s2,".IFNB")){
            i=strlen(s3)-1;
            if(*s3!='<' || s3[i]!='>' || i<1){
                display_error(error='F');
                return 1;
            }
            return(pushcond(i>1));
        }
        if(!strcmp(&p2[strlen(s2)-3],"IDN")){ 
            parses3(1);
            while(*s3)
                s3++;
            s3++;
            if(!strcmp(s2,".IFIDN")){
                j=strlen(p3)-1;
                i=strlen(s3)-1;
                if(*s3!='<' || s3[i]!='>' || i<1 || *p3!='<' || p3[j]!='>' || j<1){
                    display_error(error='F');
                    return 1;
                }
                return(pushcond(!strcmp(s3,p3)));
            }
            if(!strcmp(s2,".IFNIDN")){
                j=strlen(p3)-1;
                i=strlen(s3)-1;
                if(*s3!='<' || s3[i]!='>' || i<1 || *p3!='<' || p3[j]!='>' || j<1){
                    display_error(error='F');
                    return 1;
                }
                return(pushcond( strcmp(s3,p3)));
            }
        }else{
            i=evaluate(s3,&l);      /* i is defined, l is value */
            equ=TRUE;
            d[data++]=(l>>8)&255;
            d[data++]=l&255;
            if(!i && !strcmp(&p2[strlen(s2)-3],"DEF"))
                for(error=j=0;j<5;j++){
                    if(error_field[j]=='U')     /* no undefined error if */
                        error_field[j]=' ';     /* testing for defined */
                    error|=!isspace(error_field[j]);
                }
            if(!strcmp(s2,".IFDEF"))
                return(pushcond(i));
            if(!strcmp(s2,".IFNDEF"))
                return(pushcond(!i));
            if(i){
                if(!strcmp(s2,".IFE"))
                    return(pushcond(l==0));
                if(!strcmp(s2,".IFGE"))
                    return(pushcond(l>=0));
                if(!strcmp(s2,".IFGT"))
                    return(pushcond(l>0));
                if(!strcmp(s2,".IFLE"))
                    return(pushcond(l<=0));
                if(!strcmp(s2,".IFLT"))
                    return(pushcond(l<0));
                if(!strcmp(s2,".IFN"))
                    return(pushcond(l!=0));
                display_error(error='O');
                return 1;
            }else{
                display_error(error='U');
                return 1;
            }
        }
    }
    if(!condit)
        return 1;      /* not interested in others if conditional false */

/* macro directives */

    print=0;
    if(!strcmp(s2,".REPT")){            /* repeat directive */
        if(!isspace(line[0]))           /* save label if any */
            if(good_label(s1))
                get_label(s1,1);
        sprintf(p1,"___T.M.P.$$$%08.8ld",tmpcounter++);
        i=evaluate(s3,&l);
        if(i){
            char *linptr,args[4],parm[4];
            struct macdef *reptmac;
            p3[0]=p3[1]=0;
            macro_def(macprmbgn,maclinptr,macargbgn,2);
            outline(addr=0,data=0,(char *)0,(char)0,(char)1);
            reptmac=macro_head;         /* copy macro address for deletion */
            for(i=0;i<l;i++){
                parm[0]=parm[1]=parm[2]=parm[3]=0;    
                args[0]=args[1]=args[2]=args[3]=0;    
                s3=reptmac->name;
                s3+=21;
                while(!*s3)
                    s3++;
                linptr=s3;
                macrolevel++;
                pass1(parm,&linptr,args);
                macrolevel--;
            }
            if((tmpmac=macro_head)==reptmac)
                macro_head=reptmac->nextmac;        /* unlink rept macro */
            else{
                while(tmpmac->nextmac!=reptmac)
                    tmpmac=tmpmac->nextmac;
                tmpmac->nextmac=reptmac->nextmac;   /* unlink rept macro */
            }
            free((char *)reptmac);  /* destroy temporary macro definition */
        }else
            display_error(error='U');
        addr=data=0;
        return 1;
    }

    if(!strcmp(s2,".IRPC")){            /* repeat directive */
        char *linptr,args[4],*parm,*chars;
        struct macdef *reptmac;
        if(!isspace(line[0]))           /* save label if any */
            if(good_label(s1))
                get_label(s1,1);
        sprintf(p1,"___T.M.P.$$$%08.8ld",tmpcounter++);
        while(*s3!=',' && *s3)
            s3++;
        *s3=0;
        if(!(chars=(char *)malloc(strlen(++s3)+2)))
            memallocerr();
        strcpy(chars,s3);
        s3=p3;
        macro_def(macprmbgn,maclinptr,macargbgn,3);
        outline(addr=0,data=0,(char *)0,(char)0,(char)1);
        reptmac=macro_head;         /* copy macro address for deletion */
        for(i=0;i<strlen(chars);i++){
            args[0]=chars[i];
            args[1]=args[2]=args[3]=0;    
            s3=reptmac->name;
            while(*s3)
                s3++;
            while(!*s3)
                s3++;
            parm=s3;
            while(*s3)
                s3++;
            while(!*s3)
                s3++;
            linptr=s3;
            macrolevel++;
            pass1(parm,&linptr,args);
            macrolevel--;
        }
        if((tmpmac=macro_head)==reptmac)
            macro_head=reptmac->nextmac;        /* unlink rept macro */
        else{
            while(tmpmac->nextmac!=reptmac)
                tmpmac=tmpmac->nextmac;
            tmpmac->nextmac=reptmac->nextmac;   /* unlink rept macro */
        }
        free((char *)reptmac);  /* destroy temporary macro definition */
        free((char *)chars);    /* return memory used for character string */
        addr=data=0;
        return 1;
    }

    if(!strcmp(s2,".IRP")){             /* repeat directive */
        char *linptr,*parm,*chars;
        struct macdef *reptmac;
        if(!isspace(line[0]))           /* save label if any */
            if(good_label(s1))
                get_label(s1,1);
        sprintf(p1,"___T.M.P.$$$%08.8ld",tmpcounter++);
        if(!(chars=(char *)calloc(strlen(p3),1)))
            memallocerr();
        parses3(1);
        while(*s3)
            s3++;
        s3++;
        i=0;
        while(*s3 || s3[1])
            chars[i++]= *s3++;
        s3=p3;
        macro_def(macprmbgn,maclinptr,macargbgn,4);
        outline((char)0,(char)0,(char *)0,(char)0,(char)1);
        reptmac=macro_head;         /* copy macro address for deletion */
        i=0;
        while(chars[i]){
            s3=reptmac->name;
            while(*s3)
                s3++;
            while(!*s3)
                s3++;
            parm=s3;
            while(*s3)
                s3++;
            while(!*s3)
                s3++;
            linptr=s3;
            macrolevel++;
            pass1(parm,&linptr,&chars[i]);
            macrolevel--;
            while(chars[i++]);      /* point to next arg */
        }
        if((tmpmac=macro_head)==reptmac)
            macro_head=reptmac->nextmac;        /* unlink rept macro */
        else{
            while(tmpmac->nextmac!=reptmac)
                tmpmac=tmpmac->nextmac;
            tmpmac->nextmac=reptmac->nextmac;   /* unlink rept macro */
        }
        free((char *)reptmac);  /* destroy temporary macro definition */
        free((char *)chars);    /* return memory used for character string */
        addr=data=0;
        return 1;
    }

    if(!strcmp(s2,".MACRO")  || !strcmp(s2,".MAC")){/* macro definition? */
        if(isspace(line[0]))
            display_error(error='F');          
        macro_def(macprmbgn,maclinptr,macargbgn,1);
        return 1;
    }

    if(!strcmp(s2,".INCLUDE")){     /* include ? */
        if(!isspace(line[0]))
            display_error(error='F');
        if(!incl_sp){
            display_error(error='N');
            return 1;
        }
        for(i=0;i<strlen(s3);i++)
            if(s3[i]<=' ' || s3[i]==';')
                s3[i]=0;            /* blank or ; terminates file name */
        incl_stack[--incl_sp]=file_src;
        filnamptr[incl_sp]=fnptr;
        strcpy(&filename[fnptr],srcfile);
        fnptr+=strlen(srcfile)+1;
        appendef(s3,".SRC");
        for(i=strlen(s3)-1;i>0;i--)
            if(s3[i]=='\\' || s3[i]==']' || s3[i]==':') /* explicit path? */
                break;                                  /* yes */
        if(i<=0){
            strcpy(srcfile,defipath);                   /* default incl path */
            strcat(srcfile,s3);
            strcpy(s3,srcfile);
        }
        for(i=0;srcfile[i]=toupper(s3[i]);i++);
        fprintf(file_tmp,"%sF%s\n",STATLINE,srcfile);
        file_src=fopen(s3,"r");
        if(!file_src){
            file_src=incl_stack[incl_sp++];
            error_msg("Could not open included file:",s3,0);
        }
        return(print=1);
    }
    return 0;
}
