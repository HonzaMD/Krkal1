#ifdef FC_COMMENT
FC version 2.0 - Copyright (c) 1990 Mike Albert        Thu Jul 13 04:33:06 2000
Options are: -m -c1 -ds5 -t8 
#endif /* FC_COMMENT */

#ifdef FC_COMMENT
Compare files:
   dpc1.c             2296   4-25-98  10:29a
   dpc2.c             2369   4-25-98  10:31a
#endif /* FC_COMMENT */

#ifdef FC_FILE_1 /* dpc1.c */
  void zobraz_dpc(void *odkaz) {
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
  void rozbal_dpc(UI velikost,void *odkud,void *kam){
#endif /* FC_FILE_2 - dpc2.c */
  asm {
    cld
#ifdef FC_FILE_1 /* dpc1.c */
    push si
    push di
    push es
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    mov ax,velikost
#endif /* FC_FILE_2 - dpc2.c */
    mov dx,ds
    mov es,dx
    push bp
    push ds
#ifdef FC_FILE_2 /* dpc2.c */
    push ax
#endif /* FC_FILE_2 - dpc2.c */
    mov di,offset op
#ifdef FC_FILE_1 /* dpc1.c */
    lds si,odkaz
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    lds si,odkud
#endif /* FC_FILE_2 - dpc2.c */
    xor ax,ax
    xor ch,ch
    xor dl,dl
    lodsb
    mov cl,al
    xor bx,bx
#ifdef FC_FILE_1 /* dpc1.c */
} l1: asm {
    lodsb
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l1:asm{ lodsb
#endif /* FC_FILE_2 - dpc2.c */
    and al,al
    jnz l2
    lodsb
    and al,al
    jz l2
    inc dl
#ifdef FC_FILE_1 /* dpc1.c */
} l2:  asm {cmp al,bl
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l2:asm{ cmp al,bl
#endif /* FC_FILE_2 - dpc2.c */
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
#ifdef FC_FILE_1 /* dpc1.c */
} l23: asm {inc ah
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l23:asm{inc ah
#endif /* FC_FILE_2 - dpc2.c */
    mov al,ah
    stosb
    xor dl,dl
    inc bx
    loop l1
    jmp l5
#ifdef FC_FILE_1 /* dpc1.c */
}l3: asm { and dl,dl
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l3:asm{ and dl,dl
#endif /* FC_FILE_2 - dpc2.c */
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
#ifdef FC_FILE_1 /* dpc1.c */
}l33: asm {inc ah
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l33:asm{inc ah
#endif /* FC_FILE_2 - dpc2.c */
    mov es:[di],ah
    inc di
    inc bx
    loop l2
    jmp l5
#ifdef FC_FILE_1 /* dpc1.c */
}l4: asm {mov es:[di],dl
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l4:asm{ mov es:[di],dl
#endif /* FC_FILE_2 - dpc2.c */
    inc di
    inc bx
    jmp l2
#ifdef FC_FILE_1 /* dpc1.c */
}l5: asm { mov cx,256
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l5: asm{mov cx,256
#endif /* FC_FILE_2 - dpc2.c */
    sub cx,bx
    jz l6
    xor al,al
rep stosb
#ifdef FC_FILE_1 /* dpc1.c */
}l6: asm {
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l6:asm{
#endif /* FC_FILE_2 - dpc2.c */


#ifdef FC_FILE_2 /* dpc2.c */
    pop ax
#endif /* FC_FILE_2 - dpc2.c */
    pop dx
#ifdef FC_FILE_1 /* dpc1.c */
    mov ax,0xa000
    mov es,ax
    xor di,di
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    les di,kam
//    stosw
 //   inc di
  //  inc di
#endif /* FC_FILE_2 - dpc2.c */
    mov bp,ds
    xor bh,bh

#ifdef FC_FILE_1 /* dpc1.c */
}l7: asm { lodsb
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l7:asm{ lodsb
#endif /* FC_FILE_2 - dpc2.c */
    mov ds,dx
    mov bl,al
    cmp byte ptr op+[bx],0
#ifdef FC_FILE_1 /* dpc1.c */
    jz l8
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    jnz k8
    jmp l8
}k8:asm{
#endif /* FC_FILE_2 - dpc2.c */
    mov al,byte ptr [op+bx]
    mov ds,bp

    cmp al,1
    jnz l71
    lodsb
    xor ch,ch
    mov cl,al
    mov al,ah
rep stosb
#ifdef FC_FILE_1 /* dpc1.c */
    cmp di,64000
    jz l9
    jmp l7

}l71: asm {cmp al,2
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    xor bl,bl
    cmp di,es:[bx]
    jnz l7
    jmp l9

}l71:asm{cmp al,2
#endif /* FC_FILE_2 - dpc2.c */
    jnz l72
    lodsb
    mov ch,al
    lodsb
    mov cl,al
    mov al,ah
rep stosb
#ifdef FC_FILE_1 /* dpc1.c */
    cmp di,64000
    jz l9
    jmp l7

}l72: asm {cmp al,3
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    xor bl,bl
    cmp di,es:[bx]
    jnz l7
    jmp l9

}l72:asm{cmp al,3
#endif /* FC_FILE_2 - dpc2.c */
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
#ifdef FC_FILE_1 /* dpc1.c */
}laa: asm {cmp al,15
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}laa:asm{cmp al,15
#endif /* FC_FILE_2 - dpc2.c */
    jnz lbb
    lodsb
    mov ch,al
    lodsb
#ifdef FC_FILE_1 /* dpc1.c */
}lbb: asm {mov cl,al
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}lbb:asm{mov cl,al
#endif /* FC_FILE_2 - dpc2.c */
    inc cx
    mov ds,dx
    mov bl,ah
    mov al,byte ptr [op2+bx]
    mov ds,bp
rep stosb
#ifdef FC_FILE_1 /* dpc1.c */
    cmp di,64000
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    xor bl,bl
    cmp di,es:[bx]
#endif /* FC_FILE_2 - dpc2.c */
    jz l9
    jmp l7

#ifdef FC_FILE_1 /* dpc1.c */
}l73: asm {xor ch,ch
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l73:asm{xor ch,ch
#endif /* FC_FILE_2 - dpc2.c */
    mov cl,al
    dec cl
    dec cl
    mov al,ah
rep stosb
#ifdef FC_FILE_1 /* dpc1.c */
    cmp di,64000
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    xor bl,bl
    cmp di,es:[bx]
#endif /* FC_FILE_2 - dpc2.c */
    jz l9
    jmp l7

#ifdef FC_FILE_1 /* dpc1.c */
}l8: asm { stosb
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l8:asm{ stosb
#endif /* FC_FILE_2 - dpc2.c */
    mov ds,bp
    mov ah,al
#ifdef FC_FILE_1 /* dpc1.c */
    cmp di,64000
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
    xor bl,bl
    cmp di,es:[bx]
#endif /* FC_FILE_2 - dpc2.c */
    jz l9
    jmp l7
#ifdef FC_FILE_1 /* dpc1.c */
}l9: asm { pop bp
    mov ds,dx
    pop es
    pop di
    pop si
  }}
#endif /* FC_FILE_1 - dpc1.c */
#ifdef FC_FILE_2 /* dpc2.c */
}l9: asm{pop bp
    mov ds,dx
  }
}
#endif /* FC_FILE_2 - dpc2.c */
#ifdef FC_COMMENT
Comparison complete
#endif /* FC_COMMENT */

