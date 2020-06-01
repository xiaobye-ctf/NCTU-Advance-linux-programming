#!/bin/bash
echo addsub1:
python addsub1.py|grep -o 'ASM{.*}'
echo ''

echo addsub2:
cat addsub2.asm | nc aup.zoolab.org 2501 | grep -o 'ASM{.*}'
echo ''

echo bubble:
cat bubble.asm | nc aup.zoolab.org 2502 | grep -o 'ASM{.*}'
echo ''


