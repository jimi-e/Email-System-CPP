#include <QtWidgets>
#include "coloritem.h"

ColorItem::ColorItem(const QColor &color, QWidget *parent) :
    QLabel(parent)
{
    this->setFixedSize(ITEM_SIZE, ITEM_SIZE);

    m_isEnter = false;
    m_color = color;
}

ColorItem::~ColorItem()
{

}

void ColorItem::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRect(0, 0, this->width(), this->height());

    if (m_isEnter) {
        painter.setPen(QPen(Qt::white, 3));
        painter.drawRect(1, 1, ITEM_SIZE - 2, ITEM_SIZE - 2);
    }
}

void ColorItem::mousePressEvent(QMouseEvent */*event*/)
{
    emit itemClicked(m_color);
}

void ColorItem::enterEvent(QEvent */*event*/)
{
    m_isEnter = true;
    update();
}

void ColorItem::leaveEvent(QEvent */*event*/)
{
    m_isEnter = false;
    update();
}
