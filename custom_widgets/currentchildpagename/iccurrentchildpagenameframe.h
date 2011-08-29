#ifndef ICCURRENTCHILDPAGENAMEFRAME_H
#define ICCURRENTCHILDPAGENAMEFRAME_H

#include <QFrame>

namespace Ui {
    class ICCurrentChildPageNameFrame;
}

class ICCurrentChildPageNameFrame : public QFrame
{
    Q_OBJECT

public:
    static ICCurrentChildPageNameFrame * Instance(QWidget *parent = 0)
    {
        if(instance_ == NULL)
        {
            instance_ = new ICCurrentChildPageNameFrame(parent);
        }
        return instance_;
    }
    ~ICCurrentChildPageNameFrame();

    void SetCurrentChildPageName(const QString & currentChildPageName);

private:
    Ui::ICCurrentChildPageNameFrame *ui;
    static ICCurrentChildPageNameFrame * instance_;
    explicit ICCurrentChildPageNameFrame(QWidget *parent = 0);
};

#endif // ICCURRENTCHILDPAGENAMEFRAME_H
