#ifndef ICCOMMUNICTIONCOMMANDQUEUE_H
#define ICCOMMUNICTIONCOMMANDQUEUE_H

#include "ICCore_global.h"
#include <QQueue>
#include "ictransceiverbase.h"

class ICCORESHARED_EXPORT ICCommunictionCommandQueue
{
public:
    ICCommunictionCommandQueue();
    ~ICCommunictionCommandQueue();
    void EnQueue(const ICTransceiverData* frame);
    void DeQueue();
    void InsertQueue(const ICTransceiverData* frame);
    const ICTransceiverData* Head() const{ return queue_.head();}
    bool IsEmpty() const { return queue_.isEmpty();}

    int Size() const { return queue_.size();}
    void Clear() { qDeleteAll(queue_); queue_.clear();}
private:
    QQueue<const ICTransceiverData*> queue_;
};

inline void ICCommunictionCommandQueue::EnQueue(const ICTransceiverData *frame)
{
    if(frame == NULL)
    {
        return;
    }
    if(queue_.isEmpty())
    {
        queue_.enqueue(frame);
    }
    else if(!frame->IsEqual(queue_.last()))
    {
        queue_.enqueue(frame);
    }
}

inline void ICCommunictionCommandQueue::InsertQueue(const ICTransceiverData *frame)
{
    if(frame == NULL)
    {
        return;
    }
    if(queue_.isEmpty())
    {
        queue_.push_front(frame);
    }
    else if(queue_.size() == 1)
    {
        queue_.enqueue(frame);
    }
    else if(!frame->IsEqual(queue_.at(1)))
    {
        queue_.insert(1, frame);
    }
}

inline void ICCommunictionCommandQueue::DeQueue()
{
    if(!queue_.isEmpty())
    {
        delete queue_.dequeue();
    }
}
#endif // ICCOMMUNICTIONCOMMANDQUEUE_H
