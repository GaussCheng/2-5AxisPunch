#! /bin/sh
sudo rm -r ~/ArmLinux/Application/*
cp /home/gausscheng/workprojects/3a8Multi-axisManipulatorSystem-build-armv4t/Multi-axisManipulatorSystem ~/ArmLinux/Application/
cp Multi-axisManipulatorSystem*.qm ~/ArmLinux/Application/
cp -r sysconfig ~/ArmLinux/Application/
cp -r records ~/ArmLinux/Application/
cp -r subs  ~/ArmLinux/Application/
cp -r userpictures ~/ArmLinux/Application/resource
cd ~/ArmLinux/2300AutoScript/ && ./BuildFileSystem.sh