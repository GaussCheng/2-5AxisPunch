#include "testcommunicatepage.h"
#include "ui_testcommunicatepage.h"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <QDebug>

TestCommunicatePage::TestCommunicatePage(QWidget *parent) :
    TestPageBase(parent),
    ui(new Ui::TestCommunicatePage)
{
    ui->setupUi(this);
    connect(&timer_,
            SIGNAL(timeout()),
            SLOT(OnTimerOut()));

    serialFD_ = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(serialFD_ < 0)
    {
        perror("open");
        qCritical()<<"Error Can't open the device /dev/ttyS0 (errno"<<errno<<")";
        return;
    }
    tcgetattr(serialFD_, &(oldTios_));
    termios tios;
    memset(&tios, 0, sizeof(struct termios));
    if((cfsetispeed(&tios, B115200) < 0) ||
            (cfsetospeed(&tios, B115200) < 0))
    {
        perror("cfsetispeed/cfsetospeed\n");
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
        return;
    }

    memset(sentBuf, 0, kSentSize);
    sentCount_ = -1;
    succeesfulCount_ = -1;

}

TestCommunicatePage::~TestCommunicatePage()
{
    ::close(serialFD_);
    delete ui;
}

void TestCommunicatePage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TestCommunicatePage::showEvent(QShowEvent *e)
{
    timer_.start(200);
    TestPageBase::showEvent(e);
}

void TestCommunicatePage::hideEvent(QHideEvent *e)
{
    timer_.stop();
    TestPageBase::hideEvent(e);
}

void TestCommunicatePage::OnTimerOut()
{
    char buf[kSentSize];
    memset(buf, 0, sizeof(buf));

    read(serialFD_, buf, kSentSize);
    bool isSucceeful = true;
    for(int i = 0; i != kSentSize; ++i)
    {
        if(sentBuf[i] != buf[i])
        {
            isSucceeful = false;
            break;
        }
    }
    if(isSucceeful)
    {
        ++succeesfulCount_;
        ui->successCount->setText(QString::number(succeesfulCount_));
    }
    ++sentCount_;
    ui->sentCount->setText(QString::number(sentCount_));

    if(sentCount_ >= 100)
    {
        if(succeesfulCount_ >= 90)
        {
            isTestPassed_ = true;
        }
        else
        {
            isTestPassed_ = false;
        }
        testDescription_ = QString::fromUtf8("发送 %1 帧；成功 %2 帧").arg(sentCount_).arg(succeesfulCount_);
        emit TestFinished();
        timer_.stop();
        return;
    }
    ui->sentLabel->setText(QByteArray(sentBuf).toHex());
    ui->recvLabel->setText(QByteArray(buf).toHex());
    for(int i = 0; i != kSentSize; ++i)
    {
        sentBuf[i] = rand() % 255;
    }
    write(serialFD_, sentBuf, 24);
}
