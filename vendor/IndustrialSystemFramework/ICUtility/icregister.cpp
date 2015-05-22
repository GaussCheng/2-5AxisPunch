#include "icregister.h"

#include <cstdlib>
#include <QDateTime>
#include "icutility.h"

#define SETTINGS_GROUP "ICRegister"
#define SETTINGS_LEFT_USE_TIME "LeftUseTime"
#define SETTINGS_LAST_LOG_TIME "LastLogTime"
ICRegisterPtr ICRegister::instance_;
ICRegister::ICRegister()
{
    int leftTime = LeftUseTime();
    isTimeOver_ = false;
    if(leftTime == 0)
    {
        isTimeOver_ = false;
        return;
    }
    QDateTime dt = LastLogTime_();
    int hourGo = qAbs(dt.secsTo(QDateTime::currentDateTime()) / 3600);
    leftTime -= hourGo;
    if(leftTime <= 0)
    {
        leftTime = -1;
        isTimeOver_ = true;
    }
    SetUseTime(leftTime);
    SetLastLogTime_(QDateTime::currentDateTime());
    connect(&checkTimer_,
            SIGNAL(timeout()),
            SLOT(OnHourGo()));
    checkTimer_.start(3600000);
}

QString ICRegister::GenerateMachineCode()
{
    QString ret;
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int i = 0; i != 6; ++i)
    {
        ret.append(QString::number(qrand() % 10));
    }
    return ret;
}

int ICRegister::Register(const QString &fC, const QString &mC, const QString &code)
{
    QString machineCode = fC + mC;
    int i,j;
    int pMap[10] = {0};
    int sortMap[20] = {0};
    char sortRet[19] = {0};
    long beg;
    quint8 buffer[20];

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
    sortMap[16] = 17;
    sortMap[17] = 16;
    sortMap[18] = 0;
    sortMap[19] = 0;

    if(code.size() != 20)
    {
        return -1;
    }
    for(i = 0; i != 18; ++i)
    {
        sortRet[sortMap[i]] = code[i].toAscii();
        buffer[i] = code[i].digitValue();
    }

    quint16 crcValue = ICUtility::CRC16(buffer,18);

    quint8 crc_Hi = crcValue >> 8 & 0xff;
    quint8 crc_Lo = crcValue & 0xff;
    if(crc_Hi > 9)
    {
        crc_Hi %= 9;
    }
    if(crc_Lo > 9)
    {
        crc_Lo %= 9;
    }
    if(crc_Hi != code[18].digitValue() || crc_Lo != code[19].digitValue())
    {
        return -1;
    }

    beg = 0;
    for(i = 0; i != 12; ++i)
    {
        beg += (machineCode[i].toAscii() - 48);
    }
    beg /= 10;
    beg %= 10;

    j = 0;
    for(i = beg; i < 10; ++i)
    {
        pMap[j++] = i;
    }
    for(i = 0; i < beg; ++i)
    {
        pMap[j++] = i;
    }

    for(i = 0; i != 18; ++i)
    {
        for(j = 0; j != 10; ++j)
        {
            if(j == (sortRet[i] - 48))
            {
                sortRet[i] = (pMap[j] + 48);
                break;
            }
        }
    }
    sortRet[18] = 0;
    for(i = 0; i != 12; ++i)
    {
        if(sortRet[i] != machineCode[i])
        {
            return -1;
        }
    }
    return atoi(sortRet + 12) * 168;
}

int ICRegister::LeftUseTime() const
{
    settings_.beginGroup(SETTINGS_GROUP);
    int ret = settings_.value(SETTINGS_LEFT_USE_TIME, 0).toInt();
    settings_.endGroup();
    return ret;
}

void ICRegister::SetUseTime(int hour)
{
    settings_.beginGroup(SETTINGS_GROUP);
    settings_.setValue(SETTINGS_LEFT_USE_TIME, hour);
    settings_.endGroup();
    isTimeOver_ = (hour < 0);
}

QDateTime ICRegister::LastLogTime_() const
{
    settings_.beginGroup(SETTINGS_GROUP);
    QDateTime ret = settings_.value(SETTINGS_LAST_LOG_TIME, QDateTime::currentDateTime()).toDateTime();
    settings_.endGroup();
    return ret;
}

void ICRegister::SetLastLogTime_(const QDateTime &dt)
{
    settings_.beginGroup(SETTINGS_GROUP);
    settings_.setValue(SETTINGS_LAST_LOG_TIME, dt);
    settings_.endGroup();
}

void ICRegister::OnHourGo()
{
    int leftTime = LeftUseTime() - 1;
    if(leftTime == 0)
    {
        leftTime = -1;
        isTimeOver_ = true;
    }
    SetUseTime(leftTime);
    SetLastLogTime_(QDateTime::currentDateTime());
}
