mov al,[0x600000]
and al,0b11011111
mov [0x600001],al
done:
