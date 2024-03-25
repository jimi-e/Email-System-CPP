#ifndef RECVWIDGET_H
#define RECVWIDGET_H
#include <QWidget>
#include <vector>
#include <QListWidgetItem>
namespace Ui {
    class RecvWidget;
}

class RecvWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecvWidget(QWidget* parent = nullptr);
    ~RecvWidget();

signals:
    void sendData(QString data);
    void sendData(QString, int);
private slots:
    void receiveData(QString data);

    void receiveData(int flag_type);
    void i1Clicked(QListWidgetItem* item);

private:
    Ui::RecvWidget* ui;
    void reload();
    //    Infomation *INFOmation;
};

#endif // RECVWIDGET_H
