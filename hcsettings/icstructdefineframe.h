#ifndef ICSTRUCTDEFINEFRAME_H
#define ICSTRUCTDEFINEFRAME_H

#include <QWidget>
#include <QMap>

namespace Ui {
    class ICStructDefineFrame;
}

class ICStructDefineFrame : public QWidget
{
    Q_OBJECT

public:
    explicit ICStructDefineFrame(QWidget *parent = 0);
    ~ICStructDefineFrame();

protected:
    void changeEvent(QEvent *e);

private slots:
    void OnSelectChanged(int index);

    void on_saveButton_clicked();

private:
    Ui::ICStructDefineFrame *ui;
    uint armStruct_;
    QMap<QString, uint> armTypeMaskMap_;
};

#endif // ICSTRUCTDEFINEFRAME_H
