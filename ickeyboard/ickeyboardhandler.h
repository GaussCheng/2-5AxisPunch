#ifndef ICKEYBOARDHANDLER_H
#define ICKEYBOARDHANDLER_H

#include <QObject>
#include <QMap>
#include <QScopedPointer>

class ICKeyboardHandler : public QObject
{
    Q_OBJECT
public:
    static ICKeyboardHandler* Instance()
    {
        if(instance_.isNull())
        {
            instance_.reset(new ICKeyboardHandler());
        }
        return instance_.data();
    }

signals:

public slots:
    void Keypressed(int keyValue);
    void SwitchChanged(int value);
    void PulleyChanged(int value);
private Q_SLOTS:

private:
    static QScopedPointer<ICKeyboardHandler> instance_;
    QMap<int, int> virtualKeyMap_;
    explicit ICKeyboardHandler(QObject *parent = 0);
    int pulleyTurn_;
};

#endif // ICKEYBOARDHANDLER_H
