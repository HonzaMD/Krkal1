#include <stdlib.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>
extern void hazej(UI x,UI y,UC o);
extern void zobraz_menu(UI x,UI y,UC ddx,UC ddy);
extern char pal[768];
extern signed int sx,sy;

extern void SetPalette(char pal[768]);
extern void putp(UI x,UI y,char b);
extern char getp(UI x,UI y);
extern void sputimage(UI x,UI y,UI ddx,UI ddy,void *p);
extern void sputimage0(UI x,UI y,UI ddx,UI ddy,void *p);
extern void sfill(UI x,UI y,UI ddx,UI ddy,UC barva);
extern void sgetimage(UI x,UI y,UI ddx,UI ddy,void *p);
extern void vr();
extern void ssc(int oy,int ox,int p);
extern void scrinit(int,int);
extern void setxy(int,int);

typedef UC videoramt[64000];
extern videoramt *videoram;
extern UI cxy;
extern UC col256;
extern UC handle_nahraneho_fontu;  //{255-CHYBA-font se nenahral}
extern UC aktivni_handle;  //{0 - Zadne } { Nemenit - Pouze informacni vyznam }
extern UC volnych_handle;  //             { Nemenit - Pouze informacni vyznam }
extern   void init256();
extern   void done256();
extern   void initvga256();
extern   void aktivfont(UC handle) ;
extern   void nahrajfont(FILE *s) ; //Soubor musi byt otevreny a seek nastaveny na zacatek fontu}
extern   void vyradfont(UC handle) ;
extern   void donefont(void);
extern   void vgotoxy(UI x, UI y);
extern   void vwrite(char txt[255]) ;
extern   void vwritec(char txt[255]) ;
extern  UI vlength(UC txt[255]);
extern void nastav_paletu(UC od,UI kolik,void *pal) ;
extern void nastav_paletu0() ;
extern void precti_paletu(UC od,UI kolik,void *pal) ;
extern   void nastav_1barvu(UC barva,UC re,UC g,UC b);
extern   void precti_1barvu(UC barva,UC *re,UC *g,UC *b);
extern void nastav_okraj(UC barva) ;
extern void vputpixelb(UI x,UI y,UC barva);
extern void vputpixel(UI x,UI y);
extern UC vgetpixel(UI x,UI y);
extern void vwrite2(UC barva1,UC barva2,UC typ,char text[255]) ;
extern   void vcls(UC barva) ;
extern   void zobraz_dpc(void *odkaz) ;
extern   void rozbal_dpc(UI velikost,void *odkud,void *kam);
extern   void vgetimage (UI x1,UI y1,UI ddx,UI ddy,void *p) ;
extern   void vputimage (UI x1,UI y1,UI ddx,UI ddy,void *p) ;
extern   void vputimage0 (UI x1,UI y1,UI ddx,UI ddy,void *p) ;
extern   void fill (UI x1,UI y1,UI ddx,UI ddy,UC barva) ;
extern void vline(UI x1,UI y1,UI x2,UI y2) ;
