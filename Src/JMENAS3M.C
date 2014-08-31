#include <dir.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>

int PS3M=0,PEFF=0;

long prvS3M,prvEFF;

char buf[55000];
long tmp;

char files[100][13];
long len[100];
FILE *s,*f;
struct {
  long int pos;
  unsigned char jmeno[28];
} hudby[100];

char filesef[20][13];
long lenef[20];
struct{
long pos,len;
}efekty[20];

void main()
{
  int i,j,k,l,handle;
  printf("\n\nCtu nazvy s3m ze souboru filess3m.dat...\n");
  if ((s = fopen("filess3m.dat","rb"))==NULL){
    printf("\nSoubor filess3m.dat neexistuje !\n");
    return;
  }
  i=filelength(fileno(s));
  fread(buf,i,1,s);
  fclose(s);
  if (memicmp(&buf[0],"s3m:",4)!=0){
    printf("\nChybi navesti 's3m:'!\n)");
    return;
  }
  j=4;
  k=-1;
  while (j<i)
  {
    l=0;
    while (buf[j]==10||buf[j]==13||buf[j]==','||buf[j]==' ') j++;
    while (buf[j]!=10&&buf[j]!=13&&buf[j]!=','&&buf[j]!=' '&&j<i){
      if (l==0) {k++;printf("\r            \r%3i:",k+1);}
      if (l>=8) {printf("\nPrilis dlouhy nazev s3m !\n");return;}
      files[k][l]=buf[j];
      printf("%c",buf[j]);
      l++;
      j++;
    }
  }
  PS3M=k+1;
  printf("\rPocet s3m: %i",PS3M);

  printf("\nCtu nazvy efektu ze souboru fileseff.dat...\n");
  if ((s = fopen("fileseff.dat","rb"))==NULL){
    printf("\nSoubor fileseff.dat neexistuje !\n");
    return;
  }
  i=filelength(fileno(s));
  fread(buf,i,1,s);
  fclose(s);
  if (memicmp(&buf[0],"efekty:",4)!=0){
    printf("\nChybi navesti 'efekty:'!\n)");
    return;
  }
  j=7;
  k=-1;
  while (j<i)
  {
    l=0;
    while (buf[j]==10||buf[j]==13||buf[j]==','||buf[j]==' ') j++;
    while (buf[j]!=10&&buf[j]!=13&&buf[j]!=','&&buf[j]!=' '&&j<i){
      if (l==0) {k++;printf("\r            \r%3i:",k+1);}
      if (l>=8) {printf("\nPrilis dlouhy nazev efektu !\n");return;}
      filesef[k][l]=buf[j];
      printf("%c",buf[j]);
      l++;
      j++;
    }
  }
  PEFF=k+1;
  printf("\rPocet efektu: %i\n",PEFF);

  prvS3M=14+PS3M*32+PEFF*8;

  for (i=0;i<PS3M;i++)
  {
    strcat(files[i],".s3m");
    printf("\rZjistuju delku s3m: %s             ",files[i]);
    if ((handle = open(files[i],O_RDONLY|O_BINARY))==-1)
    {
    printf("\nSoubor %s neexistuje !!",files[i]);
    return;
    }
    len[i]=filelength(handle);
    read(handle, hudby[i].jmeno, 28);
    close(handle);
//    memcpy(hudby[i].jmeno,files[i],13);
    if (i) hudby[i].pos=hudby[i-1].pos+len[i-1]; else hudby[i].pos=prvS3M;
  }
  prvEFF=hudby[PS3M-1].pos+len[PS3M-1];
  for (i=0;i<PEFF;i++)
  {
    strcat(filesef[i],".eff");
    printf("\rZjistuju delku efektu: %s          ",filesef[i]);
    if ((handle = open(filesef[i],O_RDONLY|O_BINARY))==-1)
    {
    printf("\nSoubor %s neexistuje !!",filesef[i]);
    return;
    }
    efekty[i].len=lenef[i]=filelength(handle);
    close(handle);
    if (i) efekty[i].pos=efekty[i-1].pos+lenef[i-1]; else efekty[i].pos=prvEFF;
  }

  printf("\rNacitam hlavicku...                              ");
  if ((s=fopen("hlavicka.","rb"))==NULL) {
    printf("\nSoubor hlavicka neexistuje !\n");
    return;
  }
  fread(buf,10,1,s);
  fclose(s);
  f = fopen("krkal.mus","w+b");
  printf("\rZapisuju hlavicku do krkal.mus                         ");
  fwrite(buf,10,1,f);
  tmp=14+PS3M*32;
  fwrite(&tmp,4,1,f);
  printf("\rZapisuju offsety a nazvy s3m a efektu do krkal.mus               ");
  fwrite(hudby,PS3M,32,f);
  fwrite(efekty,PEFF,8,f);
  for (i=0;i<PS3M;i++)
  {
     printf("\rZapisuju %12s do krkal.mus %28s ",files[i],hudby[i].jmeno);
     s=fopen(files[i],"rb");
     tmp=len[i];
     while(tmp>0){
       if (tmp>55000) {
	 fread(buf,55000,1,s);
	 fwrite(buf,55000,1,f);
	 tmp-=55000;
       } else {
	 fread(buf,tmp,1,s);
	 fwrite(buf,tmp,1,f);
	 tmp=0;
       }
     }
     fclose(s);
  }
  for (i=0;i<PEFF;i++)
  {
     printf("\rZapisuju %s do krkal.mus                              ",filesef[i]);
     s=fopen(filesef[i],"rb");
     tmp=lenef[i];
     while(tmp>0){
       if (tmp>55000) {
	 fread(buf,55000,1,s);
	 fwrite(buf,55000,1,f);
	 tmp-=55000;
       } else {
	 fread(buf,tmp,1,s);
	 fwrite(buf,tmp,1,f);
	 tmp=0;
       }
     }
     fclose(s);
  }

  fclose(f);
  printf("\rO.K.                                               \n");
}