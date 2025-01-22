#include "views/widgets/failurewidget.h"

#include "ui_failurewidget.h"

FailureWidget::FailureWidget(QWidget* parent): QWidget(parent), ui(new Ui::FailureWidget)
{
    ui->setupUi(this);
}

FailureWidget::~FailureWidget()
{
    delete ui;
}

void FailureWidget::setThisScore(int thisScore)
{
    ui->thisScoreValueLabel->setNum(thisScore);
}

void FailureWidget::setBestScore(int bestScore)
{
    ui->bestScoreValueLabel->setNum(bestScore);
}
