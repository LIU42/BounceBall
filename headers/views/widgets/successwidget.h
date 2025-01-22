#ifndef __VIEWS_WIDGETS_SUCCESSWIDGET_H__
#define __VIEWS_WIDGETS_SUCCESSWIDGET_H__

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class SuccessWidget;
}

QT_END_NAMESPACE

class SuccessWidget : public QWidget
{
    Q_OBJECT

    private:
        Ui::SuccessWidget* ui;

    public:
        SuccessWidget(QWidget* parent = nullptr);
        ~SuccessWidget();
};
#endif
