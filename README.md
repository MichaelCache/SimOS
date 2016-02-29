# SimOS
This is the repository of a simple OS made by myself.This repository is used to synchronize the code from work place and home.
Now the version is 0.2,just at very begining(working on graphic system now).
At this time,it can only change the screen's color from light blue to yellow when the key "A" is pressed.
There is no license protect,you can take it freely. :)

How to make it:
In this way you must have installed qemu.
Just download the whole directory and type "make all",you will get the disk image SimOS.img.And "make clean" will recover the directory.
Or just type "make run" will run the system automaticly.

How to run it:
Run "qemu-system-i386 -hda SimOS.img" or brun it on your IDE disk.
The third way is loading DSB.vdi on your VirtualBox.
