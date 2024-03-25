#pragma once

#include <QWidget>

namespace Ui {
class ContentWidget;
}

class ContentWidget : public QWidget
{
    Q_OBJECT
signals:
     void sendData(QString data);

private slots:
     void receiveData(QString data);

public:
    explicit ContentWidget(QWidget *parent = nullptr);
    ~ContentWidget();

private:
    Ui::ContentWidget *ui;
};

