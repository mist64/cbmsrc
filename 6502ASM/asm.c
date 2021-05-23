#include <time.h>
#define LX 
#include "asm.h"
#include "template.h"

unsigned _STACK = 8192;             /* stack size for lattice compiler */
                                    /* has no effect on VAX */

int main(argc,argv)
int argc;
char *argv[];
{
    long oldbytes;
    int i,first=TRUE;
    open_files(argc,argv);              /* get command line */
    while(*src){                        /* as long as there are more src files */
        for(i=0;srcfile[i]=toupper(src[i]);i++);
        appendef(srcfile,".SRC");
        fprintf(file_tmp,"%sF%s\n",STATLINE,srcfile);
        for(i=strlen(srcfile);i>0;i--)  /* extract path, if any for tmp file */
            if(srcfile[i]=='\\' || srcfile[i]==']' || srcfile[i]==':'){
                strcpy(defipath,srcfile);/* copy src file name */
                defipath[i+1]=0;        /* terminate after path */
                break;
            }
        if(first){
            if((file_src=fopen(srcfile,"r"))==NULL)
                error_msg("error opening source file:",srcfile,4);
        }else
            if(!freopen(srcfile,"r",file_src))
                error_msg("error opening source file:",srcfile,4);
        while(*src++);                  /* point to next file */
        pass1((char *)NULL,(char **)NULL,(char *)NULL); /* pass1 processor */
        first=FALSE;
    }
    curpass=2;
    oldbytes=1000000000L;               /* for word rel. branch compression */
    while(codebytes!=oldbytes
         && curpass<=maxpass){          /* if no change in obj length */
        pass=2;
        if(prnerrcon){                  /* check if ok to print to console */
            printf("\nstarting pass %d.\n",curpass);
            if(curpass>2)
                printf("  %ld byte change.\n",codebytes-oldbytes);
        }
        oldbytes=codebytes;             /* save for next compare */
        binlin=binadr=binptr=chksum=0;
        codebytes=errcount=loc_cntr=0;
        lbn[0]=lbn[1]=lbn[2]=lbn[3]='A';/* reinitial local block number */
        if(emitobj){
            fclose(file_obj);           /* close obj file */
            file_obj=fopen(objfile,"w+");/* reopen it */
            fseek(file_obj,0L,0);       /* position at beginning of file */
        }
        pass2();                        /* pass2 processor */
        curpass++;
        if(cputype!=3)                  /* check cputype (/cx switch) */
            oldbytes=codebytes;     /* don't repeat if no word relative inst */
    }
    if(emitobj){                        /* close obj file */
        outhex((unsigned int)(loc_cntr+5),(char)0);     /* number of lines */
        fprintf(file_obj,";00%04X%04X\n",binlin,(255&binlin>>8)+(binlin&255));
    }
    if(emitlst){                        /* list file requested? */
        clean_list();                   /* output list file */
        if(prnsymtab){                  /* symbol table requested? */
            list_sym();                 /* output symbol table */
            macro_tab();                /* output macro table */
        }
    }
    fclose(file_tmp);                   /* close temporary file */
    remove(tmpdev);                     /* delete temporary file */
    return(1);                          /* close files, release memory, exit */
}

void open_files(argc,argv)              /* open files and initialize things */
int argc;
char *argv[];
{
    int i;
    char *p,c,listfile[LINELENG];
    long tyme;

    assumeL=FALSE;
    wide=absasm=prnerrcon=prnsymtab=prnxref=TRUE;
    cputype=DEFCPUTYPE;                 /* /cx overrides default */
    maxpass=15;                         /* default maximum number of passes */
    maxmaclev=50;                       /* default maximum macro nesting depth */
    *tmpdev=0;
    if(argc>1)                          /* check command line */
        strcpy(comline,argv[1]);        /* get command line */
    else{
        printf("\nC65>");               /* prompt user for instructions */
        scanf("%s",comline);            /* read command line from console */
    }
    if(!strcmp(comline,"/H") || !strcmp(comline,"/h")){
        help();
        exit(0);
    }
    emitobj=0;                          /* assume no obj file */
    p=comline;                          /* pointer to command line */
    while(*p!=',' && *p!='=' && *p!='/' && *p)
        objfile[emitobj++]= *p++;       /* get obj file name from command line */
    objfile[emitobj]=0;                 /* terminate obj file name */
    appendef(objfile,".OBJ");
    for(i=strlen(objfile)-1;i>0;i--)    /* extract path, if any for tmp file */
        if(objfile[i]=='\\' || objfile[i]==']' || objfile[i]==':'){
            strcpy(tmpdev,objfile);     /* copy list file name */
            tmpdev[i+1]=0;              /* terminate after path */
            break;
        }
    emitlst=0;                          /* assume no list file */
    if(*p++==',')                       /* was there a comma in command line? */
        while(*p!=',' && *p!='=' && *p!='/' && *p)
            listfile[emitlst++]= *p++;  /* get list file name from command line */
    listfile[emitlst]=0;                /* terminate lst file name */
    appendef(listfile,".LST");
    for(i=strlen(listfile);i>0;i--)     /* extract path, if any for tmp file */
        if(listfile[i]=='\\' || listfile[i]==']' || listfile[i]==':'){
            strcpy(tmpdev,listfile);    /* copy list file name */
            tmpdev[i+1]=0;              /* terminate after path */
            break;
        }
    if(*p!='=')                         /* find src file(s) */
        error_msg("too many output files specified","",2);    /* error */
    if(*p=='=')                         /* src files */
        src= ++p;               /* pointer to beginning of source file list */

    while(*p){                  /* get switches */
        while(*p!='/' && *p)    /* find a slash */
            p++;
        if(*p=='/'){
            p++;
            switch(toupper(c= *p)){
            case 'A':   absasm=TRUE;                    break;
            case 'L':   assumeL=TRUE;                   break;  
            case 'N':   prnerrcon=FALSE;                break;
            case 'S':   wide=FALSE;                     break;
            case 'T':   prnsymtab=FALSE;                break;
            case 'V':   prnxref=FALSE;                  break;
            case 'X':   prnxref=TRUE;                   break;
            case 'H':   help();                         exit(0);
            case 'M':   for(maxmaclev=0;
                            isdigit(*++p)&&maxmaclev<100;
                            maxmaclev=maxmaclev*10+*p-'0');
                        if(maxmaclev>1&&maxmaclev<1000)
                            break;
                        error_msg("bad /M switch - max macro depth must be 2-999","",2);
            case 'P':   for(maxpass=0;
                            isdigit(*++p)&&maxpass<100;
                            maxpass=maxpass*10+*p-'0');
                        if(maxpass>1&&maxpass<100)
                            break;
                        error_msg("bad /P switch - maxpass must be 2-99","",2);
            case 'C':   cputype= *++p-'0';
                        if(cputype>=0 && cputype<=3)    break;
                        error_msg("illegal parameter after /C switch",p,2);
            case 'D':   i=0;
                        while(c= *++p){
                            if(c=='/')
                                break;
                            tmpdev[i++]=c;
                            tmpdev[i]=0;
                        }
                                                        break;
        case 'R':   error_msg("Illegal switch, absolute assembly only.",p,2);
            default:    error_msg("unknown switch",p,2);
            }
        }
    }
    tmpnam(s1=p1);                      /* generate temporary file name */
    if(*s1=='\\')                       /* PC puts a backslash here */
        for(;*s1;s1++)                  /* remove it */
            *s1=s1[1];
    strcat(tmpdev,p1);
    strcat(tmpdev,".TMP");
    if(emitobj)
        if((file_obj=fopen( objfile,"w" ))==NULL)
            error_msg("Could not open object file: ",objfile,4);
    if(emitlst)
        if((file_lst=fopen(listfile,"w"))==NULL)
            error_msg("Could not open list file: ",listfile,4);
    if((file_tmp=fopen(tmpdev,"w+"))==NULL)
        error_msg("Could not open temp file: ","",4);
    p=src;
    while(*p&&!isspace(*p)){
        if(*p==',')
            *p=0;               /* null separates src file names */
        if(*p=='/')             /* end of list of source files */
            break;
        p++;
    }
    *p++=0;
    *p++=0;                     /* double null at end of list of source files */

    if(!*src)
        error_msg("command syntax error","",2); /* no source files */

/* initialize a few things */

    lbn[0]=lbn[1]=lbn[2]=lbn[3]='A';
    dbyt=word=byte=FALSE;
    macro_head=NULL;                        /* no macros defined yet */
    root_symtab=NULL;                       /* symbols defined yet */
    pass=1;
    codebytes=binlin=binadr=binptr=chksum=0;
    fnptr=forcelong=errcount=tmpcounter=0;
    condlevel=macrolevel=endflag=loc_cntr=subttl[0]=0;
    radix=10;
    condit=TRUE;
    linenum=list=mlist=gen=1;
    clist=1;
    formln=55;
    strcpy(name,".MAIN.");                  /* default title */
    spechar=' ';
    strcpy(cur_sect,"A");                   /* default section name */
    insert_symbol(cur_sect,0L,TRUE,TRUE,TRUE);  /* enter section name in table */
    ptr->section=TRUE;                      /* flag it as a section name */
    time(&tyme);
    strcpy(datetime,asctime(localtime(&tyme)));/* get time and date */
    for(i=strlen(datetime)-1;i && isspace(datetime[i]);i--)
        datetime[i]=0;
}

void display_error(c)
char c;
/* append another error to error field if this error has not occurred before */
{
    char *p=error_field;
    while(*p){
        if(*p==' ')
            *p=c;
        if(*p==c)
            break;
        p++;
    }
}

void error_msg(msg,info,fatal)
char *msg,*info;
int fatal;
/* serious error messages */
{
    printf("\n%s%s",msg,info);
    if(fatal<=1)
        return;
    fclose(file_tmp);                   /* close temporary file */
    remove(tmpdev);                     /* delete temporary file */
    exit(fatal);
}

void appendef(file,ext)
char *file,*ext;
{
    char *pos;                          /* position of ] if any */
    pos=strchr(file,']');               /* end of vax path */
    if(!strchr(pos?pos:file,'.'))       /* already a dot? */
        strcat(file,ext);               /* if not, append extention */
}
