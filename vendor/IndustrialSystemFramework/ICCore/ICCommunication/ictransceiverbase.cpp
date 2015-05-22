#include "ictransceiverbase.h"
#include "icoptimize.h"
#include <QDebug>

ICTransceiverBase::ICTransceiverBase()
    :transeBuffer_(512),
      frameTransceiverDataMapper_(NULL),
      isDebug_(false),
      crcErrCount_(0),
      lenghtErrCount_(0),
      blockTime_(0),
      isHex_(false)
{
}

bool ICTransceiverBase::Read(ICTransceiverData *recvData, const ICTransceiverData *sentData)
{
    if(unlikely(frameTransceiverDataMapper_ == NULL))
    {
        return false;
    }
    int size = ReadImpl(transeBuffer_.data(), recvData->MaxFrameLength()); //返回实际的读取字符长度
    if(unlikely(size <= 0))
    {
        lastReadFrame_.clear();
        return false;
    }
    uint8_t* rb = transeBuffer_.data();
    if(IsCommunicateDebug())
    {
        lastReadFrame_.clear();
        lastReadFrame_ += "Read Size:" + QByteArray::number(size);
        for(int i = 0; i < size; ++i)
        {
            if(IsHexLogFormat())
            {
                lastReadFrame_ += "[" + QByteArray::number(transeBuffer_[i], 16).toUpper() + "]";
            }
            else
            {
                lastReadFrame_ += "[" + QByteArray::number(transeBuffer_[i]) + "]";
            }
        }
    }
    if(unlikely(size > transeBuffer_.capacity()))
    {
        transeBuffer_.resize(size);
    }
    return frameTransceiverDataMapper_->FrameToTransceiverData(recvData, rb, size, sentData);
}

bool ICTransceiverBase::Write(const ICTransceiverData *data)
{
    if(unlikely(frameTransceiverDataMapper_ == NULL))
    {
        return false;
    }
    size_t size = frameTransceiverDataMapper_->TransceiverDataToFrame(transeBuffer_.data(), transeBuffer_.capacity(), data);

    if(size == 0)
    {
        transeBuffer_.resize(transeBuffer_.capacity() + 64);
        return false;
    }
    if(IsCommunicateDebug())
    {
        lastWriteFrame_.clear();
        lastWriteFrame_ += "Wrte Size:" + QByteArray::number(size);
        for(int i = 0; i < static_cast<int>(size); ++i)
        {
            if(IsHexLogFormat())
            {
                lastWriteFrame_ += "[" + QByteArray::number(transeBuffer_[i], 16).toUpper() + "]";
            }
            else
            {
                lastWriteFrame_ += "[" + QByteArray::number(transeBuffer_[i]) + "]";
            }
        }
    }
    return WriteImpl(transeBuffer_.constData(), size) == static_cast<int>(size); //WriteImpl返回字节数
}
