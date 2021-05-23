#define LX extern
#include "asm.h"
#include "template.h"

/* 
returns a line from the current source file or from the current macro
*/
void parselin(){
    int i=0,j;
    char linestat[FIRSTCHAR+1];

    while(s3=strchr(line,'\f')){         /* search for form feeds */
        for(linestat[FIRSTCHAR]=j=0;j<FIRSTCHAR;linestat[j++]=' ');
        fprintf(file_tmp,"%s%c%s\n",linestat,'p'," .page"); /* generate .page */
        *s3=' ';
    }

    p1[0]=p2[0]=p3[0]=0;        /* fields */
    s1=p1;  s2=p2;  s3=p3;      /* pointers to fields */
 
    j=strlen(line)-1;           /* remove trailing spaces */
    if(j<0)
        return;
    while(isspace(line[j])&&j)
        line[j--]=0;
    if(j<0)
        return;
    if(line[i]!=';' && line[i]){/* don't get fields for comments & blank lines */
        if(!isspace(line[i]))
            for(j=0;!isspace(line[i])&&line[i];i++,j++)
                p1[j]=toupper(line[i]);
        p1[j]=0;
        while(isspace(line[i]))
            i++;
        for(j=0;!isspace(line[i])&&line[i];j++,i++)
            p2[j]=toupper(line[i]);
        p2[j]=0;
        while(isspace(line[i])&&line[i])
            i++;
        while(line[i])
            *s3++=line[i++];    /* the rest of the line */
        *s3++=0;
        *s3++=0;
        s3=p3;
    }
    if(*s2==';')
        *s2=0;
    if(*s3==';')
        *s3=0;
}

void parses3(enablang)
int enablang;
{
    angleb=squote=dquote=0;

    if(*s3=='='){           /* special 4 field case "label symbol = exp" */
        s3++;
        while(isspace(*s3))
            s3++;           /* 3rd field is "= exp" */
    }
    while(*s3 && (*s3!=';' || squote || dquote)){
        char *pnt;
        if(*s3=='~'){       /* special treatment for the tilde char */
            pnt=s3+1;       /* save pointer to char after char after ~ */
            if(!*pnt){      /* if last char is tilde add a space */
                *pnt=' ';
                *(pnt+1)=0;
            }
            while(*s3){     /* shift field */
                *s3= s3[1];
                s3++;
            }
            s3=pnt;         /* pointing to char after char after ~ */
            continue;
        }
        if(*s3=='\'' && !dquote)        /* if sngl quote not in dbl quotes */
            squote=!squote;             /* toggle single quote status */
        if(*s3=='\"' && !squote)        /* if dbl quote not in sngl quotes */
            dquote=!dquote;             /* toggle double quote status */
        if(*s3=='<' && !dquote && !squote)  /* count angle brackets < */
            angleb++;
        if(*s3=='>' && !dquote && !squote)  /* count angle brackets > */
            angleb--;
        if(!(enablang*angleb|dquote|squote)){   /* if delimiters are balanced */
            if(*s3==','){               /* if comma - separate subfields */
                *s3=0;                  /* then terminate sub-field */
                while(isspace(*(pnt=s3+1)))
                    while(*pnt){
                        *pnt=pnt[1];
                        pnt++;
                    }
            }
            if(isspace(*s3))            /* end of field */
                break;
        }
        s3++;                           /* scan next character */
    }
    *s3++=0;                            /* field ends in double null */
    *s3=0;
    s3=p3;
}

void macsubst(parm,arg,subbed)              /* substitute macro args for parms */
char *parm,*arg,*subbed;
{
    int angleb,dif,i,j,k,la,lp,ll;
    la=strlen(arg);
    lp=strlen(parm);
    ll=strlen(line);
    angleb=(arg[0]=='<') && (arg[la-1]=='>');  /* is arg in <> ? */
    for(i=0;i<=ll-lp;i++){
        if(strncmp(&line[i],parm,lp))       /* search for parms */
            continue;                       /* no match */
        for(k=j=0;!j&&k<lp;k++)
            j|=subbed[k+i];                 /* test if part of recent subs */
        if(j)
            continue;                       /* don't replace if part of sub */
        dif=la-lp;                          /* length diff of parm and arg */
        dif-=2*angleb;                      /* 2 less if angle brackets */
        if(dif>0)
            for(j=strlen(line)+dif+1;j>i;j--){  /* arg is longer than parm */
                line[j]=line[j-dif];        /* shift line */
                subbed[j]=subbed[j-dif];    /* and sub'd flags */
            }
        if(dif<0)
            for(j=i;j<strlen(line)+dif+1;j++){  /* parm is longer than arg */
                line[j]=line[j-dif];        /* shift line */
                subbed[j]=subbed[j-dif];    /* and sub's flags */
            }

        for(j=angleb;j<la-angleb;i++,j++){  /* subst arg for parm */
            subbed[i]=1;                    /* substitution made */
            line[i]=arg[j];                 /* insert arg */
        ll=strlen(line);                    /* line length may have changed */
        }
        i--;                                /* will be incremented by for */
    }
}

int get_line(macprmbgn,maclinptr,macargbgn) 
/*
    get a line from current source or include file 
    or from the expansion of the current macro
*/
char *macprmbgn,**maclinptr,*macargbgn;
{
    int i,c;
    char *macprmptr,*macargptr,*maclinp;

    error_field[5]=error=line[0]=0;
    error_field[0]=error_field[1]=
    error_field[2]=error_field[3]=error_field[4]=' ';
    if(macrolevel){
        c='\n';
        maclinp= *maclinptr;            /* address of next line of macro def */
/*
        expand current macro
*/
        if(*maclinp){                   /* if more lines left of this macro */
            char *subbed;
            subbed=(char *)calloc(LINELENG,2);
            strcpy(line,maclinp);       /* copy the line */
            while(*maclinp++);          /* point to start of next line */
            *maclinptr=maclinp;         /* for next call to this function */
/* 
        make substitutions 
*/
            macprmptr=macprmbgn;        /* start of parameter list */
            macargptr=macargbgn;        /* start of argument list */
            while(*macprmptr){          /* check if any parameters remain */
                macsubst(macprmptr,macargptr,subbed);/* make substitutions */
                while(*macprmptr++);    /* next parameter */
                if(*macargptr)
                    while(*macargptr++);/* next argument */
            }
            free(subbed);
        }else
            c=EOF;
    }else
/*
        read source line
*/
        for(i=0; (c=fgetc(file_src)) != EOF ;i++)
            if(c!='\n')
                if(i<LINELENG-1)
                    line[i]=c;
                else{
                    error_msg("Error reading source file","",error=1);
                    for(c=fgetc(file_src);c!=EOF&&c!='\n';c=fgetc(file_src));
                }
            else{
                if(endflag){
                    i=0;
                    if(endflag++==1)            /* only display error once */
                    error_msg("Additional input after end statement ignored","",0);
                }
                line[i]=0;                      /* terminate line */
                break;
            }

    if(c==EOF)
        line[0]=0;
    else
        parselin();
    return(c);
}
