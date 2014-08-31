uses basic;
type pt = array[0..63999] of byte;
var s,w:file;
    i:longint;
    e,q:integer;
    r:array[0..255,0..2] of byte;
    a:byte;
    p,p2 : ^pt;
    ptr : word;
    pocet : byte;
const as : string[10]= 'Krkal 0.0 ';
procedure pis;
begin
  p^[ptr] := a;
  inc(ptr);
end;
procedure pis2(a : byte);
var x,y : word;
begin
  if a = 0 then a := 255 else if a = 255 then a := 0;
  x := ptr div 340;
  y := ptr mod 340;
  p2^[x*340+(y mod 4)*85+(y div 4)] := a;
  inc(ptr);
end;

begin
  if paramcount <> 1 then begin
    writeln('PCX_KRK prevadi obrazky pro KRKALA z PCX formatu');
    writeln;
    writeln('SYNTAXE : PCX_KRK <cast(0 jsou nemenne obrazky)>');
    exit;
  end else writeln('PREKLADAM . . .');
  pocet := vals(paramstr(1));
  new(p);
  new(p2);
  ptr := 0;
  if pocet = 0 then begin
    assign(w,'constenv.krk');
    assign(s,'constenv.pcx');
  end else begin
    assign(w,'cast'+paramstr(1)+'.krk');
    assign(s,'cast'+paramstr(1)+'.pcx');
  end;
  rewrite(w,1);
  reset(s,1);
  seek(s,filesize(s)-256*3);
  blockread(s,r,256*3);
  for f := 0 to 255 do for g := 0 to 2 do r[f,g] := r[f,g] shr 2;
  seek(s,128);
  for i:=128 to filesize(s)-256*3-2 do
  begin
    blockread(s,a,1);
    if a>191 then
      begin
        q:=a-192;
        blockread(s,a,1);
        inc(i);
        for e:=1 to q do pis;
      end
    else pis;
  end;
  ptr := 0;
  r[0,0] := 0;
  r[0,1] := 0;
  r[0,2] := 0;
  r[255,0] := 0;
  r[255,1] := 0;
  r[255,2] := 0;
  if pocet = 0 then blockwrite(w,r,256*3);
  if pocet = 0 then pocet := 97 else pocet := 84;
  for f := 1 to pocet div 15 do begin
    for g := 0 to 14 do begin
      for q := 0 to 16 do for e := 0 to 19 do pis2(p^[(f-1)*5760+q*320+g*21+e]);
    end;
  end;
  if pocet mod 15 <> 0 then begin
    f := (pocet div 15)+1;
    for g := 0 to (pocet mod 15)-1 do begin
      for q := 0 to 16 do for e := 0 to 19 do pis2(p^[(f-1)*5760+q*320+g*21+e]);
    end;
  end;
  blockwrite(w,p2^,ptr);
  dispose(p);
  dispose(p2);
  close(s);
  close(w);
end.