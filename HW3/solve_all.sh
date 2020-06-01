#!/bin/bash
echo addsub1:
python addsub1.py | grep -o 'ASM{.*}'
echo ''

echo addsub2:
cat addsub2.asm | nc aup.zoolab.org 2501 | grep -o 'ASM{.*}'
echo ''

echo bubble:
cat bubble.asm | nc aup.zoolab.org 2502 | grep -o 'ASM{.*}'
echo ''

echo clear17:
cat clear17.asm | nc aup.zoolab.org 2503 | grep -o 'ASM{.*}'
echo ''

echo dec2ascii:
cat dec2ascii.asm | nc aup.zoolab.org 2504 | grep -o 'ASM{.*}'
echo ''

echo dispbin:
cat dispbin.asm | nc aup.zoolab.org 2505 | grep -o 'ASM{.*}'
echo ''

echo eval1:
cat eval1.asm | nc aup.zoolab.org 2506 | grep -o 'ASM{.*}'
echo ''

echo isolatebit:
cat isolatebit.asm | nc aup.zoolab.org 2507 | grep -o 'ASM{.*}'
echo ''

echo leax:
cat leax.asm | nc aup.zoolab.org 2508 | grep -o 'ASM{.*}'
echo ''

echo loop15:
cat loop.asm | nc aup.zoolab.org 2509 | grep -o 'ASM{.*}'
echo ''

echo math1:
cat math1.asm | nc aup.zoolab.org 2510 | grep -o 'ASM{.*}'
echo ''

echo math2:
cat math2.asm | nc aup.zoolab.org 2511 | grep -o 'ASM{.*}'
echo ''

echo math3:
cat math3.asm | nc aup.zoolab.org 2512 | grep -o 'ASM{.*}'
echo ''

echo math4
cat math4.asm | nc aup.zoolab.org 2513 | grep -o 'ASM{.*}'
echo ''

echo math5
cat math5.asm | nc aup.zoolab.org 2514 | grep -o 'ASM{.*}'
echo ''

echo minicall:
cat minicall.asm | nc aup.zoolab.org 2515 | grep -o 'ASM{.*}'
echo ''

echo mulbyshift
cat mulbyshift.asm | nc aup.zoolab.org 2516 | grep -o 'ASM{.*}'
echo ''

echo posneg:
cat posneg.asm | nc aup.zoolab.org 2517 | grep -o 'ASM{.*}'
echo ''

echo recur:
python recur.py | grep -o 'ASM{.*}'
echo ''

echo swapmem:
cat swapmem.asm | nc aup.zoolab.org 2519 | grep -o 'ASM{.*}'
echo ''

echo swapreg:
cat swapreg.asm | nc aup.zoolab.org 2520 | grep -o 'ASM{.*}'
echo ''

echo tolower:
cat tolower.asm | nc aup.zoolab.org 2521 | grep -o 'ASM{.*}'
echo ''

echo ul+lu:
cat ul+lu.asm | nc aup.zoolab.org 2522 | grep -o 'ASM{.*}'
echo ''
