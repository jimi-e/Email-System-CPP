#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QMainWindow>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QMainWindow
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
    explicit LoginWidget(QWidget* parent = nullptr);
    ~LoginWidget();

signals:
    void sendData(QString data);

private slots:
    void on_button_login_clicked();

    void on_button_view_pressed();

    void on_button_view_released();

    void on_lineEdit_pw_returnPressed();

    void on_button_quit_clicked();

    void on_button_signup_clicked();

    void receiveData(QString data);




private:
    Ui::LoginWidget* ui;
};

#endif // LOGINWIDGET_H
