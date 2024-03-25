#ifndef COLORITEM_H
#define COLORITEM_H

#include <QLabel>

#define ITEM_SIZE 20

class ColorItem : public QLabel
{
    Q_OBJECT

public:
    explicit ColorItem(const QColor &color, QWidget *parent = Q_NULLPTR);
    virtual ~ColorItem();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void itemClicked(const QColor &color);

private:
    bool    m_isEnter;
    QColor  m_color;

};

#endif // COLORITEM_H
