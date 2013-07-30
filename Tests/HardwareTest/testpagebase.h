#ifndef TESTPAGEBASE_H
#define TESTPAGEBASE_H

#include <QWidget>

class TestPageBase : public QWidget
{
    Q_OBJECT
public:
    explicit TestPageBase(QWidget *parent = 0);

    virtual bool IsTestPassed() const = 0;
    virtual QString TestDescription() const = 0;
    
signals:
    void TestFinished();
    
public slots:
    
};

#endif // TESTPAGEBASE_H
