#include "contentwidget.h"
#include "ui_contentwidget.h"

ContentWidget::ContentWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ContentWidget)
{
    ui->setupUi(this);
}

ContentWidget::~ContentWidget()
{
    delete ui;
}
void ContentWidget::receiveData(QString data) {};
