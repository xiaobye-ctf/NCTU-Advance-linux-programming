import pwn
import re
code = '''
	push %s
    call r
    pop rbx
    jmp exit
r:
    mov rbx,[rsp+8]
    cmp rbx,0
    jg c1
    mov rax,0
    jmp end
c1:
    cmp rbx,1
    jne c2
    mov rax,1
    jmp end
c2:
    sub rbx,1
    push rbx
    call r
    pop rbx
    push rax
    sub rbx,1
    push rbx
    call r
	pop rbx
    pop rbx
    imul rbx,rbx,2
    imul rax,rax,3
	add rax,rbx
end:
	ret
exit:
done:
'''
r = pwn.remote("aup.zoolab.org",2518)
p = r.recvuntil("(type 'done:' when done)\n")
p = re.findall('r\([0-9]*\)',p)[0][2:-1]

code = code %(p)

r.send(code)
out = r.recvall()
print out
