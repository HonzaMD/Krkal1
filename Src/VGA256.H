#include <alloc.h>
#include <stdio.h>
#include <dos.h>
#define UC unsigned char
#define UI unsigned int

extern UC far buffer[64000];
extern UC brv2font[450];
extern UC far mnu[40000];
extern UI menu_x;

typedef UC videoramt[64000];
videoramt *videoram;
UI cxy;
UC col256;
UC handle_nahraneho_fontu;  //{255-CHYBA-font se nenahral}
UC aktivni_handle;  //{0 - Zadne } { Nemenit - Pouze informacni vyznam }
UC volnych_handle;  //             { Nemenit - Pouze informacni vyznam }


/*
  procedure init256; { Otevreni grafiky }
  initvga256()  //Je treba volat vzdy (inicializuje promeny)
  procedure done256; { Vrati textovy rezim Co80 }
  procedure nahrajfont(var s : file); {Soubor musi byt otevreny a seek nastaveny na zacatek fontu}
  procedure vyradfont(handle : byte);
  procedure aktivfont(handle : byte);
  procedure donefont;                 {Vyradi vsechny fonty}
  procedure vwrite(text : string);
  procedure vwritec(text : string);
  procedure vwrite2(barva1,barva2 : byte; typ : byte; text : string);

  procedure nastav_paletu(od,kolik : word; ukazatel : pointer);
  procedure nastav_paletu0;
  procedure precti_paletu(od,kolik : word; ukazatel : pointer);
  procedure nastav_1barvu(barva : word; re,g,b : byte);
  procedure precti_1barvu(barva : word; var re,g,b : byte);
  procedure nastav_okraj(barva : byte);

  procedure vgotoxy(x,y : word);
  procedure vputpixelb(x,y : word; barva : byte);
  procedure vputpixel(x,y : word);
  function vgetpixel(x,y : word) : byte;

  procedure vcls(barva : byte);
  procedure zobraz_dpc(odkaz : pointer);
  procedure rozbal_dpc(velikost : word;{sirka vyska obrazu}odkud,kam : pointer);
  procedure vgetimage(x1,y1,dx,dy : word;p : pointer);
  procedure vputimage(x1,y1,dx,dy : word;p : pointer);
  procedure vputimage0(x1,y1,dx,dy : word;p : pointer);
  procedure fill (x1,y1,dx,dy: word;barva : byte);
  procedure vline(x1,y1,x2,y2 : word);


 ************************************************************************
 ************************************************************************
 *******         ***************      *********************    **********
 ****                ********                 ************       ********
 ***                  ***********               *******        **********
 ***********        *****************        ************     ***********
 ************************************************************************
 ************************************************************************


  */

UC op[256];
UC op2[4];
UI dpcvelikost;
void *afont;
UC sloupce;
struct {
  UC inst; //0-neinstalovano,1-instalovano,2-ofset byl 4,3-8,4-12
  void *p;
  UC sloupce;
  UC velikost;
} fonty[10];

  void init256(){
  asm {
    mov ax,0013h
    int 10h
    mov cxy,0
    mov col256,15
  }}

  void done256(){
  asm {
    mov ax,0003h
    int 10h
  }}

  void initvga256(){
    UC f;
    videoram = MK_FP(0xa000,0);
    handle_nahraneho_fontu = 0;
    aktivni_handle = 0;
    volnych_handle = 10;
    for (f = 0;f<10;f++) fonty[f].inst = 0;
  }

  void aktivfont(UC handle) {
    if (fonty[handle-1].inst > 0) {
      aktivni_handle = handle;
      afont = fonty[handle-1].p;
      sloupce = fonty[handle-1].sloupce;
    }
  }

  void nahrajfont(FILE *s) { //Soubor musi byt otevreny a seek nastaveny na zacatek fontu}
    UC a;
    a=0;
    do {
      a++;
      if (a>10) {
        handle_nahraneho_fontu = 255;
        return;
      }
    } while ((a<11)&&(fonty[a-1].inst > 0));
    volnych_handle--;
    handle_nahraneho_fontu =a;
    fread(&fonty[a-1].sloupce,1,1,s);
    fonty[a-1].sloupce++;
    fread(&fonty[a-1].velikost,1,1,s);
    fonty[a-1].p = malloc((fonty[a-1].velikost+3)*8);
    fonty[a-1].inst = FP_OFF(fonty[a-1].p)/4+1;
    if (FP_OFF(fonty[a-1].p) != 0) fonty[a-1].p = MK_FP(FP_SEG(fonty[a-1].p)+1,0);
    fread(fonty[a-1].p,(fonty[a-1].velikost+1),8,s);
    if (aktivni_handle == 0) aktivfont(a);
  };

  void vyradfont(UC handle) {
    UC a;
    if (fonty[handle-1].inst > 0) {
      if (fonty[handle-1].inst > 1) fonty[handle-1].p = MK_FP(FP_SEG(fonty[handle-1].p)-1,(fonty[handle-1].inst-1)*4);
      fonty[handle-1].inst = 0;
      if (aktivni_handle == handle) {
        a = 0;
        while ((fonty[a].inst == 0)&&(a<10)) a++;
        if (a>9) aktivni_handle = 0; else aktivfont(a+1);
      }
      volnych_handle++;
      free(fonty[handle-1].p);
    }
  }


  void donefont(void){
    UC f;
    for (f=1;f<11;f++) vyradfont(f);
  }

  void vgotoxy(UI x, UI y){
  asm {
    mov ax,y
    mov dx,ax
    shl ax,8
    shl dx,6
    add ax,dx
    add ax,x
    mov cxy,ax
  }};

  void v4write2hc(UI *xy3,UC *rov2,UC barva1,UC barva2,UC typ,UC txt) {
    UC rov;
    UI xy,xy2;
    UC b,c,f;
    UI xx,yy;
    UC sl;
    unsigned long aa,bb;
    txt -= 127;
    xy = *xy3-480;
    rov= *rov2;
    if (barva1==0){
      xx=(xy%160);
      yy=(xy-xx)/160;
      xx=xx*4+rov;
    }
    sl=0;
    do {
      if (rov>=4) {
	rov=0;
	xy++;
      }
      xy2=xy;
      asm {
	mov dx,0x3c4
	mov ax,0x0102
	mov cl,rov
	shl ah,cl
	out dx,ax
      }
      do {
	b = brv2font[txt*15+sl*3+2]&0x04;
	sl++;
      } while(typ!=0&&(b)&&sl<6);
      sl--;
      if (sl<5) {
        bb = brv2font[txt*15+sl*3];
        aa = bb*65536;
        bb = brv2font[txt*15+sl*3+1];
	aa = aa+bb*256;
	aa +=brv2font[txt*15+sl*3+2];
	for (f=0;f<10;f++) {
	  if (barva1==0)
	  {
	    if ((aa>>(22-(f<<1)))&3) {
	      barva2=mnu[xx-85+196*(f+yy-105)];
	      asm {
		mov ax,0xa000
		mov es,ax
		mov di,xy
		mov al,barva2
		mov es:[di],al
	      }
	    }
	  }else{
	    if ((aa>>(22-(f<<1)))&1) {
	      asm {
		mov ax,0xa000
		mov es,ax
		mov di,xy
		mov al,barva1
		mov es:[di],al
	      }
	    }
	    if ((aa>>(22-(f<<1)))&2) {
	      asm {
		mov ax,0xa000
		mov es,ax
		mov di,xy
		mov al,barva2
		mov es:[di],al
	      }
	    }
	  }
	  xy+=160;
	}
	xy=xy2;
        rov++;
	sl++;
	xx++;
      }
    } while (sl<5);
    if (rov>=4) {
      rov=0;
      xy++;
    }
    rov++;
    *xy3 = xy+480;
    *rov2 = rov;
  }

  void v4write2(int x,int y,UC barva1,UC barva2,UC typ,UC txt[255]) {
    UC rov;
    UI xy,xy2;
    UC a=0,b,c,f,g;
    UC sl1,sl2;
    if (txt[0]==0) return;
    y+=26;
    xy = y*160+x/4;
    rov=x % 4;
    do {
      if (txt[a] > 126) v4write2hc(&xy,&rov,barva1,barva2,typ,txt[a]); else {
	sl2=sl1=0;
	do {
	  if (rov>=4) {
	    rov=0;
	    xy++;
	  }
	  xy2=xy;
	  asm {
	    mov dx,0x3c4
	    mov ax,0x0102
	    mov cl,rov
	    shl ah,cl
	    out dx,ax
	  }
	  do {
	    b = peekb(FP_SEG(fonty[aktivni_handle-1].p),FP_OFF(fonty[aktivni_handle-1].p)+(txt[a]-32)*8+sl1);
	    if (sl1<=sloupce)sl1++;
	  } while(typ!=0&&(!(b&1))&&sl1<=sloupce);
	  do {
	    c = peekb(FP_SEG(fonty[aktivni_handle].p),FP_OFF(fonty[aktivni_handle].p)+(txt[a]-32)*8+sl2);
	    if (sl2<=sloupce)sl2++;
	  } while(typ!=0&&(!(c&1))&&sl2<=sloupce);
	  g=128;
	  for (f=0;f<7;f++) {
	    if ((b&g)&&sl1<=sloupce) {
	      asm {
	        mov ax,0xa000
	        mov es,ax
	        mov di,xy
	        mov al,barva1
	        mov es:[di],al
	      }
	    } else if ((c&g)&&sl2<=sloupce) {
	      asm {
		mov ax,0xa000
		mov es,ax
		mov di,xy
		mov al,barva2
		mov es:[di],al
	      }
	    }
	    g=g>>1;
	    xy+=160;
	  }
	  xy=xy2;
	  rov++;
	} while (sl1!=sloupce+1||sl2!=sloupce+1);
      }
      a++;
    } while (txt[a]);
  }


UC pocrad(UC *txt,UI x,UI y,UI pos)
{
  UI *rstart=(UI*)(mnu+19404);
  UI a=0,f;
  while (txt[a]) {
    if (x>191&&txt[a]!=13){
      x=0;
      y++;
      rstart[y]=pos;
    }
    if(txt[a]!=13){
      x++;
      if (txt[a]<127) {
	for (f=0;f<sloupce;f++) if (1&peekb(FP_SEG(fonty[aktivni_handle-1].p),FP_OFF(fonty[aktivni_handle-1].p)+(txt[a]-32)*8+f)) x++;
      } else for (f=0;f<5;f++) if (!(brv2font[(txt[a]-127)*15+f*3+2]&0x04)) x++;
    }
    if (txt[a]==13){
      x=0;
      y++;
      rstart[y]=pos;
    }
    a++;
    pos++;
  }
  rstart[y+1]=0;
  return y;
}


  void tex_vr(int x,int y,UC barva1,UC barva2,UC *txt,UC wh) {
    UC rov;
    UI xy,xy2;
    int a=0;
    UC b,c,f,g;
    UC sl1,sl2;
    int xx=x;
    if (txt[0]==0) return;
    y+=26;
    xy = y*160+(x+menu_x)/4;
    rov=x % 4;
    do {
      if (xx>276||txt[a]==13){
	xx=85;
	y+=11;
	xy=y*160+(85+menu_x)/4;
	rov=85%4;
	if (txt[a]==13&&wh==1) return;
      }
      if (txt[a]!=13){
	if (txt[a] > 126) {
	  v4write2hc(&xy,&rov,barva1,barva2,1,txt[a]);
	  xx++;
	  for (f=0;f<5;f++) if (!(brv2font[(txt[a]-127)*15+f*3+2]&0x04)) xx++;
	}else {
	  sl2=sl1=0;
	  do {
	    if (rov>=4) {
	      rov=0;
	      xy++;
	    }
	    xy2=xy;
	    asm {
	      mov dx,0x3c4
	      mov ax,0x0102
	      mov cl,rov
	      shl ah,cl
	      out dx,ax
	    }
	    do {
	      b = peekb(FP_SEG(fonty[aktivni_handle-1].p),FP_OFF(fonty[aktivni_handle-1].p)+(txt[a]-32)*8+sl1);
	      if (sl1<=sloupce)sl1++;
	    } while((!(b&1))&&sl1<=sloupce);
	    do {
	      c = peekb(FP_SEG(fonty[aktivni_handle].p),FP_OFF(fonty[aktivni_handle].p)+(txt[a]-32)*8+sl2);
	      if (sl2<=sloupce)sl2++;
	    } while((!(c&1))&&sl2<=sloupce);
	    g=128;
	    for (f=0;f<7;f++) {
	      if (barva1==0)
	      {
		if (((b&g)&&sl1<=sloupce)||((c&g)&&sl2<=sloupce))
		{
		  barva2=mnu[xx-85+196*(f+y-105)];
		  asm {
		    mov ax,0xa000
		    mov es,ax
		    mov di,xy
		    mov al,barva2
		    mov es:[di],al
		  }
		}
	      }else{
		if ((b&g)&&sl1<=sloupce) {
		  asm {
		    mov ax,0xa000
		    mov es,ax
		    mov di,xy
		    mov al,barva1
		    mov es:[di],al
		  }
		} else if ((c&g)&&sl2<=sloupce) {
		  asm {
		    mov ax,0xa000
		    mov es,ax
		    mov di,xy
		    mov al,barva2
		    mov es:[di],al
		  }
		}
	      }
	      g=g>>1;
	      xy+=160;
	    }
	    xy=xy2;
	    rov++;
	    xx++;
	  } while (sl1!=sloupce+1||sl2!=sloupce+1);
	}
      }
      a++;
    } while (txt[a]);
  }


  UI vlength(UC txt[255]) {
    UI l=0;
    UC a=0,f;
    while (txt[a]) {
      if(txt[a]!=13){
	l++;
	if (txt[a]<127) {
	  for (f=0;f<sloupce;f++) if (1&peekb(FP_SEG(fonty[aktivni_handle-1].p),FP_OFF(fonty[aktivni_handle-1].p)+(txt[a]-32)*8+f)) l++;
	} else for (f=0;f<5;f++) if (!(brv2font[(txt[a]-127)*15+f*3+2]&0x04)) l++;
      }
      a++;
    }
    if (l>0) l--;
    return l;
  }

  void vwrite(char txt[255]) {
  UC a;
  char text[255];
  a = 0;
  if (txt[0]==0) return;
  while (txt[a] != 0) {
    text[a+1] = txt[a];
    a++;
  }
  text[0] = a;
  asm {
    cli
    cld
    push ds
    push bp
    mov cl,sloupce
    mov di,cxy
    mov al,col256
    lds si,afont
    mov dx,0xa000
    mov es,dx
    sub bp,258
    xor ch,ch
    mov ah,[bp]
    inc bp
    mov dx,320
    sub dx,cx
    push dx
    mov dl,al
    mov dh,cl

}l1:asm{ xor bh,bh
    mov bl,[bp]
    sub bl,32
    shl bx,3
    mov si,bx
    inc bp

    xor bx,bx
}l2:asm{ mov al,[si+bx]
    inc bx
    and al,0x80
    jz l22
    mov es:[di],dl
}l22:asm{inc di
    loop l2
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l3:asm{ mov al,[si+bx]
    inc bx
    and al,0x40
    jz l33
    mov es:[di],dl
}l33:asm{inc di
    loop l3
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l4:asm{ mov al,[si+bx]
    inc bx
    and al,0x20
    jz l44
    mov es:[di],dl
}l44:asm{inc di
    loop l4
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l5:asm{ mov al,[si+bx]
    inc bx
    and al,0x10
    jz l55
    mov es:[di],dl
}l55:asm{inc di
    loop l5
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l6:asm{mov al,[si+bx]
    inc bx
    and al,0x08
    jz l66
    mov es:[di],dl
}l66:asm{inc di
    loop l6
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l7:asm{ mov al,[si+bx]
    inc bx
    and al,0x04
    jz l77
    mov es:[di],dl
}l77:asm{inc di
    loop l7
    mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
}l8:asm{ mov al,[si+bx]
    inc bx
    and al,0x02
    jz l88
    mov es:[di],dl
}l88:asm{inc di
    loop l8
    mov cl,dh

    sub di,1920
    inc di
    dec ah
    jz l9
    jmp l1
}l9:asm{

    pop dx
    pop bp
    pop ds
    mov cxy,di
    sti
  }
 }

  void vwritec(char txt[255]) {
  UC a;
  char text[255];
  if (txt[0]==0) return;
  a = 0;
  while (txt[a] != 0) {
    text[a+1] = txt[a];
    a++;
  }
  text[0] = a;
  asm {
    cli
    cld
    push ds
    push bp
    mov cl,sloupce
    mov di,cxy
    mov al,col256
    lds si,afont
    mov dx,0xa000
    mov es,dx
    sub bp,258
    xor ch,ch
    mov ah,[bp]
    inc bp
    mov dx,320
    sub dx,cx
    push dx
    mov dl,al
    mov dh,cl

}l1:asm{ xor bh,bh
    mov bl,[bp]
    sub bl,32
    shl bx,3
    mov si,bx

    xor bx,bx
    mov [bp],bl
}l2:asm{ mov al,[si+bx]
    inc bx
    and al,0x81
    jz l23
    jnp l22
    mov es:[di],dl
    inc di
    loop l2
    jmp l24
}l22:asm{and al,1
    jz l23
    inc di
    loop l2
    jmp l24
}l23:asm{inc byte ptr [bp]
    loop l2
}l24:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l3: asm{mov al,[si+bx]
    inc bx
    and al,0x41
    jz l33
    jnp l32
    mov es:[di],dl
    inc di
    loop l3
    jmp l34
}l32:asm{and al,1
    jz l33
    inc di
    loop l3
    jmp l34
}l33:asm{inc byte ptr [bp]
    loop l3
}l34:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l4: asm{mov al,[si+bx]
    inc bx
    and al,0x21
    jz l43
    jnp l42
    mov es:[di],dl
    inc di
    loop l4
    jmp l44
}l42:asm{and al,1
    jz l43
    inc di
    loop l4
    jmp l44
}l43:asm{inc byte ptr [bp]
    loop l4
}l44:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l5: asm{mov al,[si+bx]
    inc bx
    and al,0x11
    jz l53
    jnp l52
    mov es:[di],dl
    inc di
    loop l5
    jmp l54
}l52:asm{and al,1
    jz l53
    inc di
    loop l5
    jmp l54
}l53:asm{inc byte ptr [bp]
    loop l5
}l54:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l6: asm{mov al,[si+bx]
    inc bx
    and al,0x09
    jz l63
    jnp l62
    mov es:[di],dl
    inc di
    loop l6
    jmp l64
}l62:asm{and al,1
    jz l63
    inc di
    loop l6
    jmp l64
}l63:asm{inc byte ptr [bp]
    loop l6
}l64:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l7: asm{mov al,[si+bx]
    inc bx
    and al,0x05
    jz l73
    jnp l72
    mov es:[di],dl
    inc di
    loop l7
    jmp l74
}l72:asm{and al,1
    jz l73
    inc di
    loop l7
    jmp l74
}l73:asm{inc byte ptr [bp]
    loop l7
}l74:asm{mov cl,dh
    mov al,dl
    pop dx
    dec sp
    dec sp
    add di,dx
    xor dh,dh
    mov dl,[bp]
    add di,dx
    mov dl,al
    mov dh,cl

    xor bx,bx
    mov [bp],bl
}l8: asm{mov al,[si+bx]
    inc bx
    and al,0x03
    jz l83
    jnp l82
    mov es:[di],dl
    inc di
    loop l8
    jmp l84
}l82:asm{and al,1
    jz l83
    inc di
    loop l8
    jmp l84
}l83:asm{inc byte ptr [bp]
    loop l8
}l84:asm{mov cl,dh


    inc di
    sub di,1920
    inc bp
    dec ah
    jz l9
    jmp l1
}l9:asm{

    pop dx
    pop bp
    pop ds
    mov cxy,di
    sti
  }}

void nastav_paletu(UC od,UI kolik,void *pal) {
  asm{
    push ds
    mov dx,0x3c8
    mov al,od
    out dx,al
    mov cx,kolik
    mov bx,cx
    shl cx,1
    add cx,bx
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

void nastav_paletu0() {
  asm {
    mov dx,0x3c8
    xor al,al
    out dx,al
    mov cx,768
    inc dx
  } a: asm {
    out dx,al
    dec cx
    jnz a
  }
}


void precti_paletu(UC od,UI kolik,void *pal) {
  asm{
    push ds
    mov dx,0x3c7
    mov al,od
    out dx,al
    mov cx,kolik
    mov bx,cx
    shl cx,1
    add cx,bx
    inc dx
    inc dx
    lds di,pal
}a:asm{
    in al,dx
    mov byte ptr [di],al
    inc di
    dec cx
    jnz a
    pop ds
//    cli
  }
}

  void nastav_1barvu(UC barva,UC re,UC g,UC b){
  asm{
    mov dx,0x3c8
    mov al,barva
    out dx,al
    inc dx
    mov al,re
    out dx,al
    mov al,g
    out dx,al
    mov al,b
    out dx,al
  }
}

  void precti_1barvu(UC barva,UC *re,UC *g,UC *b){
  asm{
    mov dx,0x3c7
    mov al,barva
    out dx,al
    inc dx
    inc dx
    in al,dx
    les di,re
    mov es:[di],al
    in al,dx
    les di,g
    mov es:[di],al
    in al,dx
    les di,b
    mov es:[di],al
  }
}

void nastav_okraj(UC barva) {
  asm {
    mov ax,0x1001
    mov bh,barva
    int 0x10
  }
}

void vputpixelb(UI x,UI y,UC barva){
  asm {
    mov ax,0xa000
    mov es,ax
    mov ax,y
    mov bx,ax
    shl ax,8
    shl bx,6
    add bx,ax
    add bx,x
    mov al,barva
    mov es:[bx],al
  }
}

void vputpixel(UI x,UI y){
  asm {
    mov ax,0xa000
    mov es,ax
    mov ax,y
    mov bx,ax
    shl ax,8
    shl bx,6
    add bx,ax
    add bx,x
    mov al,col256
    mov es:[bx],al
  }
}

UC vgetpixel(UI x,UI y){
  UC barva;
  asm {
    mov ax,0xa000
    mov es,ax
    mov ax,y
    mov bx,ax
    shl ax,8
    shl bx,6
    add bx,ax
    add bx,x
    mov al,es:[bx]
    mov barva,al
  }
  return(barva);
}


void vwrite2(UC barva1,UC barva2,UC typ,char text[255]) {
  UC aa;
  UI bb;
  bb = cxy;
  col256 = barva1;
  aa = aktivni_handle;
  if (typ == 0) vwrite(text); else vwritec(text);
  cxy = bb;
  col256 = barva2;
  aktivfont(aa+1);
  if (typ == 0) vwrite(text); else vwritec(text);
  aktivfont(aa);
}

void vwrite2hc(UC barva1,UC barva2,UC typ,UC text[255]) {
  UC text2[2] = {0,0};
  UI xy;
  UC a,b,c,f;
  UC sl;
  unsigned long aa,bb;
  a=0;
  while(text[a]) {
    if (text[a]>=127) {
      sl=0;
      do {
        xy = cxy-960;
        do {
          b = brv2font[(text[a]-127)*15+sl*3+2]&0x04;
	  sl++;
        } while(typ!=0&&(b)&&sl<6);
        sl--;
        if (sl<5) {
          bb = brv2font[(text[a]-127)*15+sl*3];
          aa = bb*65536;
          bb = brv2font[(text[a]-127)*15+sl*3+1];
          aa = aa+bb*256;
          aa +=brv2font[(text[a]-127)*15+sl*3+2];
          for (f=0;f<10;f++) {
	    if ((aa>>(22-(f<<1)))&1) {
	      asm {
	        mov ax,0xa000
	        mov es,ax
	        mov di,xy
	        mov al,barva1
	        mov es:[di],al
	      }
	    }
            if ((aa>>(22-(f<<1)))&2) {
	      asm {
	        mov ax,0xa000
	        mov es,ax
	        mov di,xy
	        mov al,barva2
	        mov es:[di],al
	      }
	    }
	    xy+=320;
          }
          sl++;
          cxy++;
        }
      } while (sl<5);
      cxy++;
    } else {
      text2[0] = text[a];
      vwrite2(barva1,barva2,typ,text2);
    }
    a++;
  }
}

  void vcls(UC barva) {
  asm {
    cld
    mov al,barva
    mov ah,al
    mov cx,32000
    mov di,0
    mov dx,0xa000
    mov es,dx
    rep stosw
  }
}

  void zobraz_dpc(void *odkaz) {
  asm {
    cld
    push si
    push di
    push es
    mov dx,ds
    mov es,dx
    push bp
    push ds
    mov di,offset op
    lds si,odkaz
    xor ax,ax
    xor ch,ch
    xor dl,dl
    lodsb
    mov cl,al
    xor bx,bx
} l1: asm {
    lodsb
    and al,al
    jnz l2
    lodsb
    and al,al
    jz l2
    inc dl
} l2:  asm {cmp al,bl
    jnz l3
    cmp ah,3
    ja l23
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
} l23: asm {inc ah
    mov al,ah
    stosb
    xor dl,dl
    inc bx
    loop l1
    jmp l5
}l3: asm { and dl,dl
    jz l4
    cmp ah,3
    ja l33
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
}l33: asm {inc ah
    mov es:[di],ah
    inc di
    inc bx
    loop l2
    jmp l5
}l4: asm {mov es:[di],dl
    inc di
    inc bx
    jmp l2
}l5: asm { mov cx,256
    sub cx,bx
    jz l6
    xor al,al
rep stosb
}l6: asm {


    pop dx
    mov ax,0xa000
    mov es,ax
    xor di,di
    mov bp,ds
    xor bh,bh

}l7: asm { lodsb
    mov ds,dx
    mov bl,al
    cmp byte ptr op+[bx],0
    jz l8
    mov al,byte ptr [op+bx]
    mov ds,bp

    cmp al,1
    jnz l71
    lodsb
    xor ch,ch
    mov cl,al
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l71: asm {cmp al,2
    jnz l72
    lodsb
    mov ch,al
    lodsb
    mov cl,al
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l72: asm {cmp al,3
    jnz l73
    lodsb
    mov ah,al
    and ax,0xf00f
    shr ah,4
    xor ch,ch
    cmp al,14
    jnz laa
    lodsb
    jmp lbb
}laa: asm {cmp al,15
    jnz lbb
    lodsb
    mov ch,al
    lodsb
}lbb: asm {mov cl,al
    inc cx
    mov ds,dx
    mov bl,ah
    mov al,byte ptr [op2+bx]
    mov ds,bp
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l73: asm {xor ch,ch
    mov cl,al
    dec cl
    dec cl
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l8: asm { stosb
    mov ds,bp
    mov ah,al
    cmp di,64000
    jz l9
    jmp l7
}l9: asm { pop bp
    mov ds,dx
    pop es
    pop di
    pop si
  }}

  void rozbal_dpc(UI velikost,void *odkud,void *kam){
  asm {
    cld
    mov ax,velikost
    mov dx,ds
    mov es,dx
    push bp
    push ds
    push ax
    mov di,offset op
    lds si,odkud
    xor ax,ax
    xor ch,ch
    xor dl,dl
    lodsb
    mov cl,al
    xor bx,bx
}l1:asm{ lodsb
    and al,al
    jnz l2
    lodsb
    and al,al
    jz l2
    inc dl
}l2:asm{ cmp al,bl
    jnz l3
    cmp ah,3
    ja l23
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
}l23:asm{inc ah
    mov al,ah
    stosb
    xor dl,dl
    inc bx
    loop l1
    jmp l5
}l3:asm{ and dl,dl
    jz l4
    cmp ah,3
    ja l33
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
}l33:asm{inc ah
    mov es:[di],ah
    inc di
    inc bx
    loop l2
    jmp l5
}l4:asm{ mov es:[di],dl
    inc di
    inc bx
    jmp l2
}l5: asm{mov cx,256
    sub cx,bx
    jz l6
    xor al,al
rep stosb
}l6:asm{


    pop ax
    pop dx
    les di,kam
    mov bp,ds
    xor bh,bh
    mov ds,dx
    add ax,di
    mov dpcvelikost,ax


}l7:asm{mov ds,bp
    lodsb
    mov ds,dx
    mov bl,al
    cmp byte ptr op+[bx],0
    jnz k8
    jmp l8
}k8:asm{
    mov al,byte ptr [op+bx]
    mov ds,bp

    cmp al,1
    jnz l71
    lodsb
    xor ch,ch
    mov cl,al
    mov al,ah
rep stosb
    xor bl,bl
    mov ds,dx
    cmp di,dpcvelikost
    jnz l7
    jmp l9

}l71:asm{cmp al,2
    jnz l72
    lodsb
    mov ch,al
    lodsb
    mov cl,al
    mov al,ah
rep stosb
    xor bl,bl
    mov ds,dx
    cmp di,dpcvelikost
    jnz l7
    jmp l9

}l72:asm{cmp al,3
    jnz l73
    lodsb
    mov ah,al
    and ax,0xf00f
    shr ah,4
    xor ch,ch
    cmp al,14
    jnz laa
    lodsb
    jmp lbb
}laa:asm{cmp al,15
    jnz lbb
    lodsb
    mov ch,al
    lodsb
}lbb:asm{mov cl,al
    inc cx
    mov ds,dx
    mov bl,ah
    mov al,byte ptr [op2+bx]
    mov ds,bp
rep stosb
    xor bl,bl
    mov ds,dx
    cmp di,dpcvelikost
    jz l9
    jmp l7

}l73:asm{xor ch,ch
    mov cl,al
    dec cl
    dec cl
    mov al,ah
rep stosb
    xor bl,bl
    mov ds,dx
    cmp di,dpcvelikost
    jz l9
    jmp l7

}l8:asm{ stosb
    mov ds,bp
    mov ah,al
    xor bl,bl
    mov ds,dx
    cmp di,dpcvelikost
    jz l9
    jmp l7
}l9: asm{pop bp
  }
}


  void vgetimage (UI x1,UI y1,UI ddx,UI ddy,void *p) {
  asm {
    cld
    mov ax,y1
    mov si,ax
    shl ax,8
    shl si,6
    add si,ax
    add si,x1
    push ds
    les di,p
    mov ax,0xa000
    mov ds,ax
    mov dx,ddx
    mov ax,320
    sub ax,dx
    mov bx,ddy
    shr dx,1
    jc k2
}k1:asm{
    mov cx,dx
rep movsw
    dec bx
    jz k3
    add si,ax
    jmp short k1
}k2:asm{
    mov cx,dx
rep movsw
    movsb
    dec bx
    jz k3
    add si,ax
    jmp short k2
}k3:asm{
    pop ds
  }
}

  void vputimage (UI x1,UI y1,UI ddx,UI ddy,void *p) {
  asm {
    cld
    mov ax,y1
    mov di,ax
    shl ax,8
    shl di,6
    add di,ax
    add di,x1
    push ds
    lds si,p
    mov ax,0xa000
    mov es,ax
    mov dx,ddx
    mov ax,320
    sub ax,dx
    mov bx,ddy
    shr dx,1
    jc k2
}k1:asm{
    mov cx,dx
rep movsw
    dec bx
    jz k3
    add di,ax
    jmp short k1
}k2:asm{
    mov cx,dx
rep movsw
    movsb
    dec bx
    jz k3
    add di,ax
    jmp short k2
}k3:asm{
    pop ds
  }
}


  void vputimage0 (UI x1,UI y1,UI ddx,UI ddy,void *p) {
  asm {
    cld
    mov ax,y1
    mov di,ax
    shl ax,8
    shl di,6
    add di,ax
    add di,x1
    push ds
    lds si,p
    mov ax,0xa000
    mov es,ax
    mov dx,320
    sub dx,ddx
    mov bx,ddy
}k1:asm{
    mov cx,ddx
}k2:asm{
    lodsb
    and al,al
    jz k3
    mov es:[di],al
}k3:asm{
    inc di
    dec cx
    jnz k2
    dec bx
    jz k4
    add di,dx
    jmp short k1
}k4:asm{
    pop ds
  }
}

  void fill (UI x1,UI y1,UI ddx,UI ddy,UC barva) {
  asm {
    cld
    mov ax,y1
    mov di,ax
    shl ax,8
    shl di,6
    add di,ax
    add di,x1
    mov ax,0xa000
    mov es,ax
    mov dx,ddx
    mov si,320
    sub si,dx
    mov bx,ddy
    mov al,barva
    mov ah,al
    shr dx,1
    jc k2
}k1:asm{
    mov cx,dx
rep stosw
    dec bx
    jz k3
    add di,si
    jmp short k1
}k2:asm{
    mov cx,dx
rep stosw
    stosb
    dec bx
    jz k3
    add di,si
    jmp short k2
}k3:asm{
  }
}


void vline(UI x1,UI y1,UI x2,UI y2) {
  asm {
    xor si,si
    xor di,di
    mov ax,x1
    mov bx,x2
    cmp ax,bx
    ja l1
    sub bx,ax
    mov ax,bx
    inc si
    jmp l2
}l1:asm{
    sub ax,bx
}l2:asm{
    mov cx,y1
    mov dx,y2
    cmp cx,dx
    ja l3
    sub dx,cx
    mov cx,dx
    inc di
    jmp l4
}l3:asm{
    sub cx,dx
}l4:asm{
    cmp ax,cx
    jna loy
    jmp lox

}loy:asm{
    xor si,di
    jz ly3
    mov si,65535
    jmp ly4
}ly3:asm{
    mov si,1

}ly4:asm{
    and di,di
    jz ly1
    mov bx,x1
    mov di,y1
    jmp ly2
}ly1:asm{
    mov di,y2
    mov bx,x2
}ly2:asm{
    mov dx,di
    shl di,8
    shl dx,6
    add di,dx
    add di,bx

    mov x1,ax
    shl ax,1
    sub ax,cx
    mov dx,ax
    mov bl,col256
    mov ax,0xa000
    push ds
    mov ds,ax
    mov ax,x1
    sub ax,cx
    shl ax,1
    mov y1,ax
    mov ax,x1
    shl ax,1

    mov [di],bl
}ly5:asm{
    add di,320
    and dx,dx
    js ly6
    add di,si
    add dx,y1
    jmp ly7
}ly6:asm{
    add dx,ax
}ly7:asm{
    mov [di],bl
    loop ly5
    jmp lend

}lox:asm{
    xor di,si
    jz lx3
    mov di,65216
    jmp lx4
}lx3:asm{
    mov di,320

}lx4:asm{
    and si,si
    jz lx1
    mov bx,x1
    mov si,y1
    jmp lx2
}lx1:asm{
    mov si,y2
    mov bx,x2
}lx2:asm{
    mov dx,si
    shl si,8
    shl dx,6
    add si,dx
    add si,bx

    xchg ax,cx
    mov x1,ax
    shl ax,1
    sub ax,cx
    mov dx,ax
    mov bl,col256
    mov ax,0xa000
    push ds
    mov ds,ax
    mov ax,x1
    sub ax,cx
    shl ax,1
    mov y1,ax
    mov ax,x1
    shl ax,1

    mov [si],bl
}lx5:asm{
    inc si
    and dx,dx
    js lx6
    add si,di
    add dx,y1
    jmp lx7
}lx6:asm{
    add dx,ax
}lx7:asm{
    mov [si],bl
    loop lx5


}lend:asm{
    pop ds
  }
}
