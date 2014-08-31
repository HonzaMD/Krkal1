extern void PlayEffect(ushort instHandle, ulong rate, ushort volume,short panning);
extern void mreset() ;
extern void mysuj() ;
extern void nahraj_data();
extern void uv_obr();
extern void zatmi();
extern void dialogove_okno(UC okno) ;
extern void menu();
extern void interrupt handler();
extern UC alt,ctrl;

#define PREPINAC(x) (vec_1[x]&0x80)
#define LASER(x) (vec_1[x]&0x40)
#define VECI(x) (vec_1[x]&0x20)
#define ZRCADLA(x) (vec_1[x]&0x10)
#define STOP_LASER(x) (vec_1[x]&0x08)
#define PAS(x) (vec_1[x]&0x04)
#define DIRA(x) (vec_1[x]&0x02)
#define SOUPATELNA_VEC(x) (vec_1[x]&0x01)
#define DELO(x) (vec_2[x]&0x80)
#define ELEKTRODA(x) (vec_2[x]&0x40)
#define MAGNET(x) (vec_2[x]&0x20)
#define TY(x) (vec_2[x]&0x10)
#define POTVORY(x) (vec_2[x]&0x08)
#define NEVYBUCHUJE(x) (vec_2[x]&0x04)
#define KULATY(x) (vec_2[x]&0x02)
#define PADA(x) (vec_2[x]&0x01)


