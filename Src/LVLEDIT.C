//   ÚÄÄ-Ä-Ä--ùùú KRKAL úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
//   ³   		     Editor levelu
//   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú


#define UC unsigned char
#define UI unsigned int

#include <dos.h>
#include "midas.h"
#include "krkal.h"
#include "menu.h"

#define BLACK 1
#define COL1 246
#define COL2 143
#define COL3 118

extern UC PLAYER;
extern UC ANIM;
extern emsBlock *anim[4];
extern UC *animbuff;
extern void rotuj_paletu();
extern UC pvp,pvv,topv,topp,topgp;
extern struct {
  UC vec,v;
}pole[4][32][22];

extern struct {
  UC v,k; //1 - Doprava, 2 - nahoru, 3 doleva, 4 dolu
} laser[32][22][4];
extern UC vybuch[32][22];

extern UC pocet_env;
extern struct {
  UC jm[26];
  unsigned long int odkaz;
  UC ani;
} environment[30];

extern struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC brneni;
  UC otrava;
  UC odpocitavac;
  UC i[11];
} ty;

extern UC far obr[185][17][20];
extern UC postvec;
extern struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC a1,a2,a3;
  UC obr,frm;
} veci[250];


extern struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC zvp;
  struct{
    UC x,y,vec;
  }a[6];
}prep[100];

extern struct {
  UC jmeno[13]; //XOR s "Nemec je BLB"
  UC data[12];
  unsigned long int crc;
  UC pocet;
  UC level;
  UC hlasitost;
  UC klavesy[17];
} hraci[16];
extern UC hrac;
extern mpModule *mod;
extern mpInformation *info;
extern UC usek,hudusek;
extern UI nasobkyx[32],nasobkyy[22];
extern UC vec_1[182],vec_2[182],patro[182];
extern struct {
  unsigned long int odkaz;
  UC jmeno[16];
} levly[128];
extern UC far buffer[64000];
extern UC pal2[768];
extern UC graphmode;
extern UI mysx,mysy,mystl;
extern UI menu_x;

UC key_pr[128];
UC cur_key;

UC hudba,hudba_poc;

UC far mnu[40000],far msm[4][21][14];
UC S1;
UC press;
UC mex;
UI mbut,mbut2,mx,my,omx,omy,msx=9,msy=7;
UC far mkur[12][340];
UC mck;
UI ox,oy;
char SX,SY;
UC mm,m_pl,m_pr,m_shw,m_cdl,m_scr;
char m_pos;
UC hv,mv;
int m_sel;
UC jmeno[16];
UC cmnu;
UC cur_podl,cur_v,cur_a,cur_y;
UC move_x1,move_y1,move_x2,move_y2,move_nff;
UC changed;
int far m_cd[8]={57,13,8,29,7,9,20,28};
UC far m_dl[8][60]={{0,1,2,3,4,5,6,73,7,8,9,46,47,48,49,44,45,50,51,52,53,54,55,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43},
		 {56,57,58,59,63,64,65,66,67,68,150,151,152},
		 {86,87,88,89,90,119,120,121},
		 {200,62,107,108,109,110,111,112,113,114,115,116,124,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106},
		 {69,70,71,72,77,78,79},
		 {52,53,54,55,117,118,122,123,149},
		 {125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,145,146,147,148},
		 {153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180}
		 };
UC autom,a_poc=1;
UC az[2][18]={{ 91,106, 93, 95, 97,100, 94, 92, 99,105, 96,101, 91,102, 98,103,104,106},
	      {107,113,109,111,113,255,110,108,255,255,112,255,107,255,255,255,255,255}
	      };
UC apod[2]={8,9};
UC ap[56][2]={{  0, 0},{  0, 8},{  0, 0},{  0, 0},{  0, 0},{  0, 0},{  0, 0},{  0, 8},{  0,17},{  0,38},
	      {255, 1},{191, 1},{239, 1},{253, 1},{247, 1},{ 47, 1},{151, 1},{233, 1},{244, 1},{ 41, 1},
	      {  7, 1},{148, 1},{224, 1},{  1, 1},{  4, 1},{ 32, 1},{128, 1},{255, 6},{191, 6},{239, 6},
	      {253, 6},{247, 6},{ 47, 6},{151, 6},{233, 6},{244, 6},{ 41, 6},{  7, 6},{148, 6},{224, 6},
	      {  1, 6},{  4, 6},{ 32, 6},{128, 6},{  0, 0},{  0, 4},{  0, 0},{  0, 4},{  0, 0},{  0, 4},
	      {  0, 0},{  0, 4},{  0, 0},{  0, 4},{  0, 0},{  0, 4}
	     };

UC spojeno;
UC animm=0,anim_move_cnt=0;
char anim_move_sm=1;
UC sh_ctrl_alt;
char iszk;
UC ed_key;
int X,Y,X2,Y2,cX1,cY1,cX2,cY2,cSX,cSY;
UC vv,vp;
struct{
  UC vec,v;
}far clipboard[2][100];
struct {
  UC a1,a2,a3;
} far c_veci[100];
struct {
  UC zvp;
  struct{
    UC vc;      //1=vec je v clipboardu
    UC x,y,vec;
  }a[6];
}far c_prep[100];

UC ces_kl[9]={146,151,143,150,156,155,142,147,145};
UC ces_kl2[13][3]={{'a',142,1},{'c',143,2},{'d',144,2},{'e',145,3},
		   {'i',147,1},{'n',148,2},{'o',149,1},{'r',150,2},
		   {'s',151,2},{'t',152,2},{'u',153,3},{'y',155,1},
		   {'z',156,2},};

UI tex1_len,tex2_len;

UC VODA(UI f, UI g)
{
  UC cv;
  if (g%2) cv=f%4; else cv=(f+2)%4;
  if (cv==0) return 1;
  if (cv==1) return 74;
  if (cv==2) return 75;
  if (cv==3) return 76;
  return 1;
}


void kurz();

void gimg(int x,int y,char *b)
{
  int f,g;
  UI i=0;
  for (g=y;g<y+17;g++)
    for (f=x;f<x+20;f++)
    {
      if (f>=0&&g>=0&&f<320&&g<200){
      if (g>173) b[i]=getp(f,g-200); else b[i]=getp(f+sx,g+sy);
      }
      i++;
    }
}
void pimg(int x,int y,char *b)
{
  int f,g;
  UI i=0;
  for (g=y;g<y+17;g++)
    for (f=x;f<x+20;f++)
    {
      if (f>=0&&g>=0&&f<320&&g<200){
      if (g>173) putp(f,g-200,b[i]);else putp(f+ox,g+oy,b[i]);
      }
      i++;
    }
}
void pimg0(int x,int y,char *b)
{
  int f,g;
  UI i=0;
  for (g=y;g<y+17;g++)
    for (f=x;f<x+20;f++)
    {
      if (b[i]&&f>=0&&g>=0&&f<320&&g<200) if (g>173) putp(f,g-200,b[i]);else putp(f+sx,g+sy,b[i]);
      i++;
    }
}

void hkur()
{
  if (iszk){
  iszk=0;
  pimg(omx-msx,omy-msy,mkur[0]);
  ox=sx;oy=sy;
  }
}
void zkur()
{
  if (!iszk){
    iszk=1;
    gimg(mx-msx,my-msy,mkur[0]);
    pimg0(mx-msx,my-msy,mkur[mck]);
  }
}
void rk()
{
  iszk=1;
  pimg(omx-msx,omy-msy,mkur[0]);
  ox=sx;oy=sy;
  gimg(mx-msx,my-msy,mkur[0]);
  pimg0(mx-msx,my-msy,mkur[mck]);
}

void h3(UC x,UC y)
{
  hkur();
  hazej(nasobkyx[x],nasobkyy[y],pole[0][x][y].vec);
  if (pole[1][x][y].vec) hazej(nasobkyx[x],nasobkyy[y],pole[1][x][y].vec);
}

void zbp()
{
UC x,y;
for (y=0;y<22;y++)
  for (x=0;x<32;x++)
  {
    hazej(nasobkyx[x],nasobkyy[y],pole[0][x][y].vec);
    if (pole[1][x][y].vec) hazej(nasobkyx[x],nasobkyy[y],pole[1][x][y].vec);
  }
}
void gmxy(){
  asm {
    mov ax,3
    int 33h
    mov mbut,bx
    shr cx,1
    mov mx,cx
    mov my,dx
  }
}

void SMP()
{

asm{
mov ax,4
mov cx,mx
shl cx,1
mov dx,my
int 33h
}
}

void mouse() {
  UC tmp;
  if (mex) {
    tmp=mbut;
    gmxy();
    if ((hv||mv!=255)&&my>173) {my=173;SMP();}
    if (mx!=omx||my!=omy||sx!=ox||sy!=oy) rk();
    omx=mx;omy=my;
    mbut2=0;
    if (mbut!=0) {
      press=1;
    } else {
      if (tmp) mbut2=tmp;else{press = 0;hv=0;mv=255;m_scr=0;X2=255;Y2=255;}
    }
    kurz();
  } else mbut=0;
}

void mrst() {
  press = 1;
  mck=1;
  omx=-100;omy=-100;
  ox=sx;oy=sy;
  SX=0;SY=0;
  m_shw=0;
  iszk=0;
  asm {
    xor ax,ax
    int 33h
    mov mex,al
    and al,al
    jz konec
    mov ax,7
    mov cx,0
    mov dx,639
    int 33h
    mov ax,8
    mov cx,0
    mov dx,199
    int 33h
  }
  gmxy();
  konec:
}

//Zobrazi sipky v menu
void zsm(char smq)
{
int x,y;
for (y=0;y<21;y++)
  for (x=0;x<14;x++)
    if (smq==0||smq==2)
      {if (msm[smq][y][x])putp(x+64,-23+y,msm[smq][y][x]);} else
      if (msm[smq][y][x])putp(x+302,-23+y,msm[smq][y][x]);

}
//Zobrazi default podlahu
void zobraz_defpodl()
{
   hazej(40,-21,cur_podl);
}
//Zobrazi dlazdice v menu
void zb_mdl()
{
  UC i;
  zobraz_defpodl();
  if (m_pos==0) {zsm(2);m_pl=0;} else {zsm(0);m_pl=1;}
  if (m_pos+11>=m_cd[m_cdl]) {zsm(3);m_pr=0;} else {zsm(1);m_pr=1;}
  for (i=0;i<11;i++)
    {
    hazej(80+20*i,-21,181);
    if (i<m_cd[m_cdl]){
    if (m_dl[m_cdl][m_pos+i]==200) hazej(80+20*i,-21,183); else
    hazej(80+20*i,-21,m_dl[m_cdl][m_pos+i]);

    if (m_sel==m_dl[m_cdl][m_pos+i]) hazej(80+20*i,-21,182);
    }
    }
}
//Ramecek nevyplneny
void box(UI x1,UI y1, UI x2,UI y2,UC c)
{
  UI i;
  for (i=y1;i<=y2;i++) {putp(x1,i,c);putp(x2,i,c);}
  for (i=x1+1;i<x2;i++) {putp(i,y1,c);putp(i,y2,c);}
}



void dr_paste_fr(int xx,int yy)
{
  int a,b,x;
  if (xx<=31&&xx+cSX>0)
  {
    if (xx<0) a=0; else a=nasobkyx[xx];
    if (xx+cSX>31) b=639; else b=xx*20+nasobkyx[cSX]-1;
    for (x=a;x<=b;x++)
    {
      if (yy>=0&&yy<=21) putp(x,nasobkyy[yy],(x%10)+COL3);
      if (yy+cSY>0&&yy+cSY<=21) putp(x,nasobkyy[yy+cSY]-1,(x%10)+COL3);
    }
  }
  if (yy<=21&&yy+cSY>0)
  {
    if (yy<0) a=0; else a=nasobkyy[yy];
    if (yy+cSY>21) b=373; else b=yy*17+nasobkyy[cSY]-1;
    for (x=a;x<=b;x++)
    {
      if (xx>=0&&xx<=31) putp(nasobkyx[xx],x,(x%10)+COL3);
      if (xx+cSX>0&&xx+cSX<=31) putp(nasobkyx[xx+cSX]-1,x,(x%10)+COL3);
    }
  }
}

//Menu-Prepinace - turn on,off,shunt;change,on/off,move
void upd_m_pr()
{
  int f,g;
  UI i=0;
  hkur();
  sputimage(6,-23,38,21,mnu+9199);
  for (f=0;f<6;f++) sputimage(50+f*45,-24,44,23,mnu+9997);

  for (g=0;g<5;g++)
    for (f=0;f<36;f++)
      if (!mnu[9200+f+38*(g+1+(prep[cur_v].zvp-1)*7)]) putp(7+f,-22+g+(prep[cur_v].zvp-1)*7,COL2);
  for (i=0;i<6;i++)
  {
    if (prep[cur_v].a[i].x==255) box(53+i*45,-21,53+i*45+7,-5,COL1); else
    if (prep[cur_v].a[i].x==128) box(60+i*45,-13,60+i*45+15,-5,COL1); else
    if (prep[cur_v].a[i].x==129) box(75+i*45,-13,75+i*45+15,-5,COL1); else
    if (prep[cur_v].a[i].vec==251||prep[cur_v].a[i].vec==252) box(75+i*45,-21,75+i*45+15,-13,COL1);else
    box(60+i*45,-21,60+i*45+15,-13,COL1);
  }
  zkur();
}
//Prepinace - zapinani pasu
void ed_pasy(UC v,UC m)
{
  UC x=veci[v].x;
  UC y=veci[v].y;
  if (m==1){
    if (pole[2][x][y].vec==184) return;
    pole[2][x][y].vec=184;
    hazej(veci[v].ax,veci[v].ay,184);
  }else{
    if (pole[2][x][y].vec!=184) return;
    pole[2][x][y].vec=0;
    h3(x,y);
  }
  if (veci[v].vec==2&&PAS(pole[0][x+1][y].vec)&&x<31) ed_pasy(pole[0][x+1][y].v,m);
  if (veci[v].vec==4&&PAS(pole[0][x-1][y].vec)&&x>0) ed_pasy(pole[0][x-1][y].v,m);
  if (veci[v].vec==5&&PAS(pole[0][x][y+1].vec)&&y<21) ed_pasy(pole[0][x][y+1].v,m);
  if (veci[v].vec==3&&PAS(pole[0][x][y-1].vec)&&y>0) ed_pasy(pole[0][x][y-1].v,m);
}
//Prepinace - zobrazi co prepina
void upd_m_pr2()
{
  UC a;
  for (a=0;a<6;a++)if (a==cur_a||cur_a==255){
    if (prep[cur_v].a[a].x==255) ; else
    if (prep[cur_v].a[a].x==128) {
      hkur();
      if (PAS(veci[prep[cur_v].a[a].vec].vec))
	ed_pasy(prep[cur_v].a[a].vec,1);
      else hazej(veci[prep[cur_v].a[a].vec].ax,veci[prep[cur_v].a[a].vec].ay,184);
      zkur();
    }else
    if (prep[cur_v].a[a].x==129) {
      hkur();
      hazej(veci[prep[cur_v].a[a].vec].ax,veci[prep[cur_v].a[a].vec].ay,184);
      zkur();
    }else
  //  if (prep[cur_v].a[a].vec==251||prep[cur_v].a[a].vec==252); else
    {
      hkur();
      hazej(nasobkyx[prep[cur_v].a[a].x],nasobkyy[prep[cur_v].a[a].y],184);
      zkur();
    }
  }
}
//Prepinace - zrusi zobrazeni co prepina
void upd_m_pr3()
{
  UC x,y,a;
  for (a=0;a<6;a++)if (a==cur_a||cur_a==255){
    if (prep[cur_v].a[a].x==255) ; else
    if (prep[cur_v].a[a].x==128) {
      x=veci[prep[cur_v].a[a].vec].x;
      y=veci[prep[cur_v].a[a].vec].y;
      hkur();
      if (PAS(veci[prep[cur_v].a[a].vec].vec))
	ed_pasy(prep[cur_v].a[a].vec,0);
      else{
	h3(x,y);
      }
      zkur();
    }else
    if (prep[cur_v].a[a].x==129) {
      h3(veci[prep[cur_v].a[a].vec].x,veci[prep[cur_v].a[a].vec].y);
      zkur();
    }else
  //  if (prep[cur_v].a[a].vec==251||prep[cur_v].a[a].vec==252) ; else
    {
      hkur();
      h3(prep[cur_v].a[a].x,prep[cur_v].a[a].y);
      zkur();
    }
  }
}
//Prepinace - "animace"
void upd_m_pr4()
{
  UC a;
  for (a=0;a<6;a++)if (a==cur_a||cur_a==255){
    if (prep[cur_v].a[a].x==255) ; else
    if (prep[cur_v].a[a].x==128||prep[cur_v].a[a].x==129) {
      if (animm==0) {hkur();hazej(veci[prep[cur_v].a[a].vec].ax,veci[prep[cur_v].a[a].vec].ay,184);zkur();}
      if (animm==35) {hkur();h3(veci[prep[cur_v].a[a].vec].x,veci[prep[cur_v].a[a].vec].y);zkur();}
    }else
  //  if (prep[cur_v].a[a].vec==251||prep[cur_v].a[a].vec==252) ; else
    {
      if (animm==0) {
	hkur();
	h3(prep[cur_v].a[a].x,prep[cur_v].a[a].y);
	hazej(nasobkyx[prep[cur_v].a[a].x],nasobkyy[prep[cur_v].a[a].y],184);
	if (prep[cur_v].x==prep[cur_v].a[a].x&&prep[cur_v].y==prep[cur_v].a[a].y) hazej(prep[cur_v].ax,prep[cur_v].ay,182);
	zkur();

      }
      if (animm==25) {
	hkur();
	if (prep[cur_v].a[a].vec==251||prep[cur_v].a[a].vec==252){
	  hazej(nasobkyx[prep[cur_v].a[a].x],nasobkyy[prep[cur_v].a[a].y],pole[0][prep[cur_v].a[a].x][prep[cur_v].a[a].y].vec);
	  hazej(nasobkyx[prep[cur_v].a[a].x],nasobkyy[prep[cur_v].a[a].y],184);
	  if (prep[cur_v].x==prep[cur_v].a[a].x&&prep[cur_v].y==prep[cur_v].a[a].y) hazej(prep[cur_v].ax,prep[cur_v].ay,182);
	}else{
	  UC vec=prep[cur_v].a[a].vec;
	  UC p=patro[vec],x=prep[cur_v].a[a].x,y=prep[cur_v].a[a].y;
	  if (p==0) hazej(nasobkyx[x],nasobkyy[y],vec); else hazej(nasobkyx[x],nasobkyy[y],pole[0][x][y].vec);
	  if (p==1) hazej(nasobkyx[x],nasobkyy[y],vec); else if (patro[pole[1][x][y].vec]==1) hazej(nasobkyx[x],nasobkyy[y],pole[1][x][y].vec);
	  if (p==2) hazej(nasobkyx[x],nasobkyy[y],vec); else if (patro[pole[1][x][y].vec]==2) hazej(nasobkyx[x],nasobkyy[y],pole[1][x][y].vec);
	  hazej(nasobkyx[prep[cur_v].a[a].x],nasobkyy[prep[cur_v].a[a].y],184);
	  if (prep[cur_v].x==prep[cur_v].a[a].x&&prep[cur_v].y==prep[cur_v].a[a].y) hazej(prep[cur_v].ax,prep[cur_v].ay,182);
	}
       zkur();
      }
    }
  }
}

//Move - zobrazi znacky "od,do"
void zb_move()
{
  hkur();
  if (move_x1!=255) hazej(nasobkyx[move_x1],nasobkyy[move_y1],184);
  if (move_x2!=255) hazej(nasobkyx[move_x2],nasobkyy[move_y2],184);
  zkur();
}

//Move - zrusi znacky "od,do"
void zm_move()
{
  hkur();
  if (move_x1!=255) h3(move_x1,move_y1);
  if (move_x2!=255) h3(move_x2,move_y2);
  zkur();
}

//Move - animace
void anim_move(UC vec)
{
  if (move_nff) {   				//Animacni citac move
    anim_move_cnt+=anim_move_sm;
    if (anim_move_cnt==80) anim_move_sm=-1; else if (anim_move_cnt==0) anim_move_sm=1;
  }
  if (anim_move_cnt%4) return;
  hkur();
  if (anim_move_cnt>0) sfill(252+(anim_move_cnt>>2),-21,1,17,0);
  if (anim_move_cnt<80)sfill(273+(anim_move_cnt>>2),-21,1,17,0);
  hazej(253+(anim_move_cnt>>2),-21,vec);
  zkur();
}

//Menu - ON/OFF
void upd_m_nff()
{
  UC f,g;
  hkur();
    for (g=0;g<17;g++)
      for (f=0;f<20;f++)
	putp(f+150,g-21,mnu[f+150+320*(g+5)]);
    if (veci[cur_v].a3==1) hazej(150,-21,117); else hazej(150,-21,118);
  zkur();
}
//Menu - Right/Left
void upd_m_rl()
{
  UC f,g;
  hkur();
    for (g=0;g<17;g++)
      for (f=0;f<20;f++)
	putp(f+150,g-21,mnu[f+150+320*(g+5)]);
    if (veci[cur_v].a3==1) hazej(150,-21,71); else hazej(150,-21,69);
  zkur();
}
//Menu - Slow/Fast
void upd_m_slowfast()
{
  UC f,g;
  hkur();
    for (g=0;g<17;g++)
      for (f=0;f<20;f++)
	putp(f+150,g-21,mnu[f+150+320*(g+5)]);
    if (veci[cur_v].a3==1) hazej(150,-21,79); else hazej(150,-21,78);
  zkur();
}
//Menu - Teleport - zobrazi
void upd_m_tel1()
{
  hkur();
  hazej(nasobkyx[veci[cur_v].a1],nasobkyy[veci[cur_v].a2],184);
  zkur();
}
//Menu - Teleport - zrusi zobrazeni
void upd_m_tel2()
{
  hkur();
  h3(veci[cur_v].a1,veci[cur_v].a2);
  if (veci[cur_v].x==veci[cur_v].a1&&veci[cur_v].y==veci[cur_v].a2){
    hazej(veci[cur_v].ax,veci[cur_v].ay,182);
  }
  zkur();
}

//Menu - El.proud
void upd_m_elpr1()
{
  UC f,g;
  hkur();
  hazej(33+veci[cur_v].a3*6,-21,84);
  zkur();
}
void upd_m_elpr2()
{
  UC f,g;
  hkur();
  for (g=0;g<17;g++)
    for (f=0;f<20;f++)
      putp(f+33+veci[cur_v].a3*6,g-21,mnu[f+33+veci[cur_v].a3*6+320*(g+5)]);
  zkur();
}

//Hudba
void hud_ram()
{
   UC i;
   sfill(sx,sy,320,174,0);
   for (i=0;i<38;i++)
   {
     sputimage(sx+8+i*8,sy,8,8,mnu+15035);
     sputimage(sx+8+i*8,sy+166,8,8,mnu+15035);
   }
   for (i=0;i<20;i++)
   {
     sputimage(sx,sy+8+i*8,8,8,mnu+15099);
     sputimage(sx+312,sy+8+i*8,8,8,mnu+15099);
   }
   sputimage(sx,sy,8,8,mnu+14779);
   sputimage(sx+312,sy,8,8,mnu+14843);
   sputimage(sx,sy+166,8,8,mnu+14907);
   sputimage(sx+312,sy+166,8,8,mnu+14971);
}

void vypis_hudbu(UC i)
{
  UC col;
  if (hudba==i) col=165; else col=133;
  if (i<13)
    v4write2(sx+10,sy+12+i*12,col+3,col,1,buffer+i*28);
  else
    v4write2(sx+160,sy+12+(i-13)*12,col+3,col,1,buffer+i*28);
}

void vypis_hudby()
{
  FILE *f;
  int i;
  long tm;
  f=fopen("krkal.mus","rb");
  fseek(f,10,0);
  fread(&tm,4,1,f);
  hudba_poc=(tm-14)/32;
  for (i=0;i<hudba_poc;i++)
  {
    fseek(f,4,1);
    fread(buffer+i*28,28,1,f);
  }
  for (i=0;i<hudba_poc;i++) vypis_hudbu(i);
  fclose(f);
}

//GRAFIKA
void graf_prev()
{
  hkur();
  hazej(10,-21,1);
  hazej(35,-21,8);
  hazej(55,-21,9);
  hazej(80,-21,153);
  hazej(100,-21,157);
  hazej(120,-21,161);
  hazej(145,-21,165);
  hazej(180,-21,94);
  hazej(200,-21,92);
  hazej(220,-21,95);
  hazej(250,-21,110);
  hazej(270,-21,108);
  hazej(290,-21,111);
  zkur();
}

void vypis_graf(UC i)
{
  UC col;
  if (usek==i) col=165; else col=133;
  if (i<13)
    v4write2(sx+10,sy+12+i*12,col+3,col,1,environment[i].jm);
  else
    v4write2(sx+160,sy+12+(i-13)*12,col+3,col,1,environment[i].jm);
}



//Vraci 1, kdyz na tuhle vec muze prepinac neco zmenit
UC prep_chng(int vec)
{
if (mm==111&&patro[vec]==0) return 0;
if (vec==0||vec==1||(vec>=6&&vec<=51)||(vec>=56&&vec<=116)||(vec>=119&&vec<=121)||vec==124||(vec>=137&&vec<=144)) return 1;
return 0;
}
//Zobrazi menu
void zb_mnu(UC xx)
{
  int f,g;
  cmnu=xx;
  hkur();
  if (xx!=4) sputimage(0,-26,320,26,mnu); else sputimage(0,-26,320,26,mnu+16443);
  if(xx==0){
    sputimage(38,-23,24,21,mnu+8320);
    sputimage(7,-20,25,15,mnu+8824);
    sputimage(79,-22,222,1,mnu);
    sputimage(79,-4,222,1,mnu);
    sfill(79,-22,1,19,mnu[0]);
    sfill(300,-22,1,19,mnu[221]);
    zb_mdl();
  }else
  if (xx==1)
  {
    sputimage(6,-23,38,21,mnu+9199);
    for (f=0;f<6;f++) sputimage(50+f*45,-24,44,23,mnu+9997);
  }else
  if (xx==3)
  {
    sputimage0(80,-20,21,16,mnu+11009);
    sputimage0(220,-20,21,16,mnu+11345);
  }else
  if (xx==22)
  {
    sputimage0(19,-21,21,17,mnu+11681);
    sputimage0(55,-22,19,19,mnu+12038);
    sputimage0(89,-21,40,17,mnu+12399);
    sputimage0(144,-21,20,17,mnu+13079);
    sputimage0(179,-20,29,16,mnu+13419);
    sputimage0(223,-21,20,19,mnu+13883);
    sputimage0(257,-22,16,20,mnu+14263);
    sputimage0(288,-19,14,14,mnu+14583);
  }else
  if (xx==50)
  {
    sputimage0(8,-22,19,19,mnu+12038);
    sputimage(38,-23,24,21,mnu+8320);
    for (f=0;f<4;f++)
      sputimage0(125+f*25,-20,20,16,mnu+15163+f*320);
  }
  zkur();
}
//Vraci 1, kdyz de vec hazet 3x3
UC HH(UC vec)
{
if (vec>=114&&vec<=116) return 1;
if (PAS(vec)||PREPINAC(vec)||vec>51) return 0; else return 1;
}

UC ISMOVE(UC vec)
{
if (vec>=91&&vec<=106) return 1;
if (vec>=107&&vec<=113) return 1;
if (vec==124) return 1;
return 0;
}

UC JAKA_VEC(UC vec)
{
if (PAS(vec)) return 1;             //PAS
if (LASER(vec)) return 2;           //LASER
if (DELO(vec)) return 3;            //DELO
if (ELEKTRODA(vec)) return 4;       //ELEKTRODA
if (vec==149) return 5;             //TELEPORT
if (vec>=165&&vec<=168) return 10;  //OTACIVA POTVORA
if ((vec>=173&&vec<=176)||
  (vec>=177&&vec<=180)) return 11;  //ZNACKOVA POTVORA + KLAUSTROFOBNI POTVORA
if (PREPINAC(vec)) return 100;      //PREPINACE
return 255;
}

UC move_ok(UC x1,UC y1,UC x2,UC y2,UC x,UC y)
{
  if(x1==255||x2==255) return 0;
  if(x1==x&&x2==x&&y1<=y&&y2>=y) return 1;
  if(x1==x&&x2==x&&y1>=y&&y2<=y) return 1;
  if(y1==y&&y2==y&&x1<=x&&x2>=x) return 1;
  if(y1==y&&y2==y&&x1>=x&&x2<=x) return 1;
  return 0;
}

void hudba_load(UC hd,UC fade)
{
  FILE *s;
  int i;
  long s3mpos;
  if (fade){
    for (i=(hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20;i>=0;i--)
      {midasSD->SetMasterVolume(i);vr();}
  }
  s = fopen("krkal.mus","rb");
  fseek(s,14+hd*32,0);
  fread(&s3mpos,4,1,s);
  fclose(s);
  midasStopModule(mod);
  midasFreeModule(mod);
  mod = midasLoadModule("krkal.mus",s3mpos , &mpS3M, NULL);
  midasPlayModule(mod,4);
  midasSD->SetMasterVolume((hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20);
  midasSD->SetAmplification((hraci[hrac].hlasitost>>4)*20>63?(hraci[hrac].hlasitost>>4)*20:64);
}
void load_graf(UC u,UC u2)
{
  FILE *s;
  s = fopen("environ.dat","rb");
  fseek(s,10,0);
  fread(pal,3,256,s);
  if (u!=u2) {
    fseek(s,environment[u].odkaz,0);
    fread(obr,340,8,s);
    fread(obr[62],340,27,s);
    fread(obr[119],340,34,s);
    fread(obr[165],340,16,s);
    fread(obr[8],340,54,s);
    fread(obr[89],340,30,s);
    if (ANIM) {
      emsMap(anim[0],(void**)&animbuff);
      fread(animbuff+28560,32640,1,s);
      emsMap(anim[1],(void**)&animbuff);
      fread(animbuff+28560,32640,1,s);
      emsMap(anim[2],(void**)&animbuff);
      fread(animbuff+28560,1020,1,s);
      fread(animbuff+(28560+1360),24480,1,s);
      emsMap(anim[3],(void**)&animbuff);
      fread(animbuff+(28560+680),340,1,s);
      fread(animbuff+(28560+2040),4420,1,s);
      fread(animbuff+(28560+8840),17000,1,s);
    }
  }
  fclose(s);
}

void ed_reset()
{
  UC f,g;
  void *bf;
  FILE *s;
  pvp=0;pvv=0;mm=0;
  autom=1;
  hv=0;
  mv=255;
  ox=sx;oy=sy;
  changed=0;
  for (g=0;g<17;g++)
    for (f=0;f<20;f++)
    {
      obr[181][g][f]=BLACK;
    }
  s=fopen("lvledit.dat","rb");
  fread(mnu,24763,1,s);
  fread(mkur[1],340,11,s);
  fread(msm,294,4,s);
  fread(obr[182],340,3,s);
  fclose(s);
  m_pos=0;
  m_cdl=0;
  m_sel=m_dl[m_cdl][0];
  cur_a=255;
  SetPalette(pal);
  mrst();
  zb_mnu(0);
  zbp();
}

void ed_new()
{
  FILE *s;
  UI f,g;
  UC usek2;
  topv=0;topp=0;
  usek2=usek;usek=0;
  setxy(0,0);
  for (g=0;g<22;g++) for (f=0;f<32;f++) {
    pole[0][f][g].v = 255;
    pole[1][f][g].v = 255;
    pole[2][f][g].v = 255;
    if (cur_podl!=1) pole[0][f][g].vec = cur_podl; else pole[0][f][g].vec = VODA(f,g);
    pole[1][f][g].vec = 0;
    pole[2][f][g].vec = 0;
  }
  memset(vybuch,0,32*22);
  memset(laser,255,8*22*32);
  load_graf(usek,usek2);
  tex1_len=0;
  tex2_len=0;
  memset(buffer+38000,0,4000);
  ed_reset();
  changed=1;
}

void load(UC level)
{
  FILE *s;
  UC usek2=usek;
  UC f,g;
  UC tmp;
  UI p;
  memcpy(jmeno,levly[level].jmeno,16);
  s=fopen("krkal.lev","rb");
  fseek(s,levly[level].odkaz,0);
  fread(&usek,1,1,s);
  fread(&hudba,1,1,s);
  fread(&topv,1,1,s);
  fread(&topp,1,1,s);
  fread(&topgp,1,1,s);
  fread(&tex1_len,2,1,s);
  fread(&tex2_len,2,1,s);
  fread(&ty.x,1,1,s);
  fread(&ty.y,1,1,s);
  fread(buffer,2,704,s);
  for (g=0;g<22;g++)
    for (f=0;f<32;f++)
    {
      pole[0][f][g].v = 255;
      pole[1][f][g].v = 255;
      pole[2][f][g].v = 255;
      pole[0][f][g].vec = buffer[f+g*32];
      pole[1][f][g].vec = buffer[f+g*32+32*22];
      pole[2][f][g].vec = 0;
    }
  memset(vybuch,0,32*22);
  memset(laser,255,8*22*32);
  fread(buffer,topv,6,s);
  p=0;
  for (f=0;f<topv;f++) {
    veci[f].x = buffer[p];
    veci[f].y = buffer[p+1];
    veci[f].vec = buffer[p+2];
    pole[patro[veci[f].vec]!=0][veci[f].x][veci[f].y].v = f;
    veci[f].a1 = buffer[p+3];
    veci[f].a2 = buffer[p+4];
    veci[f].a3 = buffer[p+5];
    p+=6;
    veci[f].ax = veci[f].x*20;
    veci[f].ay = veci[f].y*17;
    veci[f].dx=0;
    veci[f].dy=0;
  }
  fread(buffer,topp,22,s);
  p=0;
  for (f=0;f<topp;f++) {
    prep[f].x = buffer[p];
    prep[f].y = buffer[p+1];
    prep[f].vec = buffer[p+2];
    pole[patro[prep[f].vec]!=0][prep[f].x][prep[f].y].v = f;
    prep[f].zvp = buffer[p+3];
    for (g=0;g<6;g++) {
      prep[f].a[g].x = buffer[p+4+g*3];
      prep[f].a[g].y = buffer[p+5+g*3];
      prep[f].a[g].vec = buffer[p+6+g*3];
      if (prep[f].a[g].x<32&&prep[f].a[g].vec==222){
	if (pole[1][prep[f].a[g].x][prep[f].a[g].y].vec!=0) prep[f].a[g].vec =251;
	else prep[f].a[g].vec =252;
      }
    }
    prep[f].ax = prep[f].x*20;
    prep[f].ay = prep[f].y*17;
    prep[f].dx=0;
    prep[f].dy=0;
    p+=22;
  }
  memset(buffer+38000,0,4000);
  fread(buffer+38000,tex1_len,1,s);
  fread(buffer+40000,tex2_len,1,s);
  fclose(s);
  load_graf(usek,usek2);
  if (PLAYER&&hudusek!=hudba) hudba_load(hudba,1);
  ed_reset();
}

void save(UC level)
{
  FILE *s;
  UC fname[13];
  UC f,g;
  UI i;
  if (level>=96) return;
  spojeno=0;
  changed=0;
  memcpy(levly[level].jmeno,jmeno,16);
  konvertuj_levlyjm(level,level);
  itoa(level+1,fname,10);
  strcat(fname,".lv");
  s=fopen(fname,"wb");
  fwrite(&usek,1,1,s);
  fwrite(&hudba,1,1,s);
  fwrite(&topv,1,1,s);
  fwrite(&topp,1,1,s);
  fwrite(&topgp,1,1,s);
  fwrite(&tex1_len,2,1,s);
  fwrite(&tex2_len,2,1,s);

  for (g=0;g<22;g++)
    for (f=0;f<32;f++)
      if (TY(pole[1][f][g].vec)) {ty.x=f;ty.y=g;}
  fwrite(&ty.x,1,1,s);
  fwrite(&ty.y,1,1,s);
  for (i=0;i<2;i++)
    for (g=0;g<22;g++)
      for (f=0;f<32;f++)
	buffer[i*704+g*32+f]=pole[i][f][g].vec;
  fwrite(buffer,704*2,1,s);
  i=0;
  for (f=0;f<topv;f++) {
    buffer[i]=veci[f].x;
    buffer[i+1]=veci[f].y;
    buffer[i+2]=veci[f].vec;
    buffer[i+3]=veci[f].a1;
    buffer[i+4]=veci[f].a2;
    buffer[i+5]=veci[f].a3;
    i+=6;
  }
  fwrite(buffer,i,1,s);
  i=0;
  for (f=0;f<topp;f++) {
    buffer[i]=prep[f].x;
    buffer[i+1]=prep[f].y;
    buffer[i+2]=prep[f].vec;
    buffer[i+3]=prep[f].zvp;
    for (g=0;g<6;g++)
    {
      buffer[i+4+g*3]=prep[f].a[g].x;
      buffer[i+5+g*3]=prep[f].a[g].y;
      buffer[i+6+g*3]=prep[f].a[g].vec;
    }
    i+=22;
  }
  fwrite(buffer,i,1,s);
  fwrite(buffer+38000,tex1_len,1,s);
  fwrite(buffer+40000,tex2_len,1,s);
  fclose(s);
}
void sp_lev()
{
  long i,p,pos,pos2;
  UC f;
  UC fname[13];
  FILE *s,*w;
  i=1;pos=0;
  itoa(i,fname,10);
  strcat(fname,".lv");
  w=fopen("krkal.lev","wb");
  s=fopen("hlavicka.","rb");
  fread(buffer,10,1,s);
  fclose(s);
  fwrite(buffer,10,1,w);
  fwrite(levly,20,96,w);
  pos2=1930;
  while((s=fopen(fname,"rb"))!=NULL&&i<96)
  {
    levly[i-1].odkaz=pos2;
    fseek(s,0,SEEK_END);
    p=ftell(s);
    fseek(s,0,SEEK_SET);
    pos2+=p;
    if (pos+p>64000){
      fwrite(buffer,pos,1,w);
      pos=0;
    }
    fread(&buffer[pos],p,1,s);
    pos+=p;
    fclose(s);
    i++;
    itoa(i,fname,10);
    strcat(fname,".lv");
  }
  fwrite(buffer,pos,1,w);
  fseek(w,10,SEEK_SET);
  for (f=0;f<96;f++)
  {
    if (f>=i) levly[f].odkaz=1930;
  }
  fwrite(levly,20,96,w);
  fclose(w);
}


void ed_init(UC levl)
{
  FILE *f;
  cSX=0;cSY=0;
  vv=0;vp=0;
  cur_podl=8;
  spojeno=1;
  scrinit(0,0);
  nastav_paletu0();
  if (levl<96) load(levl); else ed_new();
  if ((f=fopen("lvledit.cfg","rb"))==NULL) S1=3; else
  {
    fread(&S1,1,1,f);
    fread(&cur_podl,1,1,f);
    if (!HH(cur_podl)) cur_podl=8;
    zobraz_defpodl();
    fclose(f);
    if (S1<1||S1>6) S1=3;
  }
}

void tex_kh(int x,int y)
{
    sputimage(x+85,y*11+82,1,8,mnu+19434);
}
void tex_kz(int x,int y)
{
    sgetimage(x+85,y*11+82,1,8,mnu+19434);
    sfill(x+85,y*11+82,1,8,248);
}
void big_kh(int p)
{
    int x;
    UC tm;
    x=160-biglen(jmeno)/2;
    tm=jmeno[p];
    jmeno[p]=0;
    x+=biglen(jmeno);
    jmeno[p]=tm;
    sputimage(x,15,1,20,mnu+19454);
}
void big_kz(int p)
{
    int x;
    UC tm;
    x=160-biglen(jmeno)/2;
    tm=jmeno[p];
    jmeno[p]=0;
    x+=biglen(jmeno);
    jmeno[p]=tm;
    sgetimage(x,15,1,20,mnu+19454);
    sfill(x,15,1,20,248);
}

void wr_jm()
{
  sputimage(0,15,320,20,buffer+42000);
  writebig(160-biglen(jmeno)/2,15,jmeno);
}


void odstran_diru(UC *p,UI len,UI holepos,UI holesz)
{
  memmove(p+holepos,p+holepos+holesz,len-holepos);
  memset(p+len,0,holesz);
}
void udelej_diru(UC *p,UI len,UI holesz)
{
  memmove(p+holesz,p,len);
  memset(p,0,holesz);
}

void textedit()
{
  int x=0,y=0,t_sx,t_sy,holepos=0,holesz=250,len=0,lenpr=0;
  int posb=0,lenb=0;
  UC mode=1,text=1;
  FILE *s;
  UC pr=0,pr2=0;
  UC kl=0,klold=255,klp=0;
  UC tm,ces_hnus=0;
  UC *buf=(UC*)(buffer+38000);
  UI *rstart=(UI*)(mnu+19404);
  menu_x=0;
  t_sx=sx;
  t_sy=sy;
  nastav_paletu0();
  hkur();
  ssc(0,0,640);
  graphmode=0;
  memset(mnu,0,20000);
  scr_zm_mnu();
  {
    UC *p,*p2,f,g;
    s = fopen("menu.dat","r+b");
    fseek(s,112951,0);
    fread(buffer,1,32000,s);
    p2=buffer;
    for (f=0;f<2;f++) {
      asm {
	mov dx,0x3c4
	mov al,2
	mov ah,1
	mov cl,f
	shl ah,cl
	out dx,ax
      }
      p = (UC*)videoram+4160;
      for(g=0;g<200;g++) {
	memcpy(p,p2,80);
	p2+=80;
	p+=160;
      }
    }
    fread(buffer,1,32000,s);
    p2=buffer;
    for (f=2;f<4;f++) {
      asm {
	mov dx,0x3c4
	mov al,2
	mov ah,1
	mov cl,f
	shl ah,cl
	out dx,ax
      }
      p = (UC*)videoram+4160;
      for(g=0;g<200;g++) {
	memcpy(p,p2,80);
	p2+=80;
	p+=160;
      }
    }

    sgetimage(0,15,320,20,buffer+42000);
    sgetimage(85,79,196,99,mnu);
    fread(buffer,37912,1,s);
    fclose(s);
  }
  lenb=strlen(jmeno);
  posb=lenb;
  writebig(160-biglen(jmeno)/2,15,jmeno);
  big_kz(posb);
  big_kh(posb);
  mreset();
  v4write2(42-vlength("Švodn“")/2,91,254,253,1,"Švodn“");
  v4write2(42-vlength("Text")/2,99,254,253,1,"Text");
  v4write2(42-vlength("Konen›")/2,91+37,249,251,1,"Konen›");
  v4write2(42-vlength("Text")/2,99+37,249,251,1,"Text");
  v4write2(42-vlength("Do")/2,91+37*2,249,251,1,"Do");
  v4write2(42-vlength("Editoru")/2,99+37*2,249,251,1,"Editoru");
  v4write2(183-vlength("Editor textš")/2,56,249,250,1,"Editor textš");
  SetPalette(pal2);
  len=tex1_len;
  memset(rstart,0,30);
  udelej_diru(buf,len,holesz);
  pr=pocrad(buf+holepos+holesz,x,y,holepos);
  tex_vr(85,82,249,251,buf+holepos+holesz,0);
  tex_kz(x,y);

  while (kl!=27)
  {
    mystl=0;
    while (!kbhit()&&mystl==0) mysuj();
    if (mystl>0&&mysx>18&&mysx<67){
      if (mysy>84&&mysy<110&&text!=1) {
	v4write2(42-vlength("Švodn“")/2,91,254,253,1,"Švodn“");
	v4write2(42-vlength("Text")/2,99,254,253,1,"Text");
	v4write2(42-vlength("Konen›")/2,91+37,249,251,1,"Konen›");
	v4write2(42-vlength("Text")/2,99+37,249,251,1,"Text");
	odstran_diru(buf,len,holepos,holesz);
	if (mode==1) tex_kh(x,y);
	tex_vr(85,82,0,0,buf,0);
	tex2_len=len;
	text=1;
	buf=(UC*)(buffer+38000);
	holepos=0;holesz=250;len=tex1_len;lenpr=0;
	x=y=0;
	memset(rstart,0,30);
	udelej_diru(buf,len,holesz);
	pr=pocrad(buf+holepos+holesz,x,y,holepos);
	tex_vr(85,82,249,251,buf+holepos+holesz,0);
	if (mode==1) tex_kz(x,y);
      }else
      if (mysy>121&&mysy<147&&text!=2) {
	v4write2(42-vlength("Švodn“")/2,91,249,251,1,"Švodn“");
	v4write2(42-vlength("Text")/2,99,249,251,1,"Text");
	v4write2(42-vlength("Konen›")/2,91+37,254,253,1,"Konen›");
	v4write2(42-vlength("Text")/2,99+37,254,253,1,"Text");
	odstran_diru(buf,len,holepos,holesz);
	if (mode==1) tex_kh(x,y);
	tex_vr(85,82,0,0,buf,0);
	tex1_len=len;
	text=2;
	buf=(UC*)(buffer+40000);
	holepos=0;holesz=250;len=tex2_len;lenpr=0;
	x=y=0;
	memset(rstart,0,30);
	udelej_diru(buf,len,holesz);
	pr=pocrad(buf+holepos+holesz,x,y,holepos);
	tex_vr(85,82,249,251,buf+holepos+holesz,0);
	if (mode==1) tex_kz(x,y);
      }else
      if (mysy>158&&mysy<184) kl=27;
    }else
    if (mystl>0&&mysy<35)
    {
      UI a,b;
      big_kh(posb);
      posb=lenb+1;
      a=160-biglen(jmeno)/2;
      do {
	posb--;
	tm=jmeno[posb];
	jmeno[posb]=0;
	b=a+biglen(jmeno);
	jmeno[posb]=tm;
      }while (b>mysx&&posb>0);
      big_kz(posb);
      tex_kh(x,y);
      mode=2;
    }else
    if (mystl>0&&mysx>=85&&mysy>=82&&mysx<281&&mysy<181)
    {
      int f,g,incr=0;
      big_kh(posb);
      mode=1;
      tex_kh(x,y);
      x=(mysx-85);
      y=(mysy-82)/11;
      if (y>pr)
      {
	y=pr;
	if (len>0){
	  if (holepos<len){
	    if (holepos>=rstart[y])
	    {
		if (holepos!=rstart[y]&&(buf[rstart[y]]!=13||rstart[y]+1<holepos)) x=vlength(buf+rstart[y])+1; else x=0;
		if (len>rstart[y]+1) x+=vlength(buf+holepos+holesz)+1;
	    }else{
	      if (buf[rstart[y]+holesz]!=13||len>rstart[y]+1) x=vlength(buf+rstart[y]+holesz)+1; else x=0;
	    }
	  }else{
	    if (buf[rstart[y]]!=13||len>rstart[y]+1) x=vlength(buf+rstart[y])+1; else x=0;
	  }
	}else x=0;
	g=len;
      }else{
	g=rstart[y+1]+1;
	if (g==1) g=len+1;
	do{
	  g--;
	  if (g>=holepos) incr=holesz; else incr=0;
	  if (g>rstart[y]&&buf[g-1+incr]!=13)
	  {
	    if(holepos>=rstart[y]&&holepos<=g)
	    {
	      if (holepos!=rstart[y]&&(buf[rstart[y]]!=13||rstart[y]+1<holepos)) f=vlength(buf+rstart[y])+1; else f=0;
	      if (g!=holepos)
	      {
		tm=buf[g+incr];
		buf[g+incr]=0;
		f+=vlength(buf+holepos+holesz)+1;
		buf[g+incr]=tm;
	      }
	    }else{
	      tm=buf[g+incr];
	      buf[g+incr]=0;
	      f=vlength(buf+rstart[y]+incr)+1;
	      buf[g+incr]=tm;
	    }
	  }else f=0;
	}while (f>x+1);
	x=f;
      }
      if (holepos>g){
	if (holepos-g>holesz)
	{
	  memmove(buf+holesz+g,buf+g,holepos-g);   //tm
	  memset(buf+g,0,holesz);
	}else{
	  memmove(buf+holesz+g,buf+g,holepos-g);
	  memset(buf+g,0,holepos-g);
	}
      }else{
	memmove(buf+holepos,buf+holepos+holesz,g-holepos);
	memset(buf+g,0,holesz);
      }
      holepos=g;
      lenpr=g;
      tex_kz(x,y);
    }else
    if (mystl==0){
      kl=getch();
      klp=cur_key;
      sh_ctrl_alt=peekb(0,0x417);
      if (klp==13&&(sh_ctrl_alt&12)==0)
      {
	if(sh_ctrl_alt&3) ces_hnus=2; else ces_hnus=1;
      }else
      if (mode==1){
	if (klold!=0&&kl==9){
	  mode=2;
	  tex_kh(x,y);
	  big_kz(posb);
	}else
	if (klold==0&&kl==0x4B&&holepos>0){  //left
	  tex_kh(x,y);
	  holepos--;
	  if (buf[holepos]!=13) x-=vlength(buf+holepos)+1; else
	  {
	    buf[holepos]=0;
	    y--;
	    if (rstart[y+1]>0&&buf[rstart[y+1]-1]!=13) x=vlength(buf+rstart[y])+1; else x=0;
	    buf[holepos]=13;
	  }
	  buf[holepos+holesz]=buf[holepos];
	  buf[holepos]=0;
	  lenpr--;
	  if (holepos<rstart[y])
	  {
	    y--;
	    x=vlength(buf+rstart[y])+1;
	   }
	  tex_kz(x,y);
	}else
	if (klold==0&&kl==0x4D&&lenpr<len){        //right
	  tex_kh(x,y);
	  buf[holepos]=buf[holepos+holesz];
	  buf[holepos+holesz]=0;
	  if (buf[holepos]!=13) x+=vlength(buf+holepos)+1;
	  holepos++;
	  lenpr++;
	  if (holepos>rstart[y+1]&&lenpr<len&&y<pr)
	  {
	    y++;
	    if (buf[holepos-1]!=13) x=vlength(buf+holepos-1)+1; else x=0;
	   }
	  tex_kz(x,y);
	}else
	if (klold==0&&kl==0x48&&holepos>0&&y>0){  //up
	  int f,g;
	  tex_kh(x,y);
	  y--;
	  g=rstart[y+1]+1;
	  do{
	    g--;
	    if (g>rstart[y]&&buf[g-1]!=13)
	    {
	      tm=buf[g];
	      buf[g]=0;
	      f=vlength(buf+rstart[y])+1;
	      buf[g]=tm;
	    }else f=0;
	  }while (f>x+1);
	  x=f;
	  if (holepos-g>holesz)
	  {
	    memmove(buf+holesz+g,buf+g,holepos-g);   //tm
	    memset(buf+g,0,holesz);
	  }else{
	    memmove(buf+holesz+g,buf+g,holepos-g);
	    memset(buf+g,0,holepos-g);
	  }
	  holepos=g;
	  lenpr=g;
	  tex_kz(x,y);
	}else
	if (klold==0&&kl==0x50&&(rstart[y+1]>0||(lenpr<len&&holepos==0&&buf[holesz]==13))){  //down
	  int f,g;
	  tex_kh(x,y);
	  y++;
	  g=rstart[y+1]+1;
	  if (g==1) g=len+1;
	  do{
	    g--;
	    if (g>rstart[y]&&buf[g+holesz-1]!=13)
	    {
	      tm=buf[g+holesz];
	      buf[g+holesz]=0;
	      f=vlength(buf+rstart[y]+holesz)+1;
	      buf[g+holesz]=tm;
	    }else f=0;
	  }while (f>x+1);
	  x=f;
	  memmove(buf+holepos,buf+holepos+holesz,g-holepos);
	  memset(buf+g,0,holesz);
	  holepos=g;
	  lenpr=g;
	  tex_kz(x,y);
	}else
	if (klold==0&&(kl==0x47||kl==0x48)&&holepos>rstart[y])  //home
	{
	  int g=rstart[y];
	  tex_kh(x,y);
	  x=0;
	  if (buf[g]==13) g++;
	  if (holepos-g>holesz)
	  {
	    memmove(buf+holesz+g,buf+g,holepos-g); //tm
	    memset(buf+g,0,holesz);
	  }else{
	    memmove(buf+holesz+g,buf+g,holepos-g);
	    memset(buf+g,0,holepos-g);
	  }
	  holepos=g;
	  lenpr=g;
	  tex_kz(x,y);
	}else
	if (klold==0&&(kl==0x4F||kl==0x50)&&lenpr<len)  //end
	{
	  int g=rstart[y+1];
	  if (holepos!=0||buf[holesz]!=13){
	    if (g==0) g=len;
	    tex_kh(x,y);
	    memmove(buf+holepos,buf+holepos+holesz,g-holepos);
	    memset(buf+g,0,holesz);
	    holepos=g;
	    lenpr=g;
	    x=vlength(buf+rstart[y]);
	    if (x!=0) x++;
	    tex_kz(x,y);
	  }
	}else
	if (klold==0&&kl==0x53&&len!=lenpr){  //del
	  tex_kh(x,y);
	  changed=1;
	  pr2=pr;
	  pr=pocrad(buf+holepos+holesz+1,x,y,holepos);
	  if (buf[holepos+holesz]!=13)
	    tex_vr(x+85,y*11+82,0,0,buf+holepos+holesz,pr==pr2);
	  else
	    tex_vr(85,(y+1)*11+82,0,0,buf+holepos+holesz+1,pr==pr2);
	  buf[holepos+holesz]=0;
	  holesz++;
	  len--;
	  if (len!=lenpr) tex_vr(x+85,y*11+82,249,250,buf+holepos+holesz,pr==pr2); else rstart[y+1]=0;
	  tex_kz(x,y);
	}else
	if (klold!=0&&kl==0x8&&lenpr>0){  //backspace
	  UC en=0;
	  tex_kh(x,y);
	  holepos--;
	  changed=1;
	  if (holepos<rstart[y]||buf[holepos]==13)
	  {
	    y--;
	    if (buf[holepos]!=13) en=1;
	    if (rstart[y+1]>0&&buf[rstart[y+1]-1]!=13) x=vlength(buf+rstart[y])+1; else x=0;
	   }
	  if (buf[holepos]!=13)
	  {
	    x-=vlength(buf+holepos)+1;
	  }
	  pr2=pr;
	  pr=pocrad(buf+holepos+holesz+1,x,y,holepos);

	  if (en) {
	    tex_vr(x+85,y*11+82,0,0,buf+holepos,1);
	    tex_vr(85,(y+1)*11+82,0,0,buf+holepos+holesz+1,pr==pr2);
	  }else
	  {
	    if (buf[holepos]!=13){
	      buf[holepos+holesz]=buf[holepos];
	      tex_vr(x+85,y*11+82,0,0,buf+holepos+holesz,pr==pr2);
	      buf[holepos+holesz]=0;
	    }else tex_vr(85,(y+1)*11+82,0,0,buf+holepos+holesz+1,pr==pr2);
	  }
	  holesz++;
	  lenpr--;
	  len--;
	  buf[holepos]=0;
	  if (len!=lenpr) tex_vr(x+85,y*11+82,249,250,buf+holepos+holesz,pr==pr2); else rstart[y+1]=0;
	  tex_kz(x,y);
	}else
	if (klold==0&&kl==0x49&&lenpr>0){  //pgup
	  tex_kh(x,y);
	  memmove(buf+holesz,buf,holepos); //tm
	  memset(buf,0,holesz);
	  y=0;
	  x=0;
	  lenpr=0;
	  holepos=0;
	  tex_kz(x,y);
	}else
	if (klold==0&&kl==0x51&&lenpr<len){  //pgdn
	  tex_kh(x,y);
	  y=1;
	  if (holepos==0&&buf[holesz]==13) y++;
	  if (holepos!=0&&buf[0]==13) y++;
	  while(rstart[y]) y++;
	  y--;
	  memmove(buf+holepos,buf+holepos+holesz,len-lenpr);
	  memset(buf+len,0,holesz);
	  if (buf[rstart[y]]!=13||len>rstart[y]+1) x=vlength(buf+rstart[y])+1; else x=0;
	  lenpr=len;
	  holepos=len;
	  tex_kz(x,y);
	}else
	if ((klold!=0||(kl!=0&&((klp>=2&&klp<=11)||klp==13)))&&((kl>=32&&kl<=126)||kl==13||(kl==131&&klp==13))){   //psani
	  tex_kh(x,y);
	  if (klp>=3&&klp<=11&&(sh_ctrl_alt&79)==0) kl=ces_kl[klp-3]; else
	  if (klp>=3&&klp<=11&&(sh_ctrl_alt&15)==0) kl=ces_kl[klp-3]-15; else
	  if (klp>=2&&klp<=11&&(sh_ctrl_alt&12)) {
	    if (klp==11) kl = 48; else kl = klp+47;
	  }
	  if (klp==13){
	    if ((sh_ctrl_alt&3)) kl='+'; else kl='=';
	  }
	  if (ces_hnus)
	  {
	    UC g;
	    for (g=0;g<13;g++) {
	      if (kl==ces_kl2[g][0]) {
		if (ces_hnus==ces_kl2[g][2]||(ces_kl2[g][2]==3))
		{
		  if(ces_kl2[g][2]==3) kl=ces_kl2[g][1]+ces_hnus-1; else
		  kl=ces_kl2[g][1];
		}
	      }else
	      if (kl+32==ces_kl2[g][0]) {
		if (ces_hnus==ces_kl2[g][2]||(ces_kl2[g][2]==3))
		{
		  if(ces_kl2[g][2]==3) kl=ces_kl2[g][1]+ces_hnus-1-15; else
		  kl=ces_kl2[g][1]-15;
		}
	      }
	    }
	  }
	  pr2=pr;
	  buf[holepos+holesz-1]=kl;
	  pr=pocrad(buf+holepos+holesz-1,x,y,holepos);
	  buf[holepos+holesz-1]=0;
	  if (pr>=9) pr=pocrad(buf+holepos+holesz,x,y,holepos); else
	  {
	    changed=1;
	    if (len!=lenpr)
	    {
	     tex_vr(x+85,y*11+82,0,0,buf+holepos+holesz,pr==pr2);
	    }
	    if(x>191||kl==13) {
	      x=0;
	      y++;
	      rstart[y]=holepos;
	    }
	    buf[holepos]=kl;
	    if (kl!=13) v4write2(x+85,y*11+82,249,250,1,buf+holepos);
	    if (kl!=13) x+=vlength(buf+holepos)+1;
	    len++;
	    lenpr++;
	    holepos++;
	    holesz--;
	    if (len!=lenpr)
	    {
	      tex_vr(x+85,y*11+82,249,250,buf+holepos+holesz,pr==pr2);
	    }
	  }
	  tex_kz(x,y);

	}
	if (holesz<10)
	{
	  memmove(buf+holepos+250,buf+holepos+holesz,len-lenpr); //tm
	  memset(buf+holepos,0,250);
	  holesz=250;
	}
      }else{   //BIG
	if (klold!=0&&(kl==13||kl==9)){
	  big_kh(posb);
	  tex_kz(x,y);
	  mode=1;
	}else
	if (klold==0&&kl==0x4B&&posb>0){  //left
	  big_kh(posb);
	  posb--;
	  big_kz(posb);
	}else
	if (klold==0&&kl==0x4D&&posb<lenb){  //right
	  big_kh(posb);
	  posb++;
	  big_kz(posb);
	}else
	if (klold==0&&(kl==0x48||kl==0x47||kl==0x49)&&posb>0){  //home
	  big_kh(posb);
	  posb=0;
	  big_kz(posb);
	}else
	if (klold==0&&(kl==0x50||kl==0x4f||kl==0x51)&&posb<lenb){  //end
	  big_kh(posb);
	  posb=lenb;
	  big_kz(posb);
	}else
	if (klold==0&&kl==0x53&&posb<lenb){  //del
	  big_kh(posb);
	  memmove(jmeno+posb,jmeno+posb+1,lenb-posb);
	  lenb--;
	  changed=1;
	  wr_jm();
	  big_kz(posb);
	}else
	if (klold!=0&&kl==0x8&&posb>0){  //backspace
	  big_kh(posb);
	  memmove(jmeno+posb-1,jmeno+posb,lenb-posb+1);
	  changed=1;
	  lenb--;
	  posb--;
	  wr_jm();
	  big_kz(posb);
	}else
	if (lenb<15&&(klold!=0||(kl!=0&&((klp>=2&&klp<=11)||klp==13)))&&((kl>=32&&kl<=126)||(kl==131&&klp==13))){   //psani
	  big_kh(posb);
	  changed=1;
	  if (klp>=3&&klp<=11&&(sh_ctrl_alt&79)==0) kl=ces_kl[klp-3]; else
	  if (klp>=3&&klp<=11&&(sh_ctrl_alt&15)==0) kl=ces_kl[klp-3]-15; else
	  if (klp>=2&&klp<=11&&(sh_ctrl_alt&12)) {
	    if (klp==11) kl = 48; else kl = klp+47;
	  }
	  if (klp==13){
	    if ((sh_ctrl_alt&3)) kl='+'; else kl='=';
	  }
	  if (ces_hnus)
	  {
	    UC g;
	    for (g=0;g<13;g++) {
	      if (kl==ces_kl2[g][0]) {
		if (ces_hnus==ces_kl2[g][2]||(ces_kl2[g][2]==3))
		{
		  if(ces_kl2[g][2]==3) kl=ces_kl2[g][1]+ces_hnus-1; else
		  kl=ces_kl2[g][1];
		}
	      }else
	      if (kl+32==ces_kl2[g][0]) {
		if (ces_hnus==ces_kl2[g][2]||(ces_kl2[g][2]==3))
		{
		  if(ces_kl2[g][2]==3) kl=ces_kl2[g][1]+ces_hnus-1-15; else
		  kl=ces_kl2[g][1]-15;
		}
	      }
	    }
	  }
	  if (posb!=lenb) memmove(jmeno+posb+1,jmeno+posb,lenb-posb+1);
	  jmeno[posb]=kl;
	  if (posb==lenb) jmeno[posb+1]=0;
	  posb++;
	  lenb++;
	  wr_jm();
	  big_kz(posb);
	}
      }

      klold=kl;
      if (klp!=13) ces_hnus=0;
    }
  }
  do {
    while(kbhit()) getch();
    for (x=0;x<10;x++) mysuj();
  } while(kbhit());
  odstran_diru(buf,len,holepos,holesz);
  if (text==1) tex1_len=len; else tex2_len=len;
  nastav_paletu0();
  s=fopen("lvledit.dat","rb");
  fread(mnu,24763,1,s);
  fclose(s);
  scr_zb_mnu();
  ssc(t_sx,t_sy,640);
  zbp();
  zb_mnu(mm);
  SetPalette(pal);
  graphmode=1;
}


//Nastavuje kurzory...
void kurz()
{
  UC mck2=1;
  UC a,b;
  m_shw=0;
  SX=0;SY=0;
  if (mm==0||mm==1||mm==2||mm==3||mm==4||mm==5||mm==6||mm==7||mm==102||mm==103||mm==104||mm==105||mm==160||mm==161||mm==200||mm==201||mm==202||mm==203){
//    if(hv||mv!=255||m_scr||!press)
  {
    if ((mx==0||key_pr[0x4b])&&sx>0) SX=-1;
    if ((my==0||key_pr[0x48])&&sy>0) SY=-1;
    if ((mx==319||key_pr[0x4d])&&sx<320) SX=1;
    if (((my==199||(my==173&&(hv||mv!=255)))||key_pr[0x50])&&sy<200) SY=1;
    if (SX==0&&SY==0) mck2=1; else
    {
      m_scr=1;
      if (mx==0||my==0||mx==319||my==199||(my==173&&(hv||mv!=255)))
      {
	if (SX==-1&&SY==0) mck2=7;else
	if (SX==1&&SY==0) mck2=6;else
	if (SX==0&&SY==-1) mck2=4;else
	if (SX==0&&SY==1) mck2=5;else
	if (SX==-1&&SY==-1) mck2=8;else
	if (SX==-1&&SY==1) mck2=9;else
	if (SX==1&&SY==1) mck2=10;else
	if (SX==1&&SY==-1) mck2=11;
      }else m_scr=2;
    }
    SX=SX*S1;SY=SY*S1;
    }
    if (my<=173&&(mck2<=3||hv||mv!=255)) {m_shw=1;X=(mx+sx)/20;Y=(my+sy)/17;}
  }else mck2=1;
  if (mm==0)   //HAZENI A MAZANI VECI
  {
    if (m_scr!=1){
      if (mck2<=3&&m_shw==1){
      if (PREPINAC(pole[0][X][Y].vec)||PREPINAC(pole[1][X][Y].vec)) {m_shw=15;mck2=2;} else
      if (JAKA_VEC(pole[0][X][Y].vec)<=3||JAKA_VEC(pole[1][X][Y].vec)<=3) {m_shw=16;mck2=2;} else
      if (JAKA_VEC(pole[1][X][Y].vec)==5) {m_shw=17;mck2=2;} else
      if (JAKA_VEC(pole[1][X][Y].vec)==10) {m_shw=18;mck2=2;} else
      if (JAKA_VEC(pole[1][X][Y].vec)==11) {m_shw=19;mck2=2;} else
      if (ISMOVE(pole[0][X][Y].vec)||ISMOVE(pole[1][X][Y].vec)) {m_shw=20;mck2=2;} else
      if (JAKA_VEC(pole[1][X][Y].vec)==4) {m_shw=21;mck2=2;}
      }
      if (my>=179&&my<199&&mx>64&&mx<78&&m_pl) {mck2=2;m_shw=10;}else
      if (my>=179&&my<199&&mx>302&&mx<316&&m_pr) {mck2=2;m_shw=11;}else
      if (mx>40&&mx<=60&&my>=179&&my<=195) {mck2=2;m_shw=12;} else
      if (mx>=7&&mx<=32&&my>=179&&my<=195) {mck2=2;m_shw=13;} else
      if (my>=179&&my<=195&&mx>80&&mx<300){
	 a=(mx-80)/20;
	 if (m_sel!=m_dl[m_cdl][a+m_pos]&&a<m_cd[m_cdl]) {m_shw=100+a;mck2=2;}else m_shw=99;
      }
    }
  }
  if (mm==1)  //PREPINACE
  {
    if(my>173&&m_scr!=1&&mck2<=3){
      if(mx>=8&&mx<=43&&my>177&&my<183&&prep[cur_v].zvp!=1) {m_shw=91;mck2=2;} else
      if(mx>=8&&mx<=43&&my>184&&my<190&&prep[cur_v].zvp!=2) {m_shw=92;mck2=2;} else
      if(mx>=8&&mx<=43&&my>191&&my<197&&prep[cur_v].zvp!=3) {m_shw=93;mck2=2;} else
      if(mx>52&&mx<318&&my>177&&my<197)
      {
	int i,j;
	i=(mx-54)/45;
	j=(mx-54)%45;
	if (j<37) m_shw=100+i*10;
	if (j>0&&j<7&&my>179&&my<195) {mck2=2;m_shw+=1;}
	if (j>7&&j<22&&my>179&&my<187) {mck2=2;m_shw+=2;}
	if (j>22&&j<37&&my>179&&my<187) {mck2=2;m_shw+=3;}
	if (j>7&&j<22&&my>187&&my<195) {mck2=2;m_shw+=4;}
	if (j>22&&j<34&&my>187&&my<195) {mck2=2;m_shw+=5;}
      }
    }
  }
  if (mm==2||mm==4||mm==5) // ON-OFF + POTVORY
  {
    if (mx>=150&&mx<=170&&my>=178&&my<=195) {mck2=2;m_shw=10;}
  }
  if (mm==3) //TELEPORT
  {
    if (m_shw==1) mck2=3;
  }
  if (mm==6) //MOVE
  {
    if (mx>=130&&mx<=170&&my>=176&&my<=198) {mck2=2;m_shw=200;}
    if (mx>=200&&mx<=240&&my>=176&&my<=198) {mck2=2;m_shw=201;}
    if (mx>253&&mx<294&&my>178&&my<196) {mck2=2;m_shw=202;}
    if (mx>305&&mx<319&&my>180&&my<194) {mck2=2;m_shw=203;}
  }else
  if (mm==7) // El.proud
  {
    if (mx>=33&&mx<=287&&my>=178&&my<=195) {mck2=2;m_shw=11;}
  }else
  if (mm==22) //Hlavni menu
  {
    if(my>=179&&my<=196&&mx>=19&&mx<=40) {mck2=2;m_shw=11;}
    if(my>=178&&my<=197&&mx>=55&&mx<=74) {mck2=2;m_shw=12;}
    if(my>=179&&my<=196&&mx>=89&&mx<=129) {mck2=2;m_shw=13;}
    if(my>=179&&my<=196&&mx>=144&&mx<=164) {mck2=2;m_shw=14;}
    if(my>=180&&my<=196&&mx>=179&&mx<=208) {mck2=2;m_shw=15;}
    if(my>=179&&my<=197&&mx>=223&&mx<=242) {mck2=2;m_shw=16;}
    if(my>=178&&my<=198&&mx>=257&&mx<=273) {mck2=2;m_shw=17;}
    if(my>=181&&my<=195&&mx>=288&&mx<=302) {mck2=2;m_shw=18;}
  }else
  if (mm==50) //Hudba
  {
    if(my>=180&&my<=196){
      if(mx>=125&&mx<=145) {mck2=2;m_shw=11;}
      if(mx>=150&&mx<=170) {mck2=2;m_shw=12;}
      if(mx>=175&&mx<=195) {mck2=2;m_shw=13;}
      if(mx>=200&&mx<=220) {mck2=2;m_shw=14;}
    }
  }else
  if (mm==10&&m_scr!=1&&mck2<=3)
  {
    mck2=3;
    if (my>=179&&my<199&&mx>64&&mx<78&&m_pl) {mck2=2;m_shw=10;}else
    if (my>=179&&my<199&&mx>302&&mx<316&&m_pr) {mck2=2;m_shw=11;}else
    if (my>=179&&my<=195&&mx>80&&mx<300){
       a=(mx-80)/20;
       if (patro[m_dl[m_cdl][a+m_pos]]==0&&HH(m_dl[m_cdl][a+m_pos])&&a<m_cd[m_cdl]) {m_shw=100+a;mck2=2;}else m_shw=99;
    }
  }
  if (mm==104&&m_scr!=1&&mck2<=3)  //PREPINAC -> ON-OFF
  {
    mck2=3;
    if (m_shw==1) if (JAKA_VEC(pole[0][X][Y].vec)<=3||JAKA_VEC(pole[1][X][Y].vec)<=3) {mck2=2;m_shw=10;}
  }
  if (mm==105&&m_scr!=1&&mck2<=3)  //PREPINAC -> MOVE
  {
    mck2=3;
    if (m_shw==1) if (pole[1][X][Y].v!=255&&ISMOVE(pole[1][X][Y].vec)) {mck2=2;m_shw=10;}
  }
  if ((mm==102||mm==103)&&m_scr!=1&&mck2<=3) //PREPINAC -> CHANGE
  {
    mck2=3;
  }
  if ((mm==110||mm==111)&&m_scr!=1&&mck2<=3)
  {
    mck2=3;
    if (my>=179&&my<199&&mx>64&&mx<78&&m_pl) {mck2=2;m_shw=10;}else
    if (my>=179&&my<199&&mx>302&&mx<316&&m_pr) {mck2=2;m_shw=11;}else
    if (my>=179&&my<=195&&mx>80&&mx<300){
       a=(mx-80)/20;
       if (prep_chng(m_dl[m_cdl][a+m_pos])&&a<m_cd[m_cdl]) {m_shw=100+a;mck2=2;}else m_shw=99;
    }
  }
  if ((mm==160||mm==161)&&m_scr!=1&&mck2<=3) //move - od do
  {
    mck2=3;
    if(m_shw==1&&(X==cur_a||Y==cur_y)) mck2=2;
  }
  if (mm==200||mm==201||mm==202||mm==203) mck2=3;
  if (cmnu==3){  			// ANO-NE
    if (mx>=64&&mx<=120&&my>=176&&my<=198) {mck2=2;m_shw=200;}
    if (mx>=200&&mx<=257&&my>=176&&my<=198) {mck2=2;m_shw=201;}
  }
  if (mck2!=mck){hkur();mck=mck2;zkur();}
}

//Automaticka zed
UC get_zed(int x,int y,UC t)
{
  int i;
  if (x<0||x>31||y<0||y>21) return 255;
  if (pole[1][x][y].vec<az[t][0]||pole[1][x][y].vec>az[t][1]) return 255;
  for (i=0;i<16;i++)
    if (pole[1][x][y].vec==az[t][i+2]) return i;
  return 255;
}

UC postav_zed(int x,int y,UC t,UC a)
{
  int i;
  if (x<0||x>31||y<0||y>21) return 255;
  if (az[t][a+2]==255) return 255;
  if (pole[1][x][y].vec!=az[t][a+2])
  {
    pole[1][x][y].vec=az[t][a+2];
    h3(x,y);
  }
  return a;
}

void choz(UC x,UC y,int vec)
{
  int t,a1=255,a2=255,a3=255,a4=255,a,aa,b1,b2,b3,b4;
  if (vec==200) t=0;
  b1=get_zed(x-1,y,t);
  b3=get_zed(x+1,y,t);
  b2=get_zed(x,y-1,t);
  b4=get_zed(x,y+1,t);
  if (b1!=255) a1=postav_zed(x-1,y,t,b1|4);
  if (b2!=255) a2=postav_zed(x,y-1,t,b2|8);
  if (b3!=255) a3=postav_zed(x+1,y,t,b3|1);
  if (b4!=255) a4=postav_zed(x,y+1,t,b4|2);
  a=0;
  if (a1==255) a1=0; else a|=1;
  if (a2==255) a2=0; else a|=2;
  if (a3==255) a3=0; else a|=4;
  if (a4==255) a4=0; else a|=8;
  aa=postav_zed(x,y,t,a);
  if (aa!=a)
  {
  postav_zed(x,y,t,0);
  if (b1!=255) a1=postav_zed(x-1,y,t,b1&11);
  if (b2!=255) a2=postav_zed(x,y-1,t,b2&7);
  if (b3!=255) a3=postav_zed(x+1,y,t,b3&14);
  if (b4!=255) a4=postav_zed(x,y+1,t,b4&13);
  }
  return;
}
void choz2(UC x,UC y,int vec)
{
  int t,a1=255,a2=255,a3=255,a4=255,a,aa,b1,b2,b3,b4;
  if (vec==200) t=0;
  b1=get_zed(x-1,y,t);
  b3=get_zed(x+1,y,t);
  b2=get_zed(x,y-1,t);
  b4=get_zed(x,y+1,t);
  if (b1!=255) a1=postav_zed(x-1,y,t,b1&11);
  if (b2!=255) a2=postav_zed(x,y-1,t,b2&7);
  if (b3!=255) a3=postav_zed(x+1,y,t,b3&14);
  if (b4!=255) a4=postav_zed(x,y+1,t,b4&13);
  a=0;
  if (a1==255) a1=0; else a&=14;
  if (a2==255) a2=0; else a&=13;
  if (a3==255) a3=0; else a&=11;
  if (a4==255) a4=0; else a&=7;
  aa=postav_zed(x,y,t,a);
  if (aa!=a)
  {
  postav_zed(x,y,t,0);
  if (b1!=255) a1=postav_zed(x-1,y,t,b1|4);
  if (b2!=255) a2=postav_zed(x,y-1,t,b2|8);
  if (b3!=255) a3=postav_zed(x+1,y,t,b3|1);
  if (b4!=255) a4=postav_zed(x,y+1,t,b4|2);
  }
  return;
}

UC ispod(int x,int y,UC pr)
{
  UC vec=pole[0][x][y].vec;
  if (x<0||x>31||y<0||y>21) return 0;
  if (vec>=74&&vec<=76) vec=1;
  if (((ap[vec][1]&12)>>2)==pr) return 1;
  return 0;
}

void postav_pod(int x,int y,UC b,UC pr)
{
  UC i,bb=0;
  if (x<0||x>31||y<0||y>21) return;
  for (i=0;i<56;i++)
    if((ap[i][1]&3)==pr&&ap[i][0]==b) {
    if (pole[0][x][y].vec!=i) bb=1;
    if (i==1) pole[0][x][y].vec=VODA(x,y);else pole[0][x][y].vec=i;
    break;
    }
  if(bb){
    h3(x,y);
    zkur();
  }
}

void chop(int x,int y)
{
  UC pr=ap[pole[0][x][y].vec][1]&3,b[8];
  UC i,aa,bb;
  int f,g;
  if (pole[0][x][y].vec>=74&&pole[0][x][y].vec<=76) pr=ap[1][1]&3;
  if(x<0||x>31||y<0||y>21) return;
  memset(b,0,8);
  i=0;
  for (g=-1;g<=1;g++)
    for (f=-1;f<=1;f++)
      if (f||g) {
	b[i]=ispod(x+f,y+g,pr);
	i++;
	}
  if (b[1]||b[3]) b[0]=1;
  if (b[1]||b[4]) b[2]=1;
  if (b[6]||b[3]) b[5]=1;
  if (b[6]||b[4]) b[7]=1;
  bb=0;
  for (i=0;i<8;i++) if(b[i]) bb+=1<<i;
  postav_pod(x,y,bb,pr);
}

void znic_vec(UC v,UC vec)
{
  UI i,j,f;
  if (PREPINAC(vec)){
    if (v>=topp) return;
    pole[patro[vec]!=0][prep[v].x][prep[v].y].v=255;
    for (i=v;i<topp-1;i++)
    {
      prep[i]=prep[i+1];
      pole[patro[prep[i].vec]!=0][prep[i].x][prep[i].y].v=i;
    }
    topp--;
  }else{
    if (v>=topv) return;
    pole[patro[vec]!=0][veci[v].x][veci[v].y].v=255;
    for (i=v;i<topv-1;i++)
      {
	veci[i]=veci[i+1];
	pole[patro[veci[i].vec]!=0][veci[i].x][veci[i].y].v=i;
      }
    for (i=0;i<topp;i++)
      for (j=0;j<6;j++)
	{
	  if((prep[i].a[j].x==128||prep[i].a[j].x==129)&&prep[i].a[j].vec==v) memset(&prep[i].a[j],255,3); else
	  if((prep[i].a[j].x==128||prep[i].a[j].x==129)&&prep[i].a[j].vec>v) prep[i].a[j].vec--;
	}
    for (i=0;i<vp;i++)
      for (j=0;j<6;j++)
	if (c_prep[i].a[j].vc!=1){
	  if((c_prep[i].a[j].x==128||c_prep[i].a[j].x==129)&&c_prep[i].a[j].vec==v) memset(&c_prep[i].a[j],255,4); else
	  if((c_prep[i].a[j].x==128||c_prep[i].a[j].x==129)&&c_prep[i].a[j].vec>v) c_prep[i].a[j].vec--;
	}
    topv--;
  }
}

void destroy(UC p,int f,int g);


//Postavi vec
void build(UC p,int f,int g,int vec)
{
  int x,y,i;
  if (f>=0&&f<32&&g>=0&&g<22)
  if (vec!=pole[p][f][g].vec||(p==0&&ap[vec][1])){
    if (pole[p][f][g].v!=255) destroy(p,f,g);
    if (autom&&(vec<az[0][0]||vec>az[0][1])&&pole[p][f][g].vec>=az[0][0]&&pole[p][f][g].vec<=az[0][1]) choz2(f,g,200);
    if (vec==200) choz(f,g,vec); else
    {
      if (JAKA_VEC(vec)!=255){
	if (JAKA_VEC(vec)<100)
	{
	  if (topv>=200) return;
	  veci[topv].x=f;
	  veci[topv].y=g;
	  veci[topv].vec=vec;
	  veci[topv].dx=0;veci[topv].dy=0;
	  veci[topv].ax=nasobkyx[f];
	  veci[topv].ay=nasobkyy[g];
	  veci[topv].a1=255;veci[topv].a2=255;veci[topv].a3=255;
	  if (JAKA_VEC(vec)<=3) veci[topv].a3=1; else //PAS+LASER+DELO
	  if (JAKA_VEC(vec)==4) veci[topv].a3=0; else //ELEKTRODA
	  if (JAKA_VEC(vec)==5) {veci[topv].a1=f;veci[topv].a2=g;} else //TELEPORT
	  if (JAKA_VEC(vec)<=11) veci[topv].a3=0; //POTVORY
	  pole[p][f][g].v=topv;
	  topv++;
	}else if (JAKA_VEC(vec)==100)
	{
	  if (topp>=100) return;
	  prep[topp].x=f;
	  prep[topp].y=g;
	  prep[topp].vec=vec;
	  prep[topp].dx=0;prep[topp].dy=0;
	  prep[topp].ax=nasobkyx[f];
	  prep[topp].ay=nasobkyy[g];
	  prep[topp].zvp=3;
	  memset(prep[topp].a,255,6*3);
	  pole[p][f][g].v=topp;
	  topp++;
	}
      }
      if (pole[p][f][g].vec!=vec) {
	if (vec!=1) pole[p][f][g].vec=vec; else pole[p][f][g].vec=VODA(f,g);
	h3(f,g);
      }
      if (p==0&&autom&&(sh_ctrl_alt&16)&&((ap[vec][1]&3)==0||(ap[vec][1]&3)==((ap[vec][1]&48)>>4))){
	pole[p][f][g].vec=vec;
	for (y=g-1;y<=g+1;y++)
	  for (x=f-1;x<=f+1;x++)
	    if (pole[0][x][y].vec>=74&&pole[0][x][y].vec<=76)
	      {if ((ap[1][1]&3)!=0) chop(x,y);}
	    else {if ((ap[pole[0][x][y].vec][1]&3)!=0) chop(x,y);}
	if (pole[p][f][g].vec==1) pole[p][f][g].vec=VODA(f,g);
      }
    }
  }
}
//Znici vec
void destroy(UC p,int f,int g)
{
  int x,y;
  if (f>=0&&f<32&&g>=0&&g<22){
    if (p==1&&pole[1][f][g].vec==0) return;
    if (p==0&&pole[0][f][g].vec==cur_podl&&!ap[cur_podl][1]) return;
    if (pole[p][f][g].v!=255) znic_vec(pole[p][f][g].v,pole[p][f][g].vec);
    if (autom&&pole[p][f][g].vec>=az[0][0]&&pole[p][f][g].vec<=az[0][1]) choz2(f,g,200);
    if (p==1&&pole[p][f][g].vec!=0) {pole[p][f][g].vec=0;h3(f,g);} else
    if (pole[p][f][g].vec!=cur_podl) {
      if (cur_podl!=1) pole[p][f][g].vec=cur_podl; else pole[p][f][g].vec=VODA(f,g);
      h3(f,g);
    }
    if (p==0&&autom&&(sh_ctrl_alt&16)){
      pole[p][f][g].vec=cur_podl;
      for (y=g-1;y<=g+1;y++)
	for (x=f-1;x<=f+1;x++)
	  if (pole[0][x][y].vec>=74&&pole[0][x][y].vec<=76)
	  {if ((ap[1][1]&3)!=0) chop(x,y);}
	  else {if ((ap[pole[0][x][y].vec][1]&3)!=0) chop(x,y);}
    }
  }
}

void interrupt (*old_ed_key_handler)();

void interrupt ed_key_handler()
{
   cur_key=inportb(0x60);
   if(cur_key&128) key_pr[cur_key&127]=0; else key_pr[cur_key]=1;
   old_ed_key_handler();
}



//   ÚÄÄ-Ä-Ä--ùùú Lvledit úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
//   ³   		         Main
//   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú

void lvledit(UC lvl)
{
  UC end=0;
  UC shft;
  int f,g;
  ed_init(lvl);
  cur_key=0;
  old_ed_key_handler=getvect(0x9);
  while(kbhit()) getch();
  setvect(0x09,ed_key_handler);
  while (!end)
  {
    rotuj_paletu();
    animm++;  						  //Animacni citac
    if (animm>50) animm=0;
    if (sh_ctrl_alt!=peekb(0,0x417)) {X2=255;Y2=255;}
    sh_ctrl_alt=peekb(0,0x417);                                  //Shift pro 3x3 hazeni
    if (((sh_ctrl_alt&3)!=0)^((sh_ctrl_alt&64)==0)) shft=0;else shft=1;
    mouse();
    ed_key=cur_key;
    while (kbhit()) getch();
    if ((ed_key==1||(ed_key==0x2D&&(sh_ctrl_alt&8)))&&mm!=253) {	  //Konec
      ed_key=0;
      if (changed){
	mm=253;
	zb_mnu(3);
	hkur();
//	v4write2(159-vlength("SAVE")/2,-17,101,99,1,"SAVE");
	sputimage0(150,-21,21,17,mnu+11681);
	zkur();
      }else
      {
	FILE *s;
	end=1;
	s=fopen("lvledit.cfg","wb");
	fwrite(&S1,1,1,s);
	fwrite(&cur_podl,1,1,s);
	fclose(s);
	if (!spojeno) sp_lev();
      }
    }
    if ((m_scr==1)||m_scr==2) ssc(sx+SX,sy+SY,S1); else vr(); //Scroll||Retrace
    if (ed_key==0x4E&&S1<6) {
      S1++;
      while (cur_key==0x4E) while (kbhit()) getch();
    }
    if (ed_key==0x4A&&S1>1) {
      S1--;
      while (cur_key==0x4A) while (kbhit()) getch();
    }

    if (ed_key==0x31&&(sh_ctrl_alt&4)) {		//NEW
      while (cur_key==0x31) while (kbhit()) getch();
      ed_key=0;
      mm=255;
      zb_mnu(3);
      hkur();
//      v4write2(159-vlength("NEW")/2,-17,101,99,1,"NEW");
      sputimage0(150,-21,20,19,mnu+13883);
      zkur();
    }
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                       Zakladni menu
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==0){
      if ((ed_key==0x52&&(sh_ctrl_alt&4))||ed_key==0x22){ //Copy
	mm=200;
      }
      if (((ed_key==0x52&&(sh_ctrl_alt&3))||ed_key==0x29)&&cSX>0&&cSY>0){ //Paste
	UC p;
	int x,y,a,b;
	mm=202;
	hkur();
	cX1=X;cY1=Y;
	for (p=0;p<=1;p++)
	  for (y=0;y<cSY;y++)
	    for (x=0;x<cSX;x++)
	      if ((x+cX1>0&&x+cX1<32&&y+cY1>0&&y+cY1<22)&&(p==0||clipboard[p][x+y*cSX].vec)) hazej(nasobkyx[x+cX1],nasobkyy[y+cY1],clipboard[p][x+y*cSX].vec);
	dr_paste_fr(cX1,cY1);
	zkur();
      }
      if (ed_key==0x14&&(sh_ctrl_alt&4)) {       //TEXT EDIT
	while (cur_key==0x14) while (kbhit()) getch();
	ed_key=0;
	hkur();
	textedit();
	hkur();
	mrst();
      }
      if ((mbut2==1&&m_shw==13)||(ed_key==0x32&&(sh_ctrl_alt&4))) {       //HLAVNI MENU
	while (cur_key==0x32) while (kbhit()) getch();
	ed_key=0;
	mm=22;
	zb_mnu(22);
      }
      if (ed_key==0x3C&&changed){				//SAVE
	while (cur_key==0x3C) while (kbhit()) getch();
	ed_key=0;
	mm=254;
	zb_mnu(3);
	hkur();
//	v4write2(159-vlength("SAVE")/2,-17,101,99,1,"SAVE");
	sputimage0(150,-21,21,17,mnu+11681);
	zkur();
      }
      if ((ed_key>=2&&ed_key<=9&&ed_key!=m_cdl+2)||(mbut2==2&&m_shw>=99&&m_shw<=110)){ //Vyber skupiny dlazdic
	m_pos=0;
	if (mbut2==2) m_cdl++; else m_cdl=ed_key-2;
	if (m_cdl>7) m_cdl=0;
	hkur();
	zb_mdl();
	zkur();
      }
      if ((ed_key==0x39||mbut2==4)&&m_shw==15)  //Prepnuti na menu prepinace
      {
	UC p;
	zb_mnu(1);
	mm=1;
	if (PREPINAC(pole[0][X][Y].vec)) p=0; else p=1;
	cur_v=pole[p][X][Y].v;
	upd_m_pr();
	upd_m_pr2();
	hkur();
	hazej(prep[cur_v].ax,prep[cur_v].ay,182);
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if ((ed_key==0x39||mbut2==4)&&(m_shw==16||m_shw==18||m_shw==19||m_shw==21)) //Prepnuti na menu ON/OFF+Potvory+El.proud
      {
	UC p;
	zb_mnu(2);
	if (JAKA_VEC(pole[0][X][Y].vec)<=3) p=0; else p=1;
	cur_v=pole[p][X][Y].v;
	if (m_shw==16) {mm=2;upd_m_nff();} else
	if (m_shw==18) {mm=4;upd_m_rl();} else
	if (m_shw==19) {mm=5;upd_m_slowfast();}
	if (m_shw==21) {
	  mm=7;
	  hkur();
	  hazej(13,-21,133);
	  hazej(287,-21,135);
	  upd_m_elpr1();
	  zkur();
	}
	hkur();
	hazej(veci[cur_v].ax,veci[cur_v].ay,182);
	if (m_shw==16)
	  for (f=0;f<topp;f++)
	  for (g=0;g<6;g++)
	  {
	    if (prep[f].a[g].x==128&&prep[f].a[g].vec==cur_v) {hazej(prep[f].ax,prep[f].ay,184);break;}
	  }
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if ((ed_key==0x39||mbut2==4)&&m_shw==17)  //Prepnuti na menu teleport
      {
	mm=3;
	cur_v=pole[1][X][Y].v;
	upd_m_tel1();
	hkur();
	hazej(veci[cur_v].ax,veci[cur_v].ay,182);
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if ((ed_key==0x39||mbut2==4)&&m_shw==20&&(pole[1][X][Y].v!=255||topv<200))  //Prepnuti na menu MOVE
      {
	mm=6;
	zb_mnu(4);
	cur_v=pole[1][X][Y].v;
	cur_a=X;
	cur_y=Y;
	if (cur_v==255){move_nff=1;move_x1=255;move_y1=255;move_x2=255;move_y2=255;}else
	{
	  if((veci[cur_v].a1&1)==0)
	  {
	    move_x1=veci[cur_v].a2;
	    move_x2=veci[cur_v].a3;
	    move_y1=move_y2=veci[cur_v].y;
	  }else{
	    move_y1=veci[cur_v].a2;
	    move_y2=veci[cur_v].a3;
	    move_x1=move_x2=veci[cur_v].x;
	  }
	  if (veci[cur_v].a1<=3) move_nff=1; else move_nff=0;
	}
	hkur();
	for (f=0;f<topp;f++)
	for (g=0;g<6;g++)
	{
	  if (prep[f].a[g].x==129&&prep[f].a[g].vec==cur_v) {hazej(prep[f].ax,prep[f].ay,184);break;}
	}
	hazej(nasobkyx[X],nasobkyy[Y],182);
	zb_move();
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if (m_shw==10&&mbut==1&&m_scr!=1){m_pos--;hkur();zb_mdl();zkur();vr();}   //Rolovani dlaznic
      if (m_shw==11&&mbut==1&&m_scr!=1){m_pos++;hkur();zb_mdl();zkur();vr();}
      if (m_shw==10&&mbut2==2&&m_scr!=1){m_pos-=11;if(m_pos<0) m_pos=0;hkur();zb_mdl();zkur();}
      if (m_shw==11&&mbut2==2&&m_scr!=1){m_pos+=11;if(m_pos>m_cd[m_cdl]-11) m_pos=m_cd[m_cdl]-11;hkur();zb_mdl();zkur();}
      if (m_shw==12&&mbut2==1&&m_scr!=1) {mm=10;hkur();hazej(40,-21,182);zkur();} //Nastaveni default podlahy
      if (m_shw>=100&&m_shw<=110&&mbut2==1) {m_sel=m_dl[m_cdl][m_shw-100+m_pos];hkur();zb_mdl();zkur();} //Vyber dlazdice
      if ((X!=X2||Y!=Y2||!hv)&&(m_shw==1||(m_shw>=15&&m_shw<=21))&&(mbut==1||(hv&&mbut==1))&&(m_scr!=1||hv)){ //Hazeni veci
	UC p=patro[m_sel]!=0;
	X2=X;Y2=Y;
	hv=1;
	mv=255;
//	if (m_sel!=pole[p][X][Y].vec||shft)
	{
	  UC sh=0;
	  changed=1;
	  if (shft&&HH(m_sel)) sh=1;
	  for (g=Y-sh;g<=Y+sh;g++)
	    for (f=X-sh;f<=X+sh;f++)
	      if (JAKA_VEC(m_sel)==255||JAKA_VEC(pole[!p][f][g].vec)==255) build(p,f,g,m_sel);
	  zkur();
	}
      }
      if ((X!=X2||Y!=Y2||mv==255)&&(m_shw==1||(m_shw>=15&&m_shw<=21))&&(mbut==2||(mv!=255&&mbut==2))&&(m_scr!=1||mv!=255)){ //Mazani veci
	UC p;
	UC sh=0;
	X2=X;
	Y2=Y;
	hv=0;
	if (mv==255) {
	  if (pole[1][X][Y].vec==0) p=0; else p=1;
	  mv=pole[p][X][Y].vec;
	  if (mv>=74&&mv<=76) mv=1;
	  if (pole[p][X][Y].vec>=74&&pole[p][X][Y].vec<=76)
	  {if (shft&&HH(1)) sh=1;}
	  else
	  {if (shft&&HH(pole[p][X][Y].vec)) sh=1;}
	}
	else {
	  p=patro[mv]!=0;
	  if (shft&&HH(mv)) sh=1;
	}
	for (g=Y-sh;g<=Y+sh;g++)
	  for (f=X-sh;f<=X+sh;f++)
	   {
	   UC pvec=pole[p][f][g].vec;
	   if (pvec>=74&&pvec<=76) pvec=1;
	    if (!sh||HH(pvec)){
	      changed=1;
	      destroy(p,f,g);
	    }
	   }
	 zkur();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                        Prepinace
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==1){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	cur_a=255;
	upd_m_pr3();
	zb_mnu(0);
	h3(prep[cur_v].x,prep[cur_v].y);
	zkur();
	mm=0;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if(m_shw>=91&&m_shw<=93&&mbut2==1)  //zapni-vypni-prepni
      {
	changed=1;
	prep[cur_v].zvp=m_shw-90;
	upd_m_pr();
      }else
      if(m_shw>=100&&m_shw<=155)  //Zobrazeni fce prepinace
      {
	if (cur_a!=(m_shw-100)/10){
	  animm=0;
          cur_a=255;
	  upd_m_pr3();
	  cur_a=(m_shw-100)/10;
	  upd_m_pr2();
	} else upd_m_pr4();
	if (mbut2==1&&m_shw%10){
	  if (m_shw%10==1)  //Zruseni cinnosti prepinace
	  {
	    changed=1;
	    upd_m_pr3();
	    memset(&prep[cur_v].a[cur_a],255,3);
	    upd_m_pr();
	    upd_m_pr2();
	  }else            //Nastaveni mm na change,on/off,move
	  {
	    mm=100+m_shw%10;
	  }
	}
      } else {if (cur_a!=255) {cur_a=255;upd_m_pr2();}cur_a=255;upd_m_pr4();}
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                       On/Off,Potvory,El.proud
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==2||mm==4||mm==5||mm==7){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	zb_mnu(0);
	h3(veci[cur_v].x,veci[cur_v].y);
	if (mm==2)
	  for (f=0;f<topp;f++)
	  for (g=0;g<6;g++)
	  {
	    if (prep[f].a[g].x==128&&prep[f].a[g].vec==cur_v) {h3(prep[f].x,prep[f].y);break;}
	  }
	zkur();
	mm=0;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if (m_shw==10&&mbut2==1){  //prepnuti
	changed=1;
	if (veci[cur_v].a3==0) veci[cur_v].a3=1; else veci[cur_v].a3=0;
	if (mm==2) upd_m_nff(); else
	if (mm==4) upd_m_rl(); else
	if (mm==5) upd_m_slowfast();
      }else
      if (m_shw==11&&mbut==1&&veci[cur_v].a3!=(mx-33)/6)  //el.proud
      {
	changed=1;
	upd_m_elpr2();
	veci[cur_v].a3=(mx-33)/6;
	if(veci[cur_v].a3>39) veci[cur_v].a3=39;
	upd_m_elpr1();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                       Teleport
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==3){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	upd_m_tel2();
	h3(veci[cur_v].x,veci[cur_v].y);
	zkur();
	mm=0;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }
      if (m_shw==1&&mbut2==1&&m_scr!=1){  //nastaveni XY
	changed=1;
	upd_m_tel2();
	veci[cur_v].a1=X;
	veci[cur_v].a2=Y;
	upd_m_tel1();
	mm=0;
	for (f=0;f<20;f++) vr();
	upd_m_tel2();
	h3(veci[cur_v].x,veci[cur_v].y);
	zkur();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                        MOVE
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==6){
      anim_move(pole[1][cur_a][cur_y].vec);
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //konec
      {
	zm_move();
	h3(cur_a,cur_y);
	for (f=0;f<topp;f++)
	for (g=0;g<6;g++)
	{
	  if (prep[f].a[g].x==129&&prep[f].a[g].vec==cur_v) {h3(prep[f].x,prep[f].y);break;}
	}
	zb_mnu(0);
	zkur();
	if (move_ok(move_x1,move_y1,move_x2,move_y2,cur_a,cur_y))
	{
	  if(cur_v==255)
	  {
	    cur_v=topv;
	    veci[topv].x=cur_a;
	    veci[topv].y=cur_y;
	    veci[topv].vec=pole[1][cur_a][cur_y].vec;
	    veci[topv].dx=0;veci[topv].dy=0;
	    veci[topv].ax=nasobkyx[cur_a];
	    veci[topv].ay=nasobkyy[cur_y];
	    pole[1][cur_a][cur_y].v=topv;
	    topv++;
	  }
	  if(move_x1==cur_a&&move_x2==cur_a){
	    if(move_y1>move_y2) veci[cur_v].a1=1; else veci[cur_v].a1=3;
	    veci[cur_v].a2=move_y1;
	    veci[cur_v].a3=move_y2;
	  }else{
	    if(move_x1>move_x2) veci[cur_v].a1=2; else veci[cur_v].a1=0;
	    veci[cur_v].a2=move_x1;
	    veci[cur_v].a3=move_x2;
	  }
	  if (!move_nff) veci[cur_v].a1+=4;
	}
	cur_a=255;
	cur_y=255;
	mm=0;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (mbut2==1&&m_shw==203) //zruseni
      {
	zm_move();
	h3(cur_a,cur_y);
	zb_mnu(0);
	zkur();
	if (cur_v!=255) znic_vec(cur_v,pole[1][cur_a][cur_y].vec);
	cur_a=255;
	cur_y=255;
	mm=0;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (mbut2==1&&m_shw==202) move_nff=!move_nff; else  //move on-off
      if (mbut2==1&&m_shw==200) mm=160; else //od
      if (mbut2==1&&m_shw==201) mm=161;      //do
    }else
    if (mm==160||mm==161){ //od+do
      if (mbut2==2) //zruseni
      {
	mm=6;
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (mbut2==1&&m_shw==1&&(X==cur_a||Y==cur_y))
      {
	changed=1;
	zm_move();
	if (mm==160){
	  move_x1=X;move_y1=Y;
	  if (!move_ok(move_x1,move_y1,move_x2,move_y2,cur_a,cur_y)){move_x2=255;move_y2=255;}
	}else
	{
	  move_x2=X;move_y2=Y;
	  if (!move_ok(move_x1,move_y1,move_x2,move_y2,cur_a,cur_y)){move_x1=255;move_y1=255;}
	}
	zb_move();
	mm=6;
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                     Default podlaha
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==10){
      if (mbut2==2&&m_shw==0) {mm=0;hkur();zobraz_defpodl();zkur();}
      if (m_shw==10&&mbut==1&&m_scr!=1){m_pos--;hkur();zb_mdl();hazej(40,-21,182);zkur();vr();}
      if (m_shw==11&&mbut==1&&m_scr!=1){m_pos++;hkur();zb_mdl();hazej(40,-21,182);zkur();vr();}
      if (m_shw==10&&mbut2==2&&m_scr!=1){m_pos-=11;if(m_pos<0) m_pos=0;hkur();zb_mdl();hazej(40,-21,182);zkur();}
      if (m_shw==11&&mbut2==2&&m_scr!=1){m_pos+=11;if(m_pos>m_cd[m_cdl]-11) m_pos=m_cd[m_cdl]-11;hkur();zb_mdl();hazej(40,-21,182);zkur();}
      if ((ed_key>=2&&ed_key<=9&&ed_key!=m_cdl+2)||(mbut2==2&&m_shw>=99&&m_shw<=110)){
	m_pos=0;
	if (mbut2==2) m_cdl++; else m_cdl=ed_key-2;
	if (m_cdl>7) m_cdl=0;
	hkur();
	zb_mdl();
	hazej(40,-21,182);
	zkur();
      }
      if (m_shw>=100&&m_shw<=110&&mbut2==1) {
	m_sel=m_dl[m_cdl][m_shw-100+m_pos];
	cur_podl=m_sel;
	hkur();zb_mdl();zkur();
	mm=0;
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                     Hlavni menu
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==22){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	mm=0;
	zb_mnu(0);
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (m_shw==11&&mbut2==1)  //SAVE
      {
	mm=254;
	zb_mnu(3);
	hkur();
//	v4write2(159-vlength("SAVE")/2,-17,101,99,1,"SAVE");
	sputimage0(150,-21,21,17,mnu+11681);
	zkur();
      }else
      if (m_shw==12&&mbut2==1)  //HUDBA
      {
	if (zkontroluj_soubor("krkal.mus"))
	{
	  mm=50;
	  zb_mnu(50);
	  hkur();
	  hud_ram();
	  vypis_hudby();
	  zkur();
	}
      }else
      if (m_shw==13&&mbut2==1)  //GRAF
      {
	UC i;
	mm=51;
	zb_mnu(51);
	hkur();
	hud_ram();
	for (i=0;i<pocet_env;i++) vypis_graf(i);
	graf_prev();
	zkur();
      }else
      if (m_shw==14&&mbut2==1)  //GLOB PREP
      {}else
      if (m_shw==15&&mbut2==1)  //EDITACE TEXTU
      {
	hkur();
	textedit();
	mm=0;
	zb_mnu(0);
	hkur();
	mrst();
      }else
      if (m_shw==16&&mbut2==1)  //NEW
      {
	mm=255;
	zb_mnu(3);
	hkur();
//	v4write2(159-vlength("NEW")/2,-17,101,99,1,"NEW");
	sputimage0(150,-21,20,19,mnu+13883);
	zkur();
      }else
      if (m_shw==17&&mbut2==1)  //EXIT
      {
	ed_key=0;
	if (changed){
	  mm=253;
	  zb_mnu(3);
	  hkur();
//	  v4write2(159-vlength("SAVE")/2,-17,101,99,1,"SAVE");
	  sputimage0(150,-21,21,17,mnu+11681);
	  zkur();
	}else
	{
	  FILE *s;
	  end=1;
	  s=fopen("lvledit.cfg","wb");
	  fwrite(&S1,1,1,s);
	  fwrite(&cur_podl,1,1,s);
	  fclose(s);
	  if (!spojeno) sp_lev();
	}
      }else
      if (m_shw==18&&mbut2==1)  //HELP
      {}
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                        Hudba
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==50){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	mm=0;
	hkur();
	zbp();
	zb_mnu(0);
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (mbut2==1&&mx>8&&my>8&&mx<312&&my<166)
      {
	UC t=hudba;
	if (my<12) hudba=0; else hudba=(my-12)/12;
	if (mx>160) hudba+=13;
	if (hudba>=hudba_poc) hudba=hudba_poc-1;
	if (t!=hudba)
	{
	  hkur();
	  vypis_hudbu(t);
	  vypis_hudbu(hudba);
	  zkur();
	  if (PLAYER) hudba_load(hudba,0);
	  changed=1;
	}
      }else
      if (PLAYER){
	UC tmc[4];
	midasMP->GetInformation(&info);
	sfill(38,-21,20,17,0);
	itoa(info->row,tmc,16);
	v4write2(45,-20,136,133,1,tmc);
	itoa(info->pos,tmc,16);
	v4write2(45,-11,136,133,1,tmc);
	if (mbut2==1&&m_shw==11){
	  midasMP->SetPosition(0);
	}else
	if (mbut2==1&&m_shw==12){
	  if (info->pos>0) midasMP->SetPosition(info->pos-1); else midasMP->SetPosition(mod->songLength-1);
	}else
	if (mbut2==1&&m_shw==13){
	  if (info->pos<mod->songLength-1) midasMP->SetPosition(info->pos+1); else midasMP->SetPosition(0);
	}else
	if (mbut2==1&&m_shw==14){
	  midasMP->SetPosition(mod->songLength-1);
	}
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                        Grafika
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==51){
      if ((ed_key==0x39||mbut2==4)||mbut2==2) //zruseni
      {
	mm=0;
	hkur();
	zbp();
	zb_mnu(0);
	zkur();
	while (cur_key==0x39) while (kbhit()) getch();
	ed_key=0;
      }else
      if (mbut2==1&&mx>8&&my>8&&mx<312&&my<166)
      {
	UC t=usek;
	if (my<12) usek=0; else usek=(my-12)/12;
	if (mx>160) usek+=13;
	if (usek>=pocet_env) usek=pocet_env-1;
	if (t!=usek)
	{
	  hkur();
	  vypis_graf(t);
	  vypis_graf(usek);
	  load_graf(usek,t);
	  graf_prev();
	  zkur();

	  changed=1;
	}
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³       	Prepinace - CHANGE,ON/OFF,MOVE
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mbut2==2&&(mm==102||mm==103||mm==104||mm==105)) mm=1; else //zruseni
    if (mm==102&&mbut2==1&&!m_scr&&m_shw==1) //Change
    {
      mm=110;
      zb_mnu(0);
    }else
    if (mm==103&&mbut2==1&&!m_scr&&m_shw==1) //Change(Vypnuti)
    {
      if (pole[1][X][Y].vec!=0){
	changed=1;
	upd_m_pr3();
	prep[cur_v].a[cur_a].x=X;
	prep[cur_v].a[cur_a].y=Y;
	prep[cur_v].a[cur_a].vec=250+patro[pole[1][X][Y].vec];
	mm=1;
	zb_mnu(1);
	upd_m_pr();
	cur_a=255;
	upd_m_pr2();
      } else{
	mm=111;
	zb_mnu(0);
      }
    }else
    if (mm==104||mm==105)	//on/off & move
    {
      if(m_shw==10&&mbut2==1)
      {
	changed=1;
	upd_m_pr3();
	if (mm==104) prep[cur_v].a[cur_a].x=128; else prep[cur_v].a[cur_a].x=129;
	prep[cur_v].a[cur_a].y=255;
	if (mm==104){
	  if (JAKA_VEC(pole[0][X][Y].vec)<=3) prep[cur_v].a[cur_a].vec=pole[0][X][Y].v; else prep[cur_v].a[cur_a].vec=pole[1][X][Y].v;
	}else prep[cur_v].a[cur_a].vec=pole[1][X][Y].v;
	upd_m_pr();
	cur_a=255;
	upd_m_pr2();
	mm=1;
      }
    }else
    if (mm==110||mm==111) //change
    {
      if (mbut2==2&&m_shw==0) {mm=1;zb_mnu(1);upd_m_pr();}
      if (m_shw==10&&mbut==1&&m_scr!=1){m_pos--;hkur();zb_mdl();zkur();vr();}
      if (m_shw==11&&mbut==1&&m_scr!=1){m_pos++;hkur();zb_mdl();zkur();vr();}
      if (m_shw==10&&mbut2==2&&m_scr!=1){m_pos-=11;if(m_pos<0) m_pos=0;hkur();zb_mdl();zkur();}
      if (m_shw==11&&mbut2==2&&m_scr!=1){m_pos+=11;if(m_pos>m_cd[m_cdl]-11) m_pos=m_cd[m_cdl]-11;hkur();zb_mdl();zkur();}
      if ((ed_key>=2&&ed_key<=9&&ed_key!=m_cdl+2)||(mbut2==2&&m_shw>=99&&m_shw<=110)){
	m_pos=0;
	if (mbut2==2) m_cdl++; else m_cdl=ed_key-2;
	if (m_cdl>7) m_cdl=0;
	hkur();
	zb_mdl();
	zkur();
      }
      if (m_shw>=100&&m_shw<=110&&mbut2==1) {
	changed=1;
	upd_m_pr3();
	prep[cur_v].a[cur_a].x=X;
	prep[cur_v].a[cur_a].y=Y;
	m_sel=m_dl[m_cdl][m_shw-100+m_pos];
	if (mm==110) prep[cur_v].a[cur_a].vec=m_sel;else
	prep[cur_v].a[cur_a].vec=250+patro[m_sel];
	hkur();zb_mdl();zkur();
	mm=1;
	zb_mnu(1);
	upd_m_pr();
	cur_a=255;
	upd_m_pr2();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                           COPY
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==200)
    {
      if (mbut2==2) mm=0;
      if (m_shw==1&&mbut2==1){
	cX1=X;
	cY1=Y;
	cX2=255;
	cY2=255;
	mm=201;
      }
    }else
    if (mm==201)
    {
      if (mbut2==2) {
	UC x;
	if (cX1>cX2){x=cX2;cX2=cX1;cX1=x;}
	if (cY1>cY2){x=cY2;cY2=cY1;cY1=x;}
	for (x=cY1;x<=cY2;x++){h3(cX1,x);h3(cX2,x);}
	for (x=cX1;x<=cX2;x++){h3(x,cY1);h3(x,cY2);}
	zkur();mm=0;
      } else
      if (m_shw==1&&mbut2==1){
	UC x,y,p;
	vv=0,vp=0;
//	cX2=X;
//	cY2=Y;
	if (cX1>cX2){x=cX2;cX2=cX1;cX1=x;}
	if (cY1>cY2){x=cY2;cY2=cY1;cY1=x;}
	cSX=cX2-cX1+1,cSY=cY2-cY1+1;

	for (p=0;p<=1;p++)
	  for (y=0;y<cSY;y++)
	    for (x=0;x<cSX;x++)
	    {
	      clipboard[p][x+y*cSX].vec=pole[p][x+cX1][y+cY1].vec;
	      if (pole[p][x+cX1][y+cY1].v!=255){
		if (PREPINAC(pole[p][x+cX1][y+cY1].vec))
		{
		  UC a;
		  clipboard[p][x+y*cSX].v=vp;
		  c_prep[vp].zvp=prep[pole[p][x+cX1][y+cY1].v].zvp;
		  for (a=0;a<6;a++)
		  {
		   c_prep[vp].a[a].vec=prep[pole[p][x+cX1][y+cY1].v].a[a].vec;
		   c_prep[vp].a[a].x=prep[pole[p][x+cX1][y+cY1].v].a[a].x;
		   c_prep[vp].a[a].y=prep[pole[p][x+cX1][y+cY1].v].a[a].y;
		   c_prep[vp].a[a].vc=255;
		   if (c_prep[vp].a[a].x==255);else
		   if (c_prep[vp].a[a].x==128||c_prep[vp].a[a].x==129){
		     if (veci[c_prep[vp].a[a].vec].x>=cX1&&veci[c_prep[vp].a[a].vec].x<=cX2&&veci[c_prep[vp].a[a].vec].y>=cY1&&veci[c_prep[vp].a[a].vec].y<=cY2)
		     {
		       c_prep[vp].a[a].vc=1;
		     }
		   }else{
		     if (c_prep[vp].a[a].x>=cX1&&c_prep[vp].a[a].x<=cX2&&c_prep[vp].a[a].y>=cY1&&c_prep[vp].a[a].y<=cY2)
		     {
		       c_prep[vp].a[a].vc=1;
		     }
		   }
		  }
		  vp++;
		}else
		{
		  clipboard[p][x+y*cSX].v=vv;
		  c_veci[vv].a1=veci[pole[p][x+cX1][y+cY1].v].a1;
		  if (ISMOVE(pole[p][x+cX1][y+cY1].vec))
		  {
		    if((c_veci[vv].a1&1)==0)
		    {
		      c_veci[vv].a2=veci[pole[p][x+cX1][y+cY1].v].a2-cX1;
		      c_veci[vv].a3=veci[pole[p][x+cX1][y+cY1].v].a3-cX1;
		    }else
		    {
		      c_veci[vv].a2=veci[pole[p][x+cX1][y+cY1].v].a2-cY1;
		      c_veci[vv].a3=veci[pole[p][x+cX1][y+cY1].v].a3-cY1;
		    }
		  }else{
		    c_veci[vv].a2=veci[pole[p][x+cX1][y+cY1].v].a2;
		    c_veci[vv].a3=veci[pole[p][x+cX1][y+cY1].v].a3;
		  }
		  vv++;
		}
	      }else clipboard[p][x+y*cSX].v=255;
	    }
	for(x=0;x<vp;x++)
	  for (y=0;y<6;y++)
	    if (c_prep[x].a[y].vc==1){
	      if (c_prep[x].a[y].x==128||c_prep[x].a[y].x==129){
		p=patro[veci[c_prep[x].a[y].vec].vec]!=0;
		c_prep[x].a[y].vec=clipboard[p][veci[c_prep[x].a[y].vec].x-cX1+(veci[c_prep[x].a[y].vec].y-cY1)*cSX].v;
	      }else{
		c_prep[x].a[y].x-=cX1;
		c_prep[x].a[y].y-=cY1;
	      }
	    }


	for (x=cY1;x<=cY2;x++){h3(cX1,x);h3(cX2,x);}
	for (x=cX1;x<=cX2;x++){h3(x,cY1);h3(x,cY2);}
	cX2=255;cY2=255;
	zkur();
	mm=0;
      }else
      if((X!=cX2||Y!=cY2)&&(abs(cX1-X)+1)*(abs(cY1-Y)+1)<=100){
	UI x1,y1,x2,y2,xx;
	hkur();
	if (cX2!=255){
	  x1=cX1;
	  y1=cY1;
	  x2=cX2;
	  y2=cY2;
	  if (x1>x2){xx=x2;x2=x1;x1=xx;}
	  if (y1>y2){xx=y2;y2=y1;y1=xx;}
	  for (xx=y1;xx<=y2;xx++){
	    h3(cX1,xx);
	    h3(cX2,xx);
	  }
	  for (xx=x1;xx<=x2;xx++){
	    h3(xx,cY1);
	    h3(xx,cY2);
	  }
	}
	cX2=X;cY2=Y;
	x1=nasobkyx[cX1];
	y1=nasobkyy[cY1];
	x2=nasobkyx[X];
	y2=nasobkyy[Y];
	if (x1>x2){xx=x2;x2=x1;x1=xx;}
	if (y1>y2){xx=y2;y2=y1;y1=xx;}
	x2+=19;y2+=16;

	for (xx=x1;xx<=x2;xx++)
	  {putp(xx,y1,(xx%10)+COL3);putp(xx,y2,(xx%10)+COL3);}
	for (xx=y1;xx<=y2;xx++)
	  {putp(x1,xx,(xx%10)+COL3);putp(x2,xx,(xx%10)+COL3);}
	zkur();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                           PASTE
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if ((mm==202||mm==203)&&mbut2==2)
    {
      UC x,y;
      for (y=0;y<cSY;y++)
	for (x=0;x<cSX;x++)
	  if (x+cX1>=0&&x+cX1<32&&y+cY1>=0&&y+cY1<22) h3(x+cX1,y+cY1);
      zkur();
      mm=0;
    }else
    if (mm==202&&mbut2==1&&m_shw){
      mm=203;
      cX2=X-cX1;cY2=Y-cY1;
    }else
    if (mm==203)
    {
      if (m_shw&&(cX1!=X-cX2||cY1!=Y-cY2||cX2==255)){
	UC p;
	int x,y;
	//if (cX2==255){cX2=X-cX1;cY2=Y-cY1;}
	hkur();
	for (y=0;y<cSY;y++)
	  for (x=0;x<cSX;x++)
	    if (x+cX1>=0&&x+cX1<32&&y+cY1>=0&&y+cY1<22)
	      if(cX1+x<X-cX2||cY1+y<Y-cY2||cX1+x>X-cX2+cSX-1||cY1+y>Y-cY2+cSY-1)
	      h3(x+cX1,y+cY1);
	cX1=X-cX2;
	cY1=Y-cY2;
	for (p=0;p<=1;p++)
	  for (y=0;y<cSY;y++)
	    for (x=0;x<cSX;x++)
	      if ((x+cX1>=0&&x+cX1<32&&y+cY1>=0&&y+cY1<22)&&(p==0||clipboard[p][x+y*cSX].vec))
	      {
		if(clipboard[p][x+y*cSX].vec==1||(clipboard[p][x+y*cSX].vec>=74&&clipboard[p][x+y*cSX].vec<=76)) hazej(nasobkyx[x+cX1],nasobkyy[y+cY1],VODA(x+cX1,y+cY1));
		else hazej(nasobkyx[x+cX1],nasobkyy[y+cY1],clipboard[p][x+y*cSX].vec);
	      }
	dr_paste_fr(cX1,cY1);
	zkur();
      }else
      if (mbut2==1&&m_shw)
      {
	UC x,y,p,t,old_topp,old_topv;
	changed=1;
	cX2=X;cY2=Y;
	X=cX1;Y=cY1;
	t=autom;
	autom=0;
	for (p=0;p<=1;p++)
	  for (y=0;y<cSY;y++)
	    for (x=0;x<cSX;x++)
	    if (X+x>=0&&Y+y>=0&&X+x<32&&Y+y<22){
	    destroy(p,X+x,Y+y);
	    }
	old_topp=topp,old_topv=topv;
	for (p=0;p<=1;p++)
	  for (y=0;y<cSY;y++)
	    for (x=0;x<cSX;x++)
	    if (X+x>=0&&Y+y>=0&&X+x<32&&Y+y<22){
	      if (clipboard[p][x+y*cSX].v!=255){
		if (PREPINAC(clipboard[p][x+y*cSX].vec)){
		  if (topp<100){
		    UC a;
		    pole[p][X+x][Y+y].vec=clipboard[p][x+y*cSX].vec;
		    pole[p][X+x][Y+y].v=topp;
		    prep[topp].x=X+x;prep[topp].dx=0;prep[topp].ax=nasobkyx[prep[topp].x];
		    prep[topp].y=Y+y;prep[topp].dy=0;prep[topp].ay=nasobkyy[prep[topp].y];
		    prep[topp].vec=clipboard[p][x+y*cSX].vec;
		    prep[topp].zvp=c_prep[clipboard[p][x+y*cSX].v].zvp;
		    for (a=0;a<6;a++)
		    {
		      prep[topp].a[a].vec=c_prep[clipboard[p][x+y*cSX].v].a[a].vec;
		      prep[topp].a[a].x=c_prep[clipboard[p][x+y*cSX].v].a[a].x;
		      prep[topp].a[a].y=c_prep[clipboard[p][x+y*cSX].v].a[a].y;
		      if((prep[topp].a[a].x==128||prep[topp].a[a].x==129)&&prep[topp].a[a].vec>=topv&&c_prep[clipboard[p][x+y*cSX].v].a[a].vc!=1) memset(&prep[topp].a[a],255,3);
		    }
		    topp++;
		  }
		}else if (topv<200){
		  pole[p][X+x][Y+y].vec=clipboard[p][x+y*cSX].vec;
		  pole[p][X+x][Y+y].v=topv;
		  veci[topv].x=X+x;veci[topv].dx=0;veci[topv].ax=nasobkyx[veci[topv].x];
		  veci[topv].y=Y+y;veci[topv].dy=0;veci[topv].ay=nasobkyy[veci[topv].y];
		  veci[topv].vec=clipboard[p][x+y*cSX].vec;
		  veci[topv].a1=c_veci[clipboard[p][x+y*cSX].v].a1;
		  if(ISMOVE(veci[topv].vec))
		  {
		    if ((veci[topv].a1&1)==0)
		    {
		      veci[topv].a2=c_veci[clipboard[p][x+y*cSX].v].a2+X;
		      veci[topv].a3=c_veci[clipboard[p][x+y*cSX].v].a3+X;
		    }else{
		      veci[topv].a2=c_veci[clipboard[p][x+y*cSX].v].a2+Y;
		      veci[topv].a3=c_veci[clipboard[p][x+y*cSX].v].a3+Y;
		    }
		  }else
		  {
		    veci[topv].a2=c_veci[clipboard[p][x+y*cSX].v].a2;
		    veci[topv].a3=c_veci[clipboard[p][x+y*cSX].v].a3;
		  }
		  topv++;
		}
	      }else {
		if (clipboard[p][x+y*cSX].vec==1||(clipboard[p][x+y*cSX].vec>=74&&clipboard[p][x+y*cSX].vec<=76)) pole[p][X+x][Y+y].vec=VODA(X+x,Y+y);
		else pole[p][X+x][Y+y].vec=clipboard[p][x+y*cSX].vec;
	      }
	      h3(X+x,Y+y);
	    }
	autom=t;
	for (x=old_topp;x<topp;x++)
	  for (y=0;y<6;y++)
	    if (c_prep[topp-old_topp-1].a[y].vc==1)
	    {
	      if(prep[x].a[y].x==128||prep[x].a[y].x==129){
		prep[x].a[y].vec+=old_topv;
		if(prep[x].a[y].vec>=topv)
		memset(&prep[x].a[y],255,3);
	      }else{
		prep[x].a[y].x+=X;
		prep[x].a[y].y+=Y;
	      }
	    }
	X=cX2;Y=cY2;
	cX2=255;cY2=255;
	zkur();
	mm=0;
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                      KONEC + SAVE
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==253)
    {
      if (mbut2==1&&m_shw==200) {
	FILE *s;
	save(lvl);
	end=1;
	s=fopen("lvledit.cfg","wb");
	fwrite(&S1,1,1,s);
	fwrite(&cur_podl,1,1,s);
	fclose(s);
	sp_lev();
      }else
      if (mbut2==1&&m_shw==201) {
	FILE *s;
	end=1;
	s=fopen("lvledit.cfg","wb");
	fwrite(&S1,1,1,s);
	fwrite(&cur_podl,1,1,s);
	fclose(s);
	if (!spojeno) sp_lev();
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                          SAVE
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==254)
    {
      if (mbut2==1&&m_shw==200) {
	save(lvl);
	mm=0;
	zb_mnu(0);
      }else
      if (mbut2==1&&m_shw==201) {
	mm=0;
	zb_mnu(0);
      }
    }else
    //   ÚÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    //   ³                          NEW
    //   À-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄÄ-Ä-Ä--ùùú úùù-Ä-Ä-ÄÄÄÄÄ-Ä-Ä--ùùú
    if (mm==255)
    {
      if (mbut2==1&&m_shw==200) {
	ed_new();
      }else
      if (mbut2==1&&m_shw==201) {
	mm=0;
	zb_mnu(0);
      }
    }
  }
  hudusek=hudba;
  setvect(0x9,old_ed_key_handler);
}
