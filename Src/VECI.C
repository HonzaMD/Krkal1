				     /*
.  - Datova struktura VECI
p  - Datova struktura PREP
[] - Datova struktura OBDELNIK
>  - Datova struktura PADY

K  - Vec se chova kulate
*  - Meni se obrazek
                                       *
ZPUSOB ZNICENI :
  V - Vybuchem     L - Lalerem, Proudem
  D - Delem        U - Umackani pohyblivou stenou
  P - Priserou     ^ - Padem predmetu ( Nici se ten predmet, na ktery neco
		       spadlo. )
======================================================
[].         0     Plyn - Hemr je odolnej, Otocny potvory z nej neradi
		  vylejzaj
[].         1     Voda - Jezek plave, potvory vybuchujou
[].         2..5  Pasy
	    6     Exit
V []        7     Led - Vybuchem roztava na vodu
	   *8..43 Podlahy
	   *44,45 Dira
V          *46,47 Propadaci podlaha - Vybuchem se propada
	   *48,49 Propadla podlaha
	   *50,51 Hrbolata podlaha
p          *52,53 Nezmacknuta naslapna podlaha
p          *54,55 Zmacknuta naslapna podlaha
======================================================
V    >     *56    H1 - Kostka - Hemr ji prelejza
VL K >     *57    H2 - Kamen
VL K       *58    H3 - Pneumatika, Neni pritahovana magnetem
VLUD(^)    *59    H4 - Balonek - Da se strkat nekolik balonku najednou,Kdyz
		  balonek narazi na stenu, odskoci do strany. Neni pritaho-
		  van magnetem. Kdyz ne naj spadne kamen, odskoci do
		  strany, jinak praskne.
	*60,61    Otevreny dvere - Nezavrou se, kdyz v nich  neco je.
	 62       Prolejzaci stena - Proleze ji hemr
VL K>    63       Brneni
VDPLU K> 64       Jabko
VDLU^ K>.65,66    Bomba - Pri zniceni vybuchuje
VDLU^ K>.67,68    Mina  ----- || ----
VL K     69..72   Znacky - Nejsou pritahovany magnetem, ty je otacis
         73       Save Point
         74..76   Voda
         77..79   Znacky
	 80,81    Paprsky laseru
     .   82       Vybuch
     .   83,84    Proud
     .   85       Strela dela
VL K>    86,87,88 Klice
=====================================================
	*89,90    Zavreny dvere
	*91..106  Steny
VLD K   *107..113 Rozbitelna stena
VLDU    *114..116 Hlina - Nicis ji vkrocenim na policko s ni
  K p   *117,118  Prepinace
  K      119..121 Zamky  - Po dodani klice zmyzi
 K p     122      Detektor laseru
 K p     123      Fotobunka
	 124      Stena rusici magneticke pole
VLD K.   125..128 Laser
VLD K.   129..132 Delo
K    .   133..136 Elektroda
V K      137..144 Zrcadla  LD - Jsou znicitelna za strany, ktera neodrazi
     .   145..148 Magnety - Pritahuji predmety (Naraz predmetu do tebe
		  te zabiji)
V K .    149      Teleport
V K .    150..152 Hajzly
V(L)DUP  153..156 Cihla - Odolna proti padu kamenu a el. proudu
VLDUP^   157..160 Hemr - Odolny proti plynu, prolejza
VLDU(P)^ 161..164 Jezek - Plave, nosi priseram jabka
VLDU^   .165..168 Otaciva potvora - Vyhyba se vsem nebezpecim krome dela,
		  miny, bomby. Rada je v plynu
VLDU^   .179..172 Samonavadeci p. - Leze taky do proudu a laseru
VLDU^(P).173..176 Znackova p. - Navic pada do der, vybourava se s vecma, co
		  jdou proti ni. Nektery otaci znacky, nektery neotaci.
VLDU^   .177..180 Klaustrofobni p. - Pohybuje se jako znackova. Vyhyba se
		  vsem nebezpecim krome dela, miny, bomby. - Kdyz se nemuze
		  hnout, vybuchuje.
    --   PRISERY zabitim vybuchuji
         181      Propadajici se podlaha