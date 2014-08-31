type pt = array[0..63999] of byte;
var s,w:file;
    i:longint;
    e,q:integer;
    a:byte;
    p,p2 : ^pt;
    ptr,ptr2,ptr3 : word;
    f,g : word;
    tab : array[1..124] of record
      dx : byte;
      offs : word;
    end;
procedure pis;
begin
  p^[ptr] := a;
  inc(ptr);
end;


procedure konvertuj(xy : word);
var
  x1,x2 : byte;
  xx : boolean;
  f,g : word;
  dx : byte;
begin
  tab[ptr3].offs := ptr2;
  x1 := 0;
  x2 := 19;
  xx := true;
  repeat
    g := xy+x1;
    for f := 0 to 19 do begin
      if p^[g] <> 0 then xx := false;
      inc(g,320);
    end;
    if xx then inc(x1);
  until xx=false;
  xx := true;
  repeat
    g := xy+x2;
    for f := 0 to 19 do begin
      if p^[g] <> 0 then xx := false;
      inc(g,320);
    end;
    if xx then dec(x2);
  until xx=false;

  dx := x2-x1+1;
  tab[ptr3].dx := dx;
  xy := xy+x1;
  for g := 1 to 20 do begin
    for f := x1 to x2 do begin
      p2^[ptr2] := p^[xy];
      inc(ptr2);
      inc(xy);
    end;
    inc(xy,320);
    dec(xy,dx);
  end;
  inc(ptr3);
end;

begin
  new(p);
  new(p2);
  ptr := 0;
  assign(w,'font2.dat');
  rewrite(w,1);
  assign(s,'font2.pcx');
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
  ptr3 := 1;
  for g := 0 to 4 do for f := 0 to 15 do konvertuj(f*20+g*20*320);
  for f := 0 to 13 do konvertuj(f*20+5*20*320);
  for f := 0 to 14 do konvertuj(f*20+6*20*320);
  for f := 0 to 14 do konvertuj(f*20+7*20*320);

  blockwrite(w,tab,124*3);
  blockwrite(w,p2^,ptr2);
  dispose(p);
  dispose(p2);
  close(w);
end.