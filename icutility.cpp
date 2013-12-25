#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>

#include <QDir>
#include "icutility.h"

static void BuildShiftMap(int beg, int* map)
{
    int index = 0;
    for(int i = beg; i < 10; ++i)
    {
        map[index++] = i;
    }
    for(int i = 0; i < beg; ++i)
    {
        map[index++] = i;
    }
}

const QString ICUtility::ScreenSaverImgPath = QDir::homePath() + "/Pictures/screensaver.jpg";
const QString ICUtility::ScreenSaverInstanceLockFile = "/var/run/screensaver.pid";

/* Table of CRC values for hi-order byte */
uint8_t ICUtility::crcTableHi_[256] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
uint8_t ICUtility::crcTableLo_[256] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
ICUtility::ICUtility()
{
}



/**
 * 给文件整体加锁，为创建单进程副本提供帮助
 * 
 * @param fd    需要加锁的文件描述符
 */
bool ICUtility::LockFile(int fd)
{
#ifndef Q_WS_WIN
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl) >= 0);
#else
    return false;
#endif
}



/**
 * 给文件整体加锁，为创建单进程副本提供帮助
 * 
 * @param file    需要加锁的文件名
 */
bool ICUtility::LockFile(const QString& file)
{
#ifndef Q_WS_WIN
    int fd = open(file.toAscii().constData(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd < 0)
    {
        qErrnoWarning("Can't open %s", file.toAscii().constData());
        return false;
    }
    if(!LockFile(fd))
    {
        if(errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            return false;
        }
    }
    ftruncate(fd, 0);
    char buf[16];
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return true;
#else
    return false;
#endif
}

uint16_t ICUtility::CRC16(const uint8_t *buffer, size_t size)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (size--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ crcTableHi_[i];
        crc_lo = crcTableLo_[i];
    }

    return (crc_hi << 8 | crc_lo);
}

int ICUtility::readn(int fd, uint8_t *ptr, size_t n)
{
    size_t nleft;
    int nread;
    nleft = n;
    while(nleft > 0)
    {
        if((nread = read(fd, ptr, nleft)) < 0)
        {
            if(nleft == n)
            {
                return -1;
//                continue;
            }
            else
            {
                break;
//                continue;
            }
        }
        else if(nread == 0)
        {
            break;
        }
//        for(int i=0;i<nread;i++)
//            qDebug() << "recv: 0x " << hex <<  ptr[i];
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
//    return read(fd, ptr, n);
}

int ICUtility::writen(int fd, const uint8_t *ptr, size_t n)
{
    size_t nleft;
    int nwritten;
    nleft = n;
    while(nleft > 0)
    {
//        qDebug() << "n:" << n;
//        for(int i=0;i<n;i++)
//            qDebug() <<"i:" << i << " " << hex << "ptr: 0x" << ptr[i];

        if((nwritten = write(fd, ptr, nleft)) < 0)
        {
            if(nwritten == n)
            {
                return -1;
            }
            else
            {
                break;
            }
        }
        else if(nwritten == 0)
        {
            break;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n - nleft;
//    return write(fd, ptr, n);
}

int ICUtility::Register(const QString &code, const QString& machineCode)
{

    int sortMap[16];
    sortMap[0] = 1;
    sortMap[1] = 2;
    sortMap[2] = 3;
    sortMap[3] = 5;
    sortMap[4] = 7;
    sortMap[5] = 11;
    sortMap[6] = 13;
    sortMap[7] = 0;
    sortMap[8] = 4;
    sortMap[9] = 6;
    sortMap[10] = 8;
    sortMap[11] = 10;
    sortMap[12] = 12;
    sortMap[13] = 14;
    sortMap[14] = 9;
    sortMap[15] = 15;
    QString rC = code;
    QString sortRet(16, '0');
    int beg;
    if(rC.size() != 16)
    {
        return - 1;
    }
    for(int i = 0; i != sortRet.size(); ++i)
    {
        sortRet[sortMap[i]] = rC.at(i);
    }
    QString mCode = machineCode.simplified();
    mCode = mCode.remove(" ");
    beg = 0;
    for(int i = 0; i != mCode.size(); ++i)
    {
        if(!mCode.at(i).isDigit())
        {
            return -1;
        }
        beg += mCode.at(i).digitValue();
    }
    beg /= mCode.size();
    beg %= 10;
    int pMap[10];
    BuildShiftMap(beg, pMap);
    for(int i = 0; i != sortRet.size(); ++i)
    {
        for(int j = 0; j != 10; ++j)
        {
//            if(pMap[j] == sortRet.at(i).digitValue())
            if(j == sortRet.at(i).digitValue())
            {
                sortRet[i] = QString::number(pMap[j]).at(0);
                break;
            }
        }
    }
    if(sortRet.left(10) != mCode)
    {
        return -1;
    }
    return sortRet.right(6).toInt() * 24 * 7;
}

//int ICUtility::Register(const char* code, const char* machineCode)
//{
//    int i;
//    int pMap[10];
//    for(i = 0; i != 10; ++i)
//    {
//        pMap[i] = i;
//    }
//    int sortMap[16];
//    sortMap[0] = 1;
//    sortMap[1] = 2;
//    sortMap[2] = 3;
//    sortMap[3] = 5;
//    sortMap[4] = 7;
//    sortMap[5] = 11;
//    sortMap[6] = 13;
//    sortMap[7] = 0;
//    sortMap[8] = 4;
//    sortMap[9] = 6;
//    sortMap[10] = 8;
//    sortMap[11] = 10;
//    sortMap[12] = 12;
//    sortMap[13] = 14;
//    sortMap[14] = 9;
//    sortMap[15] = 15;
//    char rC[16];
//    strcpy(rC, code);
//    char sortRet[16] = {0};
//    int beg;
//    if(strlen(rC) != 16)
//    {
//        return -1;
//    }
//    for(i = 0; i != 16; ++i)
//    {
//        sortRet[sortMap[i]] = rC[i];
//    }
////    QString mCode = machineCode.simplified();
//    char mCode[10];
//    strcpy(mCode, machineCode);
////    mCode = mCode.remove(" ");
//    beg = 0;
//    for(int i = 0; i != 10; ++i)
//    {
////        if(!mCode.at(i).isDigit())
////        {
////            return -1;
////        }
////        beg += mCode.at(i).digitValue();
//        beg += (mCode[i] - 48);
//    }
//    beg /= 10;
//    beg %= 10;
//    for(int i = 0; i != 16; ++i)
//    {
//        for(int j = 0; j != 10; ++j)
//        {
//            if(((pMap[j] + beg) % 10) == (sortRet[i] - 48))
//            {
////                sortRet[i] = QString::number(pMap[j]).at(0);
//                sortRet[i] = (pMap[j] + 48);
//                break;
//            }
//        }
//    }
////    if(sortRet.left(10) != mCode)
////    {
////        return -1;
////    }
//    for(i = 0; i != 10; ++i)
//    {
//        if(sortRet[i] != mCode[i])
//        {
//            return -1;
//        }
//    }
////    return sortRet.right(6).toInt() * 24 * 7;
//    return atoi(sortRet + 10);
//}
