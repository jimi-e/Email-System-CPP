#ifndef INFOMATION_H
#define INFOMATION_H

#include <QWidget>

namespace Ui {
class Infomation;
}

class Infomation : public QWidget
{
    Q_OBJECT

public:
    explicit Infomation(QWidget *parent = nullptr);
    ~Infomation();


private slots:
    void on_confirm_clicked();

private:
    Ui::Infomation *ui;
};

#endif // INFOMATION_H
