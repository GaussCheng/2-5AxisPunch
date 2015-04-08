#ifndef ICMAUALACTIONPAGE_H
#define ICMAUALACTIONPAGE_H

#include <QWidget>

namespace Ui {
class ICMaualActionPage;
}

class ICMaualActionPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICMaualActionPage(QWidget *parent = 0);
    ~ICMaualActionPage();
    
private:
    Ui::ICMaualActionPage *ui;
};

#endif // ICMAUALACTIONPAGE_H
