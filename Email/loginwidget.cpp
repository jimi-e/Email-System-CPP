#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QDebug>

#include <client_net.h>
#include <identify.h>
#include <mail.h>

#include<form.h>
#include <QWidget>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QEvent>

#include <QMessageBox>

#include "signupdialog.h"
#define IP_addr "192.168.43.38"

LoginWidget::LoginWidget(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);//center

    QRegExp rx("[a-zA-Z0-9\\_]{15}");
    ui->lineEdit_pw->setValidator(new QRegExpValidator(rx, this));
    ui->lineEdit_usr->setValidator(new QRegExpValidator(rx, this));

}

LoginWidget::~LoginWidget()
{
    delete ui;
}


/************drag**************/
void LoginWidget::mousePressEvent(QMouseEvent* event)
{

    if ((event->button() == Qt::LeftButton)) {
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
        //        event->accept();
    }
    else if (event->button() == Qt::RightButton) {
        //如果是右键
      //  this->close();

    }
}
void LoginWidget::mouseMoveEvent(QMouseEvent* event)
{


    //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
    if (mouse_press) {
        move(event->globalPos() - mousePoint);
        //        event->accept();
    }
}
void LoginWidget::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_press = false;
}

/************drag**************/


void LoginWidget::on_button_login_clicked()
{
    QString usr = this->ui->lineEdit_usr->text();

    //qDebug()<<global_usr<<endl;
    QString pw = this->ui->lineEdit_pw->text();
    if (usr.isEmpty()) {
        qDebug() << "usr is null!" << endl;  QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::warning(NULL, "错误", "用户名为空!",
            QMessageBox::Yes | QMessageBox::Yes); return;
    }
    if (pw.isEmpty()) {
        qDebug() << "pw is null!" << endl;  QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::warning(NULL, "错误", "请输入密码!",
            QMessageBox::Yes | QMessageBox::Yes); return;
    }
    Identify login_i_t;
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock(IP_addr, 5000, addr, AF_INET);//2
    if (sock == -1) { qDebug() << "connect error" << endl; }
    else
        if (sock == 0) { qDebug() << "socket error" << endl; }
        else {
            qDebug() << "connet success" << endl;

            QString pair = usr + "/" + pw;
            QByteArray PAIR;
            PAIR.append(pair);
            char* msg = PAIR.data();
            qDebug() << msg << endl;
            int flag = login_i_t.to_check(sock, msg);//3

            qDebug() << "flag:" << flag << "password was sent" << endl;
            if (flag == 0)
            {
                qDebug() << "login success" << endl;
                client_base::close_sock(sock);
                Form* form = new Form();

                connect(this, SIGNAL(sendData(QString)), form, SLOT(receiveData(QString)), Qt::UniqueConnection);
                emit  sendData(ui->lineEdit_usr->text());
                //  client_base::str_usrname=ui->lineEdit_usr->text().toStdString();
                 // client_base::name_write(ui->lineEdit_usr->text().toStdString());
                  //global_usr=ui->lineEdit_usr->text();
                form->show();
                this->close();
                return;
            }
            else if (flag == 1) {
                qDebug() << "invalid ursname" << endl;
                QApplication::setQuitOnLastWindowClosed(false);
                QMessageBox::warning(NULL, "错误", "密码错误!", QMessageBox::Yes | QMessageBox::Yes);
            }
            else if (flag == -1) {
                qDebug() << "password error" << endl;
                QApplication::setQuitOnLastWindowClosed(false);
                QMessageBox::warning(NULL, "错误", "用户名不存在!", QMessageBox::Yes | QMessageBox::Yes);
            }

            else {
                qDebug() << "uncertain error" << endl;   QApplication::setQuitOnLastWindowClosed(false);
                QMessageBox::warning(NULL, "错误", "未知错误!",
                    QMessageBox::Yes | QMessageBox::Yes);
            }
        }

    client_base::close_sock(sock);
}
void LoginWidget::on_button_view_pressed()
{
    ui->lineEdit_pw->setEchoMode(QLineEdit::Normal);
}

void LoginWidget::on_button_view_released()
{
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);
}

void LoginWidget::on_lineEdit_pw_returnPressed()
{
    LoginWidget::on_button_login_clicked();
}

void LoginWidget::on_button_quit_clicked()
{

    this->close();
}
void LoginWidget::receiveData(QString data)
{
    ui->lineEdit_usr->setText(data);
}

void LoginWidget::on_button_signup_clicked()
{
    SignupDialog* signupdialog = new SignupDialog(this);
    signupdialog->show();
}
