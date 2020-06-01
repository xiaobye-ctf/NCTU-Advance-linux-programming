mov eax,[0x600000]
mov ebx,[0x600004]
mov ecx,5
mul ecx
mov edx,0
sub ebx,3
div ebx
mov [0x600008],eax
done:
