uses dos,basic;
type
  buft = array[1..60000] of byte;
var
  buf : ^buft;
  s,s2 : file;
  jm : array[1..96] of record
    p : longint;
    j : array[1..16] of byte;
  end;
  xx : searchrec;
  f,g,lev : word;
  topbuf : word;
  v : longint;
  as : string;

function kk : boolean;
begin
  as := strs(lev)+'.lv';
  findfirst(as,archive,xx);
  if doserror<>0 then kk := false else kk := true;
end;

begin
  new(buf);
  assign(s,'levly.txt');
  reset(s,1);
  topbuf := filesize(s);
  blockread(s,buf^,topbuf);
  close(s);
  f := 1;
  for lev := 1 to 96 do begin
    if f>topbuf then begin
      as := 'LEVEL '+strs(lev);
      for g := 1 to length(as) do jm[lev].j[g] := ord(as[g]);
      jm[lev].j[length(as)+1] := 0;
    end else begin
      g := 1;
      while (buf^[f]<>13) and (f<=topbuf) do begin
        jm[lev].j[g] := buf^[f];
        inc(g);
        inc(f);
      end;
      inc(f,2);
      jm[lev].j[g] := 0;
    end;
    jm[lev].p := 1930;
  end;

  assign(s2,'krkal.lev');
  rewrite(s2,1);
  assign(s,'hlavicka');
  reset(s,1);
  blockread(s,buf^,10);
  blockwrite(s2,buf^,1930);
  close(s);
  lev := 1;
  topbuf := 0;
  while kk do begin
    assign(s,as);
    reset(s,1);
    if lev>1 then jm[lev].p := jm[lev-1].p+v;
    v := filesize(s);
    blockread(s,buf^[topbuf+1],v);
    close(s);
    inc(topbuf,v);
    inc(lev);
    if topbuf>50000 then begin
      blockwrite(s2,buf^,topbuf);
      topbuf := 0;
    end;
  end;
  if topbuf>0 then blockwrite(s2,buf^,topbuf);
  seek(s2,10);
  blockwrite(s2,jm,1920);
  close(s2);
  dispose(buf);
end.