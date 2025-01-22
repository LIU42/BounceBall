#ifndef __VIEWS_WIDGETS_PREPAREWIDGET_H__
#define __VIEWS_WIDGETS_PREPAREWIDGET_H__

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class PrepareWidget;
}

QT_END_NAMESPACE

class PrepareWidget : public QWidget
{
    Q_OBJECT

    private:
        Ui::PrepareWidget* ui;

    public:
        PrepareWidget(QWidget* parent = nullptr);
        ~PrepareWidget();
};
#endif
