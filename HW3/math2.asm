mov eax,[0x600000]
neg eax
mov ebx,[0x600004]
mov ecx,[0x600008]
imul eax,ebx
add eax,ecx
done:
