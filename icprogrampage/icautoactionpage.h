#ifndef ICAUTOACTIONPAGE_H
#define ICAUTOACTIONPAGE_H

#include <QWidget>

namespace Ui {
class ICAutoActionPage;
}

class ICAutoActionPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICAutoActionPage(QWidget *parent = 0);
    ~ICAutoActionPage();
    
private:
    Ui::ICAutoActionPage *ui;
};

#endif // ICAUTOACTIONPAGE_H
