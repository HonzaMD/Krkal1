#define UC unsigned char
#define UI unsigned int
#include "krkal.h"
#include "midas.h"

extern UC PLAYER;
extern UC ANIM;
extern UC nemec[13];
extern UC cheet[13];
extern UC chname[13];
extern UC vec_1[182],vec_2[182],patro[182];
extern UC jmena_ikon[18][9];
extern UC far obr[185][17][20];
extern UC far mnu[40000];
extern UC far buffer[64000];
extern UC pal2[768];
extern UC brv2font[450];
extern UC mys[7][49];
extern UC ram[6][100];
extern UC klavesy[17];
extern UC prettl[56];
extern UC pretprh[2][8];
extern UC hlavicka[10];
extern UC ikoc[16][20],ikof[16][20];

extern UC pocet_hracu;
extern struct {
  UC jmeno[13]; //XOR s "Nemec je BLB"
  UC data[12];
  unsigned long int crc;
  UC pocet;
  UC level;
  UC hlasitost;
  UC klavesy[17];
} hraci[16];
extern struct {
  UC a,b;
  unsigned long int c;
} usdefhr[16][50], usdeflev[32];

extern struct {
  unsigned long int odkaz;
  UC jmeno[16];
} levly[128];
extern UC udelano[128]; // 0 - neudelana, 1 - udelano,2 - Mozno hrat, 254 - neexistuje, 255 - vadny soubor
extern UC levlyjm[128][16];
extern UC ces_kl2[13][3];

extern UC pocet_env;
extern struct {
  UC jm[26];
  unsigned long int odkaz;
  UC ani;
} environment[30];

extern FILE *s;
extern UC level;
extern UC usek; //Usek levlu se stejnou grafikou
extern UC hudusek;
extern UC hrac; //Ktery SAVEGAME se pouzije
extern UC vyskoc; //Jestli se nevyskoci do menu
extern UC hral_jsi_uz;
extern UC zalohuj_sejv;
extern UC graphmode;
extern UI menu_x;
extern UC typdonemenu;
extern UI mysx,mysy,mystl;
extern UC zmacknuto,manim,mexists,dblclk/*Kdyz je 200*/;
extern unsigned ef_teleport,ef_vybuch,ef_pasy,ef_get,ef_zuch;
extern unsigned short scrSync;
extern mpModule    *mod;
extern void interrupt ( *oldhandler)();
extern UC rt;

#define FXCHANNELS 4
extern unsigned fxChannel;

extern UC key,klf[17];
extern UC muflon[4];
extern UI nasobkyx[32];
extern UI nasobkyy[22];
extern UI nasobky340[181];


int EMSinit;

emsBlock *anim[4];
UC ERROR=0;
UC *animbuff;

struct {
  UC t;
  UC jm[20];
} cfg_ikony[2][13] = {2,"Pohyb nahoru",4,"Pohyb dolu",1,"Pohyb doprava",3,"Pohyb doleva",0,"",
5,"Doprava v inv.",6,"Doleva v invent.",7,"Pouzij vec",0,"",
8,"Seber znacku",9,"Otoc znacku",
21,"Hlasitost hudby",18,"OK",
10,"Pouzij bombu",11,"Pouzij minu",12,"Pouzij jablko",0,"",
13,"Smerova znacka",14,"Zakaz vjezdu",15,"Sedesatka",16,"Povoleno sedesat",17,"Bez funkce",0,"",
20,"PUVODNI KLAVESY",22,"Hlasitost efektu",19,"CANCEL"};
UC cfg_ikony2[2][13];
UC scancodchr[89] = {255,200,'1','2','3','4','5','6','7','8','9','0','-','=',201,202,'Q','W','E','R',
 'T','Y','U','I','O','P','[',']',203,204,'A','S','D','F','G','H','J','K','L',';',39,'`',205,255,'Z','X','C','V','B',
 'N','M',',','.','/',206,'*',207,208,255,209,210,211,212,213,214,215,216,217,218,255,255,219,220,
 221,222,223,224,225,226,227,228,229,230,231,255,255,255,232,233};
UC scancodtxt[34][8] = {"ESC","Mazani","Tab","Enter","Ctrl","L.Shift","R.Shift","Alt","Mezera",
 "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","Home","Nahoru","PgUp","- (Num)","Vlevo","5 (Num)","Vpravo","+ (Num)",
 "End","Dolu","PgDn","Ins","Del","F11","F12"};


// *******************************************************************
// *******************************************************************
// *******************************************************************
// *******************************************************************
// *******************************************************************
// *                                                                 *
// *                          M U S I C                              *
// *                                                                 *
// *******************************************************************

void PlayEffect(ushort instHandle, ulong rate, ushort volume,
    short panning)
{
    int         error;
    if (!PLAYER) return;
    if ( (error = midasSD->SetInstrument(fxChannel, instHandle)) != OK )
	midasError(error);

    if ( (error = midasSD->SetVolume(fxChannel, volume)) != OK )
	midasError(error);

    if ( (error = midasSD->SetPanning(fxChannel, panning)) != OK )
	midasError(error);

    if ( (error = midasSD->PlaySound(fxChannel, rate)) != OK )
	midasError(error);

    fxChannel++;                        /* channel for next effect */
    if ( fxChannel >= FXCHANNELS )
	fxChannel = 0;
}


unsigned LoadEffect(char efnum, int looping)
{
    ushort      instHandle;             /* sound device instrument handle */
    int         error;
    FILE *f;
    long tmp;
    long        smpLength;              /* sample length */
    uchar       *smpBuf;                /* sample loading buffer */

    if ( (f=fopen("krkal.mus","rb")) ==NULL ) midasError(error);
    fseek(f,10,0);
    fread(&tmp,4,1,f);
    fseek(f,tmp+efnum*8,0);
    fread(&tmp,4,1,f);
    fread(&smpLength,4,1,f);
    fseek(f,tmp,0);
    if ( smpLength > SMPMAX ) midasError(errInvalidInst);
    if ( (error = memAlloc(smpLength, (void**) &smpBuf)) != OK ) midasError(error);
    fread(smpBuf, smpLength,1,f);
    fclose(f);

    if ( looping )
    {
	error = midasSD->AddInstrument(smpBuf, smp8bit, smpLength, 0,
	    smpLength, 64, 1, 1, &instHandle);
    }
    else
    {
	error = midasSD->AddInstrument(smpBuf, smp8bit, smpLength, 0,
	    0, 64, 0, 1, &instHandle);
    }

    if ( error != OK )
	midasError(error);

    if ( (error = memFree(smpBuf)) != OK ) midasError(error);

    return instHandle;
}

void FreeEffect(unsigned instHandle)
{
    int         error;

    /* remove instrument from Sound Device list: */
    if ( (error = midasSD->RemInstrument(instHandle)) != OK )
	midasError(error);
}



void ef(){
    if (ANIM) {
      if (ERROR==0) emsFree(anim[3]);
      if (ERROR==0||ERROR>3) emsFree(anim[2]);
      if (ERROR==0||ERROR>2) emsFree(anim[1]);
      if (ERROR==0||ERROR>1) emsFree(anim[0]);
    }
    if (PLAYER){
      tmrStopScrSync();
      midasStopModule(mod);               /* stop playing */
      FreeEffect(ef_zuch);
      FreeEffect(ef_get);
      FreeEffect(ef_pasy);
      FreeEffect(ef_vybuch);
      FreeEffect(ef_teleport);
      midasFreeModule(mod);               /* deallocate module */
      midasClose();                       /* uninitialize MIDAS */
    }
}


// *******************************************************************
// *                                                                 *
// *        O V L A D A  N I   K L A V E S N I C ,  M Y S I , ...    *
// *                                                                 *
// *******************************************************************

UC ctrl,alt;

UC oldkey=0;
void interrupt handler()
{
   UI i,f,g;
//   oldhandler();
   key=inportb(0x60);
   if (key==29) ctrl=1;
   if (key==56) alt=1;
   if (key==157) ctrl=0;
   if (key==184) alt=0;
   for (i=0;i<17;i++)
   {
     if (key==klavesy[i]) {
       if (i<4&&klf[i]==0) {
	 for(f=3;f>0;f--) muflon[f]=muflon[f-1];
	 muflon[0]=i+1;
       }
       if (klf[i]==0) klf[i]=1;
     }
     if (key==klavesy[i]+128&&(oldkey!=0xe0||key!=0xaa)) {
       klf[i]=0;
       for (g=0;g<=3;g++)
       if (muflon[g]==i+1) {
	 for(f=g;f<3;f++) muflon[f]=muflon[f+1];
	 muflon[3]=0;

       }
     }
   }
   oldkey=key;
   outportb(0x20,0x20);
   if (key==83&&alt&&ctrl){
     setvect(0x09, oldhandler);
     done256();
     exit(0);
   }
}

void preVR(void)
{
  if (rt==1)
  {
//    setx(sx&3);
//    SSA();
  }
}

void immVR(void)
{
  rt=0;
}

void inVR(void){
}

void mreset() {
  manim = 0;
  zmacknuto = 1;
  dblclk=100;
  asm {
//    cli
    xor ax,ax
    int 33h
    mov mexists,al
    and al,al
    jz konec
    mov ax,7
    mov cx,6
    mov dx,632
    int 33h
    mov ax,8
    mov cx,3
    mov dx,196
    int 33h
//    sti
  }
  konec:;
}

void mysuj() {

  if (mexists) {
    if (dblclk<100) dblclk++;
    if (dblclk==200)dblclk=100;
    asm {
//      cli
      mov ax,3
      int 33h
      mov mystl,bx
      shr cx,1
      mov mysx,cx
      mov mysy,dx
//      sti
    }
    if (mystl!=0) {
      if (zmacknuto) mystl=0; else {
        zmacknuto=1;
        if (dblclk<20) {
          dblclk=200;
          }else dblclk=0;
      }
    } else zmacknuto = 0;
    if (graphmode) {
      vgetimage(mysx-3,mysy-3,7,7,mys[6]);
      vputimage0(mysx-3,mysy-3,7,7,mys[manim/10]);
    } else {
      sgetimage(menu_x+mysx-3,mysy-3,7,7,mys[6]);
      sputimage0(menu_x+mysx-3,mysy-3,7,7,mys[manim/10]);
    }
    vr();
    if (manim == 59) manim = 0; else manim++;
    if (graphmode) vputimage(mysx-3,mysy-3,7,7,mys[6]);
    else sputimage(menu_x+mysx-3,mysy-3,7,7,mys[6]);
  } else mystl=0;
}



// *******************************************************************
// *                                                                 *
// *                P R A C E   S   D I S K E M                      *
// *                                                                 *
// *******************************************************************


void konvertuj_levlyjm(UC od, UC ddo) {
  UC f,g,d,a;
  for (f=od;f<=ddo;f++) {
    g = 0;
    do {
      a=levly[f].jmeno[g];
      if (a>=127) {
        if (a<142) {
          if (a<131) a=ces_kl2[a-127][0]-32;else
          if (a<139) a=ces_kl2[a-128][0]-32;else
          a=ces_kl2[a-129][0]-32;
        } else {
          if (a<146) a=ces_kl2[a-142][0];else
          if (a<154) a=ces_kl2[a-143][0];else
          a=ces_kl2[a-144][0];
        }
      }
      levlyjm[f][g] = a;
      g++;
    } while(a);
  }
}


unsigned long zjisti_crc(UC hrac) {
  unsigned long b,crc;
  UC f,g,c;
  UI d;
  b = 0;
  for (f=0;f<6;f++) for (g=0;g < 8;g++) {
    c = 1;
    c = c << g;
    c = hraci[hrac].data[f] & c;
    if (c != 0) b++;
  }
  crc = b;
  crc = crc << 6;
  b = 0;
  for (f=6;f<12;f++) for (g=0;g < 8;g++) {
    c = 1;
    c = c << g;
    c = hraci[hrac].data[f] & c;
    if (c != 0) b++;
  }
  crc = crc | b;
  crc = crc << 20;
  b = 0;
  for (f=0;f<12;f++) {
    g = f+6;
    if (g>11) g = g - 12;
    d = hraci[hrac].data[f];
    d = d << 8;
    b = b+d+hraci[hrac].data[g];
  }
  crc = crc | b;
  return(crc);
}

UC uprav_crc(UC hrac) {
  UI f,g,f2,g2,b=0;
  for (f=0;f<12;f++) for (g=1;g<256;g<<=1) {
    if (hraci[hrac].data[f]>=g) {
      hraci[hrac].data[f]-=g;
      if (zjisti_crc(hrac) == hraci[hrac].crc) {
        b++;
        f2=f;
        g2=g;
      }
      hraci[hrac].data[f]+=g;
    }
  }
  if (b) hraci[hrac].data[f2]-=g2;
  return b;
}

void zmen_hrace() {
  UC f,g,a,b;
  if (hrac == pocet_hracu) {
    memset(udelano,0,96);
    return;
  }
  level = hraci[hrac].level;
  memcpy(klavesy,hraci[hrac].klavesy,17);
  for (f=0;f<12;f++) {
    for (g=0;g<8;g++) {
      if ((f*8+g)%32==0) b=3;
      a = 1;
      a = a << g;
      a = hraci[hrac].data[f] & a;
      if (a) udelano[f*8+g] = 1; else if (b>0) {
        udelano[f*8+g] = 2;
        b--;
      } else udelano[f*8+g] = 0;
    }
  }
  for (f=0;f<32;f++) {
    if (udelano[96+f] < 254) {
      a = 0;
      for (g=0;g<hraci[hrac].pocet;g++) if (memcmp(&usdeflev[f],&usdefhr[hrac][g],6)==0) a = 1;
      if (a) udelano[96+f] = 1; else udelano[96+f] = 2;
    }
  }
  while ((level%32 != 0)&&((udelano[level]>2)||(udelano[level]==0))) level--;
  if (PLAYER) {
    midasSD->SetMasterVolume((hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20);
    midasSD->SetAmplification((hraci[hrac].hlasitost>>4)*20>63?(hraci[hrac].hlasitost>>4)*20:64);
  }
}

void uloz_hrace() {
  UC f,g,a;
  if (hrac < pocet_hracu) {
    hraci[hrac].level=level;
    memcpy(hraci[hrac].klavesy,klavesy,17);
    hraci[hrac].crc = zjisti_crc(hrac);
    for (f=0;f<12;f++) {
      for (g=0;g<8;g++) {
	a = 1;
	a = a << g;
	if (udelano[f*8+g] == 1) hraci[hrac].data[f] |= a;
      }
    }
    for (f=0;f<32;f++) {
      if (udelano[96+f] < 254) {
	if (udelano[96+f]==1) {
	  a = 0;
	  for (g=0;g<hraci[hrac].pocet;g++) if (memcmp(&usdeflev[f],&usdefhr[hrac][g],6)==0) a = 1;
	}
	if (a==0) {
	  if (hraci[hrac].pocet<50) {
	    usdefhr[hrac][hraci[hrac].pocet] = usdeflev[f];
	    hraci[hrac].pocet++;
	  }
	}
      }
    }
  }
}

/* vec1 1b - Prepinac,2b-Laser,3b-Ve Vecech,4b - Zrcadla,
	5b - Veci ovlivnujici lasery,6b - Pas,7b - Jakakoliv dira,8b-Soupatelna vec
   vec2 1b - Delo, 2b-Elektroda, 3b - Magnet, 4b - TY,
	5b - Potvory,6b - Vec,ktera brani vybuchu,7b - Pusobi kulate,8b Vse, co  pada, krome Aktivovany Miny a Bomby
   */

void nastav_vlastnosti() {
  UC f;
  memset(vec_1,0,182);
  memset(vec_2,0,182);
  for (f=0;f<182;f++) {
    if (f<=55||f==181||(f>=73&&f<=76)) patro[f]=0; else if (f<=88) patro[f]=1; else patro[f]=2;
    if (f==117||f==118||f==122||f==123||f==52||f==53||f==54||f==55) vec_1[f]|=0x80;
    if (f>=125&&f<=128) vec_1[f]|=0x40;
    if (f==0||f==66||f==68||(f>=82&&f<=85)/*||(f>=133&&f<=136)*/||(f>=145&&f<=148)||(f>=150&&f<=152)||f==181) vec_1[f]|=0x20;
    if (f>=137&&f<=144) vec_1[f]|=0x10;
    if ((f>=56&&f<=59)||(f>=62&&f<=68)||(f>=89&&f<=180)) vec_1[f]|=0x08;
    if (f>=2&&f<=5) vec_1[f]|=0x04;
    if (f==1||f==74||f==75||f==76||f==44||f==45||f==48||f==49) vec_1[f]|=0x02;
    if (f>=56&&f<=59) vec_1[f]|=0x01;
    if (f>=129&&f<=132) vec_2[f]|=0x80;
    if (f>=133&&f<=136) vec_2[f]|=0x40;
    if (f>=145&&f<=148) vec_2[f]|=0x20;
    if (f>=153&&f<=164) vec_2[f]|=0x10;
    if (f>=165&&f<=180) vec_2[f]|=0x08;
    if ((f>=91&&f<=106)||(f>=117&&f<=124)||(f>=133&&f<=136)||(f>=145&&f<=148)) vec_2[f]|=0x04;
    if (f==0||(f>=57&&f<=59)||(f>=63&&f<=72)||(f>=77&&f<=79)||(f>=86&&f<=88)||(f>=107&&f<=113)||(f>=125&&f<=144)||(f>=149&&f<=152)) vec_2[f]|=0x02;
    if ((f>=56&&f<=59)||(f>=63&&f<=65)||f==67||(f>=69&&f<=72)||(f>=77&&f<=79)||(f>=86&&f<=88)) vec_2[f]|=0x01;
  }
}

int zkontroluj_soubor(const UC * soubor){
  struct ffblk xx;
  if (0 == findfirst(soubor,&xx,FA_ARCH)){
    s = fopen(soubor,"r+b");
    memset(buffer,0,10);
    fread(buffer,10,1,s);
    if (0==memcmp(buffer,hlavicka,10)) return(1);
    fclose(s);
  }
  return(0);
}
void nahraj_data(){
  UI a,error,d;
  UC c,f,g,errorcrc;
  UC as[3],bs[13];
  struct ffblk xx;

  for (g=0;g<12;g++) nemec[g]--;
  for (g=0;g<12;g++) cheet[g]--;
  for (f=0;f<32;f++) nasobkyx[f]=f*20;
  for (f=0;f<22;f++) nasobkyy[f]=f*17;
  for (f=0;f<181;f++) nasobky340[f]=f*340;
  nastav_vlastnosti();
  error = 0;
  errorcrc = 0;
  usek = 255;
  initvga256();
  randomize();
  memcpy(hlavicka,"M+W Krkal0",10);
  if (PLAYER) {
    s = fopen("midas.cfg","r+b");
    fread(buffer,1,4,s);
    if (buffer[0]==4&&buffer[2]==1) PLAYER=0;
    fclose(s);
    if (zkontroluj_soubor("krkal.mus")) fclose(s);
  }
  if (zkontroluj_soubor("environ.dat")) fclose(s); else error++;
  if (zkontroluj_soubor("menu.dat")) {
    fseek(s,101050,0);
    fread(mys,6,49,s);
    fread(ram,100,6,s);
    nahrajfont(s);
    nahrajfont(s);
    fread(brv2font,450,1,s);
    fread(klavesy,17,1,s);
    fread(prettl,8,7,s);
    fread(ikoc,16,20,s);
    fread(ikof,16,20,s);
    fclose(s);
  } else error++;
  if (zkontroluj_soubor("krkal.cfg")) {
    fread(&hrac,1,1,s);
    fclose(s);
  } else {
    printf("\n\nVAROVANI - Hra nebyla nakonfigurovana.");
    printf("\n           Spust SETUP, jestlize to bude potreba.\n");
    while (kbhit()) getch();
    getch();
    s = fopen("krkal.cfg","w+b");
    fwrite(hlavicka,10,1,s);
    hrac = 0;
    fwrite(&hrac,1,1,s);
    fclose(s);
  }
  if (zkontroluj_soubor("krkal.lev")) {
    fread(levly,20,96,s);
    konvertuj_levlyjm(0,95);
    fclose(s);
  } else error++;
  if (0 == findfirst("players.inf",&xx,FA_ARCH)){
    s = fopen("players.inf","r+b");
    memset(buffer,0,10);
    fread(buffer,10,1,s);
    if (0==memcmp(buffer,hlavicka,10)) {
      UC f,g;
      fread(&pocet_hracu,1,1,s);
      fread(hraci,49,pocet_hracu,s);
      for (f=0;f<pocet_hracu;f++) for (g=0;g<12;g++) hraci[f].jmeno[g]^=nemec[g];
      fread(usdefhr,300,pocet_hracu,s);
    } else {
      printf("\n\nERROR - Soubor, kde byly ulozeny pozice hracu, byl poskozen !");
      printf("\n        Chces tento soubor smazat a vytvorit novy, przdny ? [A|N]?");
      printf("\n        Jestlize vis jak tento soubor opravit, zmackni \"N\" a udelej to.\n");
      do a = getch(); while((a!='a'&&a!='n')&&(a!='A'&&a!='N'));
      if (a=='n'||a=='N') exit(0);
      fclose(s);
      s = fopen("players.inf","w+b");
      fwrite(hlavicka,10,1,s);
      pocet_hracu = 0;
      hrac=0;
      fwrite(&pocet_hracu,1,1,s);
    }
  } else {
    s = fopen("players.inf","w+b");
    fwrite(hlavicka,10,1,s);
    pocet_hracu = 0;
    hrac=0;
    fwrite(&pocet_hracu,1,1,s);
  }
  fclose(s);
  if (pocet_hracu > 0) for (a=0;a<pocet_hracu;a++) {
    if (zjisti_crc(a) != hraci[a].crc) {
      UC b=uprav_crc(a);
      errorcrc=1;
      printf("\n\nERROR - Soubor, kde byly ulozeny pozice hracu, byl poskozen !");
      if (b) {
        printf("\n        Chces 1 level oznacit za nedohrany (z %i moznosti)",b);
        printf("\n        u hrace %s ? [A|N]?",hraci[a].jmeno);
      } else {
        printf("\n        Chces smazat informace o hraci %s ? [A|N]?",hraci[a].jmeno);
//        memset(hraci[a].data,0,12);
//        hraci[a].crc = 0;
        hraci[a].crc = zjisti_crc(a);
      }
      printf("\n        Jestlize vis, jak to opravit, zmackni \"N\" a udelej to.\n");
      do b = getch(); while((b!='a'&&b!='n')&&(b!='A'&&b!='N'));
      if (b=='n'||b=='N') exit(0);
    }
  }
  if (hrac>pocet_hracu) hrac=pocet_hracu;
  if (errorcrc != 0) zalohuj_sejv=2;
  if (error != 0)  {
    printf("\n\nERROR - Nektery dulezity soubor nebyl nalezen.");
    printf("\n        Ujisti se, zda hru spoustis ze spravneho adresare.\n");
    exit(0);
  }
  memset(usdeflev,0,192);
  for (a=0;a<32;a++) {
    itoa(a+1,as,10);
    sprintf(bs,"krk0%s.lev",as);
    if (0 == findfirst(bs,&xx,FA_ARCH)){
      s = fopen(bs,"r+b");
      memset(buffer,0,10);
      fread(buffer,10,1,s);
      if (0==memcmp(buffer,hlavicka,10)) {
	udelano[96+a] = 0;
	fread(buffer,2,1,s);
	fread(levly[96+a].jmeno,15,1,s);
	usdeflev[a].a = ((levly[96+a].jmeno[0] & 0x0f) << 4)|(levly[96+a].jmeno[1] & 0x0f);
	usdeflev[a].b = ((levly[96+a].jmeno[2] & 0x0f) << 4)|(levly[96+a].jmeno[3] & 0x0f);
	fread(&usdeflev[a].c,4,1,s);
      } else {
	udelano[96+a] = 255;
	strcpy(levly[96+a].jmeno,"E Vadny soubor");
      }
      fclose(s);
    } else udelano[96+a] = 254;
  }
  konvertuj_levlyjm(96,127);
  init256();
  if (PLAYER)
  {
    int          error, isConfig;
    long i;
    char k;
    if ( (error = fileExists("MIDAS.CFG", &isConfig)) != OK )
	midasError(error);
    if ( !isConfig )
    {
	puts("Configuration file not found - run MSETUP.EXE");
	exit(EXIT_FAILURE);
    }

    midasSetDefaults();                 // set MIDAS defaults
    midasLoadConfig("MIDAS.CFG");       // load configuration
    if ( (error = tmrGetScrSync(&scrSync)) != OK )
	midasError(error);

    midasInit();                        // initialize MIDAS Sound System

    {
      FILE *s;
      s=fopen("krkal.mus","rb");
      fseek(s,14,0);
      fread(&i,4,1,s);
      fclose(s);
    }
    mod = midasLoadModule("krkal.mus",i, &mpS3M, NULL);  // load module

ef_teleport= LoadEffect(0, 0);
ef_vybuch= LoadEffect(1, 0);
ef_pasy= LoadEffect(2, 0);
ef_get=LoadEffect(3,0);
ef_zuch=LoadEffect(4,0);

    if ( (error = tmrSyncScr(scrSync, &preVR, &immVR, &inVR)) != OK )
	midasError(error);
midasPlayModule(mod, 4);            // start playing
midasSD->SetAmplification(128);
}else emsInit(&EMSinit);

zmen_hrace();

ERROR=0;
if (ANIM) {
  if(emsAlloc(64000,(emsBlock**)&anim[0])) ERROR=1; else
  if(emsAlloc(64000,(emsBlock**)&anim[1])) ERROR=2; else
  if(emsAlloc(64000,(emsBlock**)&anim[2])) ERROR=3; else
  if(emsAlloc(64000,(emsBlock**)&anim[3])) ERROR=4;
}

atexit(ef);

if (ERROR) exit(1);

s=fopen("environ.dat","rb");
  fseek(s,778,0);
  fread(obr[153],340,12,s);
  if (ANIM) {
    emsMap(anim[0],(void**)&animbuff);
    fread(animbuff,28560,1,s);
    emsMap(anim[1],(void**)&animbuff);
    fread(animbuff,28560,1,s);
    emsMap(anim[2],(void**)&animbuff);
    fread(animbuff,9520,1,s);
    fread(animbuff+12240,16320,1,s);
    emsMap(anim[3],(void**)&animbuff);
    fread(animbuff+4080,5440,1,s);
    fread(animbuff+12240,16320,1,s);
  } else fseek(s,104720,1);
  fread(&pocet_env,1,1,s);
  fread(environment,31,pocet_env,s);
fclose(s);
}


void sejvni(){
  UC f,g;
  FILE *s;

  uloz_hrace();
  s = fopen("krkal.cfg","r+b");
  fseek(s,10,0);
  fwrite(&hrac,1,1,s);
  fclose(s);
/*  if (zalohuj_sejv==1) {
    FILE *z;
    UI a;
    s = fopen("players.inf","rb");
    z = fopen("players.bak","w+b");
    a=filelength(fileno(s));
    fread(buffer,1,a,s);
    fwrite(buffer,1,a,z);
    fclose(z);
    fclose(s);
  }*/
  s = fopen("players.inf","w+b");
  for (f=0;f<pocet_hracu;f++) for (g=0;g<12;g++) hraci[f].jmeno[g]^=nemec[g];
  fwrite(hlavicka,10,1,s);
  fwrite(&pocet_hracu,1,1,s);
  fwrite(hraci,49,pocet_hracu,s);
  fwrite(usdefhr,50*6,pocet_hracu,s);
  fclose(s);
  for (f=0;f<pocet_hracu;f++) for (g=0;g<12;g++) hraci[f].jmeno[g]^=nemec[g];
  zalohuj_sejv=0;
}


//#include "hazej.c"
// void hazej(UI x,UI y,UC o) {
// void zobraz_menu(UI x, UI y)



// *******************************************************************
// *******************************************************************
// *******************************************************************
// *******************************************************************
// *                                                                 *
// *                          M E N U                                *
// *                                                                 *
// *******************************************************************


void uv_obr(){
/*  clrscr();
  printf("\n\n\n\n\n\n              KK    KK   RRRRRR     KK    KK     AAAA     LL\n");
  printf("              KK  KK     RR    RR   KK  KK     AA    AA   LL\n");
  printf("              KKKK       RRRRRR     KKKK       AAAAAAAA   LL\n");
  printf("              KK  KK     RR  RR     KK  KK     AA    AA   LL\n");
  printf("              KK    KK   RR    RR   KK    KK   AA    AA   LLLLLLL\n");
  printf("\n\n\n\n\nNejlepsi intro - TEXTOVE intro . . .");
//  while (kbhit()) getch();
//  getch();
*/
}

void init_menu(){
  UI f,a,y,x1,x2;
  setmem(&pal,768,0);
  nastav_paletu0();
  s = fopen("menu.dat","rb");
  fseek(s,10,0);
  fread(buffer,1,48288,s);
  fread(&pal2,3,256,s);
  zobraz_dpc(buffer);
  fread(&buffer[3840],1,45120,s);
  fread(mnu,6864,1,s);
  fclose(s);
  for (a=0;a<52;a+=2){
    for(f=0;f < 768;f++) {
     if (64-a<=pal2[f]) pal[f]+=2;else
     if (63-a==pal2[f]) pal[f]++;
    }
    vr();
    SetPalette(pal);
  }
  y = 5;
  x1 = 3;
  x2 = 8;
  for (a=52;a<64;a+=4){
    for(f=0;f < 768;f++) {
     if (64-a<=pal2[f]) pal[f]+=4;else
     if (63-a==pal2[f]) pal[f]+=3;
     if (62-a==pal2[f]) pal[f]+=2; else
     if (61-a==pal2[f]) pal[f]++;
    }
    vr();
    SetPalette(pal);
    vr();
    if (y==5) {
      zobraz_menu(45,82,1,1);
      zobraz_menu(203,82,2,1);
    }
    zobraz_menu(45-x1,82-y,x1*2+1,5);
    zobraz_menu(45-x1,78+y,x1*2+1,5);
    zobraz_menu(45-x1,87-y,3,y*2-9);
    zobraz_menu(43+x1,87-y,3,y*2-9);
    zobraz_menu(203-x2,87-y,8,y*2-9);
    zobraz_menu(197+x2,87-y,8,y*2-9);
    zobraz_menu(203-x2,82-y,x2*2+2,5);
    zobraz_menu(203-x2,78+y,x2*2+2,5);
    y+=5;
    x1+=3;
    x2+=8;
  }
  y-=2;
  x1-=1;
  x2-=4;
  for (f=0;f<11;f++) {
    vr();
    zobraz_menu(45-x1,82-y,x1*2+1,3);
    zobraz_menu(45-x1,85-y,2,y*2-5);
    zobraz_menu(203-x2,85-y,4,y*2-5);
    zobraz_menu(201+x2,85-y,4,y*2-5);
    zobraz_menu(203-x2,80+y,x2*2+2,3);
    zobraz_menu(203-x2,82-y,x2*2+2,3);
    zobraz_menu(45-x1,80+y,x1*2+1,3);
    zobraz_menu(44+x1,85-y,2,y*2-5);
    y+=2;
    x1+=1;
    x2+=4;
    vr();
    zobraz_menu(45-x1,82-y,x1*2+1,2);
    zobraz_menu(45-x1,84-y,1,y*2-3);
    zobraz_menu(203-x2,84-y,4,y*2-3);
    zobraz_menu(201+x2,84-y,4,y*2-3);
    zobraz_menu(203-x2,81+y,x2*2+2,2);
    zobraz_menu(203-x2,82-y,x2*2+2,2);
    zobraz_menu(45-x1,81+y,x1*2+1,2);
    zobraz_menu(45+x1,84-y,1,y*2-3);
    y+=3;
    x1+=2;
    x2+=4;
  }
  memcpy(buffer,videoram,64000);
}

void zatmi(){
  UI a,f;
  for (a=0;a<64;a+=2){
    for(f=0;f < 768;f++) {
     if (pal[f]>1) pal[f]-=2;else
     if (1==pal[f]) pal[f]--;
    }
    vr();
    SetPalette(pal);
  }
}

void popis_ikonu(UC xx,UC jmeno,UC barva1,UC barva2) {
  UI x,y;
  UI f;
  if (jmeno>=9&&jmeno<=12) {
    if (xx==2) x=118,y=173; else if (xx==3) x=139,y=173; else if (xx==4) x=118,y=185; else x=139,y=185;
    if (xx<5) {
      for (f=0;f<xx-1;f++) fill(x+f*4,y-f,3,f+1,barva1);
    } else {
      for (f=0;f<3;f++) {
        vputpixelb(x+f*2,y,barva1);
        vputpixelb(x+10-f*2,y-3,barva1);
      }
      fill(x+3,y-3,2,2,barva1);
      fill(x+6,y-1,2,2,barva1);
    }
  } else {
     if (jmeno==13) jmeno=9;
    jmeno=(jmeno-1)*2;
    if (xx>5) x=(xx-3)*52+30; else x=xx*52+30;
    if (jmena_ikon[jmeno+1][0]) {
      vgotoxy(x-vlength(jmena_ikon[jmeno])/2,170);
      vwrite2hc(barva1,barva2,1,jmena_ikon[jmeno]);
      vgotoxy(x-vlength(jmena_ikon[jmeno+1])/2,180);
      vwrite2hc(barva1,barva2,1,jmena_ikon[jmeno+1]);
    } else {
      vgotoxy(x-vlength(jmena_ikon[jmeno])/2,175);
      vwrite2hc(barva1,barva2,1,jmena_ikon[jmeno]);
    }
  }
}

void nakresli_ikony(UC ikony[9],UC *aktiv,UC *aktiv2,UC tab){
  UC iko1[9]={1,2,9,10,11,12,3,4,5};
  UC iko2[9]={7,8,9,10,11,12,13,4,5};
  UC f,g;
  if ((tab==1)||(tab==3)) {
    for (f=0;f<10;f++) ikony[f]=iko1[f];
    if (hrac >= pocet_hracu) {
      ikony[0] = 0;
      for (f=2;f<8;f++) ikony[f] = 0;
      ikony[1] = 6;
      *aktiv = 1;
      *aktiv2 = 0;
    } else {
      *aktiv = 6;
      *aktiv2=level/32+2;
    }
  } else {
    for (f=0;f<10;f++) ikony[f]=iko2[f];
    *aktiv2=level/32+2;
    if (*aktiv2==5) {
      if (udelano[level] > 2) {
        ikony[0] = 0;
        ikony[6] = 0;
	*aktiv = 1;
      } else *aktiv = 6;
    } else {
      ikony[0] = 0;
      if (udelano[level] == 0||udelano[level]==2) {
        ikony[1] = 0;
        if (udelano[level]==0) {
	  ikony[6] = 0;
          *aktiv = *aktiv2;
	} else *aktiv = 6;
      } else *aktiv = 6;
    }
  }
  for (f=0;f<9;f++) {
    if (ikony[f]!=0) {
      if (f<3||f>5) if (f<3) vputimage(8+f*52,165,44,26,mnu+(1144*f));else vputimage(8+(f-3)*52,165,44,26,mnu+(1144*(f-3)));
      if (*aktiv == f) {
        if (tab>2) popis_ikonu(f,ikony[f],254,253); else popis_ikonu(f,ikony[f],252,253);
      } else if (*aktiv2 == f) popis_ikonu(f,ikony[f],252,253); else popis_ikonu(f,ikony[f],249,250);
    } else if (f<3||f>5) if (f<3) zobraz_menu(8+f*52,165,44,26);else zobraz_menu(8+(f-3)*52,165,44,26);
  }
}

void nakresli_okno1(UC tab) {
UC f,b;
  zobraz_menu(9,18,73,130);
  b = 250;
  for (f=0;f<16;f++) {
    if (hrac == f) {
      if (tab == 1) {
        b = 251;
        fill(9,18+8*f,73,8,43);
        col256=80;
      } else col256=56;
      vline(9,18+8*f,81,18+8*f);
      vline(9,25+8*f,81,25+8*f);
      vline(9,18+8*f,9,25+8*f);
      vline(81,18+8*f,81,25+8*f);
    }
    if (pocet_hracu > f) {
      vgotoxy(10,19+8*f);
      vwrite2(249,b,1,hraci[f].jmeno);
    }
  }
}


void nakresli_okno2(UC tab) {
UC f,g,b;
UC as[4];
  zobraz_menu(97,18,214,130);
  g = ((level)/32)*32;
  for (f=0;f<16;f++,g++) {
    if (pocet_hracu > hrac) {
      b = 250;
      if (level == g) {
        if (tab == 2) {
          b = 251;
          fill(97,18+8*f,106,8,43);
          col256=80;
        } else col256=56;
        vline(97,18+8*f,202,18+8*f);
        vline(97,25+8*f,202,25+8*f);
        vline(97,18+8*f,97,25+8*f);
        vline(202,18+8*f,202,25+8*f);
      }
      if ((udelano[g]==1)||(udelano[g]==255)||(udelano[g]==2)) {
        vgotoxy(98,19+8*f);
        if (g<96) itoa(g+1,as,10); else itoa(g-95,as,10);
        if (udelano[g]==1) vwrite2(249,b,1,as); else {
          col256 = 250;
          vwritec(as);
	}
	vgotoxy(113,19+8*f);
	if (udelano[g] == 1) vwrite2(249,b,1,levlyjm[g]); else {
          col256 = 250;
	  vwritec(levlyjm[g]);
	}
      }
      b = 250;
      if (level == g+16) {
        if (tab == 2) {
          b = 251;
          fill(205,18+8*f,106,8,43);
          col256=80;
        } else col256=56;
        vline(205,18+8*f,310,18+8*f);
        vline(205,25+8*f,310,25+8*f);
        vline(205,18+8*f,205,25+8*f);
        vline(310,18+8*f,310,25+8*f);
      }
      if ((udelano[g+16]==1)||(udelano[g+16]==255)||(udelano[g+16]==2)) {
        vgotoxy(206,19+8*f);
        if (g+16<96) itoa(g+17,as,10); else itoa(g+16-95,as,10);
        if (udelano[g+16]==1) vwrite2(249,b,1,as); else {
          col256 = 250;
          vwritec(as);
        }
	vgotoxy(221,19+8*f);
	if (udelano[g+16] == 1) vwrite2(249,b,1,levlyjm[g+16]); else {
	  col256 = 250;
	  vwritec(levlyjm[g+16]);
        }
      }
    }
  }
}

void oznac_polozku(UC ikony[9],UC aktiv,UC aktiv2,UC tab) {
  UC a,b;
  UC as[4];

  if (pocet_hracu > hrac) {
    a = level%32;
    if (a<16) {
      if (tab == 2) {
        b = 251;
        fill(97,18+8*a,106,8,43);
        col256=80;
      } else {
        col256=56;
        b=250;
      }
      vline(97,18+8*a,202,18+8*a);
      vline(97,25+8*a,202,25+8*a);
      vline(97,18+8*a,97,25+8*a);
      vline(202,18+8*a,202,25+8*a);
      if ((udelano[level]==1)||(udelano[level]==255)||(udelano[level]==2)) {
	vgotoxy(98,19+8*a);
	if (level<96) itoa(level+1,as,10); else itoa(level-95,as,10);
	if (udelano[level]==1) vwrite2(249,b,1,as); else {
          col256 = 250;
          vwritec(as);
        }
        vgotoxy(113,19+8*a);
	if (udelano[level] == 1) vwrite2(249,b,1,levlyjm[level]); else {
          col256 = 250;
          vwritec(levlyjm[level]);
	}
      }
    } else {
      if (tab == 2) {
        b = 251;
        fill(205,18+8*(a-16),106,8,43);
        col256=80;
      } else {
        col256=56;
        b=250;
      }
      vline(205,18+8*(a-16),310,18+8*(a-16));
      vline(205,25+8*(a-16),310,25+8*(a-16));
      vline(205,18+8*(a-16),205,25+8*(a-16));
      vline(310,18+8*(a-16),310,25+8*(a-16));
      if ((udelano[level]==1)||(udelano[level]==255)||(udelano[level]==2)) {
        vgotoxy(206,19+8*(a-16));
	if (level<96) itoa(level+1,as,10); else itoa(level-95,as,10);
        if (udelano[level]==1) vwrite2(249,b,1,as); else {
	  col256 = 250;
          vwritec(as);
        }
        vgotoxy(221,19+8*(a-16));
        if (udelano[level] == 1) vwrite2(249,b,1,levlyjm[level]); else {
	  col256 = 250;
	  vwritec(levlyjm[level]);
        }
      }
    }
  }
  if (tab == 1) {
    b = 251;
    fill(9,18+8*hrac,73,8,43);
    col256=80;
  } else {
    col256=56;
    b=250;
  }
  vline(9,18+8*hrac,81,18+8*hrac);
  vline(9,25+8*hrac,81,25+8*hrac);
  vline(9,18+8*hrac,9,25+8*hrac);
  vline(81,18+8*hrac,81,25+8*hrac);
  if (pocet_hracu > hrac) {
    vgotoxy(10,19+8*hrac);
    vwrite2(249,250,1,hraci[hrac].jmeno);
  }
  if (aktiv2!=0) popis_ikonu(aktiv2,ikony[aktiv2],252,253);
  if (tab>2) popis_ikonu(aktiv,ikony[aktiv],254,253);else popis_ikonu(aktiv,ikony[aktiv],252,253);
}

void odoznac_polozku(UC ikony[9],UC aktiv,UC aktiv2) {
  UC a;
  UC as[4];

  if (pocet_hracu > hrac) {
    a = level%32;
    if (a<16) {
      zobraz_menu(97,18+a*8,106,9);
      if ((udelano[level]==1)||(udelano[level]==255)||(udelano[level]==2)) {
	vgotoxy(98,19+8*a);
        if (level<96) itoa(level+1,as,10); else itoa(level-95,as,10);
        if (udelano[level]==1) vwrite2(249,250,1,as); else {
          col256 = 250;
          vwritec(as);
	}
        vgotoxy(113,19+8*a);
        if (udelano[level] == 1) vwrite2(249,250,1,levlyjm[level]); else {
          col256 = 250;
	  vwritec(levlyjm[level]);
        }
      }
    } else {
      zobraz_menu(205,18+(a-16)*8,106,9);
      if ((udelano[level]==1)||(udelano[level]==255)||(udelano[level]==2)) {
        vgotoxy(206,19+8*(a-16));
	if (level<96) itoa(level+1,as,10); else itoa(level-95,as,10);
	if (udelano[level]==1) vwrite2(249,250,1,as); else {
	  col256 = 250;
	  vwritec(as);
	}
        vgotoxy(221,19+8*(a-16));
        if (udelano[level] == 1) vwrite2(249,250,1,levlyjm[level]); else {
          col256 = 250;
          vwritec(levlyjm[level]);
	}
      }
    }
  }
  zobraz_menu(9,18+8*hrac,73,9);
  if (pocet_hracu > hrac) {
    vgotoxy(10,19+8*hrac);
    vwrite2(249,250,1,hraci[hrac].jmeno);
  }
  if (aktiv2!=0) popis_ikonu(aktiv2,ikony[aktiv2],249,250);
  popis_ikonu(aktiv,ikony[aktiv],249,250);
}

void nakresli_ikonu(UI x, UI y,UI dx) {
  UI f;
  if (graphmode) {
    col256=115;
    vline(x+1,y,x+dx-2,y);
    vline(x,y+1,x,y+9);
    col256=60;
    vline(x+1,y+10,x+dx-2,y+10);
    vline(x+dx-1,y+1,x+dx-1,y+9);
    vputpixelb(x,y,104);
    vputpixelb(x+dx-1,y,89);
    vputpixelb(x,y+10,89);
    vputpixelb(x+dx-1,y+10,32);
    fill(x+1,y+1,dx-2,9,89);
  } else {
    for (f=x+1;f<x+dx-1;f++) putp(f,y,115);
    for (f=y+1;f<y+10;f++) putp(x,f,115);
    for (f=x+1;f<x+dx-1;f++) putp(f,y+10,60);
    for (f=y+1;f<y+10;f++) putp(x+dx-1,f,60);
    putp(x,y,104);
    putp(x+dx-1,y,89);
    putp(x,y+10,89);
    putp(x+dx-1,y+10,32);
    sfill(x+1,y+1,dx-2,9,89);
  }
}

void nakresli_cfg_ikonu(UI x, UI y,UI dx) {
  col256=33;
  vline(x+1,y,x+dx-2,y);
  vline(x,y+1,x,y+5);
  col256=11;
  vline(x+1,y+6,x+dx-2,y+6);
  vline(x+dx-1,y+1,x+dx-1,y+5);
  vputpixelb(x,y,49);
  vputpixelb(x+dx-1,y,20);
  vputpixelb(x,y+6,20);
  vputpixelb(x+dx-1,y+6,1);
  fill(x+1,y+1,dx-2,5,20);
}

void oznac_cfg(UC x,UC y,UC b1,UC b2) {
  UI l;
  if (cfg_ikony[x][y].t) {
    if (cfg_ikony[x][y].t<18) {
      vgotoxy(44+x*119+2,33+y*8);
      vwrite2(b1,b2,1,cfg_ikony[x][y].jm);
      if (scancodchr[cfg_ikony2[x][y]]>=200) {
        vgotoxy(44+x*119+111-vlength(scancodtxt[scancodchr[cfg_ikony2[x][y]]-200]),33+y*8);
        vwrite2(b1,b2,1,scancodtxt[scancodchr[cfg_ikony2[x][y]]-200]);
      } else {
        l=scancodchr[cfg_ikony2[x][y]];
        vgotoxy(44+x*119+111-vlength((UC*)&l),33+y*8);
        vwrite2(b1,b2,1,(UC*)&l);
      }
    } else if (cfg_ikony[x][y].t<21) {
      if (y==10) vgotoxy(60+x*119,32+y*8);
      else if (x==0) vgotoxy(44+21,53+y*8);
      else vgotoxy(44+x*179+10,53+y*8);
      vwrite2(b1,b2,1,cfg_ikony[x][y].jm);
    } else{
      if (x==0) vgotoxy(44,38+y*8);
      else vgotoxy(276-vlength(cfg_ikony[x][y].jm),38+y*8);
      col256=b2;
      vwritec(cfg_ikony[x][y].jm);
    }
  }
}

void oznac_pruh(UC xx) {
  UI x=52+xx*119+cfg_ikony2[xx][11]*6+3;
  UI y=137;
  vputpixelb(x,y-1,254);
  vputpixelb(x-1,y-1,252);
  vputpixelb(x+1,y-1,252);
  vputpixelb(x,y,252);
  vputpixelb(x-1,y,253);
  vputpixelb(x+1,y,253);
  vputpixelb(x,y+1,253);
  vputpixelb(x,y-2,252);
  oznac_cfg(xx,11,252,253);
}

void zmen_cfg(UC x,UC y) {
  if (cfg_ikony[x][y].t) {
    if (cfg_ikony[x][y].t<18) {
      nakresli_cfg_ikonu(44+x*119,33+y*8,113);
    } else if (cfg_ikony[x][y].t<21) {
      if (y==10) nakresli_ikonu(52+x*119,29+y*8,98);
      else nakresli_ikonu(44+x*179,50+y*8,53);
    } else {
      fill(52+x*119,45+y*8,103,8,0);
      vputimage(44+x*119,47+y*8,2,4,pretprh);
      vputimage(44+x*119+111,47+y*8,2,4,pretprh[1]);
      col256=33;
      vline(46+x*119,47+y*8,46+108+x*119,47+y*8);
      col256=20;
      vline(46+x*119,48+y*8,46+108+x*119,48+y*8);
      vline(46+x*119,49+y*8,46+108+x*119,49+y*8);
      col256=11;
      vline(46+x*119,50+y*8,46+108+x*119,50+y*8);
      vputimage(52+x*119+cfg_ikony2[x][y]*6,45+y*8,7,8,prettl);
    }
    oznac_cfg(x,y,249,251);
  }
}

void nakresli_dialogove_okno(UC okno) {
  UI f,g;
  UC m[7][30] = {"Napi— sv‘ jm‘no :","Napi— nov‘ jm‘no :","Smazat hrŽe ?",
            "Vyskoit do DOSu ?","Smazat level ?","Editovat bez my—i nelze !",
            "Pouœ“vŽ— 1 klŽvesu 2krŽt !"};

  if (graphmode) {
    fill(90,60,140,50,0);
    for (f=90;f<230;f+=10) {
      vputimage(f,50,10,10,ram[4]);
      vputimage(f,110,10,10,ram[4]);
    }
    for (f=60;f<110;f+=10) {
      vputimage(80,f,10,10,ram[5]);
      vputimage(230,f,10,10,ram[5]);
    }
    vputimage(80,50,10,10,ram[0]);
    vputimage(80,110,10,10,ram[1]);
    vputimage(230,50,10,10,ram[2]);
    vputimage(230,110,10,10,ram[3]);
    if (okno<3) {
      vgotoxy(95,65);
      vwrite2hc(249,250,1,m[okno-1]);
      fill(95,75,130,10,24);
    } else {
      vgotoxy(160-vlength(m[okno-1])/2,72);
      vwrite2hc(249,250,1,m[okno-1]);
    }
    if (okno<6) {
      nakresli_ikonu(95,93,40);
      nakresli_ikonu(185,93,40);
      if (okno==2) {
        nakresli_ikonu(140,93,40);
        vgotoxy(98,96);
        vwrite2(252,253,1,"CHANGE");
        vgotoxy(188,96);
        vwrite2(249,251,1,"CANCEL");
        vgotoxy(151,96);
        vwrite2(249,251,1,"NEW");
      } else {
        vgotoxy(109,96);
        vwrite2(252,253,1,"OK");
        vgotoxy(188,96);
        vwrite2(249,251,1,"CANCEL");
      }
    } else {
      nakresli_ikonu(140,93,40);
      vgotoxy(154,96);
      vwrite2(252,253,1,"OK");
    }
  } else {
    sfill(90+menu_x,60,140,50,0);
    for (f=90+menu_x;f<230+menu_x;f+=10) {
      sputimage(f,50,10,10,ram[4]);
      sputimage(f,110,10,10,ram[4]);
    }
    for (f=60;f<110;f+=10) {
      sputimage(80+menu_x,f,10,10,ram[5]);
      sputimage(230+menu_x,f,10,10,ram[5]);
    }
    sputimage(80+menu_x,50,10,10,ram[0]);
    sputimage(80+menu_x,110,10,10,ram[1]);
    sputimage(230+menu_x,50,10,10,ram[2]);
    sputimage(230+menu_x,110,10,10,ram[3]);
    v4write2(160+menu_x-vlength(m[okno-1])/2,72,249,250,1,m[okno-1]);
    nakresli_ikonu(95+menu_x,93,40);
    nakresli_ikonu(185+menu_x,93,40);
    v4write2(109+menu_x,96,252,253,1,"OK");
    v4write2(188+menu_x,96,249,251,1,"CANCEL");
  }
}

void nakresli_konfiguracni_okno() {
  UI f,g;
  fill(40,30,240,130,0);
  for (f=40;f<280;f+=10) {
    vputimage(f,20,10,10,ram[4]);
    vputimage(f,160,10,10,ram[4]);
  }
  for (f=30;f<160;f+=10) {
    vputimage(30,f,10,10,ram[5]);
    vputimage(280,f,10,10,ram[5]);
  }
  vputimage(30,20,10,10,ram[0]);
  vputimage(30,160,10,10,ram[1]);
  vputimage(280,20,10,10,ram[2]);
  vputimage(280,160,10,10,ram[3]);
  vgotoxy(160-vlength(hraci[hrac].jmeno)/2,149);
  vwrite2(249,250,1,hraci[hrac].jmeno);

  for (f=0;f<2;f++) for (g=0;g<13;g++) zmen_cfg(f,g);
}

#define X 100
#define Y 77

void smaz(UC x1,UC x2) {
  UI xx1,dx2;
  UC f;

  if (x2 >= x1) {
    xx1 = X+(6)*(x1-1);
    dx2 = (x2-x1)*(6)+5;
    fill(xx1,Y,dx2,7,24);
  }
}

void pis(UC x1,UC x2,UC text[20],UC pz) {
  UC text2[20];
  UC f,g;
  if (x2 >= x1) {
    smaz(x1,x2);
    if (x1 <= pz) {
      vgotoxy(X+(6)*(x1-1),Y);
      for (f=x1-1,g=0;f<x2;f++,g++) {
	text2[g] = text[f];
        text2[g+1] = 0;
      }
      vwrite2(249,250,0,text2);
    }
  }
}

void oznac_polozku2(UC tab,UC okno,UC cx,UC text[20],UC pz) {
  if (okno<3) pis(cx,cx,text,pz);
  if (okno==2) {
    vgotoxy(98,96);
    if (tab==2) vwrite2(254,253,1,"CHANGE"); else if (tab==1) vwrite2(252,253,1,"CHANGE"); else vwrite2(249,251,1,"CHANGE");
    vgotoxy(188,96);
    if (tab==4) vwrite2(254,253,1,"CANCEL"); else vwrite2(249,251,1,"CANCEL");
    vgotoxy(151,96);
    if (tab==3) vwrite2(254,253,1,"NEW"); else vwrite2(249,251,1,"NEW");
  } else {
    if (graphmode) {
      vgotoxy(109,96);
      if (tab==2) vwrite2(254,253,1,"OK"); else if (tab==1) vwrite2(252,253,1,"OK");else vwrite2(249,251,1,"OK");
      vgotoxy(188,96);
      if (tab==4) vwrite2(254,253,1,"CANCEL"); else vwrite2(249,251,1,"CANCEL");
    } else {
      if (tab==2) v4write2(109+menu_x,96,254,253,1,"OK"); else if (tab==1) v4write2(109+menu_x,96,252,253,1,"OK");
      else v4write2(109+menu_x,96,249,251,1,"OK");
      if (tab==4) v4write2(188+menu_x,96,254,253,1,"CANCEL"); else v4write2(188+menu_x,96,249,251,1,"CANCEL");
    }
  }
}

void dialogove_okno(UC okno) {
  UC text[20];
  UC kl;
  UC pz,cx;
  UI citac;
  UC *ins;
  UC max=12;
  UC tab;

  if (!graphmode) sgetimage(80+menu_x,50,160,70,buffer);
  nakresli_dialogove_okno(okno);

  if (okno>5) {
    while (kbhit()) getch();
    do mysuj(); while (!(mystl||kbhit()));
    while (kbhit()) getch();
    if (okno==7) fill(80,50,160,70,0);
    else zobraz_menu(80,50,160,70);
    return;
  }

  if (okno<3) tab = 1; else tab=2;
  ins = MK_FP(0,0x0417);
  if (okno==2) {
    pz = 0;
    while (hraci[hrac].jmeno[pz] != 0) {
      text[pz] = hraci[hrac].jmeno[pz];
      pz++;
    }
    text[pz] = 0;
  } else {
    pz = 0;
    text[0] = 0;
  }
  pis(1,pz,text,pz);
  cx = pz+1;
  kl = 'j';
  while (kbhit()) getch();
  do {
    mysuj();
    if (tab==1) {
      citac++;
      if (citac % 20 == 0) pis(cx,cx,text,pz);
      if (citac % 20 == 10) {
	 if (*ins < 128) fill(X+(cx-1)*(6),Y+6,5,1,149); else
	 fill(X+(cx-1)*(6),Y,5,7,149);
      }
    }
    if (mystl>0) {
      if (((mysx>=X)&&(mysx<225))&&((mysy>74)&&(mysy<85))) {
        if (okno<3) {
          pis(cx,cx,text,pz);
          tab=1;
	  citac = 9;
	  cx = (mysx-X)/6+1;
          if (cx>pz+1) cx = pz+1;
	  oznac_polozku2(tab,okno,cx,text,pz);
	}
      } else if ((mysy>=93)&&(mysy<104)) {
	UC f;
        for (f=0;f<3;f++) if ((mysx>94+f*45)&&(mysx<135+f*45)) {
          if ((f!=1)||(okno==2)) {
	    tab=f+2;
	    oznac_polozku2(tab,okno,cx,text,pz);
            kl=13;
          }
	}
      }
    }
    if (kbhit()) {
      kl = getch();
      if (kl == 0) {
        kl = getch();
	if (kl==75) {
	  if (tab==1) {
            if (cx > 1) {
	      pis(cx,cx,text,pz);
	      cx--;
              citac = 9;
	    }
          } else if (tab>2) {
            if (okno==2) tab--; else tab -=2;
	    oznac_polozku2(tab,okno,cx,text,pz);
	  }
        } else if (kl==77) {
	  if (tab==1) {
	    if (cx < pz+1) {
              pis(cx,cx,text,pz);
              cx++;
              citac = 9;
            }
          } else if (tab<4) {
            if (okno==2) tab++; else tab+=2;
	    oznac_polozku2(tab,okno,cx,text,pz);
	  }
        } else if (tab == 1) {
	  if (kl == 71) {
	    pis(cx,cx,text,pz);
            cx = 1;
            citac = 9;
          } else if (kl==79) {
	    pis(cx,cx,text,pz);
	    cx = pz+1;
            citac = 9;
	  } else if (kl==83) {
            if (cx <= pz) {
	      UC f;
              f=cx-1;
              do {
		f++;
                text[f-1]=text[f];
              } while (text[f]!=0);
              smaz(pz,pz);
	      pz--;
	      pis(cx,pz,text,pz);
            }
	  }
	}
      } else if (kl == 8) {
        if (cx > 1) {
	  UC f;
          pis(cx,cx,text,pz);
	  cx--;
	  f=cx-1;
          do {
            f++;
	    text[f-1]=text[f];
          } while (text[f]!=0);
          smaz(pz,pz);
          pz--;
          pis(cx,pz,text,pz);
        }
      } else if (kl == 9) {
	tab++;
	if ((tab==3)&&(okno!=2)) tab++;
	if (tab==5) if (okno<3) tab = 1; else tab=2;
	oznac_polozku2(tab,okno,cx,text,pz);
      } else if (tab==1) {
        if ((kl > 31) && (kl < 123)) {
	  if (cx<=max) {
            if (*ins < 128) {
              UC f;
	      f=pz+1;
              do {
                f--;
                text[f+1]=text[f];
	      } while (f>cx-1);
              text[cx-1] =kl;
	      if (pz == max) text[pz]=0; else pz++;
              cx++;
              pis(cx-1,pz,text,pz);
            } else {
              text[cx-1] = kl;
	      if (cx > pz) {
		if (pz != max) pz++;
                text[pz] = 0;
	      }
	      pis(cx,cx,text,pz);
	      cx++;
            }
          }
	}
      }
    }
  } while ((kl != 27) && (kl != 13));
  if (kl==13) {
    if (((tab<3) && (okno==1))||(tab==3)) {
      if(!zalohuj_sejv) zalohuj_sejv=1;
      setmem(hraci[hrac].jmeno,13,random(255)+1);
      strcpy(hraci[hrac].jmeno,text);
      if (strcmp(text,cheet)==0) {
	memset(hraci[hrac].data,255,12);
	strcpy(hraci[hrac].jmeno,chname);
      } else memset(hraci[hrac].data,0,12);
      hraci[hrac].crc=0;
      hraci[hrac].pocet=0;
      hraci[hrac].level=0;
      hraci[hrac].hlasitost=170;
      memcpy(hraci[hrac].klavesy,klavesy,17);
      if (okno==1) pocet_hracu++;
      zmen_hrace();
    } else if (tab<3) {
      if (okno==2) {
        if(!zalohuj_sejv) zalohuj_sejv=1;
        strcpy(hraci[hrac].jmeno,text);
      } else if (okno==3) {
        UC f;
        if(!zalohuj_sejv) zalohuj_sejv=1;
        if (pocet_hracu!=0) for(f=hrac;f<pocet_hracu-1;f++) {
	  hraci[f]=hraci[f+1];
        }
	pocet_hracu--;
	zmen_hrace();
      } else if (okno==4) {
        if (typdonemenu==1&&graphmode==0) level++;
        zatmi();
	sejvni();
	done256();
	exit(0);
      }
    }
  }
  if (graphmode) zobraz_menu(80,50,160,70);
  else sputimage(80+menu_x,50,160,70,buffer);
}

void config_mnu() {
  UC kl;
  UI citac;
  UC x,y;
  UC f,g;
  UC konec=1;
  UC tab=0;

  for (f=0;f<2;f++) for (g=0;g<13;g++) {
    if (cfg_ikony[f][g].t&&cfg_ikony[f][g].t<18) cfg_ikony2[f][g]=klavesy[cfg_ikony[f][g].t-1];
    if (cfg_ikony[f][g].t==21) cfg_ikony2[f][g]=hraci[hrac].hlasitost/16;
    if (cfg_ikony[f][g].t==22) cfg_ikony2[f][g]=hraci[hrac].hlasitost%16;
  }
  nakresli_konfiguracni_okno();

  x=0;
  y=0;
  oznac_cfg(x,y,252,253);
  do {
    do {
      mysuj();
    } while (!kbhit()&&mystl==0);
    if (kbhit()) kl=getch();
    if (mystl) {
      UC a=5,b;
      kl=200;
      if (mysy>=32&&mysy<120) {
        if (mysx>160&&mysy>=109) {
          if (mysx>=171&&mysx<=268) {
            a=1;
	    b=10;
          }
        } else if (mysx>=44&&mysx<=156) {
          b = (mysy-32)/8;
          if (cfg_ikony[0][b].t) a=0;
        } else if (mysx>=163&&mysx<=275) {
          b = (mysy-32)/8;
          if (cfg_ikony[1][b].t) a=1;
        }
      } else if (mysy>=146&&mysy<157) {
        b=12;
        if (mysx>=44&&mysx<=96) a=0;
        if (mysx>=223&&mysx<=275) a=1;
      } else if (mysy>=126&&mysy<=140) {
	int aa=mysx;
	b=11;
	if (mysx>=44&&mysx<=156) {
	  a=0;
	  zmacknuto=0;
	  aa=(aa-52)/6;
	  if (aa<0) aa=0;
	  if (aa>15) aa=15;
	  if (aa!=cfg_ikony2[0][11]){
	    if (PLAYER) {
	      midasSD->SetMasterVolume(aa*20>63?63:aa*20);
	      midasSD->SetAmplification(aa*20>63?aa*20:64);
	    }
	  }
	  cfg_ikony2[0][11]=aa;
	} else if (mysx>=163&&mysx<=275) {
	  a=1;
	  zmacknuto=0;
	  aa=(aa-171)/6;
	  if (aa<0) aa=0;
	  if (aa>15) aa=15;
	  cfg_ikony2[1][11]=aa;
	}
      }
      if (a<2) {
	if (b==11) kl=201; else kl=13;
	if (tab&&(x!=a||y!=b)) {
	  tab=0;
	  zmen_cfg(x,y);
	} else oznac_cfg(x,y,249,251);
	x=a;
	y=b;
	if (b==11) {
	  zmen_cfg(x,y);
	  if (tab) oznac_pruh(x);
	}
	oznac_cfg(x,y,252,253);
      }
      if (kl==200&&tab) {
	tab=0;
	zmen_cfg(x,y);
	oznac_cfg(x,y,252,253);
      }
    }
    if (kl==72&&y>0) {
      if (tab) {
	tab=0;
	zmen_cfg(x,y);
      } else oznac_cfg(x,y,249,251);
      y--;
      if (!cfg_ikony[x][y].t) y--;
      oznac_cfg(x,y,252,253);
    } else if (kl==80&&y<12) {
      if (tab) {
	tab=0;
	zmen_cfg(x,y);
      } else oznac_cfg(x,y,249,251);
      y++;
      if (!cfg_ikony[x][y].t) y++;
      oznac_cfg(x,y,252,253);
    } else if (kl==77) {
      if (tab==0&&x==0) {
	oznac_cfg(x,y,249,251);
	x++;
	if (!cfg_ikony[x][y].t) y++;
	oznac_cfg(x,y,252,253);
      } else if (tab&&cfg_ikony2[x][y]<15) {
	cfg_ikony2[x][y]++;
	if (PLAYER) {
	  midasSD->SetMasterVolume(cfg_ikony2[x][11]*20>63?63:cfg_ikony2[x][11]*20);
	  midasSD->SetAmplification(cfg_ikony2[x][11]*20>63?cfg_ikony2[x][11]*20:64);
	}
	zmen_cfg(x,y);
	oznac_pruh(x);
      }
    } else if (kl==75) {
      if (tab==0&&x) {
	oznac_cfg(x,y,249,251);
	x--;
	if (!cfg_ikony[x][y].t) y++;
	oznac_cfg(x,y,252,253);
      } else if (tab&&cfg_ikony2[x][y]>0) {
	cfg_ikony2[x][y]--;
	if (PLAYER) {
	  midasSD->SetMasterVolume(cfg_ikony2[x][11]*20>63?63:cfg_ikony2[x][11]*20);
	  midasSD->SetAmplification(cfg_ikony2[x][11]*20>63?cfg_ikony2[x][11]*20:64);
	}
	zmen_cfg(x,y);
	oznac_pruh(x);
      }
    } else if (kl==27) if (tab) {
      tab=0;
      zmen_cfg(x,y);
      oznac_cfg(x,y,252,253);
    } else konec=0;
    if (kl==13) {
      UC a,b=0,c;
      if (tab) {
        tab=0;
        zmen_cfg(x,y);
	oznac_cfg(x,y,252,253);
      } else if (cfg_ikony[x][y].t<18) {
        c=a=inportb(0x60);
        do {
          vr();
          if (b%16==0) nakresli_cfg_ikonu(44+x*119,33+y*8,113);
          else if (b%16==8) {
            vgotoxy(45+x*119+2,33+y*8);
            vwrite2(252,253,1,"<= ZMACKNI KLAVESU =>");
          }
          b++;
          if (c!=a) a=c;
	  c = inportb(0x60);
        } while (c==a||c>88||scancodchr[c]==255);
        if (c!=1) cfg_ikony2[x][y]=c;
        nakresli_cfg_ikonu(44+x*119,33+y*8,113);
        oznac_cfg(x,y,252,253);
        while(kbhit()) getch();
      } else if (cfg_ikony[x][y].t==18) {
        a=0;
	for (f=0;f<2;f++) for (g=0;g<11;g++) if (cfg_ikony[f][g].t&&cfg_ikony[f][g].t<18) {
          if (f==1) {
            for (b=0;b<11;b++) if (cfg_ikony[0][b].t&&cfg_ikony[0][b].t<18&&cfg_ikony2[f][g]==cfg_ikony2[0][b]) a=1;
            for (b=0;b<g;b++) if (cfg_ikony[1][b].t&&cfg_ikony[1][b].t<18&&cfg_ikony2[f][g]==cfg_ikony2[1][b]) a=1;
          } else for (b=0;b<g;b++) if (cfg_ikony[0][b].t&&cfg_ikony[0][b].t<18&&cfg_ikony2[f][g]==cfg_ikony2[0][b]) a=1;
        }
	if (a) {
	  dialogove_okno(7);
	  for (f=0;f<2;f++) for (g=2;g<11;g++) zmen_cfg(f,g);
	} else konec=0;
      } else if (cfg_ikony[x][y].t==19) {
	kl=27;
	konec=0;
      } else if (cfg_ikony[x][y].t==20) {
	UC klavesy[17];
	FILE *s;
	s = fopen("menu.dat","rb");
	fseek(s,103918,0);
	fread(klavesy,1,17,s);
	fclose(s);
	for (f=0;f<2;f++) for (g=0;g<11;g++) if (cfg_ikony[f][g].t&&cfg_ikony[f][g].t<18) {
	  cfg_ikony2[f][g]=klavesy[cfg_ikony[f][g].t-1];
	  zmen_cfg(f,g);
	}
      } else {
	tab=1;
	oznac_pruh(x);
      }
    }
  } while (konec);
  if (kl==13) {
    if(!zalohuj_sejv) zalohuj_sejv=1;
    for (f=0;f<2;f++) for (g=0;g<13;g++) {
      if (cfg_ikony[f][g].t&&cfg_ikony[f][g].t<18) klavesy[cfg_ikony[f][g].t-1]=hraci[hrac].klavesy[cfg_ikony[f][g].t-1]=cfg_ikony2[f][g];
      if (cfg_ikony[f][g].t==21) hraci[hrac].hlasitost=cfg_ikony2[f][g]*16;
      if (cfg_ikony[f][g].t==22) hraci[hrac].hlasitost=cfg_ikony2[f][g]+hraci[hrac].hlasitost;
    }
  }
  if (PLAYER) {
    midasSD->SetMasterVolume((hraci[hrac].hlasitost>>4)*20>63?63:(hraci[hrac].hlasitost>>4)*20);
    midasSD->SetAmplification((hraci[hrac].hlasitost>>4)*20>63?(hraci[hrac].hlasitost>>4)*20:64);
  }
  zobraz_menu(30,20,100,150);
  zobraz_menu(130,20,160,150);
}

void menu(){
  UC ikony[9];
  UC aktiv,aktiv2,tab;
  UC kl;
  UC f,g;
  UC bb;
  UC notplay;

  init_menu();
  if (hral_jsi_uz) tab=2; else tab = 1;
  aktiv=0;
  ikony[0]=80;
  notplay=1;
  nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
  nakresli_okno1(tab);
  nakresli_okno2(tab);
  mreset();
  while (kbhit()) getch();
  do {
    do {
      mysuj();
    } while (!kbhit()&&mystl==0);
    if (kbhit()) kl = getch();
    if (mystl) {
      kl=200;
      if (mysy>=18&&mysy<=145) {
        if (mysx>=9&&mysx<=81) {
          if (mysy<=25+pocet_hracu*8) {
            if (tab==1&&hrac==(mysy-18)/8&&dblclk==200) {
              odoznac_polozku(ikony,aktiv,aktiv2);
              tab+=2;
              aktiv=1;
              kl=13;
              oznac_polozku(ikony,aktiv,aktiv2,tab);
            } else {
              odoznac_polozku(ikony,aktiv,aktiv2);
	      uloz_hrace();
              hrac=(mysy-18)/8;
              tab=1;
	      zmen_hrace();
	      nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	      nakresli_okno2(tab);
	      oznac_polozku(ikony,aktiv,aktiv2,tab);
            }
          } else if (tab!=1) {
            odoznac_polozku(ikony,aktiv,aktiv2);
            tab=1;
	    nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	    oznac_polozku(ikony,aktiv,aktiv2,tab);
          }
        } else if (mysx>=97&&mysx<=202&&hrac<pocet_hracu) {
          if (tab==2&&dblclk==200&&level==(level/32)*32+(mysy-18)/8) kl=13; else {
            odoznac_polozku(ikony,aktiv,aktiv2);
            level=(level/32)*32+(mysy-18)/8;
            tab=2;
	    nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	    oznac_polozku(ikony,aktiv,aktiv2,tab);
          }
        } else if (mysx>=205&&mysx<=310&&hrac<pocet_hracu) {
          if (tab==2&&dblclk==200&&level==(level/32)*32+(mysy-18)/8+16) kl=13; else {
            odoznac_polozku(ikony,aktiv,aktiv2);
            level=(level/32)*32+(mysy-18)/8+16;
            tab=2;
	    nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	    oznac_polozku(ikony,aktiv,aktiv2,tab);
          }
        }
      } else {
        UC a=255,f;
        if(mysy>=165&&mysy<=190) {
          if (ikony[2]&&mysx>=112&&mysx<=155) {
            if (mysx<134) { if (mysy<178) a=2;else a=4;} else if (mysy<178) a=3;  else a=5;
          } else {
            for (f=6;f<9;f++) if (ikony[f]&&mysx>=(f-3)*52+8&&mysx<(f-3)*52+52) a=f;
            for (f=0;f<2;f++) if (ikony[f]&&mysx>=f*52+8&&mysx<f*52+52) a=f;
          }
        }
        if (a<9) {
          odoznac_polozku(ikony,aktiv,aktiv2);
          aktiv=a;
          kl=13;
          oznac_polozku(ikony,aktiv,aktiv2,tab);
        }
      }
    }
    {
      UC a=255;
      if (kl==27||kl=='d'||kl=='D') a=8;else
      if (ikony[1]==8&&(kl=='e'||kl=='E')) a=1;else
      if (ikony[2]&&kl=='1') a=2;else
      if (ikony[2]&&kl=='2') a=3;else
      if (ikony[2]&&kl=='3') a=4;else
      if (ikony[2]&&kl=='4') a=5;
      if (a<9) {
        odoznac_polozku(ikony,aktiv,aktiv2);
        aktiv=a;
        kl=13;
        oznac_polozku(ikony,aktiv,aktiv2,tab);
      }
    }
    if (kl == 72) {
      if ((tab == 1)&&(hrac > 0)) {
	odoznac_polozku(ikony,aktiv,aktiv2);
	uloz_hrace();
	hrac--;
	if (hrac+1==pocet_hracu) nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	zmen_hrace();
        aktiv2=level/32+2;
	nakresli_okno2(tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      } else if (tab == 2) {
	bb=0;
	if (level%32==0) bb = 1;
	odoznac_polozku(ikony,aktiv,aktiv2);
	if (level==0) level = 127; else level--;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	if (bb) nakresli_okno2(tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      }
    } else if (kl == 80) {
      if ((tab == 1)&&(hrac < pocet_hracu)&&hrac<15) {
	odoznac_polozku(ikony,aktiv,aktiv2);
	uloz_hrace();
	hrac++;
	if (hrac==pocet_hracu) nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	zmen_hrace();
        if (aktiv2) aktiv2=level/32+2;
	nakresli_okno2(tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      } else if (tab == 2) {
	bb=0;
	if (level%32==31) bb = 1;
	odoznac_polozku(ikony,aktiv,aktiv2);
	if (level==127) level = 0; else level++;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	if (bb) nakresli_okno2(tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      }
    } else if (kl == 77) {
      if (tab>2) {
	bb = aktiv+1;
	while ((bb < 9)&&(ikony[bb]==0)) bb++;
	if (bb < 9) {
	  odoznac_polozku(ikony,aktiv,aktiv2);
	  aktiv = bb;
	  oznac_polozku(ikony,aktiv,aktiv2,tab);
	}
      } else if (pocet_hracu>hrac) {
	if (tab==1) {
   	  odoznac_polozku(ikony,aktiv,aktiv2);
          tab=2;
          nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	  oznac_polozku(ikony,aktiv,aktiv2,tab);
        } else if (level%32<16) {
   	  odoznac_polozku(ikony,aktiv,aktiv2);
          level+=16;
          nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	  oznac_polozku(ikony,aktiv,aktiv2,tab);
        }
      }
    } else if (kl == 75) {
      int bb;
      if (tab>2) {
	bb = aktiv-1;
	while ((bb >= 0)&&(ikony[bb]==0)) bb--;
	if (bb >= 0) {
	  odoznac_polozku(ikony,aktiv,aktiv2);
	  aktiv = bb;
	  oznac_polozku(ikony,aktiv,aktiv2,tab);
	}
      } else if (tab == 2) {
	odoznac_polozku(ikony,aktiv,aktiv2);
	if (level%32<16) tab = 1; else level -= 16;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      }
    } else if (kl==81) {
      if (level<96) level=((level+32)/32)*32+31; else level=31;
      while ((udelano[level-1]!=1)&&(level%32!=0)) level--;
      nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
      nakresli_okno2(tab);
    } else if (kl==73) {
      if (level>31) level=((level-32)/32)*32+31; else level=127;
      while ((udelano[level-1]!=1)&&(level%32!=0)) level--;
      nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
      nakresli_okno2(tab);
    } else if (kl == 9) {
      odoznac_polozku(ikony,aktiv,aktiv2);
      if (tab<3) tab+=2; else tab -= 2;
      oznac_polozku(ikony,aktiv,aktiv2,tab);
    } else if (kl == 13) {
      if (ikony[aktiv]==1) {
        // DelPl
	dialogove_okno(3);
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_okno2(tab);
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	nakresli_okno1(tab);
      } else if (ikony[aktiv]==2) {
        //ChgNm
	dialogove_okno(2);
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_okno2(tab);
	nakresli_okno1(tab);
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
      } else if (ikony[aktiv]==3) {
        //SelLv
	odoznac_polozku(ikony,aktiv,aktiv2);
	tab = 2;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	oznac_polozku(ikony,aktiv,aktiv2,tab);
      } else if (ikony[aktiv]==4) {
      //Config
	config_mnu();
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	nakresli_okno1(tab);
	nakresli_okno2(tab);
      } else if (ikony[aktiv]==5) {
        //Quit
	dialogove_okno(4);
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	nakresli_okno1(tab);
	nakresli_okno2(tab);
      } else if (ikony[aktiv]==6) {
      //NewPl
	dialogove_okno(1);
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_okno2(tab);
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	nakresli_okno1(tab);
      } else if (ikony[aktiv]==7) {
      //DelLv
	dialogove_okno(5);
	if (tab==3) tab = 1; else if (tab==4) tab =2;
	nakresli_okno2(tab);
	nakresli_okno1(tab);
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
      } else if (ikony[aktiv]==8) {
	// Edit
        if (mexists) {
	  zatmi();
	  sejvni();
	  lvledit(level);
	  uninitg();
	  init_menu();
	  tab=2;
	  aktiv=0;
	  ikony[0]=80;
	  notplay=1;
	  nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	  mreset();
        } else {
	  dialogove_okno(6);
	  if (tab==3) tab = 1; else if (tab==4) tab =2;
	  nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
        }
        nakresli_okno1(tab);
        nakresli_okno2(tab);
        while (kbhit()) getch();
      } else if (ikony[aktiv]<13) {
       //1E 2M 3H UD
	level = (ikony[aktiv]-8)*32-1;
	while ((udelano[level-1]!=1)&&(level%32!=0)) level--;
	nakresli_ikony(ikony,&aktiv,&aktiv2,tab);
	nakresli_okno2(tab);
      } else {
	notplay=0;
      }
    }
  } while (notplay);

  zatmi();
  vyskoc=2;
  sejvni();
  while (kbhit()) getch();

}

