#ifndef ICRETURNPAGE_H
#define ICRETURNPAGE_H

#include <QDialog>

namespace Ui {
    class ICReturnPage;
}

class ICReturnPage : public QDialog
{
    Q_OBJECT

public:
    explicit ICReturnPage(QWidget *parent = 0);
    ~ICReturnPage();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void StatusRefresh();
    void on_stopBtn_clicked();

private:
    Ui::ICReturnPage *ui;
    bool isRan_;
};

#endif // ICRETURNPAGE_H
