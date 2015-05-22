#ifndef ICTRANSCEIVERBASE_H
#define ICTRANSCEIVERBASE_H

#include "ICCore_global.h"
#include <stddef.h>
#include <stdint.h>
#include <QVector>
#include <QByteArray>
#include <QVarLengthArray>


inline uint8_t Get8BitNum(const quint32& data, uint8_t pos){return (data >> pos) & 0x000000FF;}

class ICCORESHARED_EXPORT ICTransceiverData
{
public:
    virtual ~ICTransceiverData(){}
    virtual bool IsQuery() const { return false;}
    virtual bool IsError() const { return false;}
    virtual bool IsEqual(const ICTransceiverData* other) const = 0;
    virtual int MaxFrameLength() const = 0;
    virtual uint8_t HostID() const { return hostID_;}
    virtual void SetHostID(uint8_t hostID) { hostID_ = hostID;}
private:
    uint8_t hostID_;
};

class ICCORESHARED_EXPORT ICFrameTransceiverDataMapper
{
public:
    virtual ~ICFrameTransceiverDataMapper(){}
    virtual bool FrameToTransceiverData(ICTransceiverData* recvData,
                                        const uint8_t* buffer,
                                        size_t size,
                                        const ICTransceiverData* sentData) = 0;
    virtual size_t TransceiverDataToFrame(uint8_t* dest,
                                          size_t bufferSize,
                                          const ICTransceiverData* data) = 0;

    virtual int NeedToRecvLength(const ICTransceiverData* sentData) const = 0;

};

class ICCORESHARED_EXPORT ICTransceiverBase
{
public:
    ICTransceiverBase();
    virtual ~ICTransceiverBase(){transeBuffer_.clear();}
    virtual bool IsBlock() { return false;}
    virtual void SetBlock(bool isBlock){ Q_UNUSED(isBlock)}
    void SetFrameTransceiverDataMapper(ICFrameTransceiverDataMapper* mapper) {frameTransceiverDataMapper_ = mapper;}
    int BlockTime() const {return blockTime_;}
    void SetBlockTime(int msec) { blockTime_ = msec;}
    bool Read(ICTransceiverData* recvData, const ICTransceiverData* sentData);
    bool Write(const ICTransceiverData* data);
    bool IsCommunicateDebug() const { return isDebug_;}
    void SetCommunicateDebug(bool debug){isDebug_ = debug;}
    int GetCRCErrCount() const { return crcErrCount_;}
    int GetLengthErrCount() const {return lenghtErrCount_;}
    QByteArray LastReadFrame() const { return lastReadFrame_;}
    QByteArray LastWriteFrame() const { return lastWriteFrame_;}
    bool IsHexLogFormat() const { return isHex_;}
    void SetHexLogFormat(bool isHex) { isHex_ = isHex; }

    virtual void StopCommunicate(){}
    virtual void StartCommunicate(){}
protected:
    virtual int ReadImpl(uint8_t* dest, size_t size) = 0;
    virtual int WriteImpl(const uint8_t* buffer, size_t size) = 0;
    void ScaleLengthErrCount(int count) { lenghtErrCount_ += count;}
    void ScaleCRCErrCount(int count) { crcErrCount_ += count;}

private:
    QVarLengthArray<uint8_t, 512> transeBuffer_;
    ICFrameTransceiverDataMapper* frameTransceiverDataMapper_;
    bool isDebug_;
    int crcErrCount_;
    int lenghtErrCount_;
    QByteArray lastReadFrame_;
    QByteArray lastWriteFrame_;
    int blockTime_;
    bool isHex_;
};

#endif // ICTRANSCEIVERBASE_H
