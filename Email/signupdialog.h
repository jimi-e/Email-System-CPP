#pragma once

#include <QDialog>
#include <infomation.h>
namespace Ui {
    class SignupDialog;
}

class SignupDialog : public QDialog
{
    Q_OBJECT
        /********mouse drag***********/
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    QPoint mousePoint;
    bool mouse_press;
    /********mouse drag***********/
public:
    explicit SignupDialog(QWidget* parent = nullptr);
    ~SignupDialog();

signals:
    void sendData(QString data);

private slots:
    void receiveData(QString data);

    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();




private:
    Ui::SignupDialog* ui;
    Infomation* infomation;
};

