uses basic,dos;
type pt = array[0..63999] of byte;
var s,w:file;
    xx : searchrec;
    f,g : word;
    i:longint;
    e,q:integer;
    r:array[0..255,0..2] of byte;
    jm : array[1..30] of record
      j : array[1..26] of byte;
      p : longint;
      ani : byte;
    end;
    env : byte;
    a:byte;
    p,p2 : ^pt;
    ptr,ptr2 : word;
    pocet : byte;
    topbuf : word;
    bs,as : string;
    anim : byte;
    anim2 : byte;

const fa : array[1..8] of record
  a : byte;
  b : array[1..6,1..4] of byte;
end = ((a:2;b:((0,0,15,5),(0,5,9,1),(0,0,0,0),(0,0,0,0),(0,0,0,0),(0,0,0,0))),
       (a:2;b:((0,0,15,5),(0,5,9,1),(0,0,0,0),(0,0,0,0),(0,0,0,0),(0,0,0,0))),
       (a:5;b:((0,0,15,1),(0,1,13,1),(6,2,9,1),(0,3,15,2),(0,5,9,1),(0,0,0,0))),
       (a:5;b:((12,0,3,1),(0,1,13,1),(6,2,9,1),(0,3,15,2),(0,5,9,1),(0,0,0,0))),
       (a:2;b:((0,0,15,6),(0,6,6,1),(6,2,9,1),(0,3,15,2),(0,5,9,1),(0,0,0,0))),
       (a:2;b:((0,0,15,6),(0,6,6,1),(6,2,9,1),(0,3,15,2),(0,5,9,1),(0,0,0,0))),
       (a:4;b:((0,0,3,1),(4,0,11,1),(0,1,15,4),(0,5,1,1),(0,5,9,1),(0,0,0,0))),
       (a:6;b:((2,0,1,1),(6,0,9,1),(0,1,4,1),(11,1,4,1),(0,2,15,3),(0,5,1,1))));

procedure pis;
begin
  p^[ptr2] := a;
  inc(ptr2);
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

procedure pis3(a : byte);
var x,y : word;
begin
  if a = 0 then a := 255 else if a = 255 then a := 0;
  x := ptr div 340;
  if (x < 69) or (x > 80) then begin
    if x>80 then dec(x,12);
    y := ptr mod 340;
    p2^[x*340+(y mod 4)*85+(y div 4)] := a;
  end;
  inc(ptr);
end;

begin
  new(p);
  new(p2);
  pocet := 0;
  repeat
    inc(pocet);
    bs := 'const'+strs(pocet)+'.pcx';
    findfirst(bs,archive,xx);
  until doserror<>0;
  dec(pocet);

  assign(s,'environ.txt');
  reset(s,1);
  topbuf := filesize(s);
  blockread(s,p^,topbuf);
  close(s);
  f := 1;
  for e := 1 to pocet do begin
    if f>topbuf then begin
      as := 'Environment cislo '+strs(e);
      for g := 1 to length(as) do jm[e].j[g] := ord(as[g]);
      jm[e].j[length(as)+1] := 0;
    end else begin
      g := 1;
      while (p^[f-1]<>13) and (f<=topbuf) do begin
        jm[e].j[g] := p^[f-1];
        inc(g);
        inc(f);
      end;
      inc(f,2);
      jm[e].j[g] := 0;
    end;
  end;

  ptr2 := 0;
  assign(w,'environ.dat');
  assign(s,'hlavicka');
  rewrite(w,1);
  reset(s,1);
  blockread(s,p^,10);
  blockwrite(w,p^,10);
  assign(s,'const1.pcx');
  reset(s,1);
  seek(s,filesize(s)-256*3);
  blockread(s,r,256*3);
  for f := 0 to 255 do for g := 0 to 2 do r[f,g] := r[f,g] shr 2;
  r[0,0] := 0;
  r[0,1] := 0;
  r[0,2] := 0;
  r[255,0] := 0;
  r[255,1] := 0;
  r[255,2] := 0;
  blockwrite(w,r,256*3);

  writeln;
  writeln;
  writeln('Konvertuju obrazky badiku . . .');
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

  ptr := 0;
  g := 4;
  for f := 9 to 14 do for e := 0 to 16 do for q := 0 to 19 do begin
    pis2(p^[g*5760+f*21+e*320+q]);
  end;
  g := 5;
  for f := 0 to 5 do for e := 0 to 16 do for q := 0 to 19 do begin
    pis2(p^[g*5760+f*21+e*320+q]);
  end;
  blockwrite(w,p2^,ptr);

  {***********************************}
  {****Animace     Badiku*************}
  {***********************************}
  {***********************************}
  for anim := 1 to 4 do begin
    writeln('Konvertuju animace badiku ',anim-1,' . . .');
    ptr2 := 0;
    assign(s,'tyanim'+strs(anim-1)+'.pcx');
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
    ptr := 0;
    for anim2 := 1 to fa[anim].a do begin
      for g := fa[anim].b[anim2,2] to fa[anim].b[anim2,2]+fa[anim].b[anim2,4]-1 do
      for f := fa[anim].b[anim2,1] to fa[anim].b[anim2,1]+fa[anim].b[anim2,3]-1 do
      for e := 0 to 16 do for q := 0 to 19 do begin
        pis2(p^[g*5760+f*21+e*320+q]);
      end;
    end;
    blockwrite(w,p2^,ptr);
  end;

  blockwrite(w,pocet,pocet*31+1);  {Seekuju ! (a pisu pocet)}

  for env := 1 to pocet do begin
    writeln('Konvertuju promenlive obrazky ',env,' . . .');
    jm[env].p := filepos(w);
    jm[env].ani := 0; {Nemam rad animace a proto zadny nebudou}

    ptr2 := 0;
    assign(s,'const'+strs(env)+'.pcx');
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
    ptr := 0;
    for g := 0 to 5 do for f := 0 to 14 do for e := 0 to 16 do for q := 0 to 19 do begin
      pis3(p^[g*5760+f*21+e*320+q]);
    end;
    g := 6;
    for f := 0 to 6 do for e := 0 to 16 do for q := 0 to 19 do begin
      pis3(p^[g*5760+f*21+e*320+q]);
    end;
    ptr2 := 0;
    assign(s,'cast'+strs(env)+'.pcx');
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
    for g := 0 to 4 do for f := 0 to 14 do for e := 0 to 16 do for q := 0 to 19 do begin
      pis3(p^[g*5760+f*21+e*320+q]);
    end;
    g := 5;
    for f := 0 to 8 do for e := 0 to 16 do for q := 0 to 19 do begin
      pis3(p^[g*5760+f*21+e*320+q]);
    end;
    blockwrite(w,p2^,57460);

  {***********************************}
  {********    Animace   *************}
  {***********************************}
  {***********************************}
    for anim := 5 to 8 do begin
      writeln('  Konvertuju promenlive animace ',env,'  ',anim-5,' . . .');
      ptr2 := 0;
      assign(s,'anim'+strs(env)+strs(anim-5)+'.pcx');
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
      ptr := 0;
      for anim2 := 1 to fa[anim].a do begin
        for g := fa[anim].b[anim2,2] to fa[anim].b[anim2,2]+fa[anim].b[anim2,4]-1 do
        for f := fa[anim].b[anim2,1] to fa[anim].b[anim2,1]+fa[anim].b[anim2,3]-1 do
        for e := 0 to 16 do for q := 0 to 19 do begin
          pis2(p^[g*5760+f*21+e*320+q]);
        end;
      end;
      blockwrite(w,p2^,ptr);
    end;

  end;

  seek(w,109579);
  blockwrite(w,jm,pocet*31);
  dispose(p);
  dispose(p2);
  close(w);
end.