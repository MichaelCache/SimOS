# SimOS
This is the repository of a simple OS made by myself.
Now the version is 0.1,just at very begining(working on interrupt now).
At this time,it can only change the screen's color from light blue to yellow when the keyboard is pressed.
This repository is used to synchronize the code from work place and home.
There is no license protect,you can take it freely.

How to make it:
Just download the whole directory and type "make all",you will get the disk image SimOS.img.And "make clean" will recover the directory.

How to run it:
Run "qemu-system-i386 -hda SimOS.img" or brun it on your IDE disk.
The third way is to download SimOS.vdi and load it on your VirtualBox.
