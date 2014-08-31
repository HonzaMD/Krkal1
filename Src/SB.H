#include <dos.h>

#define DSP_Reset 0x226
#define DSP_Read_data 0x22A
#define DSP_Write_data 0x22C
#define DSP_Write_status 0x22C
#define DSP_Avail 0x22E

char resetDSP()
{
char a;
outportb(DSP_Reset,1);
delay(10);
outportb(DSP_Reset,0);
delay(10);
if (((inportb(DSP_Avail)&0x80)==0x80)&&inportb(DSP_Read_data)==0xAA) return 0; else return 1;
}

void writeDSP(char aq)
{
while ((inportb(DSP_Write_status)&0x80)!=0) ;
outportb(DSP_Write_data,aq);
}

void writeDAC(char qa)
{
writeDSP(0x10);
writeDSP(qa);
}