#define LX extern
#include "asm.h"
#include "template.h"

void pass2(){
    char linestat[FIRSTCHAR+1],spec,c;
    int i;
    long filepos,fileeof;

    fileeof=ftell(file_tmp);
    fseek(file_tmp,0L,0);
    while((filepos=ftell(file_tmp))<fileeof){
        for(c=i=0;i<5;i++)
            c=linestat[i]=error_field[i]=fgetc(file_tmp);
        for(;i<FIRSTCHAR;i++)
            c=linestat[i]=fgetc(file_tmp);
        linestat[FIRSTCHAR]=0;
        spec=fgetc(file_tmp);
        for(c=i=0;i<2*LINELENG && c!='\n';i++)
            c=line[i]=fgetc(file_tmp);
        line[--i]=0;
        if(spec!=' ' && spec!='M')
            continue;
        addr=data=equ=0;
        print=known=1;
        sscanf(&linestat[21],"%8d",&linenum);
        forcelong=!isspace(linestat[17]);
        maclev=linestat[29];
        old_loc=loc_cntr;
        parselin();
        if((spechar=spec)=='M'){
            addr=TRUE;
            *s2= *s3=0;
        }
        if(*s2 || strchr(s1,'=')){
            if(!direct2()){
                s1=p1;  s2=p2;  s3=p3;
                if(*s1)
                    if(good_label(s1))
                        get_label(s1,1); 
                opcode();
            }
        }else
            if(*s1)
                if(good_label(s1))
                    get_label(s1,1);
        fseek(file_tmp,filepos,0);
        outline(addr,data,d,equ,known);
        fseek(file_tmp,ftell(file_tmp),0);
    }
    fseek(file_tmp,fileeof,0);
}
