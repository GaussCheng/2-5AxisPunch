#ifndef PAGESWITCHBAR_H
#define PAGESWITCHBAR_H

#include <QFrame>

namespace Ui {
    class PageSwitchBar;
}

class PageSwitchBar : public QFrame
{
    Q_OBJECT

public:
    enum CurrentState
    {
        Input,
        Output
    };
    explicit PageSwitchBar(QWidget *parent = 0);
    ~PageSwitchBar();

    int State() const;

protected:
    void changeEvent(QEvent *e);


Q_SIGNALS:
    void NextButtonClicked();
    void PreButtonClicked();
    void SelectedInput();
    void SelectedOutput();

private:
    Ui::PageSwitchBar *ui;

//private slots:
//    void on_ioSelector_currentIndexChanged(int index);
};


#endif // PAGESWITCHBAR_H
