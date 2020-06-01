mov ecx,15
mov bx,1
l1:
mov dx,ax
and dx,bx
add dx,48
mov [0x600000+ecx],dl
shr ax,1
sub ecx,1
cmp ecx,0
jge l1
done:
