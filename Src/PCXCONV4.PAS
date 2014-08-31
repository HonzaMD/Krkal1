type pt = array[1..4,0..15999] of byte;
var s,w:file;
    i:longint;
    e,q:integer;
    r:array[0..255,0..2] of byte;
    a:byte;
    p : ^pt;
    ptr : word;
    ptr2 : byte;
procedure pis;
begin
  inc(ptr2);
  p^[ptr2,ptr] := a;
  if ptr2=4 then begin
    inc(ptr);
    ptr2 := 0;
  end;
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
  ptr2 := 0;
  assign(w,paramstr(1)+'.bi4');
  rewrite(w,1);
  assign(s,paramstr(1)+'.pcx');
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
  blockwrite(w,p^[1],ptr);
  blockwrite(w,p^[2],ptr);
  blockwrite(w,p^[3],ptr);
  blockwrite(w,p^[4],ptr);
  dispose(p);
  close(s);
  close(w);
end.