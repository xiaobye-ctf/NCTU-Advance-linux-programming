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
