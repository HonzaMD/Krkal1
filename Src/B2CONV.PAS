uses basic;
type pt = array[0..63999] of byte;
var s,w:file;
    i:longint;
    e,q:integer;
    a:byte;
    p,p2 : ^pt;
    ptr,ptr2,ptr3 : word;
    f,g : word;
procedure pis;
begin
  p^[ptr] := a;
  inc(ptr);
end;


procedure konvertuj(xy : word);
var
  f : byte;
  a : byte;
  i : pst4b;

begin
  a := p2^[ptr2];
  for f := 1 to 4 do i[f] := 8;
  f := (ptr3 mod 4)+1;
  if p^[xy] = 3 then i[f] := 2
  else if p^[xy] = 4 then i[f] := 1 else i[f] := 0;
  ps2bbyte(a,i);
  p2^[ptr2] := a;
  inc (ptr3);
  if (ptr3=4) or (ptr3=8) then inc(ptr2);
  if (ptr3=11) then begin
    ptr3 := 0;
    inc(ptr2);
  end;
end;

begin
  new(p);
  new(p2);
  ptr := 0;
  assign(w,'brvn2.fnt');
  rewrite(w,1);
  assign(s,'brvnhc.pcx');
  reset(s,1);
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
  close(s);

  ptr2 := 0;
  ptr3 := 0;
  for g := 0 to 1 do for f := 0 to 14 do for e := 0 to 4 do for q := 0 to 10 do
  konvertuj(g*825+f*5+e+q*75);

  blockwrite(w,p2^,ptr2);
  dispose(p);
  dispose(p2);
  close(w);
end.