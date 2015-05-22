#ifndef ICUSER_H
#define ICUSER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QSharedPointer>
#include "ICCore_global.h"

class ICCORESHARED_EXPORT ICUser
{
public:
    ICUser() { Init_();}
    ICUser(const QString& account = QString(),
           const QString& name = QString(),
           int level = 0,
           const QString& password = QString()) {Init_(account,
                                                       name,
                                                       level,
                                                       password);}

    virtual ~ICUser(){}

    QString Account() const { return account_;}
    void SetAccount(const QString& account) {account_ = account;}

    QString Name() const { return name_;}
    void SetName(const QString& name) { name_ = name;}

    int Level() const { return level_;}
    void SetLevel(int level) { level_ = level;}

    QString Password() const { return password_;}
    void SetPassword(const QString& ps) { password_ = ps;}

    QList<int> Permissions() const { return permissions_;}
    void SetPermissions(const QList<int>& permissions);
    void SetPermission(int p);
    void ClearPermission(int p);
    void FetchPermissions();

    bool HasPermission(int p) const { return permissions_.contains(p);}

    virtual QString Display() const { return Account() + ":" + Name();}

private:
    void Init_(const QString& account = QString(),
               const QString& name = QString(),
               int level = 0,
               const QString& password = QString())
    {
        account_ = account;
        name_ = name;
        level_ = level;
        password_ = password;
    }
    QString account_;
    QString name_;
    int level_;
    QString password_;
    QList<int> permissions_;

};

typedef QSharedPointer<ICUser> ICUserPTR;
typedef QList<ICUserPTR> ICUserList;

class ICCORESHARED_EXPORT ICUserCenter
{
public:
    static ICUserList UserList() { return userList_;}

    static void FetchUsers();

    static ICUserPTR GetDefaultGuest() { return guest_;}
    static ICUserPTR CurrentUser() { return currentUser_;}
    static void SetCurrentUser(ICUserPTR user) {currentUser_ = user;}
    static ICUserPTR AddUser(const QString& account, const QString& name,
                           int level,
                           const QString& password);
    static void RemoveUser(const QString& account);

    static bool IsUserExist(const QString& account);

    static QString SuperPassword() { return superPassword_;}
    static void SetSuperPassword(const QString& super) {superPassword_ = super;}
    static bool Login(const QString& account, const QString& pa);
    static void Logout();

    static bool IsRootLogin() { return isRootLogin_;}

    static ICUserPTR FindUser(const QString& account);
    static QStringList UserAccountList(int level = 50);

    static bool ChangePassword(const QString& account, const QString& pa);
    static bool ChangePermission(const QString& account , int p, int action);
    static bool ChangeLevel(const QString& account, int level);


private:
    static ICUserList userList_;
    static ICUserPTR guest_;
    static ICUserPTR currentUser_;
    static QString superPassword_;
    static bool isRootLogin_;
};

#endif // ICUSER_H
