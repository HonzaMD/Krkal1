#include <dos.h>
#include <stdio.h>
#include "sb.h"

#define lo(value) (unsigned char)((value) & 0x00FF)
#define hi(value) (unsigned char)((value) >> 8)

#define SIZE 17000

char far buf[SIZE];
char far a[2][SIZE];

int cb=0;

void interrupt ( *oldhandler)();
void interrupt handler()
{
cb=!cb;
movmem(a[cb],buf,SIZE);


asm{
mov dx,0x22f
in al,dx
}

asm{
mov al,20h
out 20h,al
}
}


void main()
{
int i;
unsigned adr;
unsigned char page;
unsigned long addr;

FILE *f;
f=fopen("test.raw","rb");
fread(a,SIZE*2,1,f);
fclose(f);
movmem(a,buf,SIZE);

addr = (unsigned long)FP_SEG(buf)*16 + (unsigned long)FP_OFF(buf);
page = addr >> 16;
adr  = addr/2;


resetDSP();

outportb(0xD4,5);
outportb(0xD8,0);
outportb(0xD6,0x59);
outportb(0x8B,page);
outportb(0xC4,lo(adr));
outportb(0xC4,hi(adr));
outportb(0xC6,lo(SIZE/2-1));
outportb(0xC6,hi(SIZE/2-1));
outportb(0xD4,0x1);


writeDSP(0x41);
writeDSP(hi(44100));
writeDSP(lo(44100));


   oldhandler = getvect(0x0d);
   setvect(0x0d, handler);
asm{
in al,21h
and al,0dfh
out 21h,al
}


writeDSP(0xB6);
writeDSP(0x10);
writeDSP(lo(SIZE/2-1));
writeDSP(hi(SIZE/2-1));

while (kbhit()) getch();
getch();

asm{
in al,21h
or al,20h
out 21h,al
}
  setvect(0x0d, oldhandler);
writeDSP(0xD5);

}