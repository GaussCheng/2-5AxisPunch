#ifndef ICBUTTONGROUP_H
#define ICBUTTONGROUP_H

#include <QObject>
#include <QButtonGroup>

class ICButtonGroup : public QObject
{
    Q_OBJECT
public:
    explicit ICButtonGroup(QButtonGroup* bg, QObject *parent = 0);

    int LastValue() const { return oldValue_;}
    QButtonGroup* ButtongGroup() { return bg_;}

signals:
    void buttonClicked(int id, int oldID);

private slots:
    void OnIDChanged(int id);
private:
    QButtonGroup* bg_;
    int oldValue_;
};

#endif // ICBUTTONGROUP_H
