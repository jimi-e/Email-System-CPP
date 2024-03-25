#include "form.h"
#include "ui_form.h"
#include "emaildialog.h"
#include <infomation.h>
#include <recvwidget.h>
#include <contentwidget.h>
#include "loginwidget.h"

#include <client_net.h>
#include <identify.h>
#include <mail.h>

#include <QPixmap>

#include <QDesktopWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QAction>
#include <string>
#include <QDebug>

#define IP_addr "192.168.43.38"

std::vector<std::string> NameList;
Form::Form(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);//center


    qDebug() << "form is open" << endl;

    ui->stackedWidget->setCurrentIndex(1);
    emaildialog = new EmailDialog();
    ui->verticalLayout->addWidget(emaildialog);
    qDebug() << "NewMail is open" << endl;
}


Form::~Form()
{
    this->disconnect();
    delete ui;
}

/************drag**************/
void Form::mousePressEvent(QMouseEvent* event)
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
void Form::mouseMoveEvent(QMouseEvent* event)
{


    //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
    if (mouse_press) {
        move(event->globalPos() - mousePoint);
        //        event->accept();
    }
}
void Form::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_press = false;
}

/************drag**************/



void Form::on_b_recv_clicked()
{
    int i = 2;
    while (i--) {
        qDebug() << "receving 0" << endl;
        ui->verticalLayout_6->removeWidget(recvwidget);
        recvwidget = new RecvWidget();
        connect(this, SIGNAL(sendData(int)), recvwidget, SLOT(receiveData(int)), Qt::UniqueConnection);
        emit  sendData(0);

        qDebug() << "receved 0" << endl;
    }     ui->stackedWidget->setCurrentIndex(1); ui->verticalLayout_6->addWidget(recvwidget);




}



void Form::on_b_write_clicked()
{
    connect(this, SIGNAL(sendData(QString)), emaildialog, SLOT(receiveData(QString)), Qt::UniqueConnection);
    emit  sendData(ui->label_usr->text());
    ui->stackedWidget->setCurrentIndex(0);

}

void Form::on_commandLinkButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}

void Form::receiveData(QString data)
{
    int i = 2;
    while (i--) {
        ui->verticalLayout_6->removeWidget(recvwidget);
        qDebug() << "recvdata_2" << endl;
        this->ui->label_usr->setText(data);

        recvwidget = new RecvWidget();
        connect(this, SIGNAL(sendData(int)), recvwidget, SLOT(receiveData(int)));
        emit  sendData(0);

        connect(this, SIGNAL(sendData(QString)), recvwidget, SLOT(receiveData(QString)));
        emit  sendData(ui->label_usr->text());
        qDebug() << "mailbox has been loaded" << endl;
    } ui->verticalLayout_6->addWidget(recvwidget);
};
void Form::receiveData(QString data, int tmp)
{
    qDebug() << "recvdata_1" << endl;
    qDebug() << data << endl;
    qDebug() << tmp << endl;
    ui->mailcontent->clear();
    ui->label_time->clear();
    ui->label_title->clear();
    ui->label_sender->clear();
    ui->stackedWidget->setCurrentIndex(2);
    Mail_recv send_t;
    std::vector<Mail_head> mail_content;
    std::string str1 = data.toStdString();
    std::string id;
    std::string sender;
    std::string reciver;
    std::string time;
    std::string title;
    int i = 0;
    for (i = 0; str1[i] != '/'; i++)
    {
        id += str1[i];
    }
    for (i = i + 1; str1[i] != '/'; i++)
    {
        sender += str1[i];
    }
    for (i = i + 1; str1[i] != '/'; i++)
    {
        reciver += str1[i];
    }
    for (i = i + 1; str1[i] != '/'; i++)
    {
        time += str1[i];
    }
    for (i = i + 1; i < str1.length(); i++)
    {
        title += str1[i];
    }

    Form::sender_ = QString::fromStdString(sender);
    Form::title = QString::fromStdString(title);
    Form::File_ID = QString::fromStdString(id);


    send_t.set_ID(id);
    qDebug() << "id:" << QString::fromStdString(id) << endl;

    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock(IP_addr, 5000, addr, AF_INET);//2
    qDebug() << "connet success" << endl;
    /*********connecting*********/



    send_t.set_sock(sock);
    send_t.get_text();
    QString str = send_t.text().c_str();
    ui->label_time->setText("Time: " + QString::fromStdString(time));
    //qDebug()<<time<<endl;
    ui->label_title->setText("Title: " + QString::fromStdString(title));
    ui->label_sender->setText("From: " + QString::fromStdString(sender));
    ui->mailcontent->setHtml(QString::fromStdString(send_t.text()));
    /*********disconnecting*********/
    client_base::close_sock(sock);
};


void Form::on_b_reply_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Form::on_b_draft_clicked()
{
    int i = 2;
    while (i--)
    {
        qDebug() << "receving 2" << endl;
        ui->verticalLayout_6->removeWidget(recvwidget);
        recvwidget = new RecvWidget();
        connect(this, SIGNAL(sendData(int)), recvwidget, SLOT(receiveData(int)));
        emit  sendData(2);

        qDebug() << "receved 2" << endl;
    }     ui->stackedWidget->setCurrentIndex(1); ui->verticalLayout_6->addWidget(recvwidget);
}

void Form::on_b_send_clicked()
{
    int i = 2;
    while (i--)
    {
        qDebug() << "receving 1" << endl;
        ui->verticalLayout_6->removeWidget(recvwidget);
        recvwidget = new RecvWidget();
        connect(this, SIGNAL(sendData(int)), recvwidget, SLOT(receiveData(int)));
        emit  sendData(1);
        qDebug() << "receved 1" << endl;
    }   ui->stackedWidget->setCurrentIndex(1); ui->verticalLayout_6->addWidget(recvwidget);
}

void Form::on_b_bin_clicked()
{
    int i = 2;
    while (i--)
    {
        qDebug() << "receving 3" << endl;
        ui->verticalLayout_6->removeWidget(recvwidget);
        recvwidget = new RecvWidget();
        connect(this, SIGNAL(sendData(int)), recvwidget, SLOT(receiveData(int)));
        emit  sendData(3);
        qDebug() << "receved 3" << endl;
    }  ui->stackedWidget->setCurrentIndex(1); ui->verticalLayout_6->addWidget(recvwidget);
}

void Form::on_b_exit_clicked()
{
    this->close();
    delete this;
}

void Form::on_pushButton_clicked()
{

    Infomation* infoMation = new Infomation();
    infoMation->show();
}

void Form::on_b_downloadfile_clicked()
{
    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock(IP_addr, 5000, addr, AF_INET);//2
    qDebug() << "connet success" << endl;
    /*********connecting*********/

    Mail_recv tmp;
    tmp.set_ID(Form::File_ID.toStdString());
    tmp.set_sock(sock);
    std::string PaTh = "/home/natsu/Desktop/Email/download/" + ui->label_usr->text().toStdString();
    NameList.clear();
    tmp.get_attach(PaTh.c_str(), NameList);
    if (NameList.empty())
    {
        QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::warning(NULL, "错误", "无附件可下载!",
            QMessageBox::Yes | QMessageBox::Yes);
    }

    else
    {
        QApplication::setQuitOnLastWindowClosed(false);
        QMessageBox::warning(NULL, "", "文件下载成功!",
            QMessageBox::Yes | QMessageBox::Yes);
    }
    /*********disconnecting*********/
    client_base::close_sock(sock);
}

void Form::on_b_close_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Form::on_b_delete_clicked()
{
    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock(IP_addr, 5000, addr, AF_INET);//2
    qDebug() << "connet success" << endl;
    /*********connecting*********/

    QApplication::setQuitOnLastWindowClosed(false);
    if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("Warning!"), QStringLiteral("确认删除邮件吗?"), QMessageBox::Yes | QMessageBox::No))
    {
        qDebug() << "yes";
    }
    else
    {
        qDebug() << "no"; return;
    }
    Form::on_b_close_clicked();
    Mail_send tmp;
    tmp.delete_mail(sock, Form::File_ID.toStdString());
    /*********disconnecting*********/
    client_base::close_sock(sock);
}

void Form::on_b_forward_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Form::on_change_icon_clicked()
{
    f_actInsertImage = new QAction(QIcon(tr(":/image/image.png")), tr(""), this);
    f_actInsertImage->setToolTip(tr("插入图片"));
    connect(f_actInsertImage, SIGNAL(triggered()), this, SLOT(onInsertImage()));
}
/******没用的代码******/
void Form::onInsertImage()
{

}
/******没用的代码******/
void Form::on_b_favor_clicked()
{
    LoginWidget* LoGiN = new LoginWidget();
    LoGiN->show();
    this->hide();
}
