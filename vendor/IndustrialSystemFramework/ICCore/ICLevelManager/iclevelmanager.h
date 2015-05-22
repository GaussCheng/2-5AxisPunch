#ifndef ICLEVELMANAGER_H
#define ICLEVELMANAGER_H

#include <QString>
#include "ICCore_global.h"

class ICCORESHARED_EXPORT ICLevelManager
{
public:
    ICLevelManager();
    static int CurrentLevel() { return currentLevel_;}
    static QString CurrentUser() { return currentUser_;}
    bool Login(const QString& account, const QString& password);
    void Logout() { LogoutImpl();}
    bool ResetPassword(const QString &account, const QString& oldPassword, const QString newPassword);
    int AccountLevel(const QString &account) const { return AccountLevelImpl(account);}
    QString CurrentLevelString() const { return CurrentLevelStringImpl();}

protected:
    virtual bool LoginImpl(const QString &account, const QString& password) = 0;
    virtual bool ResetPasswordImpl(const QString &account, const QString& oldPassword, const QString newPassword) = 0;
    virtual void LogoutImpl() { currentLevel_ = 0;}
    virtual int AccountLevelImpl(const QString& account) const = 0;
    virtual QString CurrentLevelStringImpl() const = 0;
private:
    static int currentLevel_;
    static QString currentUser_;
};

inline bool ICLevelManager::Login(const QString &account, const QString &password)
{
    int oldCL = currentLevel_;
    QString oldCU = currentUser_;
    currentLevel_ = AccountLevel(account);
    currentUser_ = account;
    bool isOk = LoginImpl(account, password);
    if(!isOk)
    {
        currentLevel_ = oldCL;
        currentUser_ = oldCU;
    }
    return isOk;
}

inline bool ICLevelManager::ResetPassword(const QString &account, const QString &oldPassword, const QString newPassword)
{
    return ResetPasswordImpl(account, oldPassword, newPassword);
}

#endif // ICLEVELMANAGER_H
