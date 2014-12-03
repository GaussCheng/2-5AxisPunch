#! /bin/sh

mount /dev/mmcblk0p1 /mnt/mmc1/boot/ 
cp MLO /mnt/mmc1/boot/ -f
cp u-boot.img /mnt/mmc1/boot/ -f

