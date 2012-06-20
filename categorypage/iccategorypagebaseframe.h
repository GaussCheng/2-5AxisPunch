#ifndef ICCATEGORYPAGEBASEFRAME_H
#define ICCATEGORYPAGEBASEFRAME_H

#include <QFrame>

class QTimer;

namespace Ui {
    class ICCategoryPageBaseFrame;
}

class ICCategoryPageBaseFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ICCategoryPageBaseFrame(QWidget *parent = 0);
    ~ICCategoryPageBaseFrame();

protected:

private:
    void InitSignal();

private:
    Ui::ICCategoryPageBaseFrame *ui;

    QTimer *showWindowTimer_;

private slots:

    void MoveShortCutFrame();
};

#endif // ICCATEGORYPAGEBASEFRAME_H
