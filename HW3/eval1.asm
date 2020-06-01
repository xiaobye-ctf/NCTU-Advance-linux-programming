mov eax,[0x600000]
neg eax
mov ebx,[0x600004]
mov ecx,[0x600008]
sub ebx,ecx
add eax,ebx
mov [0x60000c],eax
done:

