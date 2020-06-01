mov eax,[0x600000]
mov ebx,[0x600004]
neg ebx
mov ecx,[0x600008]
imul eax,eax,-5
mov esi,eax
mov eax,ebx
cdq
idiv ecx
mov ebx,edx
mov eax,esi
cdq
idiv ebx
mov [0x60000c],eax
done:

