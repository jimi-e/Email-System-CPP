#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = Q_NULLPTR);
    virtual ~ColorWidget();

    void initColorWidget();

signals:
    void colorItemSelected(const QColor &color);

};

#endif // COLORWIDGET_H
