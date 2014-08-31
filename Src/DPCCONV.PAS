program pokus;
type
  buf1t = array[0..63999] of byte;
  buf2t = array[1..65535] of byte;
var
  s : file;
  buf1 : ^buf1t;
  max : word;
  buf2 : ^buf2t;
  p : word;
  op : byte;
  o : array[1..256] of byte;
  as,bs : string;
const
  startas = 3*256;

procedure precti_data;
var f : word;
begin
  assign(s,as);
  reset(s,1);
  seek(s,startas);
  max := filesize(s)-startas;
  blockread(s,buf1^,max);
  close(s);
end;



procedure zapis(a : byte);
begin
  inc(p);
  if p > 65534 then writeln('ERROR');
  buf2^[p] := a;
end;

procedure zjisti_opak;
var
  f : word;
  x : array[0..255] of word;
  a : word;
  b : byte;
begin
  op := 0;
  for f := 0 to 255 do x[f] := 0;
  for f := 0 to max-1 do if f = 0 then inc(x[buf1^[f]]) else if buf1^[f-1] <> buf1^[f] then inc(x[buf1^[f]]);
  for f := 0 to 255 do if x[f] = 0 then begin
    inc(op);
    o[op] := f;
  end;
  if op < 4 then begin
    inc(op);
    for f := op to 4 do o[f] := 256-f;
    for f := 0 to 256 do if (not (f in [o[1],o[2],o[3],o[4]])) then begin
      if (x[f] < x[o[op]]) then begin
        if (op+1 < 5) and (x[o[op]] < x[o[op+1]]) then begin
          if (op+2 < 5) and (x[o[op+1]] < x[o[op+2]]) then begin
            if (op+3 < 5) and (x[o[op+2]] < x[o[op+3]]) then o[op+3] := o[op+2];
            o[op+2] := o[op+1];
          end;
          o[op+1] := o[op];
        end;
        o[op] := f;
      end else begin
        if (op+1 < 5) and (x[f] < x[o[op+1]]) then begin
          if (op+2 < 5) and (x[o[op+1]] < x[o[op+2]]) then begin
            if (op+3 < 5) and (x[o[op+2]] < x[o[op+3]]) then o[op+3] := o[op+2];
            o[op+2] := o[op+1];
          end;
          o[op+1] := f;
        end else begin
          if (op+2 < 5) and (x[f] < x[o[op+2]]) then begin
            if (op+3 < 5) and (x[o[op+2]] < x[o[op+3]]) then o[op+3] := o[op+2];
            o[op+2] := f;
          end else if (op+3 < 5) and (x[f] < x[o[op+3]]) then o[op+3] := f;
        end;
      end;
    end;
    op := 4;
    for f := 1 to 3 do begin
      for a := f+1 to 4 do if o[f] > o[a] then begin
        b := o[a];
        o[a] := o[f];
        o[f] := b;
      end;
    end;
  end;
  zapis(op);
  a := 350;
  b := 0;
  for f := 1 to op do begin
    if (a+1 = o[f]) and (o[f] <> 0) then begin
      if b = 0 then inc(b) else b := 2;
      inc(a);
    end else begin
      if b = 1 then zapis(a);
      if b = 2 then begin
        zapis(0);
        zapis(a)
      end;
      a := o[f];
      zapis(a);
      if a = 0 then zapis(0);
      b := 0;
    end;
  end;
  if b = 1 then zapis(a);
  if b = 2 then begin
    zapis(0);
    zapis(a)
  end;
end;

procedure zapis_data;
var
  a : word;
  f : word;
  g : word;
  c : boolean;
begin
  p := 0;
  f := 0;
  g := 0;
  a := 350;
  c := true;
  zjisti_opak;
  for f := 0 to max-1 do begin
    if a = buf1^[f] then inc(g) else begin
      if g > 0 then if c then begin
        case g of
          1 : zapis(a);
          2..255 : if g < op-1 then zapis(o[g+2]) else begin
            zapis(o[1]);
            zapis(g);
          end;
          256..65535 : begin
            zapis(o[2]);
            zapis(g div 256);
            zapis(g mod 256);
          end;
        end;
        g := 0;
      end else begin
        case g of
          1..13 : inc(buf2^[p],g);
          14..255 : begin
            inc(buf2^[p],14);
            zapis(g);
          end;
          256..65535 : begin
            inc(buf2^[p],15);
            zapis(g div 256);
            zapis(g mod 256);
          end;
        end;
        g := 0;
      end;
      a := buf1^[f];
      if a in [o[1],o[2],o[3],o[4]] then begin
        zapis(o[3]);
        if a = o[1] then zapis(0);
        if a = o[2] then zapis(16);
        if a = o[3] then zapis(32);
        if a = o[4] then zapis(48);
        c := false;
      end else begin
        zapis(a);
        c := true;
      end;
    end;
  end;
  if g > 0 then if c then begin
    case g of
      1 : zapis(a);
      2..255 : if g < op-1 then zapis(o[g+2]) else begin
        zapis(o[1]);
        zapis(g);
      end;
      256..65535 : begin
        zapis(o[2]);
        zapis(g div 256);
        zapis(g mod 256);
      end;
    end;
  end else begin
    case g of
      1..2 : inc(buf2^[p],g);
      3..255 : begin
        inc(buf2^[p],3);
        zapis(g);
      end;
      256..65535 : begin
        inc(buf2^[p],4);
        zapis(g div 256);
        zapis(g mod 256);
      end;
    end;
  end;
  assign(s,bs);
  rewrite(s,1);
  blockwrite(s,buf2^,p);
  close(s);
end;

begin
  if paramcount <> 1 then begin
    writeln;
    writeln('ERROR - Nebyl zadan parametr.');
    writeln;
    writeln('SYNTAXE : PCXCONV <Jmeno BIN souboru bez pripony>');
    writeln;
    exit;
  end;
  as := paramstr(1)+'.bin';
  bs := paramstr(1)+'.dpc';
  writeln;
  writeln('PREKLADAM ...');
  new(buf1);
  new(buf2);
  precti_data;
  zapis_data;
  dispose(buf1);
  dispose(buf2);
end.