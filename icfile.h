#ifndef ICFILE_H
#define ICFILE_H

#include <QString>
//enum ICFileOpenMode
//{
//    NotOpen = 0x0000,
//    ReadOnly = 0x0001,
//    WriteOnly = 0x0002,
//    ReadWrite = ReadOnly | WriteOnly,
//    Append = 0x0004,
//    Truncate = 0x0008,
//    Text = 0x0010,
//    Unbuffered = 0x0020
//};

class ICFile
{
public:
    ICFile(const QString& filename);
//    bool open(ICFileOpenMode openMode);
//    void close();
    bool ICWrite(const QByteArray& toWrite);

private:
    QString fileName_;
};

#endif // ICFILE_H
