var
  s,s2 : file;
  buffer : array[0..60000] of byte;
  f : word;
  as : string;

begin
  if paramcount <> 1 then begin
    writeln;
    writeln('ERROR - Nebyl zadan parametr.');
    writeln;
    writeln('SYNTAXE : LEVCONV <Jmeno LEV souboru bez pripony>');
    writeln;
    exit;
  end;
  as := paramstr(1);
  assign(s,as+'.lev');
  assign(s2,as+'.lv');
  reset(s,1);
  rewrite(s2,1);
  blockread(s,buffer,filesize(s));
  for f := 13 to 1420 do begin
    case buffer[f] of
      11..16 : buffer[f]:=(buffer[f]-11)*2+44;
      17..22 : inc(buffer[f],39);
      31..53 : inc(buffer[f],31);
      54..75 : inc(buffer[f],32);
      76 : buffer[f]:=114;
      77..140 : inc(buffer[f],40);
    end;
  end;
  blockwrite(s2,buffer,filesize(s));
  close(s);
  close(s2);
end.

