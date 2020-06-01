mov ecx,14
l1:

mov al,[0x600000+ecx]
or al,0b00100000
mov [0x600010+ecx],al

sub ecx,1
cmp ecx,0
jge l1 
done:
