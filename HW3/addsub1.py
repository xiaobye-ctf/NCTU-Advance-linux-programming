import pwn
import re
code = '''
mov eax,%s
add eax,%s
sub eax,%s

done:
'''
r = pwn.remote("aup.zoolab.org",2500)
p = r.recvuntil("(type 'done:' when done)\n")
p = p.split('\n\t')
v1 = p[1].split(' ')[2]
v2 = p[2].split(' ')[4]
v3 = p[3].split(' ')[4].split('\n')[0]
#print v1,v2,v3
code = code %(v1,v2,v3)

r.send(code)
out = r.recvall()
print re.findall('ASM{.*}',out)[0]
