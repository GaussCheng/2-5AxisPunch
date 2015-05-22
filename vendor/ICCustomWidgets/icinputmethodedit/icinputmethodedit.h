#ifndef ICINPUTMETHODEDIT_H
#define ICINPUTMETHODEDIT_H

#include <QLineEdit>
#include <QMultiMap>
#include <QTimer>
#include "ICCustomWidgets_global.h"

class ICCUSTOMWIDGETSSHARED_EXPORT ICInputMethodEdit : public QLineEdit
{
    Q_OBJECT
public:
    enum EchoMode{EchoMode_Normal, EchoMode_PasswordAfter};
    explicit ICInputMethodEdit(QWidget *parent = 0);
    QSize sizeHint() const { return QSize(100, 30);}
    QSize minimumSizeHint() const { return sizeHint();}

    QFont TextFont() const { return this->font();}
    void SetTextFont(const QFont& font);

    QString ActualText() const { return actualText_;}
    void SetEchoMode(EchoMode echo) { echoMode_ = echo;}
    EchoMode GetEchoMode() const { return echoMode_;}

    bool IsDigitalOnly() const { return isDigital_;}
    void SetDigitalOnly(bool isDigital) { isDigital_ = isDigital;}
    void SetUpper(bool upper){ isUpper_ = upper;}

signals:

public slots:
    void Clear();
private slots:
    void OnEditTimeOut();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

#ifdef Q_WS_QWS
    void inputMethodEvent( QInputMethodEvent * event);
#endif

private:
    QTimer timer_;
    int oldKey_;
    int currentKeyChar_;
    QString actualText_;
    bool hasSelected_;
    EchoMode echoMode_;
    bool isDigital_;
    bool isUpper_;
    static QMultiMap<int, QChar> keyToCharMap_;
    static QMultiMap<int, QChar> digitalKeyToCharMap_;

};


inline void ICInputMethodEdit::SetTextFont(const QFont &font)
{
    QFont f = font;
    f.setPixelSize((f.pointSize() << 2) / 3);
    this->setFont(f);
}

#endif // ICINPUTMETHODEDIT_H
