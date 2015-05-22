#ifndef ICPIC32UARTBOOTLOADERPROTOCOL_H
#define ICPIC32UARTBOOTLOADERPROTOCOL_H

#include <QByteArray>
#include "icutility.h"

enum PIC32BootLoaderCommandType
{
    kPIC32BootLoader_ReadVersion = 0x01,
    kPIC32BootLoader_EraseFlash,
    kPIC32BootLoader_ProgramFlash,
    kPIC32BootLoader_ReadCRC,
    kPIC32BootLoader_RunApp
};

#define PIC32BootLoader_SOH ((char)0x01)
#define PIC32BootLoader_EOT ((char)0x04)
#define PIC32BootLoader_DLE ((char)0x10)


extern QByteArray PIC32BootLoaderCommand(char command, const QByteArray& content = QByteArray());
static inline QByteArray PIC32BootLoaderReadVersion()
{
    return PIC32BootLoaderCommand(kPIC32BootLoader_ReadVersion);
}

static inline QByteArray PIC32BootLoaderEraseFlash()
{
    return PIC32BootLoaderCommand(kPIC32BootLoader_EraseFlash);
}

static inline QByteArray PIC32BootLoaderRunApp()
{
    return PIC32BootLoaderCommand(kPIC32BootLoader_RunApp);
}

extern QByteArray PIC32BootLoaderProgramFlash(const QByteArray& hexRecord);
extern QByteArray PIC32BootLoaderReadCRC(uint begin, uint size);

#endif // ICPIC32UARTBOOTLOADERPROTOCOL_H
