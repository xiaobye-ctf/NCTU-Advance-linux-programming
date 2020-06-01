test eax,eax
js s1
	mov DWORD PTR [0x600000],1
	jmp n1
s1:
	mov DWORD PTR[0x600000],-1
n1:
test ebx,ebx
js s2
	mov DWORD PTR [0x600004],1
	jmp n2
s2:
	mov DWORD PTR [0x600004],-1
n2:
test ecx,ecx
js s3
	mov DWORD PTR [0x600008],1
	jmp n3
s3:
	mov DWORD PTR [0x600008],-1
n3:
test edx,edx
js s4
	mov DWORD PTR [0x60000c],1
	jmp n4
s4:
	mov DWORD PTR [0x60000c],-1
n4:
done:
