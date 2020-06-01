mov eax,[0x600000]
mov ebx,[0x600004]
imul eax,eax,5
sub ebx,3
cdq
idiv ebx
mov [0x600008],eax
done:
