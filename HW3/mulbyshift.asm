mov eax,[0x600000]
mov ebx,eax
mov ecx,eax
shl ecx,1
shl eax,3
shl ebx,4
add eax,ebx
add eax,ecx
mov [0x600004],eax
done:
