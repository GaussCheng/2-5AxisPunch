#ifndef ICHCTRANSCEIVERDATA_H
#define ICHCTRANSCEIVERDATA_H

#include "ictransceiverbase.h"
#include "icoptimize.h"
#include "icobjectpool.h"
#include "ICCore_global.h"

class ICCORESHARED_EXPORT ICHCTransceiverData: public ICTransceiverData
{
public:
    typedef QVector<quint32> ICTransceiverDataBuffer;
    ICHCTransceiverData()
        :functionCode_(0), addr_(0), length_(0){data_.clear();SetHostID(0);}
    ICHCTransceiverData(
            uint8_t hostID,
            int fc,
            int addr,
            int length,
            const ICTransceiverDataBuffer& data = ICTransceiverDataBuffer())
        :functionCode_(fc), addr_(addr), length_(length), data_(data){SetHostID(hostID);}

    virtual ~ICHCTransceiverData(){data_.clear();}
    virtual bool IsEqual(const ICTransceiverData* other) const;
    virtual int MaxFrameLength() const { return 256;}
    int GetFunctionCode() const { return functionCode_;}
    void SetFunctionCode(int fc) { functionCode_ = fc;}
    int GetAddr() const { return addr_;}
    void SetAddr(int addr) { addr_ = addr;}
    int GetLength() const {return length_;}
    void SetLength(int length) { length_ = length;}
    int GetTrueLength() const {return truelength_;}
    void SetTrueLength(int length) {truelength_ = length;}
    ICTransceiverDataBuffer Data() const { return data_;}
    void SetData(const ICTransceiverDataBuffer& data) { data_ = data;}

//    void* operator new(size_t size)
//    {
//        Q_UNUSED(size)
//        return objectPool_.MallocData();
//    }

//    void operator delete(void* p)
//    {
//        objectPool_.FreeData(p);
//    }


private:
    int functionCode_;
    int addr_;
    int length_;
    int truelength_;  //发送PLC命令时，记录PLC命令的实际长度
    ICTransceiverDataBuffer data_;
//    static ICObjectPool<ICHCTransceiverData> objectPool_;
};

inline bool ICHCTransceiverData::IsEqual(const ICTransceiverData* other) const
{
    const ICHCTransceiverData* that = static_cast<const ICHCTransceiverData*>(other);
    if(unlikely(that == NULL))
    {
        return false;
    }
    return this->GetFunctionCode() == that->GetFunctionCode() &&
            this->GetAddr() == that->GetAddr() &&
            this->GetLength() == that->GetLength() &&
            this->Data() == that->Data();
}

class ICCORESHARED_EXPORT ICHCFrameTransceiverDataMapper : public ICFrameTransceiverDataMapper
{
public:
    ICHCFrameTransceiverDataMapper();
    virtual bool FrameToTransceiverData(ICTransceiverData *recvData,
                                        const uint8_t *buffer,
                                        size_t size,
                                        const ICTransceiverData* sentData);
    virtual size_t TransceiverDataToFrame(uint8_t *dest, size_t bufferSize, const ICTransceiverData *data);
    virtual bool IsFunctionCodeValid(int fc) const = 0;
    virtual bool IsFunctionAddrValid(int addr, int fc) const = 0;
    virtual int GetAddrFromBuffer(const uint8_t* buffer) const = 0;
    virtual size_t GetBufferDataLength(const uint8_t* buffer) const = 0;
    virtual size_t FrameMinSize() const = 0;

private:
    ICHCTransceiverData::ICTransceiverDataBuffer dataBuffer_;
};


#endif // ICHCTRANSCEIVERDATA_H
