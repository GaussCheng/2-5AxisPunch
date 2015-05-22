#include "icpic32uartbootloaderprotocol.h"

QByteArray PIC32BootLoaderCommand(char command, const QByteArray& content)
{
    const int contentSize = content.size();
    QByteArray ret(contentSize + 5, 0);
    ret[0] = (PIC32BootLoader_SOH);
    ret[1] = (command);
    int j = 2;
    for(int i = 0; i != contentSize;++i)
    {
        ret[j] = content.at(i);
        ++j;
    }
    uint16_t crc = ICUtility::CRC16((const uint8_t*)(ret.data()), ret.size());
    ret[j++] = (crc & 0xFF);
    ret[j++] = (crc >> 8);
    for(int i = 1; i < ret.size(); ++i)
    {
        if(ret.at(i) == PIC32BootLoader_SOH ||
           ret.at(i) == PIC32BootLoader_EOT ||
           ret.at(i) == PIC32BootLoader_DLE)
        {
            ret.insert(i, PIC32BootLoader_DLE);
            ++i;
        }
    }
    ret[j] = (PIC32BootLoader_EOT);

    return ret;
}

QByteArray PIC32BootLoaderProgramFlash(const QByteArray &hexRecord)
{
    if(hexRecord.isEmpty())
    {
        return QByteArray();
    }
    QByteArray hex = hexRecord.right(hexRecord.size() - 1); // remove the :
    return PIC32BootLoaderCommand(kPIC32BootLoader_ProgramFlash, hex.toHex());
}

QByteArray PIC32BootLoaderReadCRC(uint begin, uint size)
{
    QByteArray content(8, 0);
    content[0] = begin & 0x000000FF;
    content[1] = (begin >> 8) & 0x000000FF;
    content[2] = (begin >> 16) & 0x000000FF;
    content[3] = (begin >> 24) & 0x000000FF;
    content[4] = size & 0x000000FF;
    content[5] = (size >> 8) & 0x000000FF;
    content[6] = (size >> 16) & 0x000000FF;
    content[7] = (size >> 24) & 0x000000FF;
    return PIC32BootLoaderCommand(kPIC32BootLoader_ReadCRC, content);
}
