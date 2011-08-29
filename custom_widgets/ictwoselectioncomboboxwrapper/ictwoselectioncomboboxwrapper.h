#ifndef ICTWOSELECTIONCOMBOBOXWRAPPER_H
#define ICTWOSELECTIONCOMBOBOXWRAPPER_H

#include <QObject>

class QComboBox;

class ICTwoSelectionComboBoxWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ICTwoSelectionComboBoxWrapper(QComboBox* comboBox, int addr, QObject *parent = 0);

    void UpdateParameter();
signals:

public slots:
private Q_SLOTS:
    void IndexChanged(int index);
private:
    QComboBox* wrappedWidget_;
    int addr_;

};

#endif // ICTWOSELECTIONCOMBOBOXWRAPPER_H
