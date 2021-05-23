#define LX extern
#include "asm.h"
#include "template.h"

/*
        0 = standard NMOS 6502 from MOS Technology
        1 = CMOS 6502 from WDC, NCR and others
        2 = CMOS 6502 with bit inst. from Rockwell, VTI
        3 = CMOS 4502 from Commodore 

Least significant nibble 0 1 2 3 4 5 6 7 8 9 A B C D E F   most sig. nibble */

char mpulevel[256]={     0,0,3,3,1,0,0,2,0,0,0,3,1,0,0,2,  /* 00 */
                         0,0,1,3,1,0,0,2,0,0,1,3,1,0,0,2,  /* 10 */
                         0,0,3,3,0,0,0,2,0,0,0,3,0,0,0,2,  /* 20 */
                         0,0,1,3,1,0,0,2,0,0,1,3,1,0,0,2,  /* 30 */
                         0,0,3,3,3,0,0,2,0,0,0,3,0,0,0,2,  /* 40 */
                         0,0,1,3,3,0,0,2,0,0,1,3,3,0,0,2,  /* 50 */
                         0,0,3,3,1,0,0,2,0,0,0,3,0,0,0,2,  /* 60 */
                         0,0,1,3,1,0,0,2,0,0,1,3,1,0,0,2,  /* 70 */
                         1,0,3,3,0,0,0,2,0,1,0,3,0,0,0,2,  /* 80 */
                         0,0,1,3,0,0,0,2,0,0,0,3,1,0,1,2,  /* 90 */
                         0,0,0,3,0,0,0,2,0,0,0,3,0,0,0,2,  /* A0 */
                         0,0,1,3,0,0,0,2,0,0,0,3,0,0,0,2,  /* B0 */
                         0,0,3,3,0,0,0,2,0,0,0,3,0,0,0,2,  /* C0 */
                         0,0,1,3,3,0,0,2,0,0,1,3,3,0,0,2,  /* D0 */
                         0,0,3,3,0,0,0,2,0,0,0,3,0,0,0,2,  /* E0 */
                         0,0,1,3,3,0,0,2,0,0,1,3,3,0,0,2}; /* F0 */
/*
        single byte opcodes - 42.
*/
char nm0[]="AUGBRKCLCCLDCLECLICLVDEXDEYDEZINXINYINZNOPPHAPHPPHXPHYPHZPLAPLPPLXPLYPLZRTIRTSSECSEDSEESEITABTAXTAYTAZTBATSXTSYTXATXSTYATYSTZA";
char op0[]={0x5C,0x00,0x18,0xD8,0x02,0x58,0xB8,0xCA,0x88,0x3B
           ,0xE8,0xC8,0x1B,0xEA,0x48,0x08,0xDA,0x5A,0xDB,0x68
           ,0x28,0xFA,0x7A,0xFB,0x40,0x60,0x38,0xF8,0x03,0x78
           ,0x5B,0xAA,0xA8,0x4B,0x7B,0xBA,0x0B,0x8A,0x9A,0x98,0x2B,0x6B};
/*
        accumulator addressing mode - 8.
*/
char nm1[]="ASLASRDECINCLSRNEGROLROR";
char op1[]={0x0A,0x43,0x3A,0x1A,0x4A,0x42,0x2A,0x6A};
/*
        reset and set bit - 16.
*/
char nm2[]="RMB0RMB1RMB2RMB3RMB4RMB5RMB6RMB7SMB0SMB1SMB2SMB3SMB4SMB5SMB6SMB7";
char op2[]={0x07,0x17,0x27,0x37,0x47,0x57,0x67,0x77
           ,0x87,0x97,0xa7,0xb7,0xc7,0xd7,0xe7,0xf7};
/*
        br if reset and br if set - 16.
*/
char nm3[]="BBR0BBR1BBR2BBR3BBR4BBR5BBR6BBR7BBS0BBS1BBS2BBS3BBS4BBS5BBS6BBS7";
char op3[]={0x0f,0x1f,0x2f,0x3f,0x4f,0x5f,0x6f,0x7f
           ,0x8f,0x9f,0xaf,0xbf,0xcf,0xdf,0xef,0xff};
/*
        (bp,X) addressing mode - 8.
*/
char nm4[]="ADCANDCMPEORLDAORASBCSTA";
char op4[]={0x61,0x21,0xC1,0x41,0xA1,0x01,0xE1,0x81};
/*
        absolute or bp addressing modes - 57.

        assembler cannot usually distinguish between these addesssing
        modes by syntax alone. the value of the operand must be
        examined to determine whether or not it is less than 0xff.

A 0 in the table indicates address mode not implement for that instruction.
*/
char nm5[]="ADCANDASLASRASWBITCMPCPXCPYCPZDECDEWEORINCINWLDALDXLDYLDZLSRORAPHDROLRORROWSBCSTASTYSTXSTZTRBTSB";
char op5[]={0x65,0x25,0x06,0x44,0   ,0x24,0xC5,0xE4,0xC4,0xD4
           ,0xC6,0xC3,0x45,0xE6,0xE3,0xA5,0xA6,0xA4,0   ,0x46
           ,0x05,0,   0x26,0x66,0   ,0xE5,0x85,0x84,0x86,0x64,0x14,0x04};/*bp*/
char op6[]={0x6D,0x2D,0x0E,0   ,0xCB,0x2C,0xCD,0xEC,0xCC,0xDC
           ,0xCE,0   ,0x4D,0xEE,0   ,0xAD,0xAE,0xAC,0xAB,0x4E
           ,0x0D,0xFC,0x2E,0x6E,0xEB,0xED,0x8D,0x8C,0x8E,0x9C,0x1C,0x0C};/*abs*/
/*
        # immediate addressing mode - 16.
*/
char nm7[]="ADCANDBITCMPCPXCPYCPZEORLDALDXLDYLDZORAPHDRTNSBC";
char op7[]={0x69,0x29,0x89,0xC9,0xE0,0xC0,0xC2,0x49,0xA9,0xA2
           ,0xA0,0xA3,0x09,0xF4,0x62,0xE9};
/*
        absolute,X or bp,X addressing modes - 38.

        assembler cannot usually distinguish between these addesssing
        modes by syntax alone. the value of the operand must be
        examined to determine whether or not it is less than 0xff.

A 0 in the table indicates address mode not implement for that instruction.
*/
char nm8[]="ADCANDASLASRBITCMPDECEORINCLDALDYLDZLSRORAROLRORSBCSTASTYSTZ";
char op8[]={0x75,0x35,0x16,0x54,0x34,0xD5,0xD6,0x55,0xF6,0xB5
           ,0xB4,0   ,0x56,0x15,0x36,0x76,0xF5,0x95,0x94,0x74};/*bp*/
char op9[]={0x7D,0x3D,0x1E,0,   0x3C,0xDD,0xDE,0x5D,0xFE,0xBD
           ,0xBC,0xBB,0x5E,0x1D,0x3E,0x7E,0xFD,0x9D,0x8B,0x9E};/*abs*/
/*
        (bp),Y addressing mode - 8.
*/
char nma[]="ADCANDCMPEORLDAORASBCSTA";
char opa[]={0x71,0x31,0xD1,0x51,0xB1,0x11,0xF1,0x91};
/*
        absolute,Y addressing mode - 10.
*/
char nmb[]="ADCANDCMPEORLDALDXORASBCSTASTX";
char opb[]={0x79,0x39,0xD9,0x59,0xB9,0xBE,0x19,0xF9,0x99,0x9B};
/*
        bp,Y addressing mode - 2.
*/
char nmc[]="LDXSTX";
char opc[]={0xB6,0x96};
/*
        relative short, relative long addressing modes - 18.

        assembler cannot usually distinguish between these addesssing
        modes by syntax alone. the value of the operand must be
        examined to determine whether or not it is in the range -128 to 127.
*/
char nmd[]="BCCBCSBEQBMIBNEBPLBRABRUBVCBVS";
char opd[]={0x90,0xB0,0xF0,0x30,0xD0,0x10,0x80,0x80,0x50,0x70};
char ope[]={0x93,0xB3,0xF3,0x33,0xD3,0x13,0x83,0x83,0x53,0x73};
/*
        (bp) or (bp),Z addressing mode - 8.
*/
char nmf[]="ADCANDCMPEORLDAORASBCSTA";
char opf[]={0x72,0x32,0xD2,0x52,0xB2,0x12,0xF2,0x92};
/*
        (d,SP),Y addressing mode - 2.
*/
char nmg[]="LDASTA";
char opg[]={0xE2,0x82};
/*
        calls and jumps - 7.
*/
char nmh[]="BSRJMPJSR";
char oph[]={0x63,0x4c,0x6C,0x7C,0x20,0x22,0x23};
/*
        instructions thats do writes to memory locations for cross ref
        inc,dec,ror,rol,row,lsr,asl,asr,smb,rmb,trb,tsb,sta,stx,sty,syz
        unless operand is A
*/
char nmi[]="INDEROLSASSMRMTRTSST";

void opcode(){
    int i;
    for(wrt=i=0;nmi[i];i+=2)
        wrt |= s2[0]==nmi[i] && s2[1]==nmi[i+1] &&      /* is a write and */
             ((s3[0]!='a' && s3[0]!='A') || s3[1]);     /* not accumulator */
    addr=print=data=known=1;
    i=d[0]=parseopr();
    if(data){
        loc_cntr+=data;         /* increment the current location counter */
        if(mpulevel[i&255]>cputype)
            display_error(error='I');
    }
}

int searchstr(str,key,size)
char *str,*key;
int size;
{
    int i,j,k;
    for(i=j=0;str[i];i+=size,j++){
        for(k=0;  k<size && str[i+k]==key[k];  k++);
        if(k==size)
            return(j);
    }
    return(-1);
}

char parseopr(){
    char tmp[LINELENG];
    long l;
    long s;
    int i,j,k,s3l;
/* 
        check for implied address mode first to avoid wasting time
        trying to parse a comment
*/
    if((j=searchstr(nm0,s2,3))>=0)      return(op0[j]);
/*
        test for valid opcode
*/
    k=TRUE;
    if(strlen(s2)==3){                  /* three char opcode name */
        if(searchstr(nm1,s2,3)>=0)      k=FALSE;
        if(searchstr(nm5,s2,3)>=0)      k=FALSE;
        if(searchstr(nm7,s2,3)>=0)      k=FALSE;
        if(searchstr(nmd,s2,3)>=0)      k=FALSE;
        if(searchstr(nmh,s2,3)>=0)      k=FALSE;
    }
    if(strlen(s2)==4){                  /* four char opcode name */
        if(searchstr(nm2,s2,4)>=0)      k=FALSE;
        if(searchstr(nm3,s2,4)>=0)      k=FALSE;
    }
    if(k){                               /* unknown opcode */
        display_error(error='N');
        return(data=0);
    }
    squote=dquote=i=0;
    while(i<LINELENG&&p3[i]){           /* change first space to null */
        if(p3[i]=='~'){
            i++;
            if(!p3[i]){
                p3[i++]=' ';
                p3[i]=0;
            }
            continue;
        }
        if(p3[i]=='\'')
            squote=!squote;
        if(p3[i]=='\"')
            dquote=!dquote;
        if(isspace(p3[i])&&!dquote&&!squote)
            p3[i]=0;
        else
            i++;
    }
/*
        try some of these easiest to check addressing modes next
*/
    if(toupper(*s3)=='A'&&!s3[1]){              /* accumulator addressing mode */
        if((j=searchstr(nm1,s2,3))>=0)          return(op1[j]);
        display_error(error='O');
        return(data=0);
    }
    if(*s3=='#'){                               /* immediate addressing mode */
        s3++;
        if((*s3=='\'' || *s3=='\"') && isspace(s3[2]))
            s3[2]= *s3;
        known=evaluate(s3,&l);
        j=searchstr(nm7,s2,3);
        if(j>=0){
            d[data++]=l&255;
            if((op7[j]&0x00ff)==0x00f4)
                d[data++]=l>>8;
            if(!known || l<256 || (op7[j]&0x00ff)==0x00f4)
                return(op7[j]);
        }
        display_error(error='O');
        return(data=0);
    }             
    s3l=strlen(s3);

    if(toupper(s3[s3l-5])=='S'&&s3[s3l-6]==','&&        /* SP indirect,Y */
       toupper(s3[s3l-4])=='P'&&s3[s3l-3]==')'&&
       toupper(s3[s3l-1])=='Y'&&s3[s3l-2]==','){
        strcpy(tmp,&s3[1]);
        tmp[s3l-7]=0;
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        if(!known || l<256)
            if((j=searchstr(nmg,s2,3))>=0)      return(opg[j]);
        display_error(error='O');
        return(data=0);
    }
/*
        calls and jmps
*/
    if(!strcmp(s2,"BSR")){          /* relative call */
        known=evaluate(s3,&s);
        s-=loc_cntr+2;
        d[data++]=s&255;
        d[data++]=s>>8;
        return(oph[0]);
    }
    if(!strcmp(s2,"JMP")){          /* jmp - 3 flavors */
        strcpy(tmp,s3);
        i=1;
        if(s3[0]=='(' && s3[s3l-1]==')'){
            strcpy(tmp,&s3[1]);
            if(i=(s3[s3l-3]==','&&toupper(s3[s3l-2])=='X'))
                tmp[s3l-4]=0;
            else
                tmp[s3l-2]=0;
            i+=2;
        }
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        d[data++]=l>>8;
        return(oph[i]);
    }
    if(!strcmp(s2,"JSR")){          /* call - 3 flavors */
        strcpy(tmp,s3);
        i=4;
        if(s3[0]=='(' && s3[s3l-1]==')'){
            strcpy(tmp,&s3[1]);
            if(i=(s3[s3l-3]==','&&toupper(s3[s3l-2])=='X'))
                tmp[s3l-4]=0;
            else
                tmp[s3l-2]=0;
            i+=5;
        }
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        d[data++]=l>>8;
        return(oph[i]);
    }
/*      indirect X */
    if(s3[s3l-3]==','&&toupper(s3[s3l-2])=='X'&&s3[s3l-1]==')'&&s3[0]=='('){
        strcpy(tmp,&s3[1]);
        tmp[s3l-4]=0;
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        if(!known || l<256)
            if((j=searchstr(nm4,s2,3))>=0)      return(op4[j]);
        display_error(error='O');
        return(data=0);
    }
/*      indirect Y */
    if(s3[s3l-3]==')'&&toupper(s3[s3l-1])=='Y'&&s3[s3l-2]==','&&s3[0]=='('){
        strcpy(tmp,&s3[1]);
        tmp[s3l-4]=0;
        known=evaluate(tmp,&l);
        d[data++]=l;
        if(!known || l<256)
            if((j=searchstr(nma,s2,3))>=0)      return(opa[j]);
        display_error(error='O');
        return(data=0);
    }
/*
        (bp),Z      indirect Z  if cputype = 3 
        (bp)        indirect    if cputype = 1 or 2
                    error       if cputype = 0
*/
    if(s3[0]=='(' && toupper(s3[s3l-1])=='Z' && s3[s3l-2]==',' && s3[s3l-3]==')'
       && cputype==3 || cputype && cputype<3 && s3[0]=='(' && s3[s3l-1]==')'){
        strcpy(tmp,&s3[1]);
        tmp[s3l-4]=0;
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        if(!known || l<256)
            if((j=searchstr(nmf,s2,3))>=0)      return(opf[j]);
        display_error(error='O');
        return(data=0);
    }                                                         

    if(s3[s3l-2]==','&&toupper(s3[s3l-1])=='Y'){        /* abs,Y or bp,Y */
        strcpy(tmp,s3);
        tmp[s3l-2]=0;
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        if(known && l>-129 && l<256 && !forcelong)
            if((j=searchstr(nmc,s2,3))>=0)      return(opc[j]);
        d[data++]=l>>8;
        if((j=searchstr(nmb,s2,3))>=0)          return(opb[j]);
        display_error(error='O');
        return(data=0);
    }

    if(s3[s3l-2]==','&&toupper(s3[s3l-1])=='X'){        /* abs,X or bp,X */
        strcpy(tmp,s3);
        tmp[s3l-2]=0;
        known=evaluate(tmp,&l);
        d[data++]=l&255;
        if(!known || l>255 || forcelong){
            d[data++]=l>>8;
            if((j=searchstr(nm8,s2,3))>=0) 
                if(op9[j])
                    return(op9[j]);   /* abs,x */
        }
        if(known && l<256)
            if((j=searchstr(nm8,s2,3))>=0) 
                if(op8[j])
                   return(op8[j]);   /* bp,x */
        display_error(error='O');
        return(data=0);
    }
/*
        now test the easiest instructions to identify
*/
    if(isdigit(s2[3])&&s2[4]==0){       /* must be a rockwell bit instruction */
        if((j=searchstr(nm2,s2,4))>=0){
            known=evaluate(s3,&l);
            d[data++]=l&255;
            return(op2[j]);   /* set or reset */
        }
        if((j=searchstr(nm3,s2,4))>=0){
            char *p;
            strcpy(tmp,s3);
            if(p=strchr(tmp,',')){         /* find the , */
                *p=0;
                known=evaluate(tmp,&l);
                d[data++]=l&255;
                if(known && l>255){
                    display_error(error='O');
                    return(data=0);
                }
                l=known;
                known=evaluate(++p,&s);
                s-=3+loc_cntr;
                d[data++]=s&255;
                if(known && (s>127 || s<-128)){
                    display_error(error='O');
                    return(data=0);
                }
                known&=l;
                return(op3[j]);   /* br set or br reset */
            }
        }
        display_error(error='O');
        return(data=0);
    }
/*
        test for branches
*/
    if((j=searchstr(nmd,s2,3))>=0){
        int petite;
        known=evaluate(s3,&s);
        s-=2+loc_cntr;
        petite=s<128 && s>-129 ;
        d[data++]=s&255;                /* low byte whether long or short */
        if(curpass<maxpass){            /* okay to change operand size */
            if(known && petite)
                return(opd[j]);         /* short branch */
            if(cputype<3){
                if(known)
                    display_error(error='A');
                return(opd[j]);
            }
            if( (assumeL && !known) || (known && !petite) ){ /* long */
                d[data++]=s>>8;
                return(ope[j]);         /* long */
            }
            return opd[j];              /* else short */
        }else                           /* last pass, can't change operand size */
            if(forcelong){              /* long branch */
                d[data++]=s>>8;
                return(ope[j]);
            }else{                      /* short branch */
                if(!petite)
                    display_error(error='A');
                return(opd[j]);
        }
    }
/*
        must be absolute or bp
*/
    known=evaluate(s3,&l);
    d[data++]=l&255;
    j=searchstr(nm5,s2,3);
    i=!known || l<-128 || l>255 || forcelong || !op5[j];
    if(k=( i && op6[j] ))
        d[data++]=l>>8&255;
    if(k?op6[j]:op5[j])
        return(k?op6[j]:op5[j]);   /* ?abs:bp */
    display_error(error='O');
    return(data=0);
}
