#include "recvwidget.h"
#include "ui_recvwidget.h"
#include <QListWidgetItem>

#include <QDebug>
#include <vector>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QFrame>

#include <client_net.h>
#include <mail.h>
#include <identify.h>

#define IP_sever "192.168.43.38"

int FLAGTYPE = -1;
QString name_tmp;
RecvWidget::RecvWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::RecvWidget)
{
    ui->setupUi(this); ;
    ui->usrname->setText(name_tmp);
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(i1Clicked(QListWidgetItem*)), Qt::UniqueConnection);
    qDebug() << "loding flag=" << FLAGTYPE << endl;
    reload();
    ui->usrname->setVisible(false);


}

RecvWidget::~RecvWidget()
{

    delete ui;
}



void RecvWidget::reload()
{
    qDebug() << "FlagType:" << FLAGTYPE << " mailbox is updating" << endl;
    std::vector<Mail_head> usrinfo;
    Mail_recv_list t_mail_recv;
    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "mail box conneting" << endl;
    int sock = client_base::run_sock(IP_sever, 5000, addr, AF_INET);//2
    qDebug() << "mail box connet success" << endl;
    /*********connecting*********/
    QString str_tmp = ui->usrname->text();
    QByteArray USR;
    USR.append(str_tmp);
    char* usr = USR.data();
    t_mail_recv.set_name(usr);
    t_mail_recv.set_sock(sock);
    t_mail_recv.set_is_sender(false);
    t_mail_recv.get_mail(usrinfo, FLAGTYPE);
    qDebug() << str_tmp << endl;
    Mail_recv tmp;

    ui->listWidget->clear();
    QListWidgetItem* i1_t = new QListWidgetItem();
    QLabel* l_type = new QLabel;
    l_type->setEnabled(false);
    i1_t->setSizeHint(QSize(5, 100));
    switch (FLAGTYPE)
    {
    case 0: {l_type->setText("收件箱"); break; }
    case 1: {l_type->setText("发件箱"); break; }
    case 2: {l_type->setText("草稿箱"); break; }
    case 3: {l_type->setText("回收站"); break; }
    }
    i1_t->setData(Qt::UserRole, "-1");
    ui->listWidget->addItem(i1_t);
    ui->listWidget->setItemWidget(i1_t, l_type);

    for (int i = usrinfo.size() - 1; i >= 0; i--)
    {
        qDebug() << "i:" << endl;
        QString data = usrinfo[i].ID;
        data += "/"; data += usrinfo[i].sender; data += "/"; data += usrinfo[i].recver; data += "/"; data += usrinfo[i].time; data += "/"; data += usrinfo[i].title;
        QListWidgetItem* i1 = new QListWidgetItem();
        i1->setSizeHint(QSize(10, 100));
        i1->setData(Qt::UserRole, data);
        ui->listWidget->addItem(i1);
        QWidget* w = new QWidget;
        QLabel* l_title = new QLabel;
        QLabel* l_sender = new QLabel;
        QLabel* l_time = new QLabel;
        QLabel* l_check = new QLabel;

        QVBoxLayout* LayOut = new QVBoxLayout;
        QVBoxLayout* vlayout = new QVBoxLayout;
        QHBoxLayout* hlayout = new QHBoxLayout;

        l_title->setText("主题: " + QString::fromUtf8(usrinfo[i].title));
        l_title->setStyleSheet("font:25px;");
        l_sender->setText("FROM: " + QString::fromUtf8(usrinfo[i].sender));
        l_time->setText(usrinfo[i].time);
        l_time->setStyleSheet("color:grey;");
        if (usrinfo[i].state[0] == '0') { l_check->setText("未读"); }
        else { l_check->setText("已读"); }
        QFrame* m_line = new QFrame();
        m_line->setFrameShape(QFrame::HLine);
        m_line->setFrameShadow(QFrame::Raised);
        vlayout->addWidget(l_title);
        vlayout->addWidget(l_sender);
        vlayout->addWidget(l_time);
        hlayout->addLayout(vlayout);
        if (FLAGTYPE == 0)
        {
            hlayout->addWidget(l_check);
        }
        LayOut->addLayout(hlayout);
        LayOut->addWidget(m_line);
        w->setLayout(LayOut);
        ui->listWidget->setItemWidget(i1, w);


        qDebug() << "title:  " << usrinfo[i].title << "usrid:  " << usrinfo[i].ID << "time: " << usrinfo[i].time << "sender:" << usrinfo[i].sender << " checked:  " << usrinfo[i].state << endl;
    }

    client_base::close_sock(sock);
}

void RecvWidget::i1Clicked(QListWidgetItem* item)
{
    QString strName = item->data(Qt::UserRole).toString();
    if (strName != "-1") {
        connect(this, SIGNAL(sendData(QString, int)), parent()->parent()->parent(), SLOT(receiveData(QString, int)), Qt::UniqueConnection);
        emit  sendData(strName, 0);
    }
    qDebug() << strName << endl;

}

void RecvWidget::receiveData(QString data) { qDebug() << "Data_sent:" << data << endl; name_tmp = data; }

void RecvWidget::receiveData(int flag_type) { FLAGTYPE = flag_type; }
