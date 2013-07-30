#ifndef ICPICTUREVIEW_H
#define ICPICTUREVIEW_H

#include <QWidget>
#include <QDir>
#include <QListWidgetItem>

namespace Ui {
class ICPictureView;
}

class ICPictureView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ICPictureView(QWidget *parent = 0);
    ~ICPictureView();

    void ScanPictures(const QString& path);
    QString CurrentSelectedPicture() const;
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_picList_itemClicked(QListWidgetItem *item);

private:
    Ui::ICPictureView *ui;
    QDir picturesDir_;
    QString currentPic_;
};

#endif // ICPICTUREVIEW_H
