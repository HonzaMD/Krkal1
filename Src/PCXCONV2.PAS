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
    writeln('PCXCONV2 vytvari BI soubory. Na rozdil od BIN souboru nemaji paletu.');
    writeln('SYNTAXE : PCXCONV2 <Jmeno PCX souboru bez pripony>');
    writeln;
    exit;
  end;
  new(p);
  ptr := 0;
  assign(w,paramstr(1)+'.bi');
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
  blockwrite(w,p^,ptr);
  dispose(p);
  close(s);
  close(w);
end.