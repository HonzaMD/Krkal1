#include <stdlib.h>
#include <math.h>
#define UI unsigned int
#define UC unsigned char

UC extern PLAYER;
UC extern rt;

unsigned int scr;
signed int sx,sy;
char pal[768];

void SetPalette(char pal[768]) {
  asm{
    push ds
    mov dx,0x3c8
    xor al,al
    out dx,al
    mov cx,768
    inc dx
    lds di,pal
  }a:asm{
    mov al,byte ptr [di]
    out dx,al
    inc di
    dec cx
    jnz a
    pop ds
  }
}

void ssp(UI s)
{
asm{
    mov bx,s
    mov     al,18h
    mov     ah,bl
    mov     dx,3d4h
    out     dx,ax

    mov     al,09h
    out     dx,al
    inc     dx
    in      al,dx
    mov     ah,bh
    and     ah,00000010b
    shl     ah,5
    and     al,10111111b
    or      al,ah
    out     dx,al

    dec     dx
    mov     al,07h
    out     dx,al
    inc     dx
    in      al,dx
    and     al,11101111b
    mov     ah,bh
    and     ah,00000001b
    shl     ah,4
    or      al,ah
    out     dx,al

}
}


void SSA()
{

  asm {
  mov ax,sx
  shr ax,2
  mov cx,sy
  mov bx,cx
  add bx,26
  mov cx,bx
  shl bx,5
  add ax,bx
  shl cx,7
  add ax,cx
  mov scr,ax
}
asm{
    mov ax,sx
    and al,3
    shl ax,9
    mov     dx,3dah
    in      al,dx
    mov     dx,3c0h
    mov     al,33h
    out     dx,al
    mov     al,ah
    out     dx,al
}
asm{
    mov bx,scr
    mov dx,3d4h
    mov al,0dh
    out dx,al
    inc dx
    mov al,bl
    out dx,al
    dec dx
    mov al,0ch
    out dx,al
    inc dx
    mov al,bh
    out dx,al
 }
}


void initg(void)
{
  asm {
//    mov ax,13h
//    int 10h
    mov dx,3ceh
    mov al,5
    out dx,al
    inc dx
    in al,dx
    and al,11101111b
    out dx,al
    dec dx
    mov al,6
    out dx,al
    inc dx
    in al,dx
    and al,11111101b
    out dx,al
    mov dx,3c4h
    mov al,4
    out dx,al
    inc dx
    in al,dx
    and al,11110111b
    or al,4
    out dx,al
    mov ax,0a000h
    mov es,ax
    xor di,di
    mov ax,di
    mov cx,8000h
    rep stosw
    mov dx,3d4h
    mov al,14h
    out dx,al
    inc dx
    in al,dx
    and al,10111111b
    out dx,al
    dec dx
    mov al,17h
    out dx,al
    inc dx
    in al,dx
    or al,01000000b
    out dx,al

    mov dx,0x3d4
    mov al,0x13
    out dx,al
    inc dx
    mov al,80
    out dx,al
    mov     dx,3dah
    in      al,dx
    mov     dx,3c0h
    mov     al,30h
    out     dx,al
    inc     dx
    in      al,dx
    dec     dx
    or      al,00100000b
    out     dx,al
  }
}
void scr_zb_mnu() {ssp(347);}
void scr_zm_mnu() {ssp(500);}

void uninitg(void)
{
  asm {
    mov dx,0x3c4
    mov ax,0xf02
    out dx,ax
    mov ax,0a000h
    mov es,ax
    xor di,di
    mov ax,di
    mov cx,0x8000
    rep stosw
  }
  scr_zm_mnu();
  sx=0;
  sy=-26;
  SSA();
  asm{
    mov dx,0x3d4
    mov al,0x13
    out dx,al
    inc dx
    mov al,0x28
    out dx,al
    dec dx
    mov al,0x17
    out dx,al
    inc dx
    mov al,0xA3
    out dx,al
    dec dx
    mov al,0x14
    out dx,al
    inc dx
    mov al,0x40
    out dx,al
    mov dx,0x3c4
    mov al,4
    out dx,al
    inc dx
    mov al,0xe
    out dx,al
    mov dx,0x3ce
    mov al,6
    out dx,al
    inc dx
    mov al,0x5
    out dx,al
    dec dx
    mov al,5
    out dx,al
    inc dx
    mov al,0x40
    out dx,al
  }
}

void putp(UI x,UI y,char b)
{
   asm {
     mov ax,y
     add ax,26
     mov bx,ax
     shl ax,7
     shl bx,5
     add ax,bx
     mov cx,x
     mov bx,cx
     shr bx,1
     shr bx,1
     add ax,bx
     mov di,ax
     and cl,3
     mov ah,1
     shl ah,cl
     mov al,2
     mov dx,3c4h
     out dx,ax
     mov ax,0a000h
     mov es,ax
     mov al,b
     stosb
   }
}

UC getp(UI x,UI y)
{
 UC barva;
 asm {
 mov bx,y
 add bx,26
 mov ax,160
 mul bx
 mov cx,x
 mov bx,cx
 shr bx,1
 shr bx,1
 add ax,bx
 mov di,ax
 and cl,3
 mov ah,cl
 mov al,4
 mov dx,3ceh
 out dx,ax
 mov ax,0a000h
 mov es,ax
 mov al,es:[di]
 mov barva,al
 }
return barva;
}

void sputimage(UI x,UI y,UI ddx,UI ddy,void *p) {
  asm {
    push ds
    push bp
    lds si,p //sd:[si] zdroj obrazku
    mov cx,x
    mov ax,cx
    shr ax,2
    and cx,3 //cl-rovina
    mov di,y
    add di,26
    mov dx,di
    shl di,5
    shl dx,7
    add di,dx
    add di,ax //es:[di] pozice ve videoram
    mov ax,0xa000
    mov es,ax
    mov ax,ddx
    add ax,cx
    dec ax
    shr ax,2
    mov dx,160
    sub dx,ax //dx - Pricitac na dalsi radek
    mov ch,4
    sub ch,cl
    mov cl,ch //cl-rovina
    mov bx,ddy //citac radku
    mov bp,ddx //citac sloupcu

}start:asm{
    push bp  //uschovavam ddx
    push cx  //uschovavam pocatecni rovinu
    push dx  //uschovavam pricitac na dalsi radek

    mov dx,0x3c4 //DX-Na zmeny rovin
}pokracuj:asm{
    mov ax,0x1002
    shr ah,cl
    out dx,ax  //zmena roviny

    movsb
    dec di

    dec bp
    jz dalsi_r    //kontrola jestli uz neskoncil radek

    dec cl
    jnz pokracuj
    inc di         //zacni znova od roviny 0
    mov cl,4
    jmp pokracuj

}dalsi_r:asm{
    pop dx
    pop cx
    pop bp

    dec bx           //kontrola jestli se neudelali vsechny radky
    jz konec

    add di,dx
    jmp start

}konec:asm{
    pop bp
    pop ds
  }
}


void sputimage0(UI x,UI y,UI ddx,UI ddy,void *p) {
  asm {
    push ds
    push bp
    lds si,p //sd:[si] zdroj obrazku
    mov cx,x
    mov ax,cx
    shr ax,2
    and cx,3 //cl-rovina
    mov di,y
    add di,26
    mov dx,di
    shl di,5
    shl dx,7
    add di,dx
    add di,ax //es:[di] pozice ve videoram
    mov ax,0xa000
    mov es,ax
    mov ax,ddx
    add ax,cx
    dec ax
    shr ax,2
    mov dx,160
    sub dx,ax //dx - Pricitac na dalsi radek
    mov ch,4
    sub ch,cl
    mov cl,ch //cl-rovina
    mov bx,ddy //citac radku
    mov bp,ddx //citac sloupcu

}start:asm{
    push bp  //uschovavam ddx
    push cx  //uschovavam pocatecni rovinu
    push dx  //uschovavam pricitac na dalsi radek

    mov dx,0x3c4 //DX-Na zmeny rovin
}pokracuj:asm{
    mov ax,0x1002
    shr ah,cl
    out dx,ax  //zmena roviny

    lodsb
    and al,al
    jz l1
    mov byte ptr es:[di],al
}l1:asm{

    dec bp
    jz dalsi_r    //kontrola jestli uz neskoncil radek

    dec cl
    jnz pokracuj
    inc di         //zacni znova od roviny 0
    mov cl,4
    jmp pokracuj

}dalsi_r:asm{
    pop dx
    pop cx
    pop bp

    dec bx           //kontrola jestli se neudelali vsechny radky
    jz konec

    add di,dx
    jmp start

}konec:asm{
    pop bp
    pop ds
  }
}


void sfill(UI x,UI y,UI ddx,UI ddy,UC barva) {
  asm {
    push bp
    mov cx,x
    mov ax,cx
    shr ax,2
    and cx,3 //cl-rovina
    mov di,y
    add di,26
    mov dx,di
    shl di,5
    shl dx,7
    add di,dx
    add di,ax //es:[di] pozice ve videoram
    mov ax,0xa000
    mov es,ax
    mov ax,ddx
    add ax,cx
    dec ax
    shr ax,2
    mov dx,160
    sub dx,ax //dx - Pricitac na dalsi radek
    mov ch,4
    sub ch,cl
    mov cl,ch //cl-rovina
    mov ch,barva
    mov bx,ddy //citac radku
    mov bp,ddx //citac sloupcu

}start:asm{
    push bp  //uschovavam ddx
    push cx  //uschovavam pocatecni rovinu
    push dx  //uschovavam pricitac na dalsi radek

    mov dx,0x3c4 //DX-Na zmeny rovin
}pokracuj:asm{
    mov ax,0x1002
    shr ah,cl
    out dx,ax  //zmena roviny

    mov byte ptr es:[di],ch

    dec bp
    jz dalsi_r    //kontrola jestli uz neskoncil radek

    dec cl
    jnz pokracuj
    inc di         //zacni znova od roviny 0
    mov cl,4
    jmp pokracuj

}dalsi_r:asm{
    pop dx
    pop cx
    pop bp

    dec bx           //kontrola jestli se neudelali vsechny radky
    jz konec

    add di,dx
    jmp start

}konec:asm{
    pop bp
  }
}


void sgetimage(UI x,UI y,UI ddx,UI ddy,void *p) {
  asm {
    push ds
    push bp
    les di,p //sd:[si] zdroj obrazku
    mov cx,x
    mov ax,cx
    shr ax,2
    and cx,3 //cl-rovina
    mov si,y
    add si,26
    mov dx,si
    shl si,5
    shl dx,7
    add si,dx
    add si,ax //es:[di] pozice ve videoram
    mov ax,0xa000
    mov ds,ax
    mov ax,ddx
    add ax,cx
    dec ax
    shr ax,2
    mov dx,160
    sub dx,ax //dx - Pricitac na dalsi radek
    mov ch,4
    sub ch,cl
    mov cl,ch //cl-rovina
    mov bx,ddy //citac radku
    mov bp,ddx //citac sloupcu

}start:asm{
    push bp  //uschovavam ddx
    push cx  //uschovavam pocatecni rovinu
    push dx  //uschovavam pricitac na dalsi radek

    mov dx,0x3ce //DX-Na zmeny rovin
}pokracuj:asm{
    mov ax,0x0404
    sub ah,cl
    out dx,ax  //zmena roviny

    movsb
    dec si

    dec bp
    jz dalsi_r    //kontrola jestli uz neskoncil radek

    dec cl
    jnz pokracuj
    inc si         //zacni znova od roviny 0
    mov cl,4
    jmp pokracuj

}dalsi_r:asm{
    pop dx
    pop cx
    pop bp

    dec bx           //kontrola jestli se neudelali vsechny radky
    jz konec

    add si,dx
    jmp start

}konec:asm{
    pop bp
    pop ds
  }
}


void vr()
{
if (!PLAYER)
{
ab:
asm {
mov dx,3dah
in al,dx
and al,0x08
jnz ab
}
ab2:
asm {
in al,dx
and al,0x08
jz ab2
}
}
else
{
rt=2;
while (rt) ;
}
}

void sxlen(void)
{
asm{
mov dx,3d4h
mov ah,80
mov al,13h
out dx,ax
}

}

void ssc(int ox,int oy,int p)
{
//while (sx!=ox||sy!=oy)
{

if (ox!=sx) {
  if (sx<ox) {
      sx+=p;
      if (sx>ox) sx=ox;
    }else{
      sx-=p;
      if (sx<ox) sx=ox;
    }
}
if (oy!=sy) {
  if (sy<oy) {
      sy+=p;
      if (sy>oy) sy=oy;
    }else{
      sy-=p;
      if (sy<oy) sy=oy;
    }
}
if (sx<0) sx=0;
if (sy<0) sy=0;
if (sx>320) sx=320;
if (sy>200) sy=200;

if (!PLAYER){
SSA();
vr();
}
else
{
SSA();
rt=1;
while (rt) ;
}
}

}

void setxy(int x, int y)
{
sx=x;
sy=y;
if (!PLAYER){
SSA();
vr();
}
else
{
SSA();
rt=1;
while (rt) ;
}
}

void scrinit(int xa,int ya)
{
initg();
scr_zb_mnu();

  sx=xa;
  sy=ya;

SSA();
vr();
}