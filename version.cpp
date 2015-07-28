#include "version.h"


bool VersionCompatibleFNC(QStringList  &items,int &versoin)
{
    bool flag= 1;
    switch(items.size()){
    case VERSION_5_1_2_FNC:
        versoin = VERSION_5_1_2;
        for(int t=0; t < VERSION_5_1_2_POINT -  VERSION_5_1_2_POINT;t++){
            items.insert(items.size()-1,"0");
        }
        break;
    case VERSION_5_1_1_FNC:
        versoin = VERSION_5_1_1;
        for(int t=0; t < VERSION_5_1_2_POINT -  VERSION_5_1_1_POINT;t++){
            items.insert(items.size()-1,"0");
        }
        break;
    case VERSION_5_0_8_FNC:
        versoin = VERSION_5_0_8;
        for(int t = 0; t < VERSION_5_0_8_POINT;t ++){
            //兼容两位小数
            quint32 v = items.at(t * 2).toInt() * 10;
            items[t*2] = QString("%1").arg(v & 0xFFFF);
            items.insert(t*2 + 1,QString("%1").arg( (v >> 16) & 0xFFFF));
        }
        for(int t=0; t < VERSION_5_1_2_POINT -  VERSION_5_0_9_POINT;t++){
            items.insert(items.size()-1,"0");
        }
        break;
    default:
        flag = 0;
        break;
    }
    return flag;
}


bool VersionCompatiblePT(QStringList  &items)
{
    bool flag= 1;
    switch(items.size()){
    case VERSION_5_0_9_PT:
        break;
    case VERSION_5_0_8_PT:
         items << "100" << "0" << "0";
        break;
    default:
        flag = 0;
        break;
    }
    return flag;
}
