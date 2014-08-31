#define UC unsigned char
#define UI unsigned int
#include "krkal.h"
#include "midas.h"
#include "menu.h"

extern UC PLAYER;
extern UC ANIM;

extern UC VODA(UI,UI);
extern emsBlock *anim[4];
extern UC *animbuff;

UC pohyb_y[10]={1,2,2,1,2,2,2,1,2,2};
UC pohyb_y2[5]={3,4,3,4,3};

UC nemec[13]="Ofnfd!kf!Cmc";
UC cheet[13]="NBGSBM!LSLBM";
UC chname[13]="CHEAT MODE";
UI nasobkyx[32];
UI nasobkyy[22];
UI nasobky340[181];
UC vec_1[182],vec_2[182],patro[182];
UC jmena_ikon[18][9] = {"Smazat","Hréèe","Zmínit","Jmëno","Vyber","Level","Config","","Do","DOSu",
                        "Novõ","Hréè","Smazat","Level","Edituj","Level","Do","Akce !!"};
UC ikojm[6][10] = {"Akce !!","Menu !","DOSu","Pokraèuj","SAVE","POINT"};
             //     *    *    *    *    *    *    *    *    *
UC hlasky[9][40] = {"Tentokrét se ti to nepovedlo . . .",
                    "Hröza ! Takhle zemñìt !",
                    "Zase chyba. Ale to tí nesmì zvyklat !",
                    "Super ! Povedlo se ti dokonèit level.",
                    "Tohle se ti povedlo. Ale co ten dalóì !",
                    "Gratuluji, äspíóní jsi dohrél level !",
                    "Vìtém tí v novëm levlu !",
                    "Dalóì level. Ten bude tíúkõ !",
                    "Hédej, co tí zachvìli èeké !"};
struct {
  UC x,y,x2,y2;
} svetla_xy[12] = {{0,0,3,3},{4,0,9,2},{10,0,15,2},{16,0,19,3},{16,4,19,7},{16,8,19,12},{16,13,19,15},
		   {10,13,15,15},{4,13,9,15},{0,12,3,15},{0,8,3,11},{0,4,3,7}};

struct {
  UC vec,v;
}pole[4][32][22];

UC zobraz1[32][22],zobraz2[32][22];

struct {
  UC v,k; //1 - Doprava, 2 - nahoru, 3 doleva, 4 dolu
} laser[32][22][4];

UC topv,postvec,pvv;
struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC a1,a2,a3;
  UC obr,frm;
} veci[250];

UC toppad,pvp;
struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC smer,ii;
  UC obr,frm;
} pady[200];
UC smer_uskoku=0;
// Smer u padu : 0-Doprava,1-Nahoru,2-Doleva,3-Dolu
//               4-Doprava Nahoru,5-Doprava dolu,6-Nahoru Doleva,7-Nahoru Doprava
//               8-Doleva Dolu,9-Doleva Nahoru,10-Dolu Doprava,11-Dolu Doleva


UC topp;
struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC zvp;
  struct{
    UC x,y,vec;
  }a[6];
}prep[100];

UC topgp;
struct {
  struct {
    UC typ;
    UC vec1,vec2;
    UC x1,y1,x2,y2;
  } pdm[6];
  UC zvp;
  struct{
    UC x,y,vec;
  }a[6];
} gprep[10];

UC far obr[185][17][20];

struct {
  UC x,y,vec;
  UI ax,ay;
  signed char dx,dy;
  UC brneni;
  UC otrava;
  UC miz;
  UC odpocitavac;
  UC i[10];
} ty;

UC far buffer[64000];
UC pal2[768];
UC brv2font[450];
UC mys[7][49];
UC ram[6][100];
UC klavesy[17];
UC prettl[56];
UC pretprh[2][8] = {{0,49,49,33,33,20,0,11},{20,0,20,20,20,11,11,1}};
extern UC far mnu[40000];
UC hlavicka[10];
UC ikoc[16][20],ikof[16][20];

UC pocet_hracu;
struct {
  UC jmeno[13]; //XOR s "Nemec je BLB"
  UC data[12];
  unsigned long int crc;
  UC pocet;
  UC level;
  UC hlasitost;
  UC klavesy[17];
} hraci[16];
struct {
  UC a,b;
  unsigned long int c;
} usdefhr[16][50], usdeflev[32];

struct {
  unsigned long int odkaz;
  UC jmeno[16];
} levly[128];
UC udelano[128]; // 0 - neudelana, 1 - udelano, 2 - Mozno hrat, 254 - neexistuje, 255 - vadny soubor
UC levlyjm[128][16];

UC pocet_env;
struct {
  UC jm[26];
  unsigned long int odkaz;
  UC ani;
} environment[30];

FILE *s;
UC level;
UC usek; //Usek levlu se stejnou grafikou
UC hudusek;
extern UI tex1_len,tex2_len;
UC hrac; //Ktery SAVEGAME se pouzije
UC vyskoc; //Jestli se nevyskoci do menu 0-udelal jsi level; 1-prosrals to; 2 - hrajes novy level z Menicka
UC sejvnuto;
UC nesejvuj_to;
UC hral_jsi_uz=0;
UC zalohuj_sejv=0;
UC graphmode=1;
UI menu_x;
UC typdonemenu;
UI mysx,mysy,mystl;
UC zmacknuto,manim,mexists,dblclk/*Kdyz je 200*/;
UC key,klf[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UC klf_citac;
UC ii,sm,pohyb;
UC muflon[4];
UC zvol_vec;
UC vybuch[32][22];
UC konec;
unsigned ef_teleport,ef_vybuch,ef_pasy,ef_get,ef_zuch;
unsigned short scrSync;
mpModule    *mod;
mpInformation *info;
UC laser_pom = 0;
UC konec_laseru;
UC jsem_v_laseru;
UC jsem_v_proudu;
UI paleta_citac;
UC soupani_citac;
UC inituju_level;

extern UC ISMOVE(UC);
void interrupt ( *oldhandler)();
UC rt=0;

#define FXCHANNELS 4
unsigned fxChannel = 0;
void vybouchni(UC v, UC x, UC y, UC p);
void init_magnet(UC x1,UC y1,UC v);
void zm_p(UC p,UC x,UC y,UC vec);
void uvolni_pad3(UC x,UC y);
void naslapna_podlaha(UC x,UC y);

#define HAZEJ_ON(x,y) zobraz1[x][y]+=2
#define HAZEJ_OFF(x,y) zobraz1[x][y]-=2
#define HAZEJ_1(x,y) zobraz1[x][y]|=1

void rozsvit_mnu(UC vec,UC a) {
  int f,g;
  a--;
  for (f=svetla_xy[a].x;f<=svetla_xy[a].x2;f++) for (g=svetla_xy[a].y;g<=svetla_xy[a].y2;g++) putp(49+vec*27+f,g-21,ikoc[g][f]);
}
void zhasni_mnu(UC vec,UC a) {
  int f,g;
  a--;
  for (f=svetla_xy[a].x;f<=svetla_xy[a].x2;f++) for (g=svetla_xy[a].y;g<=svetla_xy[a].y2;g++) putp(49+vec*27+f,g-21,ikof[g][f]);
}
void nakresli_pruh1() {
  int f,g;
  for (f=8;f<40;f++) for (g=-20;g<-16;g++) if (f-8<ty.otrava) putp(f,g,212); else putp(f,g,0);
}
void nakresli_pruh2() {
  int f,g;
  for (f=8;f<40;f++) for (g=-10;g<-6;g++) if (f-8<ty.brneni) putp(f,g,214); else putp(f,g,0);
}

void oznac_mnu() {
  int f,g;
  for (f=53+zvol_vec*27;f<53+zvol_vec*27+12;f++) for (g=-18;g<-8;g++) {
    if (getp(f,g)==0) putp(f,g,54);
  }
}
void odoznac_mnu() {
  int f,g;
  for (f=53+zvol_vec*27;f<53+zvol_vec*27+12;f++) for (g=-18;g<-8;g++) {
    if (getp(f,g)==54) putp(f,g,0);
  }
}


//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************
//***********************************************************************

UC if_laser2(UC v) {
  if (v==255) return 0;
  return LASER(veci[v].vec);
}

UC if_proud2(UC v) {
  if (v==255) return 0;
  return ELEKTRODA(veci[v].vec);
}

UC if_laspr2(UC v) {
  if (v==255) return 0;
  return (ELEKTRODA(veci[v].vec)||LASER(veci[v].vec));
}

void hazej3(UC x,UC y) {
  UI xx,yy,v,ax,ay;
  void *pp;
  if (!inituju_level) {
    if (zobraz1[x][y]>1) HAZEJ_1(x,y);
    ax = nasobkyx[x];
    ay = nasobkyy[y];
    v = pole[0][x][y].v;
    if (v!=255&&ANIM&&PREPINAC(pole[0][x][y].vec)==0&&veci[v].vec<181) {
      if (veci[v].frm) {
        emsMap(anim[veci[v].frm-1],(void**)&pp);
        (UC*)pp+=(nasobky340[veci[v].obr]);
      } else pp=(UC*)obr+(nasobky340[veci[v].obr]);
    } else pp=(UC*)obr+(nasobky340[pole[0][x][y].vec]);
    gr_hazej(pp,ax,ay,ax,ay);

    if (pole[1][x][y].vec) {
      xx = ax;
      yy = ay;
      v = pole[1][x][y].v;
      if (v!=255) {
        if (PADA(pole[1][x][y].vec)) {
	  xx = pady[v].ax;
	  yy = pady[v].ay;
          if (ANIM&&pady[v].frm) {
            emsMap(anim[pady[v].frm-1],(void**)&pp);
            (UC*)pp+=(nasobky340[pady[v].obr]);
          } else pp=(UC*)obr+(nasobky340[pady[v].obr]);
        } else if(PREPINAC(pole[1][x][y].vec)==0) {
	  xx = veci[v].ax;
	  yy = veci[v].ay;
          if (ANIM&&veci[v].frm) {
            emsMap(anim[veci[v].frm-1],(void**)&pp);
            (UC*)pp+=(nasobky340[veci[v].obr]);
          } else pp=(UC*)obr+(nasobky340[veci[v].obr]);
        } else pp=(UC*)obr+(nasobky340[pole[1][x][y].vec]);
      } else pp=(UC*)obr+(nasobky340[pole[1][x][y].vec]);
      gr_hazej(pp,ax,ay,xx,yy);
    }

    if (pole[2][x][y].vec) {
      xx = ax;
      yy = ay;
      v = pole[2][x][y].v;
      if (v!=255) {
        if (PADA(pole[2][x][y].vec)) {
	  xx = pady[v].ax;
	  yy = pady[v].ay;
          if (ANIM&&pady[v].frm) {
            emsMap(anim[pady[v].frm-1],(void**)&pp);
            (UC*)pp+=(nasobky340[pady[v].obr]);
          } else pp=(UC*)obr+(nasobky340[pady[v].obr]);
        } else if(PREPINAC(pole[2][x][y].vec)==0) {
	  xx = veci[v].ax;
	  yy = veci[v].ay;
          if (ANIM&&veci[v].frm) {
            emsMap(anim[veci[v].frm-1],(void**)&pp);
            (UC*)pp+=(nasobky340[veci[v].obr]);
          } else pp=(UC*)obr+(nasobky340[veci[v].obr]);
        } else pp=(UC*)obr+(nasobky340[pole[2][x][y].vec]);
      } else
      if (pole[2][x][y].vec>=153&&pole[2][x][y].vec<=164) {
        xx = ty.ax;
        yy = ty.ay;
        pp=(UC*)obr+(nasobky340[pole[2][x][y].vec]);
      } else pp=(UC*)obr+(nasobky340[pole[2][x][y].vec]);
      gr_hazej(pp,ax,ay,xx,yy);
    }

    if (vybuch[x][y]) gr_hazej((UC*)obr+(nasobky340[82]),ax,ay,ax,ay);
    if ((if_laser2(laser[x][y][0].v)||if_laser2(laser[x][y][2].v))&&ZRCADLA(pole[2][x][y].vec)==0&&LASER(pole[2][x][y].vec)==0) gr_hazej((UC*)obr+(nasobky340[81]),ax,ay,ax,ay);
    if ((if_laser2(laser[x][y][1].v)||if_laser2(laser[x][y][3].v))&&ZRCADLA(pole[2][x][y].vec)==0&&LASER(pole[2][x][y].vec)==0) gr_hazej((UC*)obr+(nasobky340[80]),ax,ay,ax,ay);
    if ((if_proud2(laser[x][y][0].v)||if_proud2(laser[x][y][2].v))&&ZRCADLA(pole[2][x][y].vec)==0&&ELEKTRODA(pole[2][x][y].vec)==0) gr_hazej((UC*)obr+(nasobky340[84]),ax,ay,ax,ay);
    if ((if_proud2(laser[x][y][1].v)||if_proud2(laser[x][y][3].v))&&ZRCADLA(pole[2][x][y].vec)==0&&ELEKTRODA(pole[2][x][y].vec)==0) gr_hazej((UC*)obr+(nasobky340[83]),ax,ay,ax,ay);
    hazej_buf(ax,ay);
  }
}


void pad_kaput(UC v) {
  UC p,f,g;
  p = patro[pady[v].vec];
  if (pady[v].vec!=66&&pady[v].vec!=68) {
    for (f=pady[v].x-1;f<=pady[v].x+1;f++) for (g=pady[v].y-1;g<=pady[v].y+1;g++) {
      if (pole[1][f][g].v==v&&PADA(pole[1][f][g].vec)&&(f!=pady[v].x||g!=pady[v].y))
      {
	zm_p(1,f,g,0);
      }
    }
    for (f=pady[v].x-1;f<=pady[v].x+1;f++) for (g=pady[v].y-1;g<=pady[v].y+1;g++) {
      if (pole[1][f][g].v==v&&(pole[1][f][g].vec==0||PADA(pole[1][f][g].vec))) {
	pole[1][f][g].v=255;
	HAZEJ_OFF(f,g);
      }
    }
  } else {
    for (f=pady[v].x-1;f<=pady[v].x+1;f++) for (g=pady[v].y-1;g<=pady[v].y+1;g++) {
      if (veci[pole[1][f][g].v].a2==v&&(pole[1][f][g].vec==66||pole[1][f][g].vec==68)) {
	HAZEJ_OFF(f,g);
      }
    }
    veci[pole[p][pady[v].x][pady[v].y].v].a2=255;
  }
  HAZEJ_1(pady[v].x,pady[v].y);
  pady[v].vec=255;
  if (v==toppad-1) toppad--; else pvp++;
}

UC postav_pad(UC x, UC y, UC vec) {
  UC vv,i;
  vv=255;
  if (pvp>0)
    for (i=0;i<toppad;i++)
      if (pady[i].vec==255) {vv=i;pvp--;break;}
  if (vv==255){vv=toppad;toppad++;}
  pady[vv].x=x;
  pady[vv].y=y;
  pady[vv].dx=0;
  pady[vv].dy=0;
  pady[vv].ii=0;
  pady[vv].vec=vec;
  pady[vv].obr=vec;
  pady[vv].frm=0;
  pady[vv].ax=nasobkyx[x];
  pady[vv].ay=nasobkyy[y];
  if (vec==66||vec==68) veci[pole[1][x][y].v].a2=vv; else pole[patro[vec]][x][y].v=vv;
  HAZEJ_ON(x,y);
  HAZEJ_1(x,y);
  return pole[1][x][y].v;
}

UC muzes_p(UC x,UC y,UC smer,UC vec,UC v) {
  UC ff=1,vc,pc,cc,mm=0;
  UC xx1,xx2;
  UC npv=1;
  signed char a,x1,y1,x2,y2,b,c;
  UC p=patro[vec];
  if (vec==58||vec==59||(vec>=69&&vec<=79)) npv=0;
  if (smer==0) {
    a=2;
    b=3;
    c=1;
    x1=1;
    y1=0;
    x2=0;
    y2=-1;
  } else if (smer==1) {
    a=3;
    b=0;
    c=2;
    x1=0;
    y1=-1;
    x2=-1;
    y2=0;
  } else if (smer==2) {
    a=0;
    b=1;
    c=3;
    x1=-1;
    y1=0;
    x2=0;
    y2=1;
  } else if (smer==3) {
    a=1;
    b=2;
    c=0;
    x1=0;
    y1=1;
    x2=1;
    y2=0;
  }
  vc=pole[0][x][y].vec;
  if(((laser[x][y][a].v!=255&&MAGNET(veci[laser[x][y][a].v].vec)&&npv)||(vc==smer+2&&veci[pole[0][x][y].v].a3)||(vc==7&&v!=255))&&(vc!=a+2||veci[pole[0][x][y].v].a3==0)&&vc!=50&&vc!=51&&vybuch[x][y]==0) {
    mm=1;
    pc=pole[0][x+x1][y+y1].vec;
    if (pole[1][x+x1][y+y1].vec==0&&pole[2][x+x1][y+y1].vec==0&&vybuch[x+x1][y+y1]==0&&(pc!=a+2||veci[pole[0][x+x1][y+y1].v].a3==0)&&pc!=50&&pc!=51&&(!(laser[x][y][smer].v!=255&&MAGNET(veci[laser[x][y][smer].v].vec)&&npv))) {
      if (v==255) v=postav_pad(x,y,vec);
      if (vec==66||vec==68) pady[veci[v].a2].smer=smer; else pady[v].smer=smer;
      pole[p][x+x1][y+y1].v = v;
      HAZEJ_ON(x+x1,y+y1);
      zm_p(p,x+x1,y+y1,vec);
      return 1;
    } else {
      vc=pole[1][x+x1][y+y1].vec;
      if ((PADA(vc)&&pole[1][x+x1][y+y1].v!=255)||((vc==66||vc==68)&&veci[pole[1][x+x1][y+y1].v].a2!=255)) ff=0; else {
	if (TY(pole[2][x+x1][y+y1].vec)&&x+x1==ty.x&&y+y1==ty.y&&(ty.dx>13||ty.dy>10||ty.dx<-13||ty.dy<-10)) ff=0;
      }
      if (ff&&v!=255) PlayEffect(ef_zuch,8363,(32-abs(ty.x-x))*(22-abs(ty.y-y))/22,0);
      if (ff&&KULATY(vc)&&KULATY(pole[2][x+x1][y+y1].vec)&&(vc!=0||pole[2][x+x1][y+y1].vec!=0)) {
	cc = pole[0][x][y].vec;
	pc = pole[0][x+x2][y+y2].vec;
	vc = pole[0][x+x1+x2][y+y1+y2].vec;
	xx1 = (pole[1][x+x2][y+y2].vec==0&&pole[2][x+x2][y+y2].vec==0&&pole[1][x+x1+x2][y+y1+y2].vec==0&&pole[2][x+x1+x2][y+y1+y2].vec==0&&vybuch[x+x2][y+y2]==0&&vybuch[x+x1+x2][y+y1+y2]==0&&(cc!=b+2||veci[pole[0][x][y].v].a3==0)&&(pc!=b+2||veci[pole[0][x+x2][y+y2].v].a3==0)&&pc!=50&&pc!=51&&(vc!=a+2||veci[pole[0][x+x1+x2][y+y1+y2].v].a3==0)&&vc!=50&&vc!=51&&(!(laser[x+x2][y+y2][smer].v!=255&&MAGNET(veci[laser[x+x2][y+y2][smer].v].vec)&&npv))&&(!(laser[x+x2][y+y2][c].v!=255&&MAGNET(veci[laser[x+x2][y+y2][c].v].vec)&&npv)));
	pc = pole[0][x-x2][y-y2].vec;
	vc = pole[0][x+x1-x2][y+y1-y2].vec;
	xx2 = (pole[1][x-x2][y-y2].vec==0&&pole[2][x-x2][y-y2].vec==0&&pole[1][x+x1-x2][y+y1-y2].vec==0&&pole[2][x+x1-x2][y+y1-y2].vec==0&&vybuch[x-x2][y-y2]==0&&vybuch[x+x1-x2][y+y1-y2]==0&&(cc!=c+2||veci[pole[0][x][y].v].a3==0)&&(pc!=c+2||veci[pole[0][x-x2][y-y2].v].a3==0)&&pc!=50&&pc!=51&&(vc!=a+2||veci[pole[0][x+x1-x2][y+y1-y2].v].a3==0)&&vc!=50&&vc!=51&&(!(laser[x-x2][y-y2][smer].v!=255&&MAGNET(veci[laser[x-x2][y-y2][smer].v].vec)&&npv))&&(!(laser[x-x2][y-y2][b].v!=255&&MAGNET(veci[laser[x-x2][y-y2][b].v].vec)&&npv)));
	if (xx1&&(smer_uskoku==0||(!xx2))) {
	  if (xx2) smer_uskoku=1;
	  smer=smer*2+4;
	  if (v==255) v=postav_pad(x,y,vec);
	  if (vec==66||vec==68) pady[veci[v].a2].smer=smer; else pady[v].smer=smer;
	  pole[p][x+x2][y+y2].v = v;
	  pole[p][x+x1+x2][y+y1+y2].v = v;
          HAZEJ_ON(x+x2,y+y2);
          HAZEJ_ON(x+x1+x2,y+y1+y2);
	  zm_p(p,x+x2,y+y2,vec);
	  zm_p(p,x+x1+x2,y+y1+y2,vec);
	  return 1;
	} else if (xx2) {
	  if (xx1) smer_uskoku=0;
	  smer=smer*2+5;
	  if (v==255) v=postav_pad(x,y,vec);
	  if (vec==66||vec==68) pady[veci[v].a2].smer=smer; else pady[v].smer=smer;
	  pole[p][x-x2][y-y2].v = v;
	  pole[p][x+x1-x2][y+y1-y2].v = v;
          HAZEJ_ON(x-x2,y-y2);
          HAZEJ_ON(x+x1-x2,y+y1-y2);
	  zm_p(p,x-x2,y-y2,vec);
	  zm_p(p,x+x1-x2,y+y1-y2,vec);
	  return 1;
	}
      }
    }
  }
  if (v!=255) {
    if (vec==66||vec==68) {
      if (ff) {
	if (mm) {
	  if (POTVORY(pole[2][x+x1][y+y1].vec)) vybouchni(pole[2][x+x1][y+y1].v,x+x1,y+y1,2); else
	  if ((pole[2][x+x1][y+y1].vec>=157&&pole[2][x+x1][y+y1].vec<=179)) {
	    if (ty.brneni>=10) {ty.brneni-=10;nakresli_pruh2();} else vybouchni(pole[2][x+x1][y+y1].v,x+x1,y+y1,2);
	  }
	}
	pad_kaput(veci[v].a2);
	uvolni_pad3(x,y);
      } else pady[veci[v].a2].smer+=12;
    } else {
      if (ff) {
	if (mm&&vec!=59&&(vec<69||vec>79)) {
	  if (POTVORY(pole[2][x+x1][y+y1].vec)) vybouchni(pole[2][x+x1][y+y1].v,x+x1,y+y1,2); else
	  if ((pole[2][x+x1][y+y1].vec>=157&&pole[2][x+x1][y+y1].vec<=179)) {
	    if (ty.brneni>=10) {ty.brneni-=10;nakresli_pruh2();} else vybouchni(pole[2][x+x1][y+y1].v,x+x1,y+y1,2);
	  }
	}
	pad_kaput(v);
	uvolni_pad3(x,y);
      }else pady[v].smer+=12;
    }
  }
  return 0;
}

UC uvolni_pad2(UC x,UC y,UC smer) {
  UC vec=pole[1][x][y].vec;
  if ((PADA(vec)&&pole[1][x][y].v==255)||((vec==66||vec==68)&&veci[pole[1][x][y].v].a2==255)) {
    return muzes_p(x,y,smer,vec,255);
  }
  return 0;
}

void uvolni_pad4(UC x,UC y,UC smer) {
  UC b,x1=x,y1=y,pc,pv,v;
  UC npv = (pole[1][x][y].vec==58||pole[1][x][y].vec==59);
  if (pole[1][x][y].vec!=59&&ty.vec>=157&&ty.vec<=160) return;
  if(smer==0) {b=2;x1++;} else
  if(smer==1) {b=3;y1--;} else
  if(smer==2) {b=0;x1--;} else
  if(smer==3) {b=1;y1++;}
  pc = pole[0][x][y].vec;
  pv = pole[0][x1][y1].vec;
  if (pole[1][x1][y1].vec==0&&pole[2][x1][y1].vec==0)
  if (pole[1][x][y].v==255&&
  (pc!=b+2||veci[pole[0][x][y].v].a3==0)&&(pv!=b+2||veci[pole[0][x1][y1].v].a3==0)&&pv!=50&&pc!=50&&pv!=51&&pc!=51&&
  (npv||laser[x][y][smer].v==255||(!(MAGNET(veci[laser[x][y][smer].v].vec))))) {
    v=postav_pad(x,y,pole[1][x][y].vec);
    pady[v].smer=smer;
    pole[1][x1][y1].v=v;
    HAZEJ_ON(x1,y1);
    zm_p(1,x1,y1,pole[1][x][y].vec);
  }
}

void uvolni_pad3(UC x,UC y) {
  if (!uvolni_pad2(x,y,0))
  if (!uvolni_pad2(x,y,1))
  if (!uvolni_pad2(x,y,2))
  uvolni_pad2(x,y,3);
}

void uvolni_pad(UC x,UC y) {
  if (pole[1][x][y].vec==0&&pole[2][x][y].vec==0) {
    if (!uvolni_pad2(x,y-1,3))
    if (!uvolni_pad2(x-1,y,0))
    if (!uvolni_pad2(x,y+1,1))
    if (!uvolni_pad2(x+1,y,2))
    if (smer_uskoku==1) {
      smer_uskoku=0;
      if (!uvolni_pad2(x-1,y-1,0))
      if (!uvolni_pad2(x-1,y+1,1))
      if (!uvolni_pad2(x+1,y-1,3))
      if (!uvolni_pad2(x+1,y+1,2))

      if (!uvolni_pad2(x+1,y,3))
      if (!uvolni_pad2(x,y+1,2))
      if (!uvolni_pad2(x-1,y,1))
      if (!uvolni_pad2(x,y-1,0))

      if (!uvolni_pad2(x-1,y-1,3))
      if (!uvolni_pad2(x-1,y+1,0))
      if (!uvolni_pad2(x+1,y-1,2))
      if (!uvolni_pad2(x+1,y+1,1))

      if (!uvolni_pad2(x+1,y,1))
      if (!uvolni_pad2(x,y+1,0))
      if (!uvolni_pad2(x-1,y,3))
      if (!uvolni_pad2(x,y-1,2))
      smer_uskoku=1;
    } else {
      smer_uskoku=1;
      if (!uvolni_pad2(x-1,y-1,3))
      if (!uvolni_pad2(x-1,y+1,0))
      if (!uvolni_pad2(x+1,y-1,2))
      if (!uvolni_pad2(x+1,y+1,1))

      if (!uvolni_pad2(x+1,y,1))
      if (!uvolni_pad2(x,y+1,0))
      if (!uvolni_pad2(x-1,y,3))
      if (!uvolni_pad2(x,y-1,2))

      if (!uvolni_pad2(x-1,y-1,0))
      if (!uvolni_pad2(x-1,y+1,1))
      if (!uvolni_pad2(x+1,y-1,3))
      if (!uvolni_pad2(x+1,y+1,2))

      if (!uvolni_pad2(x+1,y,3))
      if (!uvolni_pad2(x,y+1,2))
      if (!uvolni_pad2(x-1,y,1))
      if (!uvolni_pad2(x,y-1,0))
      smer_uskoku=0;
    }
  }
}

void zm_p(UC p,UC x,UC y,UC vec) {
  UC ovec=pole[p][x][y].vec;
  pole[p][x][y].vec = vec;
  if (vec==0) {
    if (pole[0][x][y].vec==54||pole[0][x][y].vec==55) naslapna_podlaha(x,y);
    uvolni_pad(x,y);
  }
  else if (PADA(vec)||vec==66||vec==68) {
    uvolni_pad3(x,y);
  }
  if (STOP_LASER(ovec)||STOP_LASER(vec)) {
    if(!(ZRCADLA(vec)&&ZRCADLA(ovec))) {
      init_magnet(x,y,laser[x+1][y][2].v);
      init_magnet(x,y,laser[x-1][y][0].v);
      init_magnet(x,y,laser[x][y+1][1].v);
      init_magnet(x,y,laser[x][y-1][3].v);
    } else {
      if (ovec==137||ovec==141) {
	init_magnet(x,y,laser[x][y-1][3].v);
	init_magnet(x,y,laser[x+1][y][2].v);
	init_magnet(x,y,laser[x-1][y][0].v);
	init_magnet(x,y,laser[x][y+1][1].v);
      } else if (ovec==138||ovec==142) {
	init_magnet(x,y,laser[x][y-1][3].v);
	init_magnet(x,y,laser[x-1][y][0].v);
	init_magnet(x,y,laser[x+1][y][2].v);
	init_magnet(x,y,laser[x][y+1][1].v);
      } else if (ovec==139||ovec==143) {
	init_magnet(x,y,laser[x-1][y][0].v);
	init_magnet(x,y,laser[x][y+1][1].v);
	init_magnet(x,y,laser[x+1][y][2].v);
	init_magnet(x,y,laser[x][y-1][3].v);
      } else if (ovec==140||ovec==144) {
	init_magnet(x,y,laser[x+1][y][2].v);
	init_magnet(x,y,laser[x][y+1][1].v);
	init_magnet(x,y,laser[x-1][y][0].v);
	init_magnet(x,y,laser[x][y-1][3].v);
      }
    }
    laser_pom = 0;
  }
}


/*UC if_laser(UC x, UC y) {
  if (if_laser2(laser[x][y][0].v)||if_laser2(laser[x][y][1].v)||if_laser2(laser[x][y][2].v)||if_laser2(laser[x][y][3].v)) return 1;
  return 0;
}
UC if_proud(UC x, UC y) {
  if (if_proud2(laser[x][y][0].v)||if_proud2(laser[x][y][1].v)||if_proud2(laser[x][y][2].v)||if_proud2(laser[x][y][3].v)) return 1;
  return 0;
} */
UC if_laspr(UC x, UC y) {
  if (if_laspr2(laser[x][y][0].v)||if_laspr2(laser[x][y][1].v)||if_laspr2(laser[x][y][2].v)||if_laspr2(laser[x][y][3].v)) return 1;
  return 0;
}


UC sm_laseru(UC x,UC y,UC v,UC puvsm) {
  UC sm=4;
  if (laser[x][y][0].v==v) sm=0;
  if (laser[x][y][1].v==v&&(sm==4||puvsm==1)) sm=1;
  if (laser[x][y][2].v==v&&(sm==4||puvsm==2)) sm=2;
  if (laser[x][y][3].v==v&&(sm==4||puvsm==3)) sm=3;
  if (sm!=4&&laser[x][y][sm].k==1) konec_laseru=1;
  return sm;
}


void centruj(UC psn){
  signed int x,y;
  x=ty.ax;
  y=ty.ay;
  if (x<=150) x=0; else if (x>470) x=320; else x-=150;
  if (y<=81) y=0; else if (y>281) y=200; else y-=81;
  if (x==sx&&y==sy) vr(); else ssc(x,y,psn);
  if (x==sx&&y==sy) pohyb=1; else pohyb=0;
}

UC sebratelna_vec2(UC x,UC y) {
  UC a=pole[1][x][y].vec;
  if (a>=69&&a<=72) return 6;
  if (a==77) return 7;
  if (a==78) return 8;
  if (a==79) return 9;
  return 100;
}

UC sebratelna_vec(UC x,UC y) {
  UC a=pole[1][x][y].vec;
  if (a==86) return 0;
  if (a==87) return 1;
  if (a==88) return 2;
  if (a==65) return 3;
  if (a==67) return 4;
  if (a==64) return 5;
  if (pole[2][x][y].vec>=114&&pole[2][x][y].vec<=116) return 11;
  if (a==63) return 12;
return 100;
}

UC muzes_na_pasy(UC p0,UC x,UC y,UC smer) {
  if (smer==3&&pole[0][x+1][y].vec==2&&veci[pole[0][x+1][y].v].a3) return 0;
  if (smer==1&&pole[0][x-1][y].vec==4&&veci[pole[0][x-1][y].v].a3) return 0;
  if (smer==2&&pole[0][x][y+1].vec==5&&veci[pole[0][x][y+1].v].a3) return 0;
  if (smer==4&&pole[0][x][y-1].vec==3&&veci[pole[0][x][y-1].v].a3) return 0;
  if (vybuch[x][y]) return 0;
  if (p0==2&&smer==3&&veci[pole[0][x][y].v].a3) return 0;
  if (p0==3&&smer==4&&veci[pole[0][x][y].v].a3) return 0;
  if (p0==4&&smer==1&&veci[pole[0][x][y].v].a3) return 0;
  if (p0==5&&smer==2&&veci[pole[0][x][y].v].a3) return 0;
  return 1;
}

UC muzesh2(UC x,UC y)
{
  UC p1=pole[1][x][y].vec,p2=pole[2][x][y].vec;
  if (pole[1][x][y].v!=255&&p1!=66&&p1!=68) return 0;
  if (p2>=114&&p2<=116) return 1;
  if (p2==119&&ty.i[0]) {zhasni_mnu(0,ty.i[0]);ty.i[0]--;zm_p(2,x,y,0);return 1;}
  if (p2==120&&ty.i[1]) {zhasni_mnu(1,ty.i[1]);ty.i[1]--;zm_p(2,x,y,0);return 1;}
  if (p2==121&&ty.i[2]) {zhasni_mnu(2,ty.i[2]);ty.i[2]--;zm_p(2,x,y,0);return 1;}
  if (p2!=0) return 0;
  if (p1==60||p1==61) return 1;
  if ((p1==56||p1==62)&&ty.vec>=157&&ty.vec<=160) return 1;
  if (p1>=63&&p1<=88) return 1;
  if (p1!=0) return 0;
  return 1;
}

UC muzesh(UC x,UC y,UC smer)
{
  UC p0=pole[0][x][y].vec,p1=pole[1][x][y].vec,p2=pole[2][x][y].vec;
  if (pole[1][x][y].v!=255&&p1!=66&&p1!=68) return 0;
  if (!muzes_na_pasy(p0,x,y,smer)) return 0;
  if (p2>=114&&p2<=116) return 1;
  if (p2==119&&ty.i[0]) {zhasni_mnu(0,ty.i[0]);ty.i[0]--;zm_p(2,x,y,0);return 1;}
  if (p2==120&&ty.i[1]) {zhasni_mnu(1,ty.i[1]);ty.i[1]--;zm_p(2,x,y,0);return 1;}
  if (p2==121&&ty.i[2]) {zhasni_mnu(2,ty.i[2]);ty.i[2]--;zm_p(2,x,y,0);return 1;}
  if (p2==117||p2==118) return 1;
  if (ZRCADLA(p2)&&p0!=50&&p0!=51) return 1;
  if (p2==149||(p2>=150&&p2<=152)){
  if (p2==150) ty.vec=153;
  if (p2==151) ty.vec=157;
  if (p2==152) ty.vec=161;
  return 1;
  }
  if (p2!=0) return 0;
  if (p1==60||p1==61) return 1;
  if ((p1==56||p1==62)&&ty.vec>=157&&ty.vec<=160) return 1;
  if (p1>=63&&p1<=88) return 1;
  if (SOUPATELNA_VEC(p1)){
    if (smer>=1&&smer<=4) {
      if (soupani_citac<7) soupani_citac++; else {
	uvolni_pad4(x,y,smer-1);
	if (pole[1][x][y].v!=255) return 1;
      }
    }
  }
  if (p1!=0) return 0;
  return 1;
}

void vec_kaput(UC v) {
  UC p,f,g,vec;
  vec=veci[v].vec;
  if (LASER(vec)||DELO(vec)||ELEKTRODA(vec)) {
    veci[v].a3=100;
    init_magnet(veci[v].x,veci[v].y,v);
  }
  if ((vec==68||vec==66)&&veci[v].a2!=255) {
    pad_kaput(veci[v].a2);
    veci[v].a2=1;
  }
  veci[v].vec=255;
  if (v==topv-1) topv--; else pvv++;

  p = patro[vec];
  if (p==0) {
    p=0;
  }
  if (vec==181) {
    pole[0][veci[v].x][veci[v].y].v =255;
    if (pole[0][veci[v].x][veci[v].y].vec<48&&veci[v].a2==10) {
      zm_p(0,veci[v].x,veci[v].y,pole[0][veci[v].x][veci[v].y].vec+2);
      if (vybuch[veci[v].x][veci[v].y]==0) HAZEJ_1(veci[v].x,veci[v].y);
    }
  }
  if (vec!=82&&vec!=181&&vec!=85){
    for (f=veci[v].x-1;f<=veci[v].x+1;f++) for (g=veci[v].y-1;g<=veci[v].y+1;g++) {
      if (pole[p][f][g].v==v&&PREPINAC(pole[p][f][g].vec)==0&&PADA(pole[p][f][g].vec)==0) {
	pole[p][f][g].v=255;
	zm_p(p,f,g,0);
	if POTVORY(vec) {
          if (zobraz1[f][g]&128) {zobraz1[f][g]&=127;HAZEJ_1(f,g);}
          HAZEJ_OFF(f,g);
        } else HAZEJ_1(f,g);
      }
    }
  }
    for (f=0;f<topp;f++)
      for (g=0;g<6;g++)
	if(prep[f].a[g].x==128||prep[f].a[g].x==129){
	 if (prep[f].a[g].vec==v) prep[f].a[g].x=255;
	}
}


void postav_v(UC x,UC y,UC vec) {
  UC p=patro[vec],vv=255,i;
  if (VECI(vec)||(vec>=169&&vec<=172)) {
    if (pvv>0)
    for (i=0;i<topv;i++)
    if (veci[i].vec==255) {vv=i;pvv--;break;}
    if (vv==255){vv=topv;topv++;}
    postvec=vv;
    veci[vv].x=x;
    veci[vv].y=y;
    veci[vv].vec=vec;
    veci[vv].obr=vec;
    veci[vv].frm=0;
    veci[vv].ax=x*20;
    veci[vv].ay=y*17;
    veci[vv].dx=0;
    veci[vv].dy=0;
    veci[vv].a1=255;
    if (POTVORY(veci[vv].vec)) HAZEJ_ON(x,y);
    if (vec==181) veci[vv].a2=10;
    if(vec==68||vec==66) {
      veci[vv].a3 = 0;
      veci[vv].a2 = 255;
    } else
    if((vec>68&&vec<=82)||vec==181||ELEKTRODA(vec))
     veci[vv].a3 = 0;
    else
    if (vec==85) {
      veci[vv].a2=0;
      veci[vv].a3=0;
    }else if (!MAGNET(vec)) veci[vv].a2 = (ty.x+ty.y)%4;
    if (vec!=82&&vec!=85) pole[p][x][y].v=vv;
  }
  if (vec!=82&&vec!=181&&vec!=85) {
    if (x>0&&x<31&&y>0&&y<21) zm_p(p,x,y,vec); else pole[p][x][y].vec=vec;
  }
  if (p<2&&vec&&vec!=181) {
    vec = pole[1][x][y].vec;
    i = pole[0][x][y].vec;
    vv = pole[1][x][y].v;
    if (PADA(vec)||vec==66||vec==68) {
      if (i==2) uvolni_pad2(x,y,0);else
      if (i==3) uvolni_pad2(x,y,1);else
      if (i==4) uvolni_pad2(x,y,2);else
      if (i==5) uvolni_pad2(x,y,3);else
      if (vec!=59) {
	if (i==52||i==53) naslapna_podlaha(x,y);
	if ((i==46||i==47)&&pole[0][x][y].v==255) postav_v(x,y,181);
	if (DIRA(i)) {
	  if (vv!=255) vec_kaput(vv); else {
	    zm_p(1,x,y,0);
	  }
	}
      }
    }
  }
}




void ty_kaput()
{
if (ty.brneni) {ty.brneni=0;nakresli_pruh2();}
konec=1;
zm_p(2,ty.x,ty.y,0);HAZEJ_OFF(ty.x,ty.y);
if (TY(pole[2][ty.x+1][ty.y].vec)) {zm_p(2,ty.x+1,ty.y,0);HAZEJ_OFF(ty.x+1,ty.y);}
if (TY(pole[2][ty.x-1][ty.y].vec)) {zm_p(2,ty.x-1,ty.y,0);HAZEJ_OFF(ty.x-1,ty.y);}
if (TY(pole[2][ty.x][ty.y+1].vec)) {zm_p(2,ty.x,ty.y+1,0);HAZEJ_OFF(ty.x,ty.y+1);}
if (TY(pole[2][ty.x][ty.y-1].vec)) {zm_p(2,ty.x,ty.y-1,0);HAZEJ_OFF(ty.x,ty.y-1);}
}

UC zjisti_zastaveni(UC x,UC y,UC smer,UC vec) {
  UC vec1 = pole[1][x][y].vec;
  UC vec2 = pole[2][x][y].vec;
  if (vec > 144) {
    if (vec2 == 124) return(6);
    return(smer);
  } else {
    if (vec1==0&&vec2==0) return smer;
    if (vec2!=0) {
      if (!STOP_LASER(vec2)) return(smer);
      if (ZRCADLA(vec2)&&(!ELEKTRODA(vec))) {
	if (vec2==137||vec2==141) {
	  if (smer == 3) return (0); else if(smer==2) return(1); else return(4);
        } else if (vec2==138||vec2==142) {
	  if (smer == 0) return (1); else if(smer==3) return(2); else return(4);
        } else if (vec2==139||vec2==143) {
	  if (smer == 0) return (3); else if(smer==1) return(2); else return(4);
	} else if (vec2==140||vec2==144) {
	  if (smer == 1) return (0); else if(smer==2) return(3); else return(4);
        }
      }
      return 4;
    } else {
      if (!STOP_LASER(vec1)) return(smer);
      return(64);
    }
  }
}

void znic_to_l(UC x,UC y,UC de) {
  UC vec = pole[2][x][y].vec,v2;
  if (vec!=0) {
    if (TY(vec)) {
      if (de==1) {
	if (ty.brneni>=20) { ty.brneni-=20;nakresli_pruh2();} else vybouchni(255,x,y,2);
      } else {
	if (x==ty.x&&y==ty.y) jsem_v_laseru=1; else jsem_v_laseru=2;
	if (de==2) jsem_v_proudu=1;
      }
    } else
    if (vec>=165&&vec<=180&&pole[2][x][y].v!=255) vybouchni(pole[2][x][y].v,x,y,2); else
    if ((vec>=107&&vec<=116)||(vec>=125&&vec<=132)) {
      if (pole[0][x][y].v==255) {
	postav_v(x,y,181);
	veci[postvec].a2=2;
      } else {
	v2 = pole[2][x][y].v;
	if (v2!=255) vec_kaput(v2);
	else zm_p(2,x,y,0);
	HAZEJ_1(x,y);
      }
    }
  } else {
    vec = pole[1][x][y].vec;
    if (vec!=0){
      if ((vec>=57&&vec<=59)||(vec>=63&&vec<=68)) {
	if (pole[0][x][y].v==255) {
	  postav_v(x,y,181);
	  veci[postvec].a2=1;
	} else {
	  v2 = pole[1][x][y].v;
	  if (vec>=65&&vec<=68) vybouchni(v2,x,y,1); else
	  if (v2!=255) {
	    if (PADA(vec)) {
	      pad_kaput(v2);
	      zm_p(1,pady[v2].x,pady[v2].y,0);
	      HAZEJ_1(pady[v2].x,pady[v2].y);
	    } else vec_kaput(v2);
	  }else zm_p(1,x,y,0);
	  HAZEJ_1(x,y);
	}
      }
    }
  }
}

void vystrel(UC x,UC y, UC v) {
  UC sm;
  if (DELO(veci[v].vec)&&veci[v].a3==1&&topv<230){
    konec_laseru = 0;
    x = veci[v].x;
    y = veci[v].y;
    sm = sm_laseru(veci[v].x,veci[v].y,v,6);
    if (sm==0) x++;
    if (sm==1) y--;
    if (sm==2) x--;
    if (sm==3) y++;
    if (konec_laseru==1) znic_to_l(x,y,1); else {
      postav_v(x,y,85);
      veci[postvec].a1 = sm;
      veci[v].a3 = 60;
    }
  }
}

void zjisti_jestli_se_vypnul() {
  UC x=ty.x,y=ty.y,a=1;
  if (if_laser2(laser[x-1][y][0].v)||if_proud2(laser[x-1][y][0].v)) a=0;
  if (if_laser2(laser[x][y-1][3].v)||if_proud2(laser[x][y-1][3].v)) a=0;
  if (if_laser2(laser[x+1][y][2].v)||if_proud2(laser[x+1][y][2].v)) a=0;
  if (if_laser2(laser[x][y+1][1].v)||if_proud2(laser[x][y+1][1].v)) a=0;
  if (jsem_v_laseru==2) {
    if (ty.dx>0) x++; else
    if (ty.dx<0) x--; else
    if (ty.dy>0) y++; else
    if (ty.dy<0) y--;
    if (if_laser2(laser[x-1][y][0].v)||if_proud2(laser[x-1][y][0].v)) a=0;
    if (if_laser2(laser[x][y-1][3].v)||if_proud2(laser[x][y-1][3].v)) a=0;
    if (if_laser2(laser[x+1][y][2].v)||if_proud2(laser[x+1][y][2].v)) a=0;
    if (if_laser2(laser[x][y+1][1].v)||if_proud2(laser[x][y+1][1].v)) a=0;
  }
  if (a) {
    jsem_v_laseru=0;
    jsem_v_proudu=0;
  }
}

void init_magnet(UC x1,UC y1,UC v) {
  UC ls,ma,el,smer,x,y,a,xx,mode=veci[v].a3;
  if (v==255) return;
  if (laser_pom) return;
  ls = LASER(veci[v].vec);
  ma = MAGNET(veci[v].vec);
  el = ELEKTRODA(veci[v].vec);
  if (ma&&(x1!=veci[v].x||y1!=veci[v].y)) {
    if (pole[2][x1][y1].vec!=124) {
      smer=veci[v].vec-145;
      if (smer==0) { if (laser[x1][y1][0].v==v) return;} else
      if (smer==1) { if (laser[x1][y1][1].v==v) return;} else
      if (smer==2) { if (laser[x1][y1][2].v==v) return;} else
      if (smer==3) { if (laser[x1][y1][3].v==v) return;}
    }
  }
  if (DELO(veci[v].vec)&&pole[2][x1][y1].vec>=153) {vystrel(x1,y1,v); return;}
  xx=0;
  if (laser[x1-1][y1][0].v==v) xx++;
  if (laser[x1][y1+1][1].v==v) xx++;
  if (laser[x1+1][y1][2].v==v) xx++;
  if (laser[x1][y1-1][3].v==v) xx++;
  if (xx!=1) {
    x1 = veci[v].x;
    y1 = veci[v].y;
  } else if (xx==2)laser_pom = 1;
  xx=1;
  x = x1;
  y = y1;
  konec_laseru=0;
  do {
    smer = sm_laseru(x,y,v,smer);
    if (smer==0){
      laser[x][y][0].v=255;
      if (ma) {
	if (PADA(pole[1][x-1][y].vec)||pole[1][x-1][y].vec==66||pole[1][x-1][y].vec==68) uvolni_pad3(x-1,y);
      } else
      if (ls) {
	if (pole[2][x][y].vec>=141&&pole[2][x][y].vec<=144) pole[2][x][y].vec-=4;
	HAZEJ_1(x,y);
      } else if (el) HAZEJ_1(x,y);
      x++;
    }
    else if (smer==1){
      laser[x][y][1].v=255;
      if (ma) {
	if (PADA(pole[1][x][y+1].vec)||pole[1][x][y+1].vec==66||pole[1][x][y+1].vec==68) uvolni_pad3(x,y+1);
      } else
      if (ls){
	if (pole[2][x][y].vec>=141&&pole[2][x][y].vec<=144) pole[2][x][y].vec-=4;
	HAZEJ_1(x,y);
      }else if (el) HAZEJ_1(x,y);
      y--;
    }
    else if (smer==2){
      laser[x][y][2].v=255;
      if (ma) {
	if (PADA(pole[1][x+1][y].vec)||pole[1][x+1][y].vec==66||pole[1][x+1][y].vec==68) uvolni_pad3(x+1,y);
      } else
      if (ls){
	if (pole[2][x][y].vec>=141&&pole[2][x][y].vec<=144) pole[2][x][y].vec-=4;
	HAZEJ_1(x,y);
      } else if (el) HAZEJ_1(x,y);
      x--;
    }
    else if (smer==3){
      laser[x][y][3].v=255;
      if (ma) {
	if (PADA(pole[1][x][y-1].vec)||pole[1][x][y-1].vec==66||pole[1][x][y-1].vec==68) uvolni_pad3(x,y-1);
      } else
      if (ls) {
	if (pole[2][x][y].vec>=141&&pole[2][x][y].vec<=144) pole[2][x][y].vec-=4;
	HAZEJ_1(x,y);
      } else if (el) HAZEJ_1(x,y);
      y++;
    } else konec_laseru = 1;
    xx = !konec_laseru;
  } while (xx);
  if ((ls||el)&&jsem_v_laseru) zjisti_jestli_se_vypnul();
  if ((mode==0&&ls)||(mode!=0&&el)||mode==100) {
//    if (el&&mode>39) veci[v].a3=39;
    return;
  }

  xx=1;
  x = x1;
  y = y1;
    if (x==veci[v].x&&y==veci[v].y) smer = (veci[v].vec-125)&0x03; else {
    if (laser[x-1][y][0].v==v) {x--;smer=0;}  else
    if (laser[x][y+1][1].v==v) {y++;smer=1;}  else
    if (laser[x+1][y][2].v==v) {x++;smer=2;}  else
    if (laser[x][y-1][3].v==v) {y--;smer=3;}
  }
  do {
    if (smer==0) {
      laser[x][y][0].v = v;
      laser[x][y][0].k = 0;
      if (ma) {
	if (PADA(pole[1][x][y].vec)||pole[1][x][y].vec==66||pole[1][x][y].vec==68) uvolni_pad3(x,y);
      }
      x++;
      if (x>31) xx=!xx; else {
	smer=zjisti_zastaveni(x,y,smer,veci[v].vec);
	if (smer>3) {
	  xx=!xx;
	  laser[x-1][y][0].k = 1;
	  if (ls||el) {
	    if(el&&(pole[2][x][y].vec!=135||veci[pole[2][x][y].v].a3>5)) veci[v].a3=45;
	    znic_to_l(x,y,el?2:0);
          }
	} else if (ls)  {
	  if (pole[2][x][y].vec>=137&&pole[2][x][y].vec<=140) {pole[2][x][y].vec+=4;hazej(nasobkyx[x],nasobkyy[y],pole[2][x][y].vec);} else
	  hazej(nasobkyx[x],nasobkyy[y],81);
	} else if (el) hazej(nasobkyx[x],nasobkyy[y],84);
      }
    } else if (smer==1) {
      laser[x][y][1].v = v;
      laser[x][y][1].k = 0;
      if (ma) {
	if (PADA(pole[1][x][y].vec)||pole[1][x][y].vec==66||pole[1][x][y].vec==68) uvolni_pad3(x,y);
      }
      y--;
      if (y>21) xx=!xx; else{
	smer=zjisti_zastaveni(x,y,smer,veci[v].vec);
	if (smer>3) {
	  xx=!xx;
	  laser[x][y+1][1].k = 1;
	  if (ls||el) {
	    if(el&&(pole[2][x][y].vec!=136||veci[pole[2][x][y].v].a3>5)) veci[v].a3=45;
	    znic_to_l(x,y,el?2:0);
          }
	} else if (ls)  {
	  if (pole[2][x][y].vec>=137&&pole[2][x][y].vec<=140) {pole[2][x][y].vec+=4;hazej(nasobkyx[x],nasobkyy[y],pole[2][x][y].vec);} else
	  hazej(nasobkyx[x],nasobkyy[y],80);
	} else if (el) hazej(nasobkyx[x],nasobkyy[y],83);
      }
    } else if (smer==2) {
      laser[x][y][2].v = v;
      laser[x][y][2].k = 0;
      if (ma) {
	if (PADA(pole[1][x][y].vec)||pole[1][x][y].vec==66||pole[1][x][y].vec==68) uvolni_pad3(x,y);
      }
      x--;
      if (x>31)xx=!xx; else {
	smer=zjisti_zastaveni(x,y,smer,veci[v].vec);
	if (smer>3) {
	  xx=!xx;
	  laser[x+1][y][2].k = 1;
	  if (ls||el) {
	    if(el&&(pole[2][x][y].vec!=133||veci[pole[2][x][y].v].a3>5)) veci[v].a3=45;
	    znic_to_l(x,y,el?2:0);
          }
	} else if (ls)  {
	  if (pole[2][x][y].vec>=137&&pole[2][x][y].vec<=140) {pole[2][x][y].vec+=4;hazej(nasobkyx[x],nasobkyy[y],pole[2][x][y].vec);} else
	  hazej(nasobkyx[x],nasobkyy[y],81);
	} else if (el) hazej(nasobkyx[x],nasobkyy[y],84);
      }
    } else {
      laser[x][y][3].v = v;
      laser[x][y][3].k = 0;
      if (ma) {
	if (PADA(pole[1][x][y].vec)||pole[1][x][y].vec==66||pole[1][x][y].vec==68) uvolni_pad3(x,y);
      }
      y++;
      if (y>21)xx=!xx; else {
	smer=zjisti_zastaveni(x,y,smer,veci[v].vec);
	if (smer>3) {
	  xx=!xx;
	  laser[x][y-1][3].k = 1;
	  if (ls||el) {
	    if(el&&(pole[2][x][y].vec!=134||veci[pole[2][x][y].v].a3>5)) veci[v].a3=45;
	    znic_to_l(x,y,el?2:0);
          }
	} else if (ls)  {
	  if (pole[2][x][y].vec>=137&&pole[2][x][y].vec<=140) {pole[2][x][y].vec+=4;hazej(nasobkyx[x],nasobkyy[y],pole[2][x][y].vec);} else
	  hazej(nasobkyx[x],nasobkyy[y],80);
	} else if (el) hazej(nasobkyx[x],nasobkyy[y],83);
      }
    }
  } while (xx);
}



// *******************************************************************
// *                                                                 *
// *               I N I T   A   D O N E   L E V E L                 *
// *                                                                 *
// *******************************************************************


void init_level(){
  FILE *s;
  UC f,g,usek2,tmp;
  UI p=704,x,y;
  nastav_paletu0();
  if (sejvnuto) {
    UC *mnu2;
    nesejvuj_to=1;
    mnu2=(UC*)mnu;
    topv = mnu[0];
    postvec = mnu[1];
    pvv = mnu[2];
    toppad = mnu[3];
    pvp = mnu[4];
    smer_uskoku = mnu[5];
    topp = mnu[6];
    level = mnu[7];
    usek = mnu[8];
    hudusek = mnu[9];
    ii = mnu[10];
    sm = mnu[11];
    pohyb = mnu[12];
    zvol_vec = mnu[13];
    laser_pom = mnu[14];
    konec_laseru = mnu[15];
    jsem_v_laseru = mnu[16];
    jsem_v_proudu = mnu[17];
    memcpy(&paleta_citac,mnu2+18,2);
    soupani_citac = mnu[20];
    konec = mnu[21];
    vyskoc = mnu[22];
    memcpy(pole,mnu2+100,5632);
    memcpy(zobraz1,mnu2+5732,704);
    memcpy(zobraz2,mnu2+6436,704);
    memcpy(laser,mnu2+7140,5632);
    memcpy(veci,mnu2+12772,4000);
    memcpy(pady,mnu2+16772,3000);
    memcpy(prep,mnu2+19772,2800);
    memcpy(&ty,mnu2+22572,23);
    memcpy(vybuch,mnu2+22596,704);
    s = fopen("environ.dat","rb");
    fseek(s,10,0);
    fread(pal,3,256,s);
    fclose(s);
  } else {
    inituju_level=1;
    nesejvuj_to=0;
    pvp=0;
    pvv=0;
    s = fopen("krkal.lev","rb");
    fseek(s,levly[level].odkaz,0);
    usek2 = usek;
    fread(&usek,1,1,s);
    fread(&ty.x,1,1,s);
    fread(&topv,1,1,s);
    fread(&topp,1,1,s);
    fread(&topgp,1,1,s);
    fread(buffer,4,1,s);
    fread(&ty.x,1,1,s);
    fread(&ty.y,1,1,s);
    fread(buffer,2,704,s);
    memset(laser,255,5632);
    tmp=topv;
    toppad = 0;
    for (g=0;g<22;g++) for (f=0;f<32;f++) {
      pole[0][f][g].v = 255;
      pole[1][f][g].v = 255;
      pole[2][f][g].v = 255;
      pole[3][f][g].v = 255;
      pole[0][f][g].vec = 8;
      pole[1][f][g].vec = 0;
      pole[2][f][g].vec = 0;
      pole[3][f][g].vec = 0;
    }

    memset(vybuch,0,32*22);
    memset(zobraz1,0,32*22);
    memset(zobraz2,0,32*22);
    for (g=0;g<22;g++) for (f=0;f<32;f++){
      if (buffer[p]) postav_v(f,g,buffer[p]);
      p++;
    }
    fread(&buffer[704],tmp,6,s);
    p=704;

    for (f=0;f<tmp;f++) {
      veci[f].x = buffer[p];
      veci[f].y = buffer[p+1];
      veci[f].vec = buffer[p+2];
      if (PAS(veci[f].vec)&&buffer[p+5]){
	veci[f].obr = buffer[p+2]+97;
	veci[f].frm = 4;
      }else{
	veci[f].obr = buffer[p+2];
	veci[f].frm = 0;
      }
      pole[patro[veci[f].vec]][veci[f].x][veci[f].y].v = f;
      veci[f].a1 = buffer[p+3];
      veci[f].a2 = buffer[p+4];
      veci[f].a3 = buffer[p+5];
      p+=6;
      veci[f].ax = veci[f].x*20;
      veci[f].ay = veci[f].y*17;
      veci[f].dx=0;
      veci[f].dy=0;
      if (POTVORY(veci[f].vec)) HAZEJ_ON(veci[f].x,veci[f].y);
    }
    fread(&buffer[704],topp,22,s);
    fclose(s);
    p=704;
    for (f=0;f<topp;f++) {
      prep[f].x = buffer[p];
      prep[f].y = buffer[p+1];
      prep[f].vec = buffer[p+2];
      pole[patro[prep[f].vec]][prep[f].x][prep[f].y].v = f; //+PREPINC;
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
    p=0;
    for (g=0;g<22;g++) for (f=0;f<32;f++) {
      if (PREPINAC(buffer[p])==0) postav_v(f,g,buffer[p]);
      p++;
    }
    p=0;
    for (g=0;g<22;g++) for (f=0;f<32;f++) {
      if (PREPINAC(buffer[p])) postav_v(f,g,buffer[p]);
      p++;
    }
    ty.ax = ty.x*20;
    ty.ay = ty.y*17;
    ty.dx=0;
    ty.dy=0;
    HAZEJ_ON(ty.x,ty.y);
    ty.vec = pole[2][ty.x][ty.y].vec;
    ty.brneni = 0;
    ty.otrava = 0;
    ty.miz = 0;
    ty.odpocitavac = 0;
    jsem_v_laseru=0;
    jsem_v_proudu=0;
    soupani_citac = 0;
    memset(ty.i,0,10);
    s = fopen("environ.dat","rb");
    fseek(s,10,0);
    fread(pal,3,256,s);
    if (usek!=usek2) {
      fseek(s,environment[usek].odkaz,0);
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
  x=ty.ax;
  y=ty.ay;
  if (x<150) x=0; else if (x>470) x=320; else x-=150;
  if (y<81) y=0; else if (y>281) y=200; else y-=81;
  setxy(x,y);
  s = fopen("menu.dat","rb");
  fseek(s,104631,0);
  fread(buffer,26,320,s);
  fclose(s);
  { UI g=0;
  for (y=0;y<26;y++) for (x=0;x<320;x++) putp(x,y-26,buffer[g++]);
  }

  inituju_level=0;
  for (g=0;g<22;g++) for (f=0;f<32;f++) hazej3(f,g);
  if (sejvnuto) {
    for (f=0;f<10;f++) for (g=1;g<=ty.i[f];g++) rozsvit_mnu(f,g);
    nakresli_pruh1();
    nakresli_pruh2();
  } else {
    for (f=0;f<topv;f++) {
      if (LASER(veci[f].vec)||DELO(veci[f].vec)||MAGNET(veci[f].vec)||ELEKTRODA(veci[f].vec)) init_magnet(veci[f].x,veci[f].y,f);
    }
    pohyb=1;
    konec=0;
    sm=0;
    ii=0;
    zvol_vec=3;
  }
  oznac_mnu();
  SetPalette(pal);

}

void rc(UI x,UI y,UC c)
{
UI f,g;
for (g=y;g<=y+1;g++)
  for (f=x;f<=x+1;f++)
   vputpixelb(f,g,c);
}

void z_volume()
{
  UC x,y;
  unsigned short l,r;
  dsmGetMainVU(&l,&r);
  for (y=0;y<64;y++)
    {
    if (l==y) rc(0,196-y*3,254); else rc(0,196-y*3,l>y?253:0);
    if (r==y) rc(318,196-y*3,254); else rc(318,196-y*3,r>y?253:0);
    }
}

extern void preVR(void);
extern void immVR(void);
extern void inVR(void);


void oznac_brf_ikonu(UC ikona) {
  UC f;
  if (!sejvnuto) ikona++;
  for (f=0;f<4;f++) {
    if (ikona==f) {
      if (f==1&&typdonemenu==1) v4write2(43-vlength(ikojm[3])/2,58+37*f,254,253,1,ikojm[3]);
      else {
        if (f==0) {
          if (sejvnuto) {
            v4write2(42+menu_x-vlength(ikojm[4])/2,54+37*f,254,253,1,ikojm[4]);
            v4write2(42+menu_x-vlength(ikojm[5])/2,62+37*f,254,253,1,ikojm[5]);
          }
        } else {
          v4write2(37+menu_x,54+37*f,254,253,1,"Do");
          v4write2(42+menu_x-vlength(ikojm[f-1])/2,62+37*f,254,253,1,ikojm[f-1]);
        }
      }
    } else {
      if (f==1&&typdonemenu==1) v4write2(43-vlength(ikojm[3])/2,58+37*f,249,251,1,ikojm[3]);
      else {
        if (f==0) {
	  if (sejvnuto) {
            v4write2(42+menu_x-vlength(ikojm[4])/2,54+37*f,249,251,1,ikojm[4]);
            v4write2(42+menu_x-vlength(ikojm[5])/2,62+37*f,249,251,1,ikojm[5]);
          }
        } else {
	  v4write2(37+menu_x,54+37*f,249,251,1,"Do");
          v4write2(42+menu_x-vlength(ikojm[f-1])/2,62+37*f,249,251,1,ikojm[f-1]);
        }
      }
    }
  }
}


void done_level(){
  UC kl,kl2,f,g;
  FILE *s;
  UC *p,*p2;
  UC my=0;

  hral_jsi_uz=1;
  graphmode=0;
  menu_x=0;
  if (vyskoc==2) {
    scrinit(0,0);
    sejvnuto=0;
  }
  nastav_paletu0();
  if (PLAYER&&vyskoc==2) {
    FILE *s;
    long s3mpos;
    int i;
    UC hudus2=hudusek;
    s = fopen("krkal.lev","rb");
    fseek(s,levly[level].odkaz+1,0);
    fread (&hudusek,1,1,s);
    fclose(s);
    if (hudusek!=hudus2) {
      for (i=(hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20;i>=0;i--)
	{midasSD->SetMasterVolume(i);vr();}
      s = fopen("krkal.mus","rb");
      fseek(s,14+hudusek*32,0);
      fread(&s3mpos,4,1,s);
      fclose(s);
      midasStopModule(mod);
      midasFreeModule(mod);
      mod = midasLoadModule("krkal.mus",s3mpos , &mpS3M, NULL);
      midasPlayModule(mod,4);
      midasSD->SetMasterVolume((hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20);
      midasSD->SetAmplification((hraci[hrac].hlasitost>>4)*20>63?(hraci[hrac].hlasitost>>4)*20:64);
    }
  }
  ssc(0,0,640);
  scr_zm_mnu();
  s = fopen("menu.dat","r+b");
  fseek(s,112951,0);
  fread(buffer,1,64000,s);
  p2=buffer;
  for (f=0;f<4;f++) {
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
  if (vyskoc==0&&level<95) {
    typdonemenu = 1;
    sejvnuto=0;
    p2=buffer;
    for (f=0;f<4;f++) {
      asm {
	mov dx,0x3c4
	mov al,2
	mov ah,1
	mov cl,f
	shl ah,cl
	out dx,ax
      }
      p = (UC*)videoram+4240;
      for(g=0;g<200;g++) {
	memcpy(p,p2,80);
	p2+=80;
	p+=160;
      }
    }
  } else if (vyskoc==0) typdonemenu = 2; else typdonemenu = 0;
  fread(buffer,1,39262,s);
  fclose(s);
  for (f=0;f<4;f++) {
    if (f==1&&typdonemenu==1) v4write2(43-vlength(ikojm[3])/2,58+37*f,5,15,1,ikojm[3]);
    else {
      if (f==0) {
	if (sejvnuto) {
	  sputimage(18,47+f*37,50,27,buffer+37912);
	  v4write2(42+menu_x-vlength(ikojm[4])/2,54+37*f,5,15,1,ikojm[4]);
	  v4write2(42+menu_x-vlength(ikojm[5])/2,62+37*f,5,15,1,ikojm[5]);
	}
      } else {
	v4write2(37+menu_x,54+37*f,5,15,1,"Do");
	v4write2(42+menu_x-vlength(ikojm[f-1])/2,62+37*f,5,15,1,ikojm[f-1]);
      }
    }
  }
  oznac_brf_ikonu(my);
  if (vyskoc==0) {
    UC a=udelano[level];
    UC aa=random(3);
    udelano[level] = 1;
    v4write2(183-vlength(hlasky[3+aa])/2,56,249,250,1,hlasky[3+aa]);
    if (level<95) {
      for (f=0;f<3;f++) {
	v4write2(358,91+37*f,5,15,1,"Do");
	v4write2(363-vlength(ikojm[f])/2,99+37*f,5,15,1,ikojm[f]);
	v4write2(357+menu_x,91+37*f,249,251,1,"Do");
	v4write2(362+menu_x-vlength(ikojm[f])/2,99+37*f,249,251,1,ikojm[f]);
      }
      aa=random(3);
      v4write2(503-vlength(hlasky[6+aa])/2,56,249,250,1,hlasky[6+aa]);
      s = fopen("krkal.lev","rb");
      fseek(s,levly[level+1].odkaz+2,0);
      fread(&topv,1,1,s);
      fread(&topp,1,1,s);
      fread(&topgp,1,1,s);
      fread(&tex1_len,2,1,s);
      fseek(s,levly[level+1].odkaz+1419+topv*6+topp*22,0);
      fread(buffer+40000,tex1_len,1,s);
      fclose(s);
      writebig(480-biglen(levly[level+1].jmeno)/2,15,levly[level+1].jmeno);
      buffer[40000+tex1_len]=0;
      menu_x=320;
      tex_vr(85,82,249,251,buffer+40000,0);
      menu_x=0;
      if (a!=1) {
	f = level+1;
	while ((udelano[f])&&f%32!=0) f++;
	if (f%32!=0) udelano[f]=2;
      }
    }
    if(!zalohuj_sejv) zalohuj_sejv=1;
    sejvni();
    vyskoc=1;
  } else {
    UC aa=random(3);
    if (vyskoc==1) {
    v4write2(183-vlength(hlasky[aa])/2,56,249,250,1,hlasky[aa]);
    }else {
      v4write2(183-vlength(hlasky[6+aa])/2,56,249,250,1,hlasky[6+aa]);
      vyskoc=1;
    }
  }
  writebig(160-biglen(levly[level].jmeno)/2,15,levly[level].jmeno);
  s = fopen("krkal.lev","rb");
  fseek(s,levly[level].odkaz+2,0);
  fread(&topv,1,1,s);
  fread(&topp,1,1,s);
  fread(&topgp,1,1,s);
  fread(&tex1_len,2,1,s);
  if (typdonemenu) {
    fread(&tex2_len,2,1,s);
    fseek(s,levly[level].odkaz+1419+topv*6+topp*22+tex1_len,0);
    fread(buffer+40000,tex2_len,1,s);
    buffer[40000+tex2_len]=0;
  } else {
    fseek(s,levly[level].odkaz+1419+topv*6+topp*22,0);
    fread(buffer+40000,tex1_len,1,s);
    buffer[40000+tex1_len]=0;
  }
  fclose(s);
  tex_vr(85,82,249,251,buffer+40000,0);
  memcpy(pal,pal2,3*256);
  SetPalette(pal2);
  mreset();
  asm {
    mov ax,4
    mov cx,84
    mov dx,115
    int 0x33
  }


  do {
    while(kbhit()) getch();
    for (f=0;f<10;f++) mysuj();
  } while(kbhit());
  kl=255;
  while (kl==255||kl==72||kl==80) {
    mysuj();
    kl2 = 1;
    if (kbhit()) {
      kl = toupper(getch());
      if (kl==0) kl=getch(),kl2=0;
      if (kl==27||kl=='D') {
	dialogove_okno(4);
	kl=255;
      } else if(kl==72&&my>0) {
        my--;
	oznac_brf_ikonu(my);
      } else if(kl==80&&(my<2||(sejvnuto&&my<3))) {
	my++;
	oznac_brf_ikonu(my);
      } else if (kl==13) {
	if (sejvnuto) {
	  if (my==1) sejvnuto=0;
	  my--;
	}
	if (my==1) kl='M';
	if (my==2) {
	  dialogove_okno(4);
	  kl=255;
	}
	if (sejvnuto) my++;
      }
    }
    if (mystl>0) {
      if (mysx>18&&mysx<67) {
	if (sejvnuto&&mysy>47&&mysy<73) kl=1;else
	if (mysy>84&&mysy<110) {
	  kl=1;
	  if (sejvnuto) sejvnuto=0;
	} else if (mysy>121&&mysy<147) kl='M';else
	if (mysy>158&&mysy<184) {
	  dialogove_okno(4);
	  kl=255;
	}
      }
    }
    if (kl!=255&&kl!=72&&kl!=80&&(kl!='M'||kl2==0)&&typdonemenu==1) {
      level++;
      if (PLAYER)
      {
	FILE *s;
	long s3mpos;
	int i,j;
	UC hudus2=hudusek;
	s = fopen("krkal.lev","rb");
	fseek(s,levly[level].odkaz+1,0);
	fread (&hudusek,1,1,s);
	fclose(s);
	if (hudusek!=hudus2) {
	  j=i=(hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20;
	  for (f=0;f<40;f++)
	  {
	    if (i>=0) midasSD->SetMasterVolume(i);
	    ssc(320,0,8);
	    if ((j<=20||j==63)&&(f&1)) i--;
	    if (j>=40) i--;
	  }
  //        for (f=f;f<40;f++) ssc(320,0,8);
	  s = fopen("krkal.mus","rb");
	  fseek(s,14+hudusek*32,0);
	  fread(&s3mpos,4,1,s);
	  fclose(s);
	  midasStopModule(mod);
	  midasFreeModule(mod);
	  mod = midasLoadModule("krkal.mus",s3mpos , &mpS3M, NULL);
	  midasPlayModule(mod,4);
	  midasSD->SetMasterVolume((hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20);
	  midasSD->SetAmplification((hraci[hrac].hlasitost>>4)*20>63?(hraci[hrac].hlasitost>>4)*20:64);
	} else for (f=0;f<40;f++) ssc(320,0,8);
      } else for (f=0;f<40;f++) ssc(320,0,8);
      typdonemenu = 0;
      menu_x=320;
      my=0;
      kl=255;
      oznac_brf_ikonu(my);
    }
  }
  if (kl=='M'&&kl2) {vyskoc=0;uninitg();} else scr_zb_mnu();
  nastav_paletu0();
  graphmode=1;
}


// *******************************************************************
// *                                                                 *
// *                   V L A  S T N I   H R A                        *
// *                                                                 *
// *******************************************************************


void pouzij_vec(UC k) {
  UC vec;
  if (k==6) vec=zvol_vec; else vec=k-6;
  if (((pole[1][ty.x][ty.y].vec==0||vec==5)&&ty.i[vec])||vec==6) {
    if (vec==3) postav_v(ty.x,ty.y,66);
    else if (vec==4) postav_v(ty.x,ty.y,68);
    else if (vec==5) {
      if (ty.vec<157||ty.vec>160){
	UC x=ty.x,y=ty.y,x2=ty.x,y2=ty.y,s;
	if (klf[0]==1) {x++;x2=x+1;s=0;} else
	if (klf[2]==1) {x--;x2=x-1;s=2;} else
	if (klf[3]==1) {y++;y2=y+1;s=3;} else
	if (klf[1]==1) {y--;y2=y-1;s=1;} else return;
	if ((STOP_LASER(pole[1][x][y].vec)||pole[2][x][y].vec!=0)&&(TY(pole[2][x][y].vec)==0||(pole[2][x2][y2].vec!=0||STOP_LASER(pole[1][x2][y2].vec)))) return;
	klf[s]=2;
	if(TY(pole[2][x][y].vec)) postav_v(x2,y2,85); else postav_v(x,y,85);
	veci[postvec].a1 = s;
	if (ty.vec>=153&&ty.vec<=156) veci[postvec].a3 = 1;
	zhasni_mnu(vec,ty.i[vec]);
	ty.i[vec]--;
	return;
      } else {
	ty.miz=40;
	zm_p(2,ty.x,ty.y,0);
	HAZEJ_OFF(ty.x,ty.y);
	PlayEffect(ef_teleport,8363,64,0);
	if (TY(pole[2][ty.x+1][ty.y].vec)) {ty.x++;zm_p(2,ty.x,ty.y,0);HAZEJ_OFF(ty.x,ty.y);}else
	if (TY(pole[2][ty.x-1][ty.y].vec)) {ty.x--;zm_p(2,ty.x,ty.y,0);HAZEJ_OFF(ty.x,ty.y);}else
	if (TY(pole[2][ty.x][ty.y+1].vec)) {ty.y++;zm_p(2,ty.x,ty.y,0);HAZEJ_OFF(ty.x,ty.y);}else
	if (TY(pole[2][ty.x][ty.y-1].vec)) {ty.y--;zm_p(2,ty.x,ty.y,0);HAZEJ_OFF(ty.x,ty.y);}
	ty.ax=nasobkyx[ty.x];
	ty.ay=nasobkyy[ty.y];
	ty.dx=0;
	ty.dy=0;
	ii=0;
	pohyb=0;
	sm=0;
      }
    }
    else if (vec==6) {
      UC  f=0;
      if (pole[1][ty.x][ty.y].vec!=0&&sebratelna_vec2(ty.x,ty.y)!=6) return;
      do {
        if (klf[f]==1) {
          klf[f]=2;
	  if (pole[1][ty.x][ty.y].vec==0) {
            if (ty.i[vec]==0) return;
            zhasni_mnu(vec,ty.i[vec]);
            ty.i[vec]--;
          }
          zm_p(1,ty.x,ty.y,69+f);
          PlayEffect(ef_get,8363,64,0);
          HAZEJ_1(ty.x,ty.y);
          f=5;
        }
        f++;
      } while (f<4);
      return;
    } else if (vec==7) zm_p(1,ty.x,ty.y,77);
    else if (vec==8) zm_p(1,ty.x,ty.y,78);
    else if (vec==9) zm_p(1,ty.x,ty.y,79);
    if (ty.miz==0) HAZEJ_1(ty.x,ty.y);
    klf[k] = 2;
    zhasni_mnu(vec,ty.i[vec]);
    ty.i[vec]--;
  }
}

void pasy(UC v,UC m)
{
UC x=veci[v].x;
UC y=veci[v].y;
if (veci[v].a3==m) return;
veci[v].a3=m;
if (m) {
  veci[v].frm=4;
  veci[v].obr+=97;
} else {
  veci[v].frm=0;
  veci[v].obr-=97;
}
HAZEJ_1(x,y);
if (PADA(pole[1][x][y].vec)||pole[1][x][y].vec==66||pole[1][x][y].vec==68) uvolni_pad3(x,y);
if (veci[v].vec==2&&PAS(pole[0][x+1][y].vec)) pasy(pole[0][x+1][y].v,m);
if (veci[v].vec==4&&PAS(pole[0][x-1][y].vec)) pasy(pole[0][x-1][y].v,m);
if (veci[v].vec==5&&PAS(pole[0][x][y+1].vec)) pasy(pole[0][x][y+1].v,m);
if (veci[v].vec==3&&PAS(pole[0][x][y-1].vec)) pasy(pole[0][x][y-1].v,m);
}

void prepni2(vv,zvp) {
  UC f,p,vec,x,y;
  for (f=0;f<6;f++){
    x = prep[vv].a[f].x;
    y = prep[vv].a[f].y;
    vec = prep[vv].a[f].vec;
    if (x==255); else
    if (x==128){
      if (PAS(veci[vec].vec)){
	if (zvp==2) pasy(vec,veci[vec].a3==0?1:0); else if (veci[vec].a3!=zvp) pasy(vec,zvp);
      }else{
	if (zvp==2) {
	  if (veci[vec].a3==1) veci[vec].a3=0; else veci[vec].a3=1;
	  if (LASER(veci[vec].vec))init_magnet(veci[vec].x,veci[vec].y,vec);
	} else if (veci[vec].a3!=zvp) {
	  veci[vec].a3=zvp;
	  if (LASER(veci[vec].vec))init_magnet(veci[vec].x,veci[vec].y,vec);
	}
      }
    } else if (x==129){
      if (zvp==2) {
	if ((veci[vec].a1&0xf)<4) veci[vec].a1+=4; else veci[vec].a1-=4;
      } else if ((veci[vec].a1&0xf)>3&&zvp==0) {
	veci[vec].a1-=4;
      } else if ((veci[vec].a1&0xf)<4&&zvp==1) {
	veci[vec].a1+=4;
      }
    } else {
      p = patro[vec];
      if (vec==251) p=1;
      if (vec==252) p=2;
      if (vec==251||vec==252) {
	if (pole[p][x][y].vec!=0&&((!TY(pole[2][x][y].vec)&&!POTVORY(pole[2][x][y].vec))||p!=2)){
	  if (zvp==2) prep[vv].a[f].vec=pole[p][x][y].vec;
	  if (pole[p][x][y].v != 255) {
	    if (PREPINAC(pole[p][x][y].vec)) pole[p][x][y].v = 255;else
	    if (PADA(pole[p][x][y].vec)) pad_kaput(pole[p][x][y].v);else vec_kaput(pole[p][x][y].v);
	  }
	  zm_p(p,x,y,0);
	  HAZEJ_1(x,y);
	}
      }else
      {
	if (pole[p][x][y].vec!=vec){
	  if (p==2&&TY(pole[2][x][y].vec)) ty_kaput();
	  if (zvp==2) {
	    UC vc=pole[p][x][y].vec;
	    if ((vc>=2&&vc<=5)||(vc>=52&&vc<=55)) vc = vec;
	    if ((vc==0&&p!=0)||vc==117||vc==118||vc==122||vc==123||(vc>=125&&vc<=136)||(vc>=145&&vc<=148)||vc==149||vc>153)
	    prep[vv].a[f].vec=250+p;else prep[vv].a[f].vec=vc;
	  }
	  if (pole[p][x][y].v != 255) {
	    if (PREPINAC(pole[p][x][y].vec)) pole[p][x][y].v = 255;else
	    if (PADA(pole[p][x][y].vec)) {
	      pad_kaput(pole[p][x][y].v);
	      prep[vv].a[f].vec=250+p;
	    } else {
	      if ((pole[p][x][y].vec==68||pole[p][x][y].vec==66)&&veci[pole[p][x][y].v].a2!=255) prep[vv].a[f].vec=250+p;
	      vec_kaput(pole[p][x][y].v);
            }
          }
	  postav_v(x,y,vec);
	  HAZEJ_1(x,y);
	}
      }
    }
  }
}

void prepni(UC x, UC y, UC *s)
{
    UC f;
    UC xxt=x,yyt=y;
    if (*s==1) xxt+=1;
    if (*s==3) xxt-=1;
    if (*s==4) yyt+=1;
    if (*s==2) yyt-=1;

    if (pole[2][xxt][yyt].vec==118||pole[2][xxt][yyt].vec==117)
    {
      if (*s>=1&&*s<=4&&(klf[*s-1]==2||klf[*s-1]==0)) {*s=0;return;}
      if (*s>=1&&*s<=4) klf[*s-1]=2;
      if (*s!=5) *s=0;
      if (prep[pole[2][xxt][yyt].v].zvp==2&&pole[2][xxt][yyt].vec==118) return;
      if (prep[pole[2][xxt][yyt].v].zvp==1&&pole[2][xxt][yyt].vec==117) return;
      if (pole[2][xxt][yyt].vec==118) zm_p(2,xxt,yyt,117); else zm_p(2,xxt,yyt,118);
      HAZEJ_1(xxt,yyt);
      prepni2(pole[2][xxt][yyt].v,2);
    }
}

void fotobunkuj(UC v){
  UC f,g,i,zm=0;

  if (prep[v].zvp==1){
    for (f=prep[v].x-1;f<=prep[v].x+1;f++)
    for (g=prep[v].y-1;g<=prep[v].y+1;g++)
    if ((pole[2][f][g].vec>=153||vybuch[f][g])&&f<32&&g<22) zm=1;
    if (zm==1) prepni2(v,1);
  } else
  if (prep[v].zvp==2){
    for (f=prep[v].x-1;f<=prep[v].x+1;f++)
    for (g=prep[v].y-1;g<=prep[v].y+1;g++)
    if ((pole[2][f][g].vec>=153||vybuch[f][g])&&f<32&&g<22) zm=1;
    if (zm==1) prepni2(v,0);
  } else
  if (prep[v].zvp>2){
    zm = 3;
    for (f=prep[v].x-1;f<=prep[v].x+1;f++)
    for (g=prep[v].y-1;g<=prep[v].y+1;g++)
    if ((pole[2][f][g].vec>=153||vybuch[f][g])&&f<32&&g<22)
    {
      zm=4;
      break;
    }
    if (zm!=prep[v].zvp){
      prep[v].zvp=zm;
      prepni2(v,2);
    }
  }
}

void detektoruj(UC v) {
  UC x,y,zm,xx;
  x = prep[v].x;
  y = prep[v].y;
  xx = ((x>0&&if_laser2(laser[x-1][y][0].v))||(y<21&&if_laser2(laser[x][y+1][1].v))
  ||(x<31&&if_laser2(laser[x+1][y][2].v))||(y>0&&if_laser2(laser[x][y-1][3].v)));
  if (prep[v].zvp==1){
    if (xx) prepni2(v,1);
  } else
  if (prep[v].zvp==2){
    if (!xx) prepni2(v,0);
  } else

  if (prep[v].zvp>2){
    zm = 3;
    if (xx) zm=4;
    if (zm!=prep[v].zvp){
      prep[v].zvp=zm;
      prepni2(v,2);
    }
  }
}
void stop_vybuch(UC p,UC x,UC y) {
  UC v,vec,f,g;
  for (f=x-1;f<=x+1;f++) for (g=y-1;g<=y+1;g++) {
    vec = pole[p][f][g].vec;
    if (!NEVYBUCHUJE(vec)) {
      v = pole[p][f][g].v;
      if (v!=255) {
	if (PADA(vec)) {
	  pad_kaput(v);
	  zm_p(p,pady[v].x,pady[v].y,0);
	  HAZEJ_1(pady[v].x,pady[v].y);
	} else if (!PREPINAC(vec)) {
	  vec_kaput(v);
	  if (POTVORY(vec)||vec==66||vec==68) postav_v(f,g,65);
	}
      } else if (TY(vec)) {
	ty_kaput();
	postav_v(f,g,65);
      }
    }
  }
}


void vybouchni(UC v, UC x, UC y, UC p) {
  UC f,g,vec;
  for (f=x-1;f<=x+1;f++) for (g=y-1;g<=y+1;g++) {
    if (!NEVYBUCHUJE(pole[2][f][g].vec)) {
      vybuch[f][g]++;
      if (vybuch[f][g]==1) HAZEJ_1(f,g);
    }
  }
  if (v!=255&&PADA(pole[p][x][y].vec)==0) vec_kaput(v);
  else {
    if (v!=255) {
      pad_kaput(v);
      zm_p(p,pady[v].x,pady[v].y,0);
    } else {
    if (TY(pole[p][x][y].vec)) ty_kaput(); else zm_p(p,x,y,0);
    }
  }
  stop_vybuch(1,x,y);
  stop_vybuch(2,x,y);
  PlayEffect(ef_vybuch,8363,64*(32-abs(ty.x-x))*(22-abs(ty.y-y))/704,0);
  postav_v(x,y,82);
}


void bombuj(UC v) {
  veci[v].a3++;
  if (veci[v].a3==40) vybouchni(v,veci[v].x,veci[v].y,1);
}
void minuj(UC v) {
  UC f,g;
  if (veci[v].a3!=40) veci[v].a3++;
  if (veci[v].a3==40) {
    for (f=veci[v].x-1;f<=veci[v].x+1;f++) for (g=veci[v].y-1;g<=veci[v].y+1;g++)
    if (vybuch[f][g]||pole[2][f][g].vec > 152) veci[v].a3=41;
  }
  if (veci[v].a3==65) vybouchni(v,veci[v].x,veci[v].y,1);
}

void vybuchuj(UC v) {
  UC f,g,vec,ff;
  UC x,y,tmp;
  veci[v].a3++;
  if (veci[v].a3==11) {
    x=veci[v].x;
    y=veci[v].y;
    for (f=x-1;f<=x+1;f++) for (g=y-1;g<=y+1;g++) {
      if (pole[0][f][g].vec==7) pole[0][f][g].vec=VODA(f,g); else
      if (pole[0][f][g].vec==46||pole[0][f][g].vec==47)	pole[0][f][g].vec+=2;
      vec=pole[1][f][g].vec;
      if (PADA(vec)&&pole[1][f][g].v!=255) pad_kaput(pole[1][f][g].v);
      if (SOUPATELNA_VEC(vec)) pole[1][f][g].vec=0; else
      if (vec>=60&&vec<=64) pole[1][f][g].vec=0; else
      if (vec>=69&&vec<=79) pole[1][f][g].vec=0; else
      if (vec>=86&&vec<=88) pole[1][f][g].vec=0; else
      if (vec>=65&&vec<=67) vybouchni(pole[1][f][g].v,f,g,1);
      vec=pole[2][f][g].vec;
      if (vec>=107&&vec<=116) pole[2][f][g].vec=0; else
      if (LASER(vec)||DELO(vec)) vec_kaput(pole[2][f][g].v); else
      if (ZRCADLA(vec)) zm_p(2,f,g,0); else
      if (vec>=149&&vec<=152) vec_kaput(pole[2][f][g].v); else
      if (TY(vec)){
	vybouchni(pole[2][f][g].v,f,g,2);
      } else
      if (POTVORY(vec)) {
	if (pole[2][f][g].v==255) pole[2][f][g].vec=0; else vybouchni(pole[2][f][g].v,f,g,2);
      }
      if (vybuch[f][g]>0) {
	vybuch[f][g]--;
	zm_p(0,f,g,pole[0][f][g].vec);
	zm_p(1,f,g,pole[1][f][g].vec);
	zm_p(2,f,g,pole[2][f][g].vec);
	if (vybuch[f][g]==0) HAZEJ_1(f,g);
      }
    }
    vec_kaput(v);
  }

}


UC muzesh3(UC x,UC y,UC smer)
{
  UC p0=pole[0][x][y].vec,p1=pole[1][x][y].vec,p2=pole[2][x][y].vec;
  if (TY(pole[2][x][y].vec)) {
    if (ty.brneni>=6) {ty.brneni-=6;nakresli_pruh2();} else
    {vybouchni(255,x,y,2);return 0;}
  }
  if (!muzes_na_pasy(p0,x,y,smer)) return 0;
  if (DIRA(p0)||if_laspr(x,y)) return 0;
  if (p2!=0) return 0;
  if ((p1>=63&&p1<=72)||(p1>=77&&p1<=88)) return 1;
  if (p1!=0) return 0;
  return 1;
}

void xy_z_sm(UC s,int *dx,int *dy)
{
  if (s==0) {*dx=1;*dy=0;}else
  if (s==1) {*dx=0;*dy=-1;}else
  if (s==2) {*dx=-1;*dy=0;}else
  if (s==3) {*dx=0;*dy=1;}
}


UC muzesh4(UC x,UC y,UC smer)
{
  UC p1=pole[1][x][y].vec;
  int dx,dy;
  xy_z_sm(smer-1,&dx,&dy);
  if (pole[2][x-dx][y-dy].vec<177&&TY(pole[2][x][y].vec)){
    if (ty.brneni>=6) {ty.brneni-=6;nakresli_pruh2();} else
    {vybouchni(255,x,y,2);return 0;}
  }
  if (!muzes_na_pasy(pole[0][x][y].vec,x,y,smer)) return 0;
  if (pole[2][x][y].vec!=0) return 0;
  if (p1==78||p1==79) return 1;
  if (p1>=77&&p1<=79) return 0;
  if ((p1>=63&&p1<=72)||(p1>=77&&p1<=88)) return 1;
  if (p1!=0) return 0;
  return 1;
}

void naslapna_podlaha(UC x,UC y) {
  UC f,vv,xx;
  xx = 1;
  if (pole[0][x][y].vec<54) pole[0][x][y].vec+=2; else {
    if ((pole[1][x][y].vec!=0&&pole[1][x][y].vec!=59)||pole[2][x][y].vec!=0) return;
    xx = 0;
    pole[0][x][y].vec-=2;
  }
  HAZEJ_1(x,y);
  vv=pole[0][x][y].v;
  prep[vv].vec=pole[0][x][y].vec;
  if (prep[vv].zvp==1&&xx) prepni2(vv,xx); else
  if (prep[vv].zvp==2&&xx==0) prepni2(vv,xx); else
  if (prep[vv].zvp==3) prepni2(vv,2);
}

void pohni_p(UC v,UC smp,UC speed)
{
  char dx=0,dy=0,x=veci[v].x,y=veci[v].y;
  if (smp==1){dx=speed==0?2:4;x++;} else
  if (smp==3){dx=speed==0?-2:-4;x--;} else
  if (smp==4){dy=speed==0?pohyb_y[veci[v].a2]:pohyb_y2[veci[v].a2>>1];y++;} else
  if (smp==2){dy=speed==0?-pohyb_y[veci[v].a2]:-pohyb_y2[veci[v].a2>>1];y--;}
  if (veci[v].a2==0){
      pole[2][x][y].v=v;
      pole[2][veci[v].x][veci[v].y].vec=veci[v].vec;
      HAZEJ_ON(x,y);
      zm_p(2,x,y,veci[v].vec);
      if (!POTVORY(veci[v].vec)) return;
  }
  if (veci[v].a2==4) {
    if (pole[0][x][y].vec==52||pole[0][x][y].vec==53) {naslapna_podlaha(x,y);if (!POTVORY(veci[v].vec)) return;}
    if (pole[0][veci[v].x][veci[v].y].vec==54||pole[0][veci[v].x][veci[v].y].vec==55) {naslapna_podlaha(veci[v].x,veci[v].y);if (!POTVORY(veci[v].vec)) return;}
  }
  if (speed==0) veci[v].a2++; else veci[v].a2+=2;
  veci[v].dx+=dx;veci[v].dy+=dy;veci[v].ax+=dx;veci[v].ay+=dy;
  if (veci[v].a2==10){
    pole[2][veci[v].x][veci[v].y].v=255;
    HAZEJ_OFF(veci[v].x,veci[v].y);
    zm_p(2,veci[v].x,veci[v].y,0);
    if (!POTVORY(veci[v].vec)) return;
    veci[v].x=x;veci[v].y=y;
    veci[v].dx=0;veci[v].dy=0;veci[v].a2=0;
  }
}


void potvoruj(UC v)
{
  UC smp,x,y,st,i;
  int dx,dy;
  if (veci[v].vec>=165&&veci[v].vec<=180)
  {
    smp=(veci[v].vec-165)&3;
    x=veci[v].x;
    y=veci[v].y;
    if(veci[v].dx==0&&veci[v].dy==0)
    {
      if ((pole[0][x][y].vec==46||pole[0][x][y].vec==47)&&pole[0][x][y].v==255) postav_v(x,y,181);
      if (DIRA(pole[0][x][y].vec)) {vec_kaput(v);return;}
      veci[v].a1=0;veci[v].a2=0;
      if (pole[0][x][y].vec==7){
	if (veci[v].vec<=172){
	  if (smp==0&&muzesh3(x+1,y,1)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=1;veci[v].vec=veci[v].vec-smp+0;veci[v].obr=veci[v].vec;} else
	  if (smp==1&&muzesh3(x,y-1,2)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=2;veci[v].vec=veci[v].vec-smp+1;veci[v].obr=veci[v].vec;} else
	  if (smp==2&&muzesh3(x-1,y,3)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=3;veci[v].vec=veci[v].vec-smp+2;veci[v].obr=veci[v].vec;} else
	  if (smp==3&&muzesh3(x,y+1,4)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=4;veci[v].vec=veci[v].vec-smp+3;veci[v].obr=veci[v].vec;}
	}else{
	  if (smp==0&&muzesh4(x+1,y,1)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=1;veci[v].vec=veci[v].vec-smp+0;veci[v].obr=veci[v].vec;} else
	  if (smp==1&&muzesh4(x,y-1,2)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=2;veci[v].vec=veci[v].vec-smp+1;veci[v].obr=veci[v].vec;} else
	  if (smp==2&&muzesh4(x-1,y,3)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=3;veci[v].vec=veci[v].vec-smp+2;veci[v].obr=veci[v].vec;} else
	  if (smp==3&&muzesh4(x,y+1,4)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=4;veci[v].vec=veci[v].vec-smp+3;veci[v].obr=veci[v].vec;}
	}
	if (!POTVORY(veci[v].vec)) return;
      }
      if (veci[v].vec<=168&&veci[v].a3==0&&veci[v].a1==0){
	st=smp-1;if (st>3) st=3;
	for (i=1;i<=4;i++)
	{
	  xy_z_sm(st,&dx,&dy);
	  if (muzesh3(x+dx,y+dy,st+1)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=st+1;veci[v].vec=veci[v].vec-smp+st;smp=st;veci[v].obr=veci[v].vec;break;}
	  if (!POTVORY(veci[v].vec)) return;
	  st++;if (st>3) st=0;
	}
      } else
      if (veci[v].vec<=168&&veci[v].a3==1&&veci[v].a1==0){
	st=smp+1;if (st>3) st=0;
	for (i=1;i<=4;i++)
	{
	  xy_z_sm(st,&dx,&dy);
	  if (muzesh3(x+dx,y+dy,st+1)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=st+1;veci[v].vec=veci[v].vec-smp+st;smp=st;veci[v].obr=veci[v].vec;break;}
	  if (!POTVORY(veci[v].vec)) return;
	  st--;if (st>3) st=3;
	}
      } else
      if (veci[v].vec<=172&&veci[v].a1==0){
	UC m1,m2,m3,m4,mtmp,t1,t2;
	m1=ty.x>x;m2=ty.y<y;m3=ty.x<x;m4=ty.y>y;
	t1=abs(ty.x-x);t2=abs(ty.y-y);

	if (m1) m1&=muzesh3(x+1,y,1);
	if (m2) m2&=muzesh3(x,y-1,2);
	if (m3) m3&=muzesh3(x-1,y,3);
	if (m4) m4&=muzesh3(x,y+1,4);

	if (m1==0&&m2==0&&m3==0&&m4==0){
	m1=muzesh3(x+1,y,1);m2=muzesh3(x,y-1,2);
	m3=muzesh3(x-1,y,3);m4=muzesh3(x,y+1,4);
	}
	mtmp=random(t1+t2);
	if (m2&&m3) {
	  if (t1<=t2&&mtmp<=t1) m2=0;
	  if (t1<=t2&&mtmp>t1)  m3=0;
	  if (t1>t2&&mtmp<=t2) m3=0;
	  if (t1>t2&&mtmp>t2)  m2=0;
	}
	if (m2&&m1) {
	  if (t1<=t2&&mtmp<=t1) m2=0;
	  if (t1<=t2&&mtmp>t1)  m1=0;
	  if (t1>t2&&mtmp<=t2) m1=0;
	  if (t1>t2&&mtmp>t2)  m2=0;
	}
	if (m4&&m3) {
	  if (t1<=t2&&mtmp<=t1) m4=0;
	  if (t1<=t2&&mtmp>t1)  m3=0;
	  if (t1>t2&&mtmp<=t2) m3=0;
	  if (t1>t2&&mtmp>t2)  m4=0;
	}
	if (m4&&m1) {
	  if (t1<=t2&&mtmp<=t1) m4=0;
	  if (t1<=t2&&mtmp>t1)  m1=0;
	  if (t1>t2&&mtmp<=t2) m1=0;
	  if (t1>t2&&mtmp>t2)  m4=0;
	}
	if (!POTVORY(veci[v].vec)) return;
	if (m1) {veci[v].a1=1;veci[v].vec=169;veci[v].obr=veci[v].vec;} else
	if (m2) {veci[v].a1=2;veci[v].vec=170;veci[v].obr=veci[v].vec;} else
	if (m3) {veci[v].a1=3;veci[v].vec=171;veci[v].obr=veci[v].vec;} else
	if (m4) {veci[v].a1=4;veci[v].vec=172;veci[v].obr=veci[v].vec;}
      } else
      if (veci[v].vec<=180&&veci[v].a1==0){
	UC tm=0;
	if (pole[1][x][y].vec==78) veci[v].a3=0;else
	if (pole[1][x][y].vec==79) veci[v].a3=1;
	st=smp;
	for (i=1;i<=8;i++)
	{
	  tm=0;
	  if (!((st-smp)&1)) tm=1;

	  if (pole[1][x][y].vec>=69&&pole[1][x][y].vec<=72){
	    if (pole[1][x][y].vec==69+st) tm=1; else tm=0;
	  }
	  if (tm){
	  xy_z_sm(st,&dx,&dy);
	  if (muzesh4(x+dx,y+dy,st+1)) {if (!POTVORY(veci[v].vec)) return;veci[v].a1=st+1;veci[v].vec=veci[v].vec-smp+st;veci[v].obr=veci[v].vec;smp=st;break;}
	  if (!POTVORY(veci[v].vec)) return;
	  }
	  st++;
	  if (st>3)st=0;
	}

      }
    }
    if (veci[v].a1!=0) {
      if (zobraz1[veci[v].x][veci[v].y]&128) {zobraz1[veci[v].x][veci[v].y]&=127;HAZEJ_1(veci[v].x,veci[v].y);}
      if (veci[v].vec<173||veci[v].a3==0) pohni_p(v,veci[v].a1,0);else pohni_p(v,veci[v].a1,1);
    }else if (veci[v].vec>=177) vybouchni(v,veci[v].x,veci[v].y,2);else {
      if (!(zobraz1[veci[v].x][veci[v].y]&128)) zobraz1[veci[v].x][veci[v].y]|=129;
    }
  }
}

void otoc_zrcadlo()
{
    UC xxt=ty.x,yyt=ty.y,vec;
    if (sm<1||sm>4) return ;
    if (sm==1) xxt+=1;
    if (sm==3) xxt-=1;
    if (sm==4) yyt+=1;
    if (sm==2) yyt-=1;
    vec=pole[2][xxt][yyt].vec-137;
    if (vec>7) return;
    if (klf[sm-1]==2||klf[sm-1]==0) {sm=0;return;}
    klf[sm-1]=2;
    vec&=3;
    if (sm==1&&vec==0) {zm_p(2,xxt,yyt,140);HAZEJ_1(xxt,yyt);} else
    if (sm==1&&vec==3) {zm_p(2,xxt,yyt,137);HAZEJ_1(xxt,yyt);} else
    if (sm==3&&vec==1) {zm_p(2,xxt,yyt,139);HAZEJ_1(xxt,yyt);} else
    if (sm==3&&vec==2) {zm_p(2,xxt,yyt,138);HAZEJ_1(xxt,yyt);} else
    if (sm==4&&vec==2) {zm_p(2,xxt,yyt,140);HAZEJ_1(xxt,yyt);} else
    if (sm==4&&vec==3) {zm_p(2,xxt,yyt,139);HAZEJ_1(xxt,yyt);} else
    if (sm==2&&vec==0) {zm_p(2,xxt,yyt,138);HAZEJ_1(xxt,yyt);} else
    if (sm==2&&vec==1) {zm_p(2,xxt,yyt,137);HAZEJ_1(xxt,yyt);}
    sm=0;
}



void propadej_se(UC v) {
  UC v2,vec;
  veci[v].a3++;
  if (veci[v].a2==10) {
    if (veci[v].a3==32) {
      if (PADA(pole[1][veci[v].x][veci[v].y].vec)&&pole[1][veci[v].x][veci[v].y].v==255) zm_p(1,veci[v].x,veci[v].y,0); else
      if (pole[1][veci[v].x][veci[v].y].vec==66||pole[1][veci[v].x][veci[v].y].vec==68) vec_kaput(pole[1][veci[v].x][veci[v].y].v);
      vec_kaput(v);
    }
  } else if (veci[v].a3==5) {
    vec = pole[veci[v].a2][veci[v].x][veci[v].y].vec;
    v2 = pole[veci[v].a2][veci[v].x][veci[v].y].v;
    if (vec>=65&&vec<=68) vybouchni(v2,veci[v].x,veci[v].y,veci[v].a2); else
    if (v2!=255) {
      if (PADA(vec)) {
	pad_kaput(v2);
	zm_p(1,pady[v2].x,pady[v2].y,0);
	HAZEJ_1(pady[v2].x,pady[v2].y);
      } else vec_kaput(v2);
    }else zm_p(veci[v].a2,veci[v].x,veci[v].y,0);
    HAZEJ_1(veci[v].x,veci[v].y);
    vec_kaput(v);
  }
}

void vybuch_hl(UC x,UC y) {
  UC f,g;
  for (f=x-1;f<=x+1;f++) for (g=y-1;g<=y+1;g++) if (pole[1][f][g].vec==0&&pole[2][f][g].vec==0) {
    zm_p(2,f,g,114);
    HAZEJ_1(f,g);
  }
}

void streluj(UC v) {
  UC sm=veci[v].a1;
  HAZEJ_1(veci[v].x,veci[v].y);
  if (veci[v].a2||(veci[v].dx==0&&veci[v].dy==0)) {
    if ((veci[v].dx==0&&veci[v].dy==0)) {
      sm=zjisti_zastaveni(veci[v].x,veci[v].y,sm,129);
    }
    if (sm==0) veci[v].a2=zjisti_zastaveni(veci[v].x+1,veci[v].y,sm,129); else
    if (sm==1) veci[v].a2=zjisti_zastaveni(veci[v].x,veci[v].y-1,sm,129); else
    if (sm==2) veci[v].a2=zjisti_zastaveni(veci[v].x-1,veci[v].y,sm,129); else
    if (sm==3) veci[v].a2=zjisti_zastaveni(veci[v].x,veci[v].y+1,sm,129);
    veci[v].a1=sm;
    if (veci[v].a2<=3) veci[v].a2=0;
  }
  if (sm==0) {
    if (veci[v].dx<=0) veci[v].dx+=7; else {
      if (veci[v].a2) {
	if (veci[v].a3==0) znic_to_l(veci[v].x+1,veci[v].y,1); else vybuch_hl(veci[v].x,veci[v].y);
	vec_kaput(v);
	return;
      } else
      veci[v].dx=-7;
      veci[v].x++;
    }
  } else
  if (sm==1) {
    if (veci[v].dy>=0) veci[v].dy-=6; else {
      if (veci[v].a2) {
	if (veci[v].a3==0) znic_to_l(veci[v].x,veci[v].y-1,1); else vybuch_hl(veci[v].x,veci[v].y);
	vec_kaput(v);
	return;
      } else
      veci[v].dy=6;
      veci[v].y--;
    }
  } else
  if (sm==2) {
    if (veci[v].dx>=0) veci[v].dx-=7; else {
      if (veci[v].a2) {
	if (veci[v].a3==0) znic_to_l(veci[v].x-1,veci[v].y,1); else vybuch_hl(veci[v].x,veci[v].y);
	vec_kaput(v);
	return;
      } else
      veci[v].dx=7;
      veci[v].x--;
    }
  } else
  if (sm==3) {
    if (veci[v].dy<=0) veci[v].dy+=6; else {
      if (veci[v].a2) {
	if (veci[v].a3==0) znic_to_l(veci[v].x,veci[v].y+1,1); else vybuch_hl(veci[v].x,veci[v].y);
	vec_kaput(v);
	return;
      } else
      veci[v].dy=-6;
      veci[v].y++;
    }
  } else {
    if (veci[v].a3==0) znic_to_l(veci[v].x,veci[v].y,1); else vybuch_hl(veci[v].x,veci[v].y);
    vec_kaput(v);
    return;
  }
  veci[v].ax=nasobkyx[veci[v].x]+veci[v].dx;
  veci[v].ay=nasobkyy[veci[v].y]+veci[v].dy;
  hazej(veci[v].ax,veci[v].ay,85);
}

void paduj(UC v) {
  UC v2=255;
  UC sm = pady[v].smer;
  UC x1=pady[v].x,y1=pady[v].y;
  UC x2=pady[v].x,y2=pady[v].y,x3=255,y3=255;
  signed char dx=0,dy=0;
  if (pady[v].vec==66||pady[v].vec==68) v2 = pole[1][pady[v].x][pady[v].y].v;
  if (sm>11) {
    pady[v].smer-=12;
    if (v2!=255) muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v2);
    else muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v);
    return;
  }
  if (sm==0) {
    x2=x1+1;
    dx=2;
  } else if (sm==1) {
    y2=y1-1;
    dy=-pohyb_y[pady[v].ii];
  } else if (sm==2) {
    x2=x1-1;
    dx=-2;
  } else if (sm==3) {
    y2=y1+1;
    dy=pohyb_y[pady[v].ii];
  } else if (sm==4) {
    y2=y1-1;
    x3=x1+1;
    y3=y2;
    if (pady[v].ii<4) dy=-3; else if (pady[v].ii==4) {dx=5;dy=-5;} else dx=3;
  } else if (sm==5) {
    y2=y1+1;
    x3=x1+1;
    y3=y2;
    if (pady[v].ii<4) dy=3; else if (pady[v].ii==4) {dx=5;dy=5;} else dx=3;
  } else if (sm==6) {
    x2=x1-1;
    x3=x2;
    y3=y1-1;
    if (pady[v].ii<5) dx=-3; else if (pady[v].ii==5) {dx=-5;dy=-5;} else dy=-3;
  } else if (sm==7) {
    x2=x1+1;
    x3=x2;
    y3=y1-1;
    if (pady[v].ii<5) dx=3; else if (pady[v].ii==5) {dx=5;dy=-5;} else dy=-3;
  } else if (sm==8) {
    y2=y1+1;
    y3=y2;
    x3=x1-1;
    if (pady[v].ii<4) dy=3; else if (pady[v].ii==4) {dx=-5;dy=5;} else dx=-3;
  } else if (sm==9) {
    y2=y1-1;
    y3=y2;
    x3=x1-1;
    if (pady[v].ii<4) dy=-3; else if (pady[v].ii==4) {dx=-5;dy=-5;} else dx=-3;
  } else if (sm==10) {
    x2=x1+1;
    x3=x2;
    y3=y1+1;
    if (pady[v].ii<5) dx=3; else if (pady[v].ii==5) {dx=5;dy=5;} else dy=3;
  } else if (sm==11) {
    x2=x1-1;
    x3=x2;
    y3=y1+1;
    if (pady[v].ii<5) dx=-3; else if (pady[v].ii==5) {dx=-5;dy=5;} else dy=3;
  }
  pady[v].ii++;
  pady[v].dx+=dx;
  pady[v].dy+=dy;
  pady[v].ax=nasobkyx[pady[v].x]+pady[v].dx;
  pady[v].ay=nasobkyy[pady[v].y]+pady[v].dy;
  if (v2!=255) {
    veci[v2].dx=pady[v].dx;
    veci[v2].dy=pady[v].dy;
    veci[v2].ax=pady[v].ax;
    veci[v2].ay=pady[v].ay;
  }
  if (pady[v].vec!=59&&(pady[v].vec<69||pady[v].vec>79)) if (pady[v].ii==5&&x3==255) {
    if (pole[0][x1][y1].vec==54||pole[0][x1][y1].vec==55) {naslapna_podlaha(x1,y1);if (pady[v].vec==255) return;}
    if (pole[0][x2][y2].vec==52||pole[0][x2][y2].vec==53) {naslapna_podlaha(x2,y2);if (pady[v].vec==255) return;}
  } else if (x3!=255) {
    if (pady[v].ii==3) {
      if (pole[0][x1][y1].vec==54||pole[0][x1][y1].vec==55) {naslapna_podlaha(x1,y1);if (pady[v].vec==255) return;}
      if (pole[0][x2][y2].vec==52||pole[0][x2][y2].vec==53) {naslapna_podlaha(x2,y2);if (pady[v].vec==255) return;}
    } else if (pady[v].ii==7) {
      if (pole[0][x2][y2].vec==54||pole[0][x2][y2].vec==55) {naslapna_podlaha(x2,y2);if (pady[v].vec==255) return;}
      if (pole[0][x3][y3].vec==52||pole[0][x3][y3].vec==53) {naslapna_podlaha(x3,y3);if (pady[v].vec==255) return;}
    }
  }
  if (pady[v].ii==10) {
    pady[v].dx = 0;
    pady[v].dy = 0;
    if (v2!=255) {
      veci[v2].dx=0;
      veci[v2].dy=0;
    }
    pady[v].ii = 0;
    if (pady[v].smer>3) {
      pady[v].smer=(pady[v].smer-4)/2;
      pole[1][x2][y2].v=255;
      HAZEJ_OFF(x2,y2);
      zm_p(1,x2,y2,0);
      pady[v].x = x3;
      pady[v].y = y3;
      if (v2!=255) {
	veci[v2].x = x3;
	veci[v2].y = y3;
      }
    } else {
      pady[v].x = x2;
      pady[v].y = y2;
      if (v2!=255) {
	veci[v2].x = x2;
	veci[v2].y = y2;
      }
    }
    pole[1][x1][y1].v=255;
    HAZEJ_OFF(x1,y1);
    zm_p(1,x1,y1,0);
    if (pady[v].vec!=59) {
      if ((pole[0][pady[v].x][pady[v].y].vec==46||pole[0][pady[v].x][pady[v].y].vec==47)&&pole[0][pady[v].x][pady[v].y].v==255) postav_v(pady[v].x,pady[v].y,181);
      if (DIRA(pole[0][pady[v].x][pady[v].y].vec)) {
	if (v2!=255) vec_kaput(v2); else {
	  pad_kaput(v);
	  zm_p(1,pady[v].x,pady[v].y,0);
	  HAZEJ_1(pady[v].x,pady[v].y);
	}
      } else {
	if (v2!=255) muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v2);
	else muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v);
      }
    } else {
      if (v2!=255) muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v2);
      else muzes_p(pady[v].x,pady[v].y,pady[v].smer,pady[v].vec,v);
    }
  }
}

UC muzes_m(UC x,UC y) {
  if(pole[1][x][y].vec==0&&pole[2][x][y].vec==0) return 1;
  return 0;
}

void muvovuj(UC vv) {
  UC aa;
  UC ii=((veci[vv].a1&0xf0)>>4);
  UC smer=veci[vv].a1&0xf;
  UC x,y;
  char dx,dy;
  if(!ii&&smer>3) return;
  if (smer>3) smer-=4;
  dy=dx=0;
  x=veci[vv].x;
  y=veci[vv].y;
  if (smer==0) {dx=2;x++;} else
  if (smer==1) {dy=-pohyb_y[ii];y--;} else
  if (smer==2) {dx=-2;x--;} else
  if (smer==3) {dy=pohyb_y[ii];y++;}
  if (ii==0) {
    if (!muzes_m(x,y)||((smer&1)&&veci[vv].y==veci[vv].a3)||(!(smer&1)&&veci[vv].x==veci[vv].a3)) {
      if (muzes_m(veci[vv].x+veci[vv].x-x,veci[vv].y+veci[vv].y-y)) {
	if(smer<2) smer+=2; else smer-=2;
	veci[vv].a1 = (veci[vv].a1&0xf0)+smer;
	aa=veci[vv].a2;veci[vv].a2=veci[vv].a3;veci[vv].a3=aa;
	if (smer==0) {dx=2;x+=2;} else
	if (smer==1) {dy=-pohyb_y[ii];y-=2;} else
	if (smer==2) {dx=-2;x-=2;} else
	if (smer==3) {dy=pohyb_y[ii];y+=2;}
      } else return;
    }
    pole[2][x][y].v=vv;
    HAZEJ_ON(x,y);
    zm_p(2,x,y,veci[vv].vec);
    if (!ISMOVE(veci[vv].vec)) return;
  }
  ii++;
  veci[vv].dx+=dx;veci[vv].dy+=dy;veci[vv].ax+=dx;veci[vv].ay+=dy;
  if (ii==10){
    pole[2][veci[vv].x][veci[vv].y].v=255;
    HAZEJ_OFF(veci[vv].x,veci[vv].y);
    zm_p(2,veci[vv].x,veci[vv].y,0);
    if (!ISMOVE(veci[vv].vec)) return;
    veci[vv].x=x;veci[vv].y=y;
    veci[vv].dx=0;veci[vv].dy=0;ii=0;
  }
  veci[vv].a1=(veci[vv].a1&0xf)+(ii<<4);
}


void xchngm(UC m1,UC m2)
{
  muflon[m1]+=muflon[m2];
  muflon[m2]=muflon[m1]-muflon[m2];
  muflon[m1]-=muflon[m2];
}

void otvrdvr(UC f,UC g) {
  zm_p(1,f,g,pole[2][f][g].vec-29);
  zm_p(2,f,g,0);
  HAZEJ_1(f,g);
}

UC if_delo2(UC v) {
  if (v==255) return 0;
  return DELO(veci[v].vec);
}
void if_delo(UC x, UC y) {
  if (if_delo2(laser[x-1][y][0].v)) vystrel(x,y,laser[x-1][y][0].v);
  if (if_delo2(laser[x][y+1][1].v)) vystrel(x,y,laser[x][y+1][1].v);
  if (if_delo2(laser[x+1][y][2].v)) vystrel(x,y,laser[x+1][y][2].v);
  if (if_delo2(laser[x][y-1][3].v)) vystrel(x,y,laser[x][y-1][3].v);
}


void na_co_sem_to_slap() {
  UC f,g;
  if (jsem_v_laseru) {
    UC a=1;
    if (jsem_v_proudu) a = 2;
    if (ty.brneni>=a) {ty.brneni-=a;nakresli_pruh2();}
    else {vybouchni(255,ty.x,ty.y,2); return;}
  }
  if (pole[0][ty.x][ty.y].vec==6) {
    konec=255;
    vyskoc=0;
  }
  if (pole[0][ty.x][ty.y].vec==0) {
    if(ty.brneni>0) {ty.brneni--;nakresli_pruh2();}else
    if(ty.otrava<32) {ty.otrava++;nakresli_pruh1();}
    if(ty.otrava==32) ty_kaput();
  } else if (ty.otrava>0) {ty.otrava--;nakresli_pruh1();}
  if (ty.dx==0&&ty.dy==0){
    if (pole[2][ty.x+1][ty.y].vec==89) otvrdvr(ty.x+1,ty.y);
    if (pole[2][ty.x-1][ty.y].vec==89) otvrdvr(ty.x-1,ty.y);
    if (pole[2][ty.x][ty.y+1].vec==90) otvrdvr(ty.x,ty.y+1);
    if (pole[2][ty.x][ty.y-1].vec==90) otvrdvr(ty.x,ty.y-1);
    if (DIRA(pole[0][ty.x][ty.y].vec)&&(ty.vec<161||(pole[0][ty.x][ty.y].vec!=1&&pole[0][ty.x][ty.y].vec!=74&&pole[0][ty.x][ty.y].vec!=75&&pole[0][ty.x][ty.y].vec!=76))) ty_kaput();
    if ((pole[0][ty.x][ty.y].vec==46||pole[0][ty.x][ty.y].vec==47)&&pole[0][ty.x][ty.y].v==255) postav_v(ty.x,ty.y,181);
    if_delo(ty.x,ty.y);
  }
}

void tah_veci() {
  UC vv=0;
  while (vv<topv){
    if (veci[vv].vec!=255){
      if (veci[vv].vec==66) bombuj(vv);else
      if (veci[vv].vec==68) minuj(vv); else
      if (veci[vv].vec==82) vybuchuj(vv); else
      if (veci[vv].vec==85) streluj(vv); else
      if (veci[vv].vec==181) propadej_se(vv); else
      if (POTVORY(veci[vv].vec)) potvoruj(vv); else
      if (DELO(veci[vv].vec)) {if (veci[vv].a3>1) veci[vv].a3--;} else
      if (ELEKTRODA(veci[vv].vec)) {
	if (veci[vv].a3>0) {
	  veci[vv].a3--;
	  if (veci[vv].a3==39) init_magnet(veci[vv].x,veci[vv].y,vv);
	  if (veci[vv].a3==0) init_magnet(veci[vv].x,veci[vv].y,vv);
	}
      } else
      if (ISMOVE(veci[vv].vec)) muvovuj(vv);
    }
    vv++;
  }
  for (vv=0;vv<topp;vv++) {
    if (prep[vv].vec==123) fotobunkuj(vv); else
    if (prep[vv].vec==122) detektoruj(vv);
  }
  for (vv=0;vv<toppad;vv++) if (pady[vv].vec!=255) paduj(vv);
}

void projdi_klavesy() {
  UC vv;
  if (klf[7]==1) {
    vv=sebratelna_vec2(ty.x,ty.y);
    if (vv<10&&ty.i[vv]<12) {
      ty.i[vv]++;
      PlayEffect(ef_get,8363,64,0);
      rozsvit_mnu(vv,ty.i[vv]);
      zm_p(1,ty.x,ty.y,0);
      HAZEJ_1(ty.x,ty.y);
    }
  }
  if (klf[8]==1) {
    vv=sebratelna_vec2(ty.x,ty.y);
    if (vv==6) {
      klf[8]=2;
      PlayEffect(ef_get,8363,64,0);
      pole[1][ty.x][ty.y].vec++;
      if(pole[1][ty.x][ty.y].vec==73) pole[1][ty.x][ty.y].vec-=4;
    }
  }
  if (((klf[4]==2&&klf_citac>10)||klf[4]==1)&&zvol_vec<9) {
    odoznac_mnu();
    zvol_vec++;
    oznac_mnu();
    klf[4]=2;
    klf_citac=0;
  }
  if (((klf[5]==2&&klf_citac>10)||klf[5]==1)&&zvol_vec>3) {
    odoznac_mnu();
    zvol_vec--;
    oznac_mnu();
    klf[5]=2;
    klf_citac=0;
  }
  if (klf[4]||klf[5]) klf_citac++;
  if (klf[6]==1) pouzij_vec(6);
  for (vv=9;vv<16;vv++) if (klf[vv]==1) pouzij_vec(vv);
}

void zjisti_muflona() {
  signed char smtmp=0;

  if (ty.dx==0&&ty.dy==0)
  {
    UC ia,chm=0,osm=sm,f;
    sm=0;
    smtmp=0;
    while (sm==0&&smtmp<4)
    {
      if (pole[0][ty.x][ty.y].vec==7&&(ty.vec<157||ty.vec>160)){
	if (osm==1&&muzesh(ty.x+1,ty.y,1)) {sm=1;break;}
	if (osm==3&&muzesh(ty.x-1,ty.y,3)) {sm=3;break;}
	if (osm==4&&muzesh(ty.x,ty.y+1,4)) {sm=4;break;}
	if (osm==2&&muzesh(ty.x,ty.y-1,2)) {sm=2;break;}
      }
      if (pole[0][ty.x][ty.y].vec==2&&veci[pole[0][ty.x][ty.y].v].a3&&muzesh(ty.x+1,ty.y,1)) {sm=1;PlayEffect(ef_pasy,8363,64,0);}
      if (pole[0][ty.x][ty.y].vec==4&&veci[pole[0][ty.x][ty.y].v].a3&&muzesh(ty.x-1,ty.y,3)) {sm=3;PlayEffect(ef_pasy,8363,64,0);}
      if (pole[0][ty.x][ty.y].vec==5&&veci[pole[0][ty.x][ty.y].v].a3&&muzesh(ty.x,ty.y+1,4)) {sm=4;PlayEffect(ef_pasy,8363,64,0);}
      if (pole[0][ty.x][ty.y].vec==3&&veci[pole[0][ty.x][ty.y].v].a3&&muzesh(ty.x,ty.y-1,2)) {sm=2;PlayEffect(ef_pasy,8363,64,0);}

      if ((klf[11]==0&&klf[12]==0)&&(klf[6]==0||(zvol_vec!=5&&zvol_vec!=6))){
	if (muflon[smtmp]==1&&klf[0]==1&&muzesh(ty.x+1,ty.y,1)) sm=1;
	if (muflon[smtmp]==3&&klf[2]==1&&muzesh(ty.x-1,ty.y,3)) sm=3;
	if (muflon[smtmp]==4&&klf[3]==1&&muzesh(ty.x,ty.y+1,4)) sm=4;
	if (muflon[smtmp]==2&&klf[1]==1&&muzesh(ty.x,ty.y-1,2)) sm=2;
	if (chm==0){
	if (sm==0&&muflon[smtmp]==3&&klf[2]==1) {for (ia=smtmp+1;ia<4;ia++) if(muflon[ia]==1) {xchngm(smtmp,ia);chm=1;}}
	else
	if (sm==0&&muflon[smtmp]==4&&klf[3]==1) {for (ia=smtmp+1;ia<4;ia++) if(muflon[ia]==2) {xchngm(smtmp,ia);chm=1;}}
	else
	if (sm==0&&muflon[smtmp]==1&&klf[0]==1) {for (ia=smtmp+1;ia<4;ia++) if(muflon[ia]==3) {xchngm(smtmp,ia);chm=1;}}
	else
	if (sm==0&&muflon[smtmp]==2&&klf[1]==1) {for (ia=smtmp+1;ia<4;ia++) if(muflon[ia]==4) {xchngm(smtmp,ia);chm=1;}}
	} else chm=0;
      }
      if (sm==0&&smtmp==3) break;
      if (sm==0&&muflon[smtmp+1]==0) break;
      if (sm==0&&chm==0) smtmp++;
    }
    if ((klf[11]==0&&klf[12]==0)&&(klf[6]==0||(zvol_vec!=5&&zvol_vec!=6)))for (f=0;f<4;f++) if (klf[f]==2&&sm!=f+1&&sm!=0) klf[f]=1;
    if (osm!=sm&&osm!=0) soupani_citac=0;
  }
}

void zvrdvr(UC x,UC y) {
  zm_p(2,x,y,pole[1][x][y].vec+29);
  zm_p(1,x,y,0);
  HAZEJ_1(x,y);
}

void zavri_dvere() {
  if (sm!=1&&pole[1][ty.x+1][ty.y].vec==60) zvrdvr(ty.x+1,ty.y);
  if (sm!=3&&pole[1][ty.x-1][ty.y].vec==60) zvrdvr(ty.x-1,ty.y);
  if (sm!=4&&pole[1][ty.x][ty.y+1].vec==61) zvrdvr(ty.x,ty.y+1);
  if (sm!=2&&pole[1][ty.x][ty.y-1].vec==61) zvrdvr(ty.x,ty.y-1);
}

void teleporty() {
  UC vv,x=ty.x,y=ty.y;
  signed char x1=0,x2=0,y1=0,y2=0;

  if (sm==1) {x++;x1=1;y2=1;}else
  if (sm==3) {x--;x1=-1;y2=-1;}else
  if (sm==4) {y++;y1=1;x2=1;}else
  if (sm==2) {y--;y1=-1;x2=-1;}else return;
  if (pole[2][x][y].vec<149||pole[2][x][y].vec>152) return;

  vv=pole[2][x][y].v;
  if (pole[2][x][y].vec==149){x=veci[vv].a1;y=veci[vv].a2;}

  if (muzesh2(x+x1,y+y1)) {x+=x1;y+=y1;} else
  if (muzesh2(x+x2,y+y2)) {x+=x2;y+=y2;} else
  if (muzesh2(x-x2,y-y2)) {x-=x2;y-=y2;} else
  if (muzesh2(x-x1,y-y1)) {x-=x1;y-=y1;} else
  {x=ty.x;y=ty.y;}

  PlayEffect(ef_teleport,8363,64,0);
  zm_p(2,ty.x,ty.y,0);
  HAZEJ_OFF(ty.x,ty.y);
  if (jsem_v_laseru) {
    jsem_v_laseru--;
    if (!jsem_v_laseru) jsem_v_proudu=0;
  }
  if (pole[1][ty.x][ty.y].vec==60||pole[1][ty.x][ty.y].vec==61) zvrdvr(ty.x,ty.y);
  if (pole[0][ty.x][ty.y].vec==54||pole[0][ty.x][ty.y].vec==55) naslapna_podlaha(ty.x,ty.y);
  ty.x=x;
  ty.y=y;
//  zm_p(2,ty.x,ty.y,ty.vec);
  ty.ax=ty.x*20;
  ty.ay=ty.y*17;
  sm=5;
  centruj(4);
}

void seber_vec(UC x,UC y) {
  UC vv=sebratelna_vec(x,y);
  if(vv<10&&ty.i[vv]<12) {
    PlayEffect(ef_get,8363,64,0);
    ty.i[vv]++;
    rozsvit_mnu(vv,ty.i[vv]);
    zm_p(1,x,y,0);
  } else if (vv==11) {
  zm_p(2,x,y,0);
  } else if(vv==12&&ty.brneni<32) {
    PlayEffect(ef_get,8363,64,0);
    ty.brneni=32;
    nakresli_pruh2();
    zm_p(1,x,y,0);
  }
}

void sejvni_level() {
   UC *mnu2;
   mnu2=(UC*)mnu;
   sejvnuto = 1;
   PlayEffect(ef_get,8363,64,0);
   PlayEffect(ef_teleport,8363,64,0);
   mnu[0] = topv;
   mnu[1] = postvec;
   mnu[2] = pvv;
   mnu[3] = toppad;
   mnu[4] = pvp;
   mnu[5] = smer_uskoku;
   mnu[6] = topp;
   mnu[7] = level;
   mnu[8] = usek;
   mnu[9] = hudusek;
   mnu[10] = ii;
   mnu[11] = sm;
   mnu[12] = pohyb;
   mnu[13] = zvol_vec;
   mnu[14] = laser_pom;
   mnu[15] = konec_laseru;
   mnu[16] = jsem_v_laseru;
   mnu[17] = jsem_v_proudu;
   memcpy(mnu2+18,&paleta_citac,2);
   mnu[20] = soupani_citac;
   mnu[21] = konec;
   mnu[22] = vyskoc;
   memcpy(mnu2+100,pole,5632);
   memcpy(mnu2+5732,zobraz1,704);
   memcpy(mnu2+6436,zobraz2,704);
   memcpy(mnu2+7140,laser,5632);
   memcpy(mnu2+12772,veci,4000);
   memcpy(mnu2+16772,pady,3000);
   memcpy(mnu2+19772,prep,2800);
   memcpy(mnu2+22572,&ty,23);
   memcpy(mnu2+22596,vybuch,704);
}

void pohni_se() {
  UC vv;
  char dx=0,dy=0,x=ty.x,y=ty.y;
  if (konec&&sm<5) {vr();return;}
  if (ty.miz>0) {
    ty.miz--;vr();
    if (ty.miz<15&&muzesh2(ty.x,ty.y)) {
      HAZEJ_ON(ty.x,ty.y);
      zm_p(2,ty.x,ty.y,ty.vec);
      ty.miz=0;
      pohyb=1;
      if (pole[0][x][y].vec==52||pole[0][x][y].vec==53) {naslapna_podlaha(x,y);if (konec) return;}
    }else
    if (ty.miz==0) {ty_kaput();vybouchni(255,ty.x,ty.y,2);}
    return;
  }
  if (pohyb==0) centruj(4);
  if (pohyb==0) return;
  if (pohyb>1) {pohyb--;vr();return;}
  if (sm==5&&konec) {sm=0;vr();return;}
  if (sm==0){vr();return;} else
  if (sm==1){dx=2;x++;ty.vec=((ty.vec-153)&12)+153;} else
  if (sm==3){dx=-2;x--;ty.vec=((ty.vec-153)&12)+155;} else
  if (sm==4){dy=pohyb_y[ii];y++;ty.vec=((ty.vec-153)&12)+156;} else
  if (sm==2){dy=-pohyb_y[ii];y--;ty.vec=((ty.vec-153)&12)+154;}
  if (ii==0){
    if (sm!=5) {
      pole[2][ty.x][ty.y].vec=ty.vec;
      HAZEJ_ON(x,y);
      zm_p(2,x,y,ty.vec);
    }
    if (konec) return;
    seber_vec(x,y);
  }
  if (ii==5) {
    if (pole[0][x][y].vec==52||pole[0][x][y].vec==53) {naslapna_podlaha(x,y);if (konec) return;}
    if (pole[0][ty.x][ty.y].vec==54||pole[0][ty.x][ty.y].vec==55) {naslapna_podlaha(ty.x,ty.y);if (konec) return;}
  }
  if (sm==5){
    if (!muzesh2(ty.x,ty.y)) {ty_kaput();vybouchni(255,ty.x,ty.y,2);return;}
    HAZEJ_ON(ty.x,ty.y);
    zm_p(2,ty.x,ty.y,ty.vec);
    if (pole[0][ty.x][ty.y].vec==52||pole[0][ty.x][ty.y].vec==53) {naslapna_podlaha(ty.x,ty.y);if (konec) return;}
    pohyb=11;
    return;
  }
  ii++;
  ty.dx+=dx;ty.dy+=dy;ty.ax+=dx;ty.ay+=dy;
  centruj(2);
  if (ii==10){
    HAZEJ_OFF(ty.x,ty.y);
    zm_p(2,ty.x,ty.y,0);
    if (jsem_v_laseru) {
      jsem_v_laseru--;
      if (!jsem_v_laseru) jsem_v_proudu=0;
    }
    ty.x=x;ty.y=y;
    ty.dx=0;ty.dy=0;ii=0;
  }
}


void hazej4() {
  int f,g;
  int x1,x2,y1,y2;
  x1=sx/20-1;
  x2=x1+18;
  y1=sy/17-1;
  y2=y1+12;
  if (x1<0) x1=0;
  if (y1<0) y1=0;
  if (x2>31) x2=31;
  if (y2>21) y2=21;

  for (f=x1;f<=x2;f++) for (g=y1;g<=y2;g++) {
/*    if (zobraz1[f][g]>230) {
      sound(1000);
      delay(5);
      nosound();
    } */
    if (zobraz1[f][g]) {
      if (zobraz1[f][g]&1) {
        zobraz1[f][g]&=254;
        hazej3(f,g);
        if (zobraz1[f][g]&128) zobraz1[f][g]&=254;
      } else {
        if (!(zobraz1[f][g]&128)) hazej3(f,g);
      }
    }
  }
}

void rotuj_paletu() {
  UC f,r,g,b,a,bb,c;
  paleta_citac++;
  if (paleta_citac%10==0) {
    precti_1barvu(224,&a,&bb,&c);
    precti_1barvu(230,&r,&g,&b);
    nastav_1barvu(224,r,g,b);
    for (f=225;f<231;f++) {
      r=a;g=bb;b=c;
      precti_1barvu(f,&a,&bb,&c);
      nastav_1barvu(f,r,g,b);
    }
  }
  if (paleta_citac%4==1) {
    precti_1barvu(234,&a,&bb,&c);
    precti_1barvu(231,&r,&g,&b);
    nastav_1barvu(234,r,g,b);
    for (f=233;f>230;f--) {
      r=a;g=bb;b=c;
      precti_1barvu(f,&a,&bb,&c);
      nastav_1barvu(f,r,g,b);
    }
    precti_1barvu(238,&a,&bb,&c);
    precti_1barvu(235,&r,&g,&b);
    nastav_1barvu(238,r,g,b);
    for (f=237;f>234;f--) {
      r=a;g=bb;b=c;
      precti_1barvu(f,&a,&bb,&c);
      nastav_1barvu(f,r,g,b);
    }
  }
}

void play_level(){
  UC vv;
  UC ns1,ns2;

  memset(muflon,0,4);
  memset(klf,0,17);
  key=0;

  ns1=peekb(0,0x417);
  ns2=peekb(0,0x418);
  pokeb(0,0x417,0);
  pokeb(0,0x418,0);
  while (kbhit()) getch();
  oldhandler = getvect(0x09);
  setvect(0x09, handler);

  while (konec<100)
  {
    if (konec!=0) {
    konec++;
    }
    rotuj_paletu();
    if (key==1) konec=255;
    if (konec==0&&pohyb==1) na_co_sem_to_slap();
    tah_veci();
    if (konec==0&&pohyb==1) projdi_klavesy();else soupani_citac = 0;
    if (pohyb==1&&konec==0) zjisti_muflona();
    if (konec==0&&ty.dx==0&&ty.dy==0) {
      if (sm!=0) zavri_dvere();
      teleporty();
      prepni(ty.x,ty.y,&sm);
      otoc_zrcadlo();
    }
    pohni_se();
    hazej4();
    if (konec==0&&pohyb==1) if (ty.dx==0&&ty.dy==0) {
      if (pole[0][ty.x][ty.y].vec==73) {
        if (!nesejvuj_to) {sejvni_level();nesejvuj_to=1;}
      } else nesejvuj_to=0;
    }
  }
  setvect(0x09, oldhandler);
  pokeb(0,0x417,ns1);
  pokeb(0,0x418,ns2);
}






void main()
{
  nahraj_data();
  uv_obr();
  while(1) {
    menu();
    done_level();
    while (vyskoc) {
      init_level();
      play_level();
      done_level();
    }
  }
}



