#ifndef PROGRAMEDITORFRAME_H
#define PROGRAMEDITORFRAME_H

#include <QFrame>

namespace Ui {
    class ProgramEditorFrame;
}

class ProgramEditorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ProgramEditorFrame(QWidget *parent = 0);
    ~ProgramEditorFrame();

private:
    Ui::ProgramEditorFrame *ui;
};

#endif // PROGRAMEDITORFRAME_H
