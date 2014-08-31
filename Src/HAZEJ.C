#define UI unsigned int
#define UC unsigned char
UC extern far obr[141][17][20];
UC extern far buffer[64000];


void gr_hazej(void *ukaz,UI x,UI y,UI ox,UI oy) {
  asm {
    cld
    push ds
    mov ax, seg buffer
    mov es,ax
    xor di,di
    lds si,ukaz
    mov bx,5*17
    mov ax,x
    mov dx,ox
    cmp ax,dx
    jz i2
    jna i1
    jmp x1
}i1:asm{
    jmp x2
}i2:asm{
    mov ax,y
    mov dx,oy
    cmp ax,dx
    jna y1
    jmp y2

}y1:asm{
    sub dx,ax
    cmp dx,17
    jb y11
    jmp konec
}y11:asm{
    mov ah,dl
    shl dl,2
    add dl,ah
    mov ah,4
    sub bl,dl
}y12:asm{
    add di,dx
    mov cx,bx
}y13:asm{
    lodsb
    and al,al
    jz y15
    stosb
    jmp short y16
}y15:asm{
    inc di
}y16:asm{
    loop y13
    add si,dx
    dec ah
    jnz y12
    jmp konec

}y2:asm{
    sub ax,dx
    cmp ax,17
    jb y21
    jmp konec
}y21:asm{
    mov dx,ax
    mov ah,dl
    shl dl,2
    add dl,ah
    mov ah,4
    sub bl,dl
}y22:asm{
    add si,dx
    mov cx,bx
}y23:asm{
    lodsb
    and al,al
    jz y25
    stosb
    jmp short y26
}y25:asm{
    inc di
}y26:asm{
    loop y23
    add di,dx
    dec ah
    jnz y22
    jmp konec

}x1:asm{
    sub ax,dx
    cmp ax,20
    jb x11
    jmp konec
}x11:asm{
    mov cx,y
    cmp cx,oy
    jz x11p
    jmp konec
}x11p:asm{
    mov dx,ax
    mov dh,4
    and dl,3
    jz x17
    shr ax,2
    inc ax
    mov ah,5
    sub dh,dl
    sub ah,al
    jz x112
    xor ch,ch
    mov cl,dh
}x13:asm{
    add di,bx
    loop x13
    xor bh,bh
    mov bl,al
}x16:asm{
    mov ch,17
}x14:asm{
    add si,bx
    mov cl,ah
}x15:asm{
    lodsb
    and al,al
    jz x19
    stosb
    jmp short x110
}x19:asm{
    inc di
}x110:asm{
    dec cl
    jnz x15
    add di,bx
    dec ch
    jnz x14
    dec dl
    jnz x16
    jmp x12
}x17:asm{
    mov dh,4
    shr ax,2
    mov ah,5
    sub ah,al
    xor bh,bh
    mov bl,al
    jmp x18
}x112:asm{
    xor ch,ch
    mov cl,dl
}x114:asm{
    add si,bx
    loop x114
    xor bh,bh
    mov bl,al
}x12:asm{
    dec bx
    inc ah
}x18:asm{
    xor di,di
}x16b:asm{
    mov ch,17
}x14b:asm{
    add si,bx
    mov cl,ah
}x15b:asm{
    lodsb
    and al,al
    jz x18b
    stosb
    jmp short x19b
}x18b:asm{
    inc di
}x19b:asm{
    dec cl
    jnz x15b
    add di,bx
    dec ch
    jnz x14b
    dec dh
    jnz x16b
    jmp konec

}x2:asm{
    sub dx,ax
    cmp dx,20
    jb x21
    jmp konec
}x21:asm{
    mov cx,y
    cmp cx,oy
    jz x21p
    jmp konec
}x21p:asm{
    mov ax,dx
    mov dh,4
    and dl,3
    jz x27
    shr ax,2
    mov ah,5
    sub ah,al
    sub dh,dl
    xor ch,ch
    mov cl,dl
}x23:asm{
    add di,bx
    loop x23
    xor bh,bh
    mov bl,al
    jmp x26
}x27:asm{
    shr ax,2
    mov ah,5
    sub ah,al
    sub dh,dl
    xor ch,ch
    mov cl,dl
    xor bh,bh
    mov bl,al
}x26:asm{
    mov ch,17
}x24:asm{
    add di,bx
    mov cl,ah
}x25:asm{
    lodsb
    and al,al
    jz x29
    stosb
    jmp short x210
}x29:asm{
    inc di
}x210:asm{
    dec cl
    jnz x25
    add si,bx
    dec ch
    jnz x24
    dec dh
    jnz x26
    and dl,dl
    jnz x28
    jmp konec
}x28:asm{
    xor di,di
//    xor bh,bh
//    mov bl,al
    dec ah
    jz konec
    inc bx
}x26b:asm{
    mov ch,17
}x24b:asm{
    add di,bx
    mov cl,ah
}x25b:asm{
    lodsb
    and al,al
    jz x29b
    stosb
    jmp short x210b
}x29b:asm{
    inc di
}x210b:asm{
    dec cl
    jnz x25b
    add si,bx
    dec ch
    jnz x24b
    dec dl
    jnz x26b

}konec:asm{
    pop ds
  }
}

void hazej_buf(UI x,UI y) {
asm{
  cld
  push ds
  push bp
  mov ax, seg buffer
  mov ds,ax
  xor si,si
  mov ax,0xa000
  mov es,ax
  mov ax,y
  add ax,26
  mov bx,ax
  shl ax,7
  shl bx,5
  add bx,ax
  mov di,x
  shr di,2
  add di,bx
  mov cl,4
  mov bx,155
  mov bp,di
  mov dx,0x3c4
} a1: asm {
  mov ax,0x1002
  shr ah,cl
  out dx,ax
  mov ch,17
  mov di,bp
}a2:asm{
  movsw
  movsw
  movsb
  add di,bx
  dec ch
  jnz a2
  dec cl
  jnz a1

  pop bp
  pop ds
}}

void hazej(UI x,UI y,UC o) {
  asm {
    mov ax, seg obr
    push ds
    mov ds,ax
    xor ah,ah
    mov al, o
    mov bx,340
    mul bx
    mov si, offset obr
    add si,ax
    mov ax,0xa000
    mov es,ax
    mov ax,y
    add ax,26
    mov bx,ax
    shl ax,7
    shl bx,5
    add bx,ax
    mov ax,x
    shr ax,1
    jnc fnu
    jmp p24
}fnu:asm{
    shr ax,1
    jnc fnu2
    jmp p3
}fnu2:asm{
    // CAST 1 ***************************************************
    add bx,ax

    mov cx,4
    mov dx,0x3c4
} sk1: asm {
    mov ax,0x1002
    shr ah,cl
    out dx,ax
    mov di,bx

    mov ah,17
}m4:asm{
    mov ch,5
}m3:asm{
    lodsb
    and al,al
    jz m1
    stosb
    jmp m2
} m1: asm {
    inc di
} m2:asm{
    dec ch
    jnz m3
    add di,155
    dec ah
    jnz m4
    dec cl
    jnz sk1
    jmp konec


}p24:asm {
    shr ax,1
    jnc fnu4
    jmp p4
}fnu4:asm{

    // CAST 2 ***************************************************
    add bx,ax

    mov cx,3
    mov dx,0x3c4
} sk2: asm {
    mov ax,0x1002
    shr ah,cl
    out dx,ax
    mov di,bx

    mov ah,17
}m42:asm{
    mov ch,5
}m32:asm{
    lodsb
    and al,al
    jz m12
    stosb
    jmp m22
} m12: asm {
    inc di
} m22:asm{
    dec ch
    jnz m32
    add di,155
    dec ah
    jnz m42
    dec cl
    jnz sk2

    mov ax,0x0102
    out dx,ax
    inc bx
    mov di,bx

    mov ah,17
}m43:asm{
    mov ch,5
}m33:asm{
    lodsb
    and al,al
    jz m13
    stosb
    jmp m23
} m13: asm {
    inc di
} m23:asm{
    dec ch
    jnz m33
    add di,155
    dec ah
    jnz m43
    jmp konec
  }

p3:
  asm {
    // CAST 3 ***************************************************
    add bx,ax

    mov cx,2
    mov dx,0x3c4
} sk4: asm {
    mov ax,0x1002
    shr ah,cl
    out dx,ax
    mov di,bx

    mov ah,17
}m424:asm{
    mov ch,5
}m324:asm{
    lodsb
    and al,al
    jz m124
    stosb
    jmp m224
} m124: asm {
    inc di
} m224:asm{
    dec ch
    jnz m324
    add di,155
    dec ah
    jnz m424
    dec cl
    jnz sk4

    mov cx,2
    inc bx
} sk5: asm {
    mov ax,0x0402
    shr ah,cl
    out dx,ax
    mov di,bx

    mov ah,17
}m434:asm{
    mov ch,5
}m334:asm{
    lodsb
    and al,al
    jz m134
    stosb
    jmp m234
} m134: asm {
    inc di
} m234:asm{
    dec ch
    jnz m334
    add di,155
    dec ah
    jnz m434
    dec cl
    jnz sk5
    jmp konec
  }
p4:
  asm {
    // CAST 4 ***************************************************
    add bx,ax

    mov dx,0x3c4
    mov ax,0x0802
    out dx,ax
    mov di,bx

    mov ah,17
}m4245:asm{
    mov ch,5
}m3245:asm{
    lodsb
    and al,al
    jz m1245
    stosb
    jmp m2245
} m1245: asm {
    inc di
} m2245:asm{
    dec ch
    jnz m3245
    add di,155
    dec ah
    jnz m4245

    mov cx,3
    inc bx
} sk7: asm {
    mov ax,0x0802
    shr ah,cl
    out dx,ax
    mov di,bx

    mov ah,17
}m4345:asm{
    mov ch,5
}m3345:asm{
    lodsb
    and al,al
    jz m1345
    stosb
    jmp m2345
} m1345: asm {
    inc di
} m2345:asm{
    dec ch
    jnz m3345
    add di,155
    dec ah
    jnz m4345
    dec cl
    jnz sk7

} konec: asm{
  pop ds
  }
}

//#####################################################################
//#####################################################################

//#####################################################################
//#####################################################################

//#####################################################################
//#####################################################################
//#####################################################################

void zobraz_menu(UI x,UI y,UC ddx,UC ddy) {
  asm {
    mov ax,y
    mov di,ax
    shl ax,8
    shl di,6
    add di,ax
    add di,x
    mov si,di
    push ds
    mov ax,seg buffer[0]
    mov ds,ax
    mov ax,0xa000
    mov es,ax
    mov ax,320
    xor bx,bx
    xor cx,cx
    mov bl,ddx
    sub ax,bx
    mov bh,ddy
    shr bl,1
    jc k5
}k3:asm{
    mov cl,bl
rep movsw
    dec bh
    jz k4
    add di,ax
    add si,ax
    jmp short k3
}k5:asm{
    mov cl,bl
rep movsw
    movsb
    dec bh
    jz k4
    add di,ax
    add si,ax
    jmp short k5
}k4:asm{
    pop ds
  }
}

UI biglen(UC *s)
{
  UI f,len=0;
  for (f=0;f<strlen(s);f++)
  {
    if (s[f]==' ') len+=8; else len+=buffer[(s[f]-33)*3];
    len++;
  }
  return len;
}

void writebig(UI x0, UI y0, UC *s)
{
  UI x;
  UI f;
  UI offs;
  for (f=0;f<strlen(s);f++) {
    if (s[f]==' ') x0+=8; else {
      offs = buffer[(s[f]-33)*3+2];
      offs*=256;
      offs += (372+buffer[(s[f]-33)*3+1]);
      sputimage0(x0,y0,buffer[(s[f]-33)*3],20,buffer+offs);
      x0+=buffer[(s[f]-33)*3];
    }
    x0++;
  }
}