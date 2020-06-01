mov ecx,9
l1:
mov edx,0
l2:
mov esi,[0x600000+edx*4]
mov edi,[0x600000+edx*4+4]
cmp esi,edi
jl c1
mov [0x600000+edx*4],edi
mov [0x600000+edx*4+4],esi
c1:
add edx,1
cmp edx,ecx
jne l2
sub ecx,1
cmp ecx,0
jne l1

done:
