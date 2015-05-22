#include <fcntl.h>
#include <errno.h>
//#include "qextserialport.h"
#include "icutility.h"
#include <QDebug>
#include "icserialtransceiver.h"

QScopedPointer<ICSerialTransceiver> ICSerialTransceiver::instance_(new ICSerialTransceiver());
ICSerialTransceiver::ICSerialTransceiver():serialFD_(-1), isError_(false)
{
//    PortSettings portSettings = {BAUD1152000, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 25};
//    serialPort_ = new QextSerialPort("/dev/ttyUSB0", portSettings, QextSerialPort::Polling);
//    serialPort_->open(QIODevice::ReadWrite);
//        serialFD_ = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
//        if(serialFD_ < 0)
//        {
//            perror("open");
//            qCritical()<<"Error Can't open the device /dev/ttyS0 (errno"<<errno<<")";
//            isError_ = true;
//            return;
//        }
//        tcgetattr(serialFD_, &(oldTios_));
//        termios tios;
//        memset(&tios, 0, sizeof(struct termios));
//        if((cfsetispeed(&tios, B115200) < 0) ||
//                (cfsetospeed(&tios, B115200) < 0))
//        {
//            perror("cfsetispeed/cfsetospeed\n");
//            isError_ = true;
//            return;
//        }
//        tios.c_cflag |= (CREAD | CLOCAL);
//        tios.c_cflag &= ~CSIZE;
//        tios.c_cflag |= CS8;
//        tios.c_cflag &= ~CSTOPB;
//        tios.c_cflag &= ~PARENB;

//        tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//        tios.c_iflag &= ~INPCK;
//        tios.c_iflag &= ~(IXON | IXOFF | IXANY);
//        tios.c_oflag &= ~(OPOST);
//        tios.c_cc[VMIN] = 0;
//        tios.c_cc[VTIME] = 0;
//        if(tcsetattr(serialFD_, TCSANOW, &tios) < 0 )
//        {
//            perror("tcsetattr\n");
//            isError_ = true;
//            return;
//        }
//    StartCommunicate();
}

ICSerialTransceiver::~ICSerialTransceiver()
{
//    close(serialFD_);
    StopCommunicate();
//    serialPort_->close();
}

int ICSerialTransceiver::ReadImpl(uint8_t *dest, size_t size)
{
    //    FD_ZERO(&fds_);
    //    FD_SET(serialFD_, &fds_);
    //    blockTime_.tv_sec = BlockTime() / 1000;
    //    blockTime_.tv_usec = (BlockTime() % 1000) * 1000;
    //    int ret = select(serialFD_ + 1, &fds_, NULL, NULL, &blockTime_);
    //    if(ret > 0)
    //    {
//    QByteArray ba((char*)dest);
//    while(serialPort_->bytesAvailable())
//    {
//        ba.append(serialPort_->readAll());
//    }
//    return ba.size();

    int ret = ICUtility::readn(serialFD_, dest, size);  //状态，数据，长度
//    int i;
//    for(i=0;i<ret;i++)
//    {
//        if(tmpBuf[i] == 0x55 && tmpBuf[i + 1] == 0x55)
//            break;
//    }
//    if(ret == i)
//        return 0;
//    int  len = tmpBuf[i + 2] + 3; //+4？
//    if(len > ret - i)
//        return 0;
//    memcpy(dest,tmpBuf+i,len);
//    qDebug() <<"ret: " << ret << "len: " << len << "  dest: " << hex << dest[0] ;
#ifndef Q_WS_WIN
    tcflush(serialFD_, TCIOFLUSH);  //清空串口缓冲区内容
#endif
    return ret;
    //    int gc = 0;
//    while( serialPort_.rdbuf()->in_avail() > 0  )
//    {
//    //        char next_byte;
//            serialPort_.get(*((char*)dest++));
//            ++gc;
//    //        qDebug() << std::hex << static_cast<int>(next_byte) << " ";
//    //        usleep(100) ;
//    }
//    serialPort_.flush();
//    return gc;
    //    }
    //    int ret;
    //    dest[0] = 0x50;
    //    dest[1] = 708 >> 8;
    //    dest[2] = 708 & 0xFF;
    //    dest[3] = 38;
    //    for(int i = 0; i != 38 * 4; ++i)
    //    {
    //        dest[4 + i] = rand() % 255;
    //    }
    //    dest[154] = 0;
    //    dest[155] = 0;

    //    ret = 38 * 4 + 6;
//    tcflush(serialFD_, TCIOFLUSH);  //清空串口缓冲区内容
//    return ret;
}

int ICSerialTransceiver::WriteImpl(const uint8_t *buffer, size_t size)
{
#ifndef Q_WS_WIN
    tcflush(serialFD_, TCIOFLUSH);  //清空串口缓冲区内容
#endif
//    for(int i = 0;i < size;++i)
//    {
//        qDebug() << "serial send data:" << i <<buffer[i];
//    }
    int ret = ICUtility::writen(serialFD_, buffer, size);
//    for(int i = 0;i < size;++i)
//    {
//        qDebug() << buffer[i];
//    }
    return ret;
//    serialPort_.flush();
//    serialPort_.write((char*)buffer, size);
////    serialPort_->write(buffer, size);
//    serialPort_->flush();
//    return  serialPort_->write((const char*)buffer, size);
}
bool ICSerialTransceiver::IsBlock()
{
#ifndef Q_WS_WIN
    tcgetattr(serialFD_, &(oldTios_));
    return (oldTios_.c_cc[VMIN] > 0);
#else
    return false;
#endif
}

void ICSerialTransceiver::SetBlock(bool isBlock)
{
#ifndef Q_WS_WIN
    int vmin = isBlock? 1 : 0;
    tcgetattr(serialFD_, &(oldTios_));
    oldTios_.c_cc[VMIN] = vmin;
    if(tcsetattr(serialFD_, TCSANOW, &oldTios_) < 0 )
    {
        perror("tcsetattr\n");
        isError_ = true;
        return;
    }
#endif
}

void ICSerialTransceiver::StartCommunicate()
{
#ifndef Q_WS_WIN
    if(serialFD_ >= 0)
    {
        return;
    }
    serialFD_ = open("/dev/ttySZHC", O_RDWR | O_NOCTTY | O_NDELAY);

    if(serialFD_ < 0)
    {
        perror("open");
        qCritical()<<"Error Can't open the device /dev/ttySZHC (errno"<<errno<<")";
        isError_ = true;
        return;
    }
    tcgetattr(serialFD_, &(oldTios_));
    termios tios;
    memset(&tios, 0, sizeof(struct termios));
    if((cfsetispeed(&tios, B115200) < 0) ||
            (cfsetospeed(&tios, B115200) < 0))
    {
        perror("cfsetispeed/cfsetospeed\n");
        isError_ = true;
        return;
    }
    tios.c_cflag |= (CREAD | CLOCAL);
    tios.c_cflag &= ~CSIZE;
    tios.c_cflag |= CS8;
    tios.c_cflag &= ~CSTOPB;
    tios.c_cflag &= ~PARENB;

    tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tios.c_iflag &= ~INPCK;
    tios.c_iflag &= ~(IXON | IXOFF | IXANY);
    tios.c_oflag &= ~(OPOST);
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;
    if(tcsetattr(serialFD_, TCSANOW, &tios) < 0 )
    {
        perror("tcsetattr\n");
        isError_ = true;
        return;
    }
#endif
}

void ICSerialTransceiver::StopCommunicate()
{
    close(serialFD_);
    serialFD_ = -1;
}
