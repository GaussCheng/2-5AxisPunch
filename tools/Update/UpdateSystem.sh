#! /bin/sh

chmod +x UpdateGUI
./UpdateGUI update_cmd -qws

sync
sync
hostPackages=`find . -name "rom.bin"`
if [ "$hostPackages" != "" ] ; then
    chmod +x ModbusUartProgramer
    ./ModbusUartProgramer /dev/ttySZHC0 rom.bin -qws
fi

sync

reboot
