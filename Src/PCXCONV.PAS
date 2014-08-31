type pt = array[0..63999] of byte;
var s,w:file;
    i:longint;
    e,q:integer;
    r:array[0..255,0..2] of byte;
    a:byte;
    p : ^pt;
    ptr : word;
procedure pis;
begin
  p^[ptr] := a;
  inc(ptr);
end;

begin
  if paramcount <> 1 then begin
    writeln;
    writeln('ERROR - Nebyl zadan parametr.');
    writeln;
    writeln('SYNTAXE : PCXCONV <Jmeno PCX souboru bez pripony>');
    writeln;
    exit;
  end;
  new(p);
  ptr := 0;
  assign(w,paramstr(1)+'.bin');
  rewrite(w,1);
  assign(s,paramstr(1)+'.pcx');
  reset(s,1);
  seek(s,filesize(s)-256*3);
  blockread(s,r,256*3);
  for e := 0 to 255 do for q := 0 to 2 do r[e,q] := r[e,q] div 4;
  blockwrite(w,r,256*3);
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
  blockwrite(w,p^,ptr);
  dispose(p);
  close(s);
  close(w);
end.