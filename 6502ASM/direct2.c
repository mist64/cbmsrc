#define LX extern
#include "asm.h"
#include "template.h"
 
int direct2(){
    spechar=' ';
    print=1;
    if(*s2=='.')
        return dir2dot();
    return dir2equ();
}

int dir2equ(){

/* handle the various forms of equates */

    long l;
    int i;
    char *sym=0,*equal=0,*exp=0,localsym[34];

    if(!strchr(p1,'=') && !strchr(p2,'=') && *s3!='=')
        return 0;                                       /* not equate */
    if(!(equal=strchr(line,'=')))                       /* search for = */
        return 0;                                       /* this isn't an equate */
    exp=strchr(line,';');                               /* check for comments */
    if(exp && exp<equal)
        return 0;                                       /* there was ; before = */
    for(exp=equal+1; isspace(*exp) && *exp     ;exp++); /* point to expression */
    for(sym=equal-1; isspace(*sym) && sym>=line;sym--); /* find non-space */
    for(           ;!isspace(*sym) && sym>=line;sym--); /* find space */
    sym++;                                              /* point to symbol */

    if(sym!=line && !isspace(*line))            /* optional label on line */
        if(good_label(s1))
            get_label(s1,1);

    known=evaluate(exp,&l);                     /* evaluate expression */
    equ=TRUE;
    if(*sym=='*')
        if(sym[1]!='=' && !isspace(sym[1]))
            display_error(error='S');
        else
            if(known)
                loc_cntr=l;
            else
                display_error(error='U');
    else{
        if(isdigit(*sym)){
            for(i=0;isdigit(*sym);sym++)
                i=i*10+*sym-'0';
            makelocal(i,localsym);
            insert_symbol(localsym,l,1,expdef,TRUE);
        }else{
            for(i=0;i<32;i++)
                if(isspace(sym[i]) || sym[i]=='='){
                    localsym[i]=0;
                    break;
                }else
                    localsym[i]=toupper(sym[i]);
            insert_symbol(localsym,l,1,expdef,FALSE);
        }
        ptr->value=l;
        ptr->known|=known;
        ptr->equate=TRUE;
    }
    data=2;
    d[0]=l>>8&255;
    d[1]=l&255;
    return 1;
}

int dir2dot(){
    int i,j;
    long l;

/* pass 2 directives */        

    if(!strcmp(s2,".END")){
        if(!isspace(line[0]))
            display_error(error='F');
        clear_local();
        if(condlevel)
            error_msg("Conditional in progress","",0);
        if(macrolevel)
            error_msg("Macro definition in progress","",0);
        return(endflag=1);
    }

    if(!strcmp(s2,".RADIX")  || !strcmp(s2,"RAD")  || !strcmp(s2,"RADI")){
        if(!isspace(line[0]))
            display_error(error='F');
        i=radix;                    /* save radix in case of error */
        radix=10;                   /* for radix operation */
        if(!evaluate(s3,&l))
            l=99;                   /* set up for error */
        if(l>1 && l<17)
            i=l;
        else
            display_error(error='F');
        radix=i;
        return 1;
    }

    if(!strcmp(s2,".ASECT")  || !strcmp(s2,".SECT")){
        if(!isspace(line[0]))
            display_error(error='F');
        while(i=(*s3!=',')&&*s3){
            *s3=toupper(*s3);
            s3++;
        }
        *s3=0;
        l=0;
        if(!strcmp(s2,".SECT")){
            if(!i && find_symbol(p3)==NULL)/* this must be a first occurance */
                if(toupper(s3[1])=='A' && toupper(s3[2])=='B' && 
                   toupper(s3[3])=='S' && (s3[4]==0 || s3[4]==','))
                    if(s3[4])
                        if(toupper(s3[5])=='L' && toupper(s3[6])=='O' &&
                           toupper(s3[7])=='C' &&         s3[8] =='=')
                            if(evaluate(&s3[9],&l)){
                                insert_symbol(p3,l,TRUE,TRUE,FALSE);
                                ptr->section=TRUE;          
                            }
        }
        if(!strcmp(s2,".ASECT")){
            if(!i && find_symbol(p3)==NULL) /* this must be a first occurance */
                if(toupper(s3[1])=='L' && toupper(s3[2])=='O' &&
                   toupper(s3[3])=='C'         && s3[4] =='=')
                    if(evaluate(&s3[5],&l)){
                        insert_symbol(p3,l,TRUE,TRUE,FALSE);
                        ptr->section=TRUE;
                    }
        }
        ptr=find_symbol(cur_sect);      /* current section */
        ptr->value=loc_cntr;            /* save current location counter */
        if((ptr=find_symbol(p3))!=NULL) /* find new section symbol */
            if(ptr->section){           /* check that it is a section name */
                loc_cntr=ptr->value;    /* set location counter if found */
                strcpy(cur_sect,p3);
            }else
                display_error(error='S');/* invalid section name */
        else
            display_error(error='U');   /* section name not found */
        return 1;
    }

    if(!strcmp(s2,".AORG")){        /* aorg ? */
        if(s1[0]&&good_label(s1))
            get_label(s1,1);
        if(!error && evaluate(s3,&l))
            loc_cntr=l;
        return 1;
    }

    if(!strcmp(s2,".RMB")){             /* reserve multiple bytes */
        if(!isspace(line[0]))           /* save label if any */
            if(good_label(s1))
                get_label(s1,1);
        parses3(1);
        if(known=evaluate(s3,&l))
            loc_cntr+=l;
        else
            display_error(error='U');
        return 1;
    }         

    if(!strcmp(s2,".WORD" )   || !strcmp(s2,".WOR") ||
       !strcmp(s2,".DBYTE")   || !strcmp(s2,".DBY") || !strcmp(s2,".DBYT")){
        if(!isspace(line[0]))
            if(good_label(p1))
                get_label(p1,1);
        if(s2[1]=='W')
            word=1;
        if(s2[1]=='D')
            dbyt=1;
        data=0;
        parses3(0);
        known=addr=1;
        while(*s3){
            if(j=evaluate(s3,&l))
                if(l>0xffffL||l<-32768L)
                    display_error(error='E');
            known&=j;
            if(word){
                d[data++]=l&255;
                d[data++]=l>>8&255;
            }else{
                d[data++]=l>>8&255;
                d[data++]=l&255;
            }
            while(*s3++);           /* point to next expression if any */
        }
        loc_cntr+=data;
        return 1;
    }

    if(!strcmp(s2,".BYTE")   || !strcmp(s2,".BYT")){
        if(!isspace(line[0]))
            if(good_label(p1))
                get_label(p1,1);
        byte=1;
        parses3(0);
        known=addr=1;
        data=0;
        while(*s3){
            if(isstring(s3)){
                char c,delimiter= *s3;
                while(c= *++s3){
                    if(c!=delimiter)
                        d[data++]=c;
                    else
                        if(*++s3==delimiter)
                            d[data++]= *s3;
                        else
                            s3--;
                }
            }else{
                known&=evaluate(s3,&l);
                if(l>255||l<-128)
                    display_error(error='E');
                d[data++]=l&255;
                while(*s3)
                    s3++;
            }
            s3++;
        }
        loc_cntr+=data;
        return 1;
    }

    if(!strcmp(s2,".LOCAL")){
        if(!isspace(line[0]))
            display_error(error='F');
        clear_local();
        return 1;
    }

/* list and forms control directives */

    if(!isspace(line[0])){
        display_error(error='F');
    }
    if(!strcmp(s2,".MLIST")  || !strcmp(s2,".MLI") || !strcmp(s2,".MLIS")){
        mlist=1;
        line[0]=mlist+'1';
        line[1]=list?'1':'0';
        line[2]=0;
        spechar='L';
        return 1;
   } 
   if(!strcmp(s2,".BLIST")  || !strcmp(s2,".BLI") || !strcmp(s2,".BLIS")){
        mlist=0 ;
        line[0]=mlist+'1';
        line[1]=list?'1':'0';
        line[2]=0;
        spechar='L';
        return 1;
    }
    if(!strcmp(s2,".NMLIST") || !strcmp(s2,".NML")
    || !strcmp(s2,".NMLI"  ) || !strcmp(s2,".NMLIS")){
        mlist= -1;
        line[0]=mlist+'1';
        line[1]=list?'1':'0';
        line[2]=0;
        spechar='L';
        return 1;
    }
    if(!strcmp(s2,".CLIST")  || !strcmp(s2,".CLI") || !strcmp(s2,".CLIS")){
        spechar='l';
        return(clist=1);
    }
    if(!strcmp(s2,".NCLIST") || !strcmp(s2,".NCL")
    || !strcmp(s2,".NCLI"  ) || !strcmp(s2,".NCLIS")){
        clist=0;
        spechar='l';
        return 1;
    }
    if(!strcmp(s2,".LIST")   || !strcmp(s2,".LIS")){
        if(list<1)
            list++;
        line[0]=mlist+'1';
        line[1]=list?'1':'0';
        line[2]=0;
        spechar='L';
        return 1;
    }
    if(!strcmp(s2,".NLIST")  || !strcmp(s2,".NLI") || !strcmp(s2,".NLIS")){
        list--;
        line[0]=mlist+'1';
        line[1]=list?'1':'0';
        line[2]=0;
        spechar='L';
        return 1;
    }
    if(!strcmp(s2,".GEN")){
        spechar='l';
        return(gen=1);
    }
    if(!strcmp(s2,".NOGEN")  || !strcmp(s2,".NOG") || !strcmp(s2,".NOGE")){
        gen=0;
        spechar='l';
        return 1;
    }
    if(!strcmp(s2,".PAGE")   || !strcmp(s2,".PAG")){
        if(!error)
            spechar='p';
        return 1;
    }
    if(!strcmp(s2,".SPACE")  || !strcmp(s2,".SKIP")){
        if(j=evaluate(s3,&l) && !error)
            sprintf(line,"%ld\0%c",l,spechar='s');
        else
            display_error(error='E');
        return 1;
    }
    if(!strcmp(s2,".FORMLN") || !strcmp(s2,".FORML") || !strcmp(s2,".FORM")){
        if(j=evaluate(s3,&l))
            sprintf(line,"%ld\0%c",l,spechar='f');
        else
            display_error(error='E');
        return 1;
    }
    if(!strcmp(s2,".SUBTTL")){
        if(!*subttl)
            strcpy(subttl,s3);
        spechar='S';
        return 1;
    }
    if(!strcmp(s2,".TITLE")  || !strcmp(s2,".NAM")){
        if(pass==1)
            return 1;
        if(strcmp(name,".MAIN.") && strcmp(name,s3))
            display_error(error='F');
        else                               
            strcpy(name,s3);
        return 1;
    }
    if(!strcmp(s2,".MESSG")){
        if(pass==2){
            if(s1=strchr(s3,';'))       /* trim comment */
                *s1=0;
            for(i=strlen(s3);i>0;i--)
                if(isspace(s3[i]))      /* trim trailing white space */
                    s3[i]=0;
                else
                    break;
            printf("\n%s\n",s3);
        }
        return 1;
    }
    return 0;
}

int isstring(p)
char *p;
{
    if(*p=='\'' || *p=='\"'){
        char c,delimiter= *p;
        while(c= *++p)
            if(c==delimiter)
                if(*++p!=delimiter)
                    return !*p;
   }else
        return 0;
}
