#ifndef ICINSTRUCTIONACTIONPAGEBASE_H
#define ICINSTRUCTIONACTIONPAGEBASE_H

#include <QWidget>

class ICInstructionActionPageBase : public QWidget
{
    Q_OBJECT
public:
    explicit ICInstructionActionPageBase(QWidget *parent = 0);
    QByteArray CreateCommand() const { return CreateCommandImpl();}

signals:

public slots:

protected:
    virtual QByteArray CreateCommandImpl() const = 0;

};

#endif // ICINSTRUCTIONACTIONPAGEBASE_H
