///////////////////////////////////////////////////////////
//  ICTimerPool.cpp
//  Implementation of the Class ICTimerPool
//  Created on:      05-九月-2011 10:05:53
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#include "ictimerpool.h"

QScopedPointer<ICTimerPool> ICTimerPool::instance_(new ICTimerPool());
ICTimerPool::ICTimerPool():timerPool_(DefaultSize), flags_(DefaultSize){
    for(int i = 0; i != timerPool_.size(); ++i)
    {
        timerPool_[i] = NULL;
    }
}



ICTimerPool::~ICTimerPool(){
    for(int i = 0; i != timerPool_.size(); ++i)
    {
        if(timerPool_.at(i) != NULL)
        {
            delete timerPool_[i];
        }
    }

}

QTimer* ICTimerPool::GetTimer_(int &id)
{
    int i;
    for(i = 0; i != flags_.size(); ++i)
    {
        if(!flags_.at(i))
        {
            flags_.setBit(i, true);
            id = i;
            if(timerPool_.at(i) == NULL)
            {
                timerPool_[i] = new QTimer();
            }
            return timerPool_[i];
        }
    }

    /*pool is full and then resize the pool*/
    timerPool_.resize(timerPool_.size() << 1); //double the size;
    flags_.resize(flags_.size() << 1);
    flags_.setBit(i, true);
    id = i;
    timerPool_[i] = new QTimer();
    return timerPool_[i];
}
