#include "signupdialog.h"
#include "ui_signupdialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QMessageBox>
#include <QtDebug>


#include <loginwidget.h>

#include <client_net.h>
#include <mail.h>

#define IP_addr "192.168.43.38"

#include <identify.h>
SignupDialog::SignupDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SignupDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);//center

    QRegExp rx("[a-zA-Z0-9\\_]{15}");
    QRegExp rp("[a-zA-Z0-9\\_]{15}");
    ui->label_invisible->setVisible(false);
    ui->lineEdit_usr->setValidator(new QRegExpValidator(rp, this));
    ui->lineEdit_nn->setValidator(new QRegExpValidator(rp, this));
    ui->lineEdit_pw->setValidator(new QRegExpValidator(rx, this));
    ui->lineEdit_cpw->setValidator(new QRegExpValidator(rx, this));
    //  qDebug()<<this->property("")<<endl;

}

SignupDialog::~SignupDialog()
{
    delete ui;
}


/************drag**************/
void SignupDialog::mousePressEvent(QMouseEvent* event)
{

    if ((event->button() == Qt::LeftButton)) {
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
        //        event->accept();
    }
    else if (event->button() == Qt::RightButton) {
        //如果是右键
       // this->close();

    }
}
void SignupDialog::mouseMoveEvent(QMouseEvent* event)
{


    //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
    if (mouse_press) {
        move(event->globalPos() - mousePoint);
        //        event->accept();
    }
}
void SignupDialog::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_press = false;
}

/************drag**************/


bool IsMatchRegX(const QString& str)
{
    QString reg = "^(?![\\d]+$)(?![a-zA-Z]+$)(?![^\\da-zA-Z]+$).{8,16}$";
    QRegExp rx(reg);
    bool ok = rx.exactMatch(str);

    return ok;
}
void SignupDialog::on_pushButton_confirm_clicked()
{
    QString QSlineEdit_pw = ui->lineEdit_pw->text();
    QString QSlineEdit_cpw = ui->lineEdit_cpw->text();
    QString QSlineEdit_usr = ui->lineEdit_usr->text();
    QString QSlineEdit_nn = ui->lineEdit_nn->text();


    infomation = new Infomation();
    if (QSlineEdit_usr == NULL || QSlineEdit_nn == NULL)
    {
        QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::warning(NULL, "错误", "用户名和昵称不能为空!!",
            QMessageBox::Yes | QMessageBox::Yes); return;
    }
    else
        if (!(IsMatchRegX(QSlineEdit_pw)))//限定长度和至少两种的函数
        {
            QApplication::setQuitOnLastWindowClosed(false);
            QMessageBox::warning(NULL, "错误", "密码长度为8-16位，且必须为数字、大小写字母或下划线中至少2种!!",
                QMessageBox::Yes | QMessageBox::Yes); return;
        }

        else if (!(QSlineEdit_pw == QSlineEdit_cpw))
        {
            QApplication::setQuitOnLastWindowClosed(false);
            QMessageBox::warning(NULL, "错误", "两次错误不一致!!",
                QMessageBox::Yes | QMessageBox::Yes); return;


        }
        else {
            /*todo:密码是否至少包含了大小写字母和数字还有下划线的其中两个*/
            Identify signup_i_t;
            /*******connecting**********/
            sockaddr_in addr;//1
            qDebug() << "conneting" << endl;
            int sock = client_base::run_sock(IP_addr, 5000, addr, AF_INET);//2
            if (sock == -1) { qDebug() << "connect error" << endl;/* QMessageBox::information(NULL,"ERROR", "connect failed!",QMessageBox::Yes);*/return; }
            else
                if (sock == 0) { qDebug() << "socket error" << endl; return; }
            qDebug() << "connet success" << endl;
            /***********connecting************/
            QString pair = QSlineEdit_usr + "/" + QSlineEdit_nn + "/" + QSlineEdit_pw;
            QByteArray PAIR;
            PAIR.append(pair);
            char* msg = PAIR.data();
            qDebug() << msg << endl;
            int flag = signup_i_t.to_register(sock, msg);//3
            qDebug() << "flag:" << flag << "msg has been sent" << endl;
            if (flag == 0) {
                connect(this, SIGNAL(sendData(QString)), parent(), SLOT(receiveData(QString)), Qt::UniqueConnection);
                emit sendData(QSlineEdit_usr); qDebug() << "register success!" << endl;/* QMessageBox::information(NULL,"Congratuation!", "Register Success",QMessageBox::Yes);*/this->close();
            }
            if (flag == 1) { qDebug() << "user existed!" << endl;/*QMessageBox::information(NULL,"ERROR", "user existed!",QMessageBox::Yes);this->close();*/ }
            if (flag == 2) { qDebug() << "register failed!" << endl;/*QMessageBox::information(NULL,"ERROR", "register failed!",QMessageBox::Yes);this->close();*/ }

            /***********disconnecting***********/
            client_base::close_sock(sock);
        }
}

void SignupDialog::receiveData(QString data) { ; }

void SignupDialog::on_pushButton_cancel_clicked()
{
    this->close();
    delete ui;
}


void SignupDialog::on_pushButton_pressed()
{
    ui->lineEdit_pw->setEchoMode(QLineEdit::Normal);
}

void SignupDialog::on_pushButton_released()
{
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);
}

void SignupDialog::on_pushButton_2_pressed()
{
    ui->lineEdit_cpw->setEchoMode(QLineEdit::Normal);
}

void SignupDialog::on_pushButton_2_released()
{
    ui->lineEdit_cpw->setEchoMode(QLineEdit::Password);
}
