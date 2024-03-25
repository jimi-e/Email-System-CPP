#include "infomation.h"
#include "ui_infomation.h"
#include <QMovie>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

Infomation::Infomation(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Infomation)
{
    ui->setupUi(this);
    QMovie* movie = new QMovie(":/res/ymn.gif");
    ui->label->setMovie(movie);
    movie->start();
}

Infomation::~Infomation()
{
    delete ui;
}



void Infomation::on_confirm_clicked()
{
    this->hide();
}


