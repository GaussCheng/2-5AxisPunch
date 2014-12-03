#! /bin/sh

cp *ko /home/root -f
cp am335x-boneblack.dtb /boot -f
cp zImage /boot -f
newVersion=`modinfo ./szhc_keypad_jixieshou.ko | grep 'vermagic' | awk '{print $2}'`
oldVersion=`ls /lib/modules/ | grep '^3.12.10'`
mv /lib/modules/$oldVersion /lib/modules/$newVersion
#mv /lib/modules/3.12.10-2014.10.10 /lib/modules/3.12.10-2014.10.25
