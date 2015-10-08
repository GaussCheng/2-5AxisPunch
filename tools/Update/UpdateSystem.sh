#! /bin/sh

hostPackages=`find . -name "rom.bin"`
if [ "$hostPackages" != "" ] ; then
    chmod +x ModbusUartProgramer
    ./ModbusUartProgramer /dev/ttySZHC0 rom.bin -qws
fi

${PWD}/UpdateGUI update_cmd -qws

sync

reboot
