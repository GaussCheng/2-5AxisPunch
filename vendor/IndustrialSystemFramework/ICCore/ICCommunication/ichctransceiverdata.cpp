#include "ichctransceiverdata.h"
#include "icutility.h"

static inline bool CheckCRC16(const uint8_t* buffer, size_t size)
{
    uint16_t crcCal = ICUtility::CRC16(buffer, size - 2);
    uint16_t crcRecv = (buffer[size - 2] << 8) | buffer[size - 1];
    return crcCal == crcRecv;
}

//ICObjectPool<ICHCTransceiverData> ICHCTransceiverData::objectPool_;
ICHCFrameTransceiverDataMapper::ICHCFrameTransceiverDataMapper()
{
}

bool ICHCFrameTransceiverDataMapper::FrameToTransceiverData(ICTransceiverData *recvData,
                                                       const uint8_t *buffer,
                                                       size_t size,
                                                       const ICTransceiverData *sentData)
{
    ICHCTransceiverData* injectionData = static_cast<ICHCTransceiverData*>(recvData);
    if(unlikely(injectionData == NULL))
    {
        return false;
    }
    if(size < FrameMinSize())
    {
        //            ++lenghtErrCount_;
        return false;
    }
    if(buffer[0] != sentData->HostID())
    {
        return false;
    }
    if(!IsFunctionCodeValid(buffer[1]))
    {
        return false;
    }
    int tmpAddr = GetAddrFromBuffer(buffer);
    if(!IsFunctionAddrValid(tmpAddr, buffer[1]))
    {
        return false;
    }
    size_t tmpLength = GetBufferDataLength(buffer);
    if(tmpLength < 4)
    {
        //            ++lenghtErrCount_;
        return false;
    }
    if(size != static_cast<size_t>(FrameMinSize() + tmpLength))
    {
        //            ++lenghtErrCount_;
        return false;
    }
    if(!CheckCRC16(buffer, size))
    {
        //            ++crcErrCount_;
        return false;
    }
    injectionData->SetFunctionCode(buffer[1]);
    injectionData->SetAddr(tmpAddr);
    dataBuffer_.clear();
    int j = 5;
    size_t dataLength = tmpLength >> 2;
    for(size_t i = 0; i != dataLength; ++i)
    {
        dataBuffer_.append((buffer[j] << 24) |
                           (buffer[j + 1]) << 16 |
                           (buffer[j + 2]) << 8 |
                           (buffer[j + 3]));
        j += 4;
    }
    injectionData->SetData(dataBuffer_);
    injectionData->SetLength(dataLength);
    if(!recvData->IsQuery())
    {
        if(!recvData->IsEqual(sentData))
        {
            return false;
        }
    }
    return true;
}

size_t ICHCFrameTransceiverDataMapper::TransceiverDataToFrame(uint8_t *dest,
                                                         size_t bufferSize,
                                                         const ICTransceiverData *data)
{
    const ICHCTransceiverData* injectionData = static_cast<const ICHCTransceiverData*>(data);
    if(unlikely(injectionData == NULL))
    {
        return 0;
    }
    dataBuffer_ = injectionData->Data();
    if(unlikely(static_cast<int>(bufferSize) < dataBuffer_.size() + 7))
    {
        return 0;
    }
    dest[0] = injectionData->HostID();
    dest[1] = injectionData->GetFunctionCode();
    dest[2] = injectionData->GetAddr() >> 8;
    dest[3] = injectionData->GetAddr() & 0x00FF;
    dest[4] = injectionData->GetLength();
    int j = 5;
    for(int i = 0; i != dataBuffer_.size(); ++i)
    {
        dest[j++] = Get8BitNum(dataBuffer_.at(i), 24);
        dest[j++] = Get8BitNum(dataBuffer_.at(i), 16);
        dest[j++] = Get8BitNum(dataBuffer_.at(i), 8);
        dest[j++] = Get8BitNum(dataBuffer_.at(i), 0);
    }
    uint16_t crc = ICUtility::CRC16(dest, j);
    dest[j++] = Get8BitNum(crc, 8);
    dest[j++] = Get8BitNum(crc, 0);
    return j;
}
