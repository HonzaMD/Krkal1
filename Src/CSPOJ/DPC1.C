  void zobraz_dpc(void *odkaz) {
  asm {
    cld
    push si
    push di
    push es
    mov dx,ds
    mov es,dx
    push bp
    push ds
    mov di,offset op
    lds si,odkaz
    xor ax,ax
    xor ch,ch
    xor dl,dl
    lodsb
    mov cl,al
    xor bx,bx
} l1: asm {
    lodsb
    and al,al
    jnz l2
    lodsb
    and al,al
    jz l2
    inc dl
} l2:  asm {cmp al,bl
    jnz l3
    cmp ah,3
    ja l23
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
} l23: asm {inc ah
    mov al,ah
    stosb
    xor dl,dl
    inc bx
    loop l1
    jmp l5
}l3: asm { and dl,dl
    jz l4
    cmp ah,3
    ja l33
    push di
    push ax
    mov al,ah
    xor ah,ah
    mov di,ax
    mov byte ptr es:[op2+di],bl
    pop ax
    pop di
}l33: asm {inc ah
    mov es:[di],ah
    inc di
    inc bx
    loop l2
    jmp l5
}l4: asm {mov es:[di],dl
    inc di
    inc bx
    jmp l2
}l5: asm { mov cx,256
    sub cx,bx
    jz l6
    xor al,al
rep stosb
}l6: asm {


    pop dx
    mov ax,0xa000
    mov es,ax
    xor di,di
    mov bp,ds
    xor bh,bh

}l7: asm { lodsb
    mov ds,dx
    mov bl,al
    cmp byte ptr op+[bx],0
    jz l8
    mov al,byte ptr [op+bx]
    mov ds,bp

    cmp al,1
    jnz l71
    lodsb
    xor ch,ch
    mov cl,al
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l71: asm {cmp al,2
    jnz l72
    lodsb
    mov ch,al
    lodsb
    mov cl,al
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l72: asm {cmp al,3
    jnz l73
    lodsb
    mov ah,al
    and ax,0xf00f
    shr ah,4
    xor ch,ch
    cmp al,14
    jnz laa
    lodsb
    jmp lbb
}laa: asm {cmp al,15
    jnz lbb
    lodsb
    mov ch,al
    lodsb
}lbb: asm {mov cl,al
    inc cx
    mov ds,dx
    mov bl,ah
    mov al,byte ptr [op2+bx]
    mov ds,bp
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l73: asm {xor ch,ch
    mov cl,al
    dec cl
    dec cl
    mov al,ah
rep stosb
    cmp di,64000
    jz l9
    jmp l7

}l8: asm { stosb
    mov ds,bp
    mov ah,al
    cmp di,64000
    jz l9
    jmp l7
}l9: asm { pop bp
    mov ds,dx
    pop es
    pop di
    pop si
  }}
