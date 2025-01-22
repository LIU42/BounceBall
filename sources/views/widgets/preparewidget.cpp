#include "views/widgets/preparewidget.h"

#include "ui_preparewidget.h"

PrepareWidget::PrepareWidget(QWidget* parent): QWidget(parent), ui(new Ui::PrepareWidget)
{
    ui->setupUi(this);
}

PrepareWidget::~PrepareWidget()
{
    delete ui;
}
