mov eax,[0x600000]
mov ecx,[0x600004]
neg ecx
mov esi,[0x600008]
imul eax,ecx
sub esi,ebx
cdq
idiv esi
mov [0x600008],eax
done:

