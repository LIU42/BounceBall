#ifndef __VIEWS_WIDGETS_FAILUREWIDGET_H__
#define __VIEWS_WIDGETS_FAILUREWIDGET_H__

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class FailureWidget;
}

QT_END_NAMESPACE

class FailureWidget : public QWidget
{
    Q_OBJECT

    private:
        Ui::FailureWidget* ui;

    public:
        FailureWidget(QWidget* parent = nullptr);
        ~FailureWidget();

    public:
        void setThisScore(int thisScore);
        void setBestScore(int bestScore);
};
#endif
