#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <emaildialog.h>
#include <infomation.h>
#include <recvwidget.h>
#include <contentwidget.h>
namespace Ui {
    class Form;
}

class Form : public QWidget
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
    explicit Form(QWidget* parent = nullptr);
    ~Form();
    void deleteAllitemsOfLayout(QLayout* layout);
signals:
    void sendData(QString data);
    void sendData(int flag_type);

private slots:
    void  onInsertImage();
    void receiveData(QString data);
    void receiveData(QString data, int tmp);

    void on_b_recv_clicked();

    void on_b_write_clicked();

    void on_commandLinkButton_3_clicked();

    void on_b_reply_clicked();

    void on_b_draft_clicked();

    void on_b_send_clicked();

    void on_b_bin_clicked();

    void on_b_exit_clicked();

    void on_pushButton_clicked();
    void on_b_downloadfile_clicked();

    void on_b_close_clicked();

    void on_b_delete_clicked();

    void on_b_forward_clicked();

    void on_change_icon_clicked();

    void on_b_favor_clicked();

private:
    QString File_ID;
    QString sender_;
    QString title;
    QString text;
    QString file_path;
    QString file_name;
    QString downloadpath;
    QString choosepath;
    Ui::Form* ui;
    EmailDialog* emaildialog;
    RecvWidget* recvwidget;
    ContentWidget* contentwidget;

    QAction* f_actInsertImage;
};

#endif // FORM_H
