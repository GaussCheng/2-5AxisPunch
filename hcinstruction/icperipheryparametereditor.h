#ifndef ICPERIPHERYPARAMETEREDITOR_H
#define ICPERIPHERYPARAMETEREDITOR_H

#include <QWidget>

namespace Ui {
    class ICPeripheryParameterEditor;
}

class ICPeripheryParameterEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ICPeripheryParameterEditor(QWidget *parent = 0);
    ~ICPeripheryParameterEditor();
    int Delay() const;
    int Times() const;

private:
    Ui::ICPeripheryParameterEditor *ui;
};

#endif // ICPERIPHERYPARAMETEREDITOR_H
