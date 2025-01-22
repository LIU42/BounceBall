#include "views/widgets/successwidget.h"

#include "ui_successwidget.h"

SuccessWidget::SuccessWidget(QWidget* parent): QWidget(parent), ui(new Ui::SuccessWidget)
{
    ui->setupUi(this);
}

SuccessWidget::~SuccessWidget()
{
    delete ui;
}
