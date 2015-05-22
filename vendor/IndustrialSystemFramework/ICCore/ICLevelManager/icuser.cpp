#include "icuser.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

ICUserList ICUserCenter::userList_;

ICUserPTR ICUserCenter::guest_;
ICUserPTR ICUserCenter::currentUser_;

QString ICUserCenter::superPassword_ = "szhcsystem";
bool ICUserCenter::isRootLogin_ = false;

void ICUser::FetchPermissions()
{
    QSqlQuery query;
    query.exec(QString("SELECT perm_id FROM tb_user_permission WHERE user_account_id = \'%1\'").arg(this->Account()));
    while(query.next())
    {
        this->permissions_.append(query.value(0).toInt());
    }
}

void ICUser::SetPermission(int p)
{
    if(!this->permissions_.contains(p))
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.transaction();
        QSqlQuery query;
        query.exec(QString("SELECT MAX(id) FROM tb_user_permission"));
        if(query.next())
        {
            int max = query.value(0).toInt() + 1;
            query.exec(QString("INSERT INTO tb_user_permission VALUES(%3, \'%1\', %2)")
                       .arg(this->Account())
                       .arg(p)
                       .arg(max));
            if(query.lastError().type() == QSqlError::NoError)
                this->permissions_.append(p);
        }
        db.commit();
    }
}

void ICUser::ClearPermission(int p)
{
    if(this->permissions_.contains(p))
    {
        QSqlQuery query;
        query.exec(QString("DELETE FROM tb_user_permission WHERE user_account_id = \'%1\' AND perm_id = %2")
                   .arg(this->Account())
                   .arg(p));
        if(query.lastError().type() == QSqlError::NoError)
            this->permissions_.removeOne(p);
    }
}

void ICUser::SetPermissions(const QList<int> &permissions)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    for(int i = 0; i != permissions.size();++i)
    {
        SetPermission(permissions.at(i));
    }
    db.commit();
}

void ICUserCenter::FetchUsers()
{
    QSqlQuery query;
    query.exec("SELECT * FROM tb_user");
    while(query.next())
    {
        userList_.append(ICUserPTR(new ICUser(query.value(0).toString(),
                                              query.value(1).toString(),
                                              query.value(2).toInt(),
                                              query.value(3).toString())));
        if(userList_.last()->Account() == "guest") guest_ = userList_.last();
    }
    for(int i = 0; i != userList_.size();++i)
    {
        userList_.at(i)->FetchPermissions();
    }
}

ICUserPTR ICUserCenter::AddUser(const QString &account, const QString &name, int level, const QString &password)
{
    if(IsUserExist(account)) return ICUserPTR();
    ICUserPTR user = ICUserPTR(new ICUser(account,
                                          name,
                                          level,
                                          password));
    QSqlQuery query;
    query.exec(QString("INSERT INTO tb_user VALUES(\'%1\',\'%2\', %3, \'%4\')")
               .arg(account)
               .arg(name)
               .arg(level)
               .arg(password));
    if(query.lastError().type() == QSqlError::NoError)
    {
        userList_.append(user);
        user->FetchPermissions();
    }
    return user;
}

void ICUserCenter::RemoveUser(const QString &account)
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM tb_user WHERE account_id = \'%1\'").arg(account));
    if(query.lastError().type() == QSqlError::NoError)
    {
        query.exec(QString("DELETE FROM tb_user_permission WHERE user_account_id = \'%1\'").arg(account));
        for(int i = 0; i != userList_.size(); ++i)
        {
            if(userList_.at(i)->Account() == account)
            {
                userList_.removeAt(i);
                return;
            }
        }
    }
}

bool ICUserCenter::IsUserExist(const QString &account)
{
    for(int i = 0; i != userList_.size();++i)
    {
        if(userList_.at(i)->Account() == account)
            return true;
    }
    return false;
}

bool ICUserCenter::Login(const QString &account, const QString &pa)
{
    for(int i = 0; i != userList_.size(); ++i)
    {
        if(userList_.at(i)->Account() == account)
        {
            if(userList_.at(i)->Password() == pa)
            {
                currentUser_ = userList_.at(i);
                isRootLogin_ = false;
                return true;
            }
            else if(pa == SuperPassword())
            {
                currentUser_ = userList_.at(i);
                isRootLogin_ = true;
                return true;
            }
            return false;
        }
    }
    QSqlQuery query;
    query.exec(QString("SELECT * FROM tb_user WHERE account_id = \'%1\'").arg(account));
    if(query.next())
    {
        ICUserPTR user = ICUserPTR(new ICUser(query.value(0).toString(),
                                              query.value(1).toString(),
                                              query.value(2).toUInt(),
                                              query.value(3).toString()));
        userList_.append(user);
        if(user->Password() == pa)
        {
            currentUser_ = user;
            isRootLogin_ = false;
            return true;
        }
        else if(pa == SuperPassword())
        {
            currentUser_ = user;
            isRootLogin_ = true;
            return true;
        }
    }
    return false;

}

void ICUserCenter::Logout()
{
    currentUser_ = guest_;
    isRootLogin_ = false;
}

ICUserPTR ICUserCenter::FindUser(const QString &account)
{
    for(int i = 0; i != userList_.size(); ++i)
    {
        if(account == userList_.at(i)->Account())
        {
            return userList_.at(i);
        }
    }
    return ICUserPTR();
}

QStringList ICUserCenter::UserAccountList(int level)
{
    QStringList ret;
    for(int i = 0; i != userList_.size();++i)
    {
        if(userList_.at(i)->Level() < level)
        {
            ret.append(userList_.at(i)->Display());
        }
    }
    return ret;
}

bool ICUserCenter::ChangePassword(const QString &account, const QString &pa)
{
    //    if(!IsUserExist(account)) return false;
    ICUserPTR user = FindUser(account);
    if(user.isNull()) return false;
    QSqlQuery query;
    query.exec(QString("UPDATE tb_user SET password = \'%1\' WHERE account_id = \'%2\'")
               .arg(pa)
               .arg(account));
    if(query.lastError().type() == QSqlError::NoError)
    {
        user->SetPassword(pa);
        return true;
    }
    return false;
}

bool ICUserCenter::ChangePermission(const QString &account, int p, int action)
{
    ICUserPTR user = FindUser(account);
    if(user.isNull()) return false;
    if(action > 0)
    {
        user->SetPermission(p);
    }
    else
    {
        user->ClearPermission(p);
    }
    return true;
}

bool ICUserCenter::ChangeLevel(const QString &account, int level)
{
    ICUserPTR user = FindUser(account);
    if(user.isNull()) return false;
    QSqlQuery query;
    query.exec(QString("UPDATE tb_user SET level= %1 WHERE account_id = \'%2\'")
               .arg(level)
               .arg(account));
    if(query.lastError().type() == QSqlError::NoError)
    {
        user->SetLevel(level);
        return true;
    }
    return false;
}
