#define LX extern
#include "asm.h"
#include "template.h"

void clean_list(){
    char linestat[FIRSTCHAR+1],c,spec,formfeed='\n';
    int i,j,page=1,subpage=0,nline=999;

    fclose(file_tmp);
    file_tmp=fopen(tmpdev,"r");

    while(!feof(file_tmp)){
        for(c=i=linestat[FIRSTCHAR]=0;i<FIRSTCHAR && c!='\n';i++)
            c=linestat[i]=fgetc(file_tmp);

        if(c=='\n'){
            linestat[--i]=0;
            spec=' ';
            *line=0;
        }else{
            spec=fgetc(file_tmp);
            for(c=i=0;i<2*LINELENG && c!='\n' && c!=EOF;i++)
                c=line[i]=fgetc(file_tmp);
            line[--i]=0;
        }

        if(c==EOF)
            continue;
        if(spec=='F'){
            strcpy(srcfile,line);
            continue;
        }
        if(spec=='S'){          /* .subttl directive */
            parselin(0);
            strcpy(subttl,s3);        
        }
        if(nline>formln){
            nline=6;
            fprintf(file_lst,"%c%s  Commodore 6502ASM %s  %s  Page %d",
                    formfeed,name,VERSION,&datetime[4],page);
            formfeed='\f';
            if(subpage)
                fprintf(file_lst,"-%d",subpage);
            subpage++;
            fprintf(file_lst,"\n%s   %s\n",subttl,srcfile);
            fprintf(file_lst,"\nError Addr  Code          Seq   Source statement\n\n");
        }
        if(spec=='p'){          /* .page directive */
            nline=999;
            page++;
            subpage=0;
            continue;
        }
        if(spec=='f'){          /* .formln directive */
            for(formln=i=0;line[i];i++)
                formln=formln*10+line[i]-'0';
            continue;
        }
        if(spec=='s'){          /* .space or .skip directive */
            sscanf(line,"%d",&j);
            for(i=0;i<j;i++,nline++)
                if(nline<formln)
                    fprintf(file_lst,"\n");
            continue;
        }    
        if(spec=='L'){          /* .mlist, .nmlist, .blist, .list, .nlist */
            mlist=line[0]-'1';
            list=line[1]-'0';
            continue;
        }
        if(spec=='l')           /* .clist and .nclist directive */
            continue;
        macrolevel=linestat[29]!=' ';
        if(spec=='M' && !macrolevel)
            linestat[29]='+';
        data=isxdigit(linestat[12]);
        if(list)
            if(mlist==1 || !macrolevel || (mlist==0 && data)){
                if(!wide){
                    linestat[FIRSTCHAR-2]=0;
                    linestat[FIRSTCHAR-3]=' ';
                }
                fprintf(file_lst,"%s%s\n",linestat,line);
                nline++;
            }
    }
}

void outline1(file,addr,data,d,equ,known)
FILE *file;
char addr,equ,data,known,*d;
{
    int i;
    if(!print && !error)        /* don't print certain directives unless error */
        return;
    if(!list)                   /* don't print if .Nlist directive active */
        return;
    if(macrolevel)
        if(!addr && !data && mlist<1)   /* no code */
            return;

    fprintf(file,"%5s",error_field);
    if(!isspace(*error_field))
        errcount++;
    if(addr)
        fprintf(file,"%04.4lX   ",old_loc);
    else
        fprintf(file,"       ");
    if(equ)
        fprintf(file,"=%02X%02X   ",255&d[0],255&d[1]);
    else{
        if(word || dbyt)
            fprintf(file,"%02X%02X    ",255&d[word],255&d[dbyt]);
        else{
            if(data==0)
                fprintf(file,"        ");
            if(data==1)
                fprintf(file,"%02X      ",255&d[0]);
            if(data==2)
                fprintf(file,"%02X %02X   ",255&d[0],255&d[1]);
            if(data>=3 && !byte) 
                fprintf(file,"%02X %02X%02X ",255&d[0],255&d[2],255&d[1]);
            if(data>=3 &&  byte) 
                fprintf(file,"%02X %02X %02X",255&d[0],255&d[1],255&d[2]);
        }
    }
    if(known)
        fprintf(file," ");
    else
        fprintf(file,"\"");    
    if(pass==1){
        maclev=' ';
        if(macrolevel)
            maclev='A'+macrolevel-1;
        if(macrolevel>26)
            maclev='a'+macrolevel-27;
        if(macrolevel>52)
         maclev='*';
    }
    fprintf(file,"%8d%c  ",linenum-1,maclev);
    fprintf(file,"%c%s",spechar,line);
    if(gen)
        if(word || dbyt)
            for(i=2;i<data;i+=2)
                fprintf(file,"\n     %04lX   %02X%02X",
                        old_loc+i,255&d[i+word],255&d[i+dbyt]);
        else
            for(i=3;i<data;i++){
                if(!(i%3))
                    fprintf(file,"\n     %04lX   ",old_loc+i);
                fprintf(file,"%02X ",255&d[i]);
            }
    fprintf(file,"\n");
    if(macrolevel>maxmaclev && file==file_tmp){
        fclose(file_tmp);
        sprintf(p2,"%d",maxmaclev);
        error_msg("\nMacros nested too deep. Max depth = ",p2,2);
    }
}

void outhex(addr,data)
unsigned int addr;
char data;
{
    if(!binptr){
        sprintf( binbuf,";00%04X",binadr=addr);
        chksum=addr>>8&255;
        chksum+=addr&255;
        binptr=5;
    }
    if(addr==binadr && binptr<37){
        sprintf(&binbuf[binptr+=2],"%02X",data&255);
        chksum+=data&255;
        binadr++;
    }else{
        int i=(binptr-5)/2;
        chksum+=i;
        if(i>15){
            binbuf[1]='1';
            i&=15;
        }
        binbuf[2]=i+(i>9?'7':'0');
        sprintf(&binbuf[binptr+=2],"%04X\0",chksum);
        fprintf(file_obj,"%s\n",binbuf);
        binlin++;
        binptr=0;
        outhex(addr,data);
    }
    binadr=addr+1;
}

void outline(addr,data,d,equ,known)
char addr,equ,data,known,*d;
{
    int i;
    linenum++;
    outline1(file_tmp,addr,data,d,equ,known);
    if(prnerrcon && *error_field!=' ' && pass==2){
        outline1(stdout,addr,data,d,equ,known);
        errcount--;
    }
    print=error=0;
    dbyt=byte=word=FALSE;
    spechar=' ';
    if(!equ)
        codebytes+=data;
    if(emitobj && pass==2 && !equ)
        for(i=0;i<data;i++)
            outhex((unsigned int)(old_loc+i),d[i]);
}
