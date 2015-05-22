#include "ictableview.h"
#include <QAction>
#include <QScrollBar>
#include <QKeyEvent>
#include "iccurrentindexmask.h"
ICTableView::ICTableView(QWidget *parent) :
    QTableView(parent),
    upDownWay_(UpDownWay_SingleRow),
    isEn_(true)
{
    font_.setPointSize(13);
    SetTextFont(font_);
    upAction_ = new QAction(this);
    downAction_ = new QAction(this);
    this->addAction(upAction_);
    this->addAction(downAction_);
    SetUpKey(Qt::Key_PageUp);
    SetDownKey(Qt::Key_PageDown);
    setSelectionMode(QAbstractItemView::MultiSelection);

    connect(upAction_,
            SIGNAL(triggered()),
            SLOT(UpActionTrigger()));
    connect(downAction_,
            SIGNAL(triggered()),
            SLOT(DownActionTrigger()));

    mask_ = new ICCurrentIndexMask(this);
    mask_->setFixedHeight(0);
}

ICTableView::~ICTableView()
{
    delete upAction_;
    delete downAction_;
    delete mask_;
}

void ICTableView::showEvent(QShowEvent *e)
{
    mask_->setFixedWidth(width());
    if(currentIndex().row() == -1)
    {
        mask_->hide();
    }
    QTableView::showEvent(e);
}

void ICTableView::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void ICTableView::keyReleaseEvent(QKeyEvent *e)
{
    e->ignore();
}

void ICTableView::SetUpKey(int key)
{
    upAction_->setShortcut(key);
}

void ICTableView::SetDownKey(int key)
{
    downAction_->setShortcut(key);
}

void ICTableView::UpActionTrigger()
{
    if(!isEn_)
    {
        return;
    }
    if(CurrentUpDownWay() == UpDownWay_Page)
    {
        if((verticalScrollBar()->value() == verticalScrollBar()->minimum()))
        {
            verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
            return;
        }
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->value() - 10);
        return;
    }
    mask_->setFixedHeight(visualRect(model()->index(0, 0)).height());
    QAbstractItemModel *tmpModel = model();
    const int currentRow = currentIndex().row();
    if(currentRow == 0 || currentRow == -1)
    {
        this->scrollToBottom();
        selectionModel()->setCurrentIndex(tmpModel->index(tmpModel->rowCount() - 1, 0), QItemSelectionModel::NoUpdate);
        emit CurrentIndexChanged(tmpModel->rowCount() - 1);
    }
    else
    {
        this->scrollTo(tmpModel->index(currentRow - 1, 0));
        selectionModel()->setCurrentIndex(tmpModel->index(currentRow - 1, 0), QItemSelectionModel::NoUpdate);
        emit CurrentIndexChanged(currentRow - 1);
    }
}

void ICTableView::DownActionTrigger()
{
    if(!isEn_)
    {
        return;
    }
    if(CurrentUpDownWay() == UpDownWay_Page)
    {
        if((verticalScrollBar()->value()) == (verticalScrollBar()->maximum()))
        {
            verticalScrollBar()->setSliderPosition(0);
            return;
        }
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->value() + 10);
        return;
    }
    mask_->setFixedHeight(visualRect(model()->index(0, 0)).height());
    QAbstractItemModel *tmpModel = model();
    const int currentRow = currentIndex().row();
    if(currentRow == model()->rowCount() - 1)
    {
        this->scrollToTop();
        selectionModel()->setCurrentIndex(tmpModel->index(0, 0), QItemSelectionModel::NoUpdate);
        emit CurrentIndexChanged(0);
    }
    else
    {
        this->scrollTo(tmpModel->index(currentRow + 1, 0));
        selectionModel()->setCurrentIndex(tmpModel->index(currentRow + 1, 0), QItemSelectionModel::NoUpdate);
        emit CurrentIndexChanged(currentRow + 1);
    }

    return;
}

QList<int> ICTableView::SelectedRowsIndex() const
{
    QList<int> ret;
    QModelIndexList indexlist = selectedIndexes();
//    QModelIndex index;
    for(int i = 0; i < indexlist.size(); i += 2)
    {
        ret.append(indexlist.at(i).row());
    }
    return ret;
}

void ICTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if(upDownWay_ == UpDownWay_Page)
    {
        return;
    }
    if(current.isValid())
    {
        mask_->move(0, visualRect(current).topLeft().y() + horizontalHeader()->height() + 1);
        mask_->show();
    }
    else
    {
        mask_->hide();
        emit CurrentIndexChanged(current.row());
    }
}
