#include <QtWidgets>
#include "colorwidget.h"
#include "coloritem.h"

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent)
{
    initColorWidget();
}

ColorWidget::~ColorWidget()
{

}

void ColorWidget::initColorWidget()
{
    ColorItem *pItem11 = new ColorItem(QColor(0, 0, 0));
    ColorItem *pItem12 = new ColorItem(QColor(170, 0, 0));
    ColorItem *pItem13 = new ColorItem(QColor(0, 85, 0));
    ColorItem *pItem14 = new ColorItem(QColor(170, 85, 0));
    ColorItem *pItem15 = new ColorItem(QColor(0, 170, 0));
    ColorItem *pItem16 = new ColorItem(QColor(170, 170, 0));
    ColorItem *pItem17 = new ColorItem(QColor(0, 255, 0));
    ColorItem *pItem18 = new ColorItem(QColor(170, 255, 0));
    QHBoxLayout *pLayout1 = new QHBoxLayout;
    pLayout1->setMargin(0);
    pLayout1->setSpacing(4);
    pLayout1->addWidget(pItem11);
    pLayout1->addWidget(pItem12);
    pLayout1->addWidget(pItem13);
    pLayout1->addWidget(pItem14);
    pLayout1->addWidget(pItem15);
    pLayout1->addWidget(pItem16);
    pLayout1->addWidget(pItem17);
    pLayout1->addWidget(pItem18);

    ColorItem *pItem21 = new ColorItem(QColor(0, 0, 127));
    ColorItem *pItem22 = new ColorItem(QColor(170, 0, 127));
    ColorItem *pItem23 = new ColorItem(QColor(0, 85, 127));
    ColorItem *pItem24 = new ColorItem(QColor(170, 85, 127));
    ColorItem *pItem25 = new ColorItem(QColor(0, 170, 127));
    ColorItem *pItem26 = new ColorItem(QColor(170, 170, 127));
    ColorItem *pItem27 = new ColorItem(QColor(0, 255, 127));
    ColorItem *pItem28 = new ColorItem(QColor(170, 255, 127));
    QHBoxLayout *pLayout2 = new QHBoxLayout;
    pLayout2->setMargin(0);
    pLayout2->setSpacing(4);
    pLayout2->addWidget(pItem21);
    pLayout2->addWidget(pItem22);
    pLayout2->addWidget(pItem23);
    pLayout2->addWidget(pItem24);
    pLayout2->addWidget(pItem25);
    pLayout2->addWidget(pItem26);
    pLayout2->addWidget(pItem27);
    pLayout2->addWidget(pItem28);

    ColorItem *pItem31 = new ColorItem(QColor(0, 0, 255));
    ColorItem *pItem32 = new ColorItem(QColor(170, 0, 255));
    ColorItem *pItem33 = new ColorItem(QColor(0, 85, 255));
    ColorItem *pItem34 = new ColorItem(QColor(170, 85, 255));
    ColorItem *pItem35 = new ColorItem(QColor(0, 170, 255));
    ColorItem *pItem36 = new ColorItem(QColor(170, 170, 255));
    ColorItem *pItem37 = new ColorItem(QColor(0, 255, 255));
    ColorItem *pItem38 = new ColorItem(QColor(170, 255, 255));
    QHBoxLayout *pLayout3 = new QHBoxLayout;
    pLayout3->setMargin(0);
    pLayout3->setSpacing(4);
    pLayout3->addWidget(pItem31);
    pLayout3->addWidget(pItem32);
    pLayout3->addWidget(pItem33);
    pLayout3->addWidget(pItem34);
    pLayout3->addWidget(pItem35);
    pLayout3->addWidget(pItem36);
    pLayout3->addWidget(pItem37);
    pLayout3->addWidget(pItem38);

    ColorItem *pItem41 = new ColorItem(QColor(85, 0, 0));
    ColorItem *pItem42 = new ColorItem(QColor(255, 0, 0));
    ColorItem *pItem43 = new ColorItem(QColor(85, 85, 0));
    ColorItem *pItem44 = new ColorItem(QColor(255, 85, 0));
    ColorItem *pItem45 = new ColorItem(QColor(85, 170, 0));
    ColorItem *pItem46 = new ColorItem(QColor(255, 170, 0));
    ColorItem *pItem47 = new ColorItem(QColor(85, 255, 0));
    ColorItem *pItem48 = new ColorItem(QColor(255, 255, 0));
    QHBoxLayout *pLayout4 = new QHBoxLayout;
    pLayout4->setMargin(0);
    pLayout4->setSpacing(4);
    pLayout4->addWidget(pItem41);
    pLayout4->addWidget(pItem42);
    pLayout4->addWidget(pItem43);
    pLayout4->addWidget(pItem44);
    pLayout4->addWidget(pItem45);
    pLayout4->addWidget(pItem46);
    pLayout4->addWidget(pItem47);
    pLayout4->addWidget(pItem48);

    ColorItem *pItem51 = new ColorItem(QColor(85, 0, 127));
    ColorItem *pItem52 = new ColorItem(QColor(255, 0, 127));
    ColorItem *pItem53 = new ColorItem(QColor(85, 85, 127));
    ColorItem *pItem54 = new ColorItem(QColor(255, 85, 127));
    ColorItem *pItem55 = new ColorItem(QColor(85, 170, 127));
    ColorItem *pItem56 = new ColorItem(QColor(255, 170, 127));
    ColorItem *pItem57 = new ColorItem(QColor(85, 255, 127));
    ColorItem *pItem58 = new ColorItem(QColor(255, 255, 127));
    QHBoxLayout *pLayout5 = new QHBoxLayout;
    pLayout5->setMargin(0);
    pLayout5->setSpacing(4);
    pLayout5->addWidget(pItem51);
    pLayout5->addWidget(pItem52);
    pLayout5->addWidget(pItem53);
    pLayout5->addWidget(pItem54);
    pLayout5->addWidget(pItem55);
    pLayout5->addWidget(pItem56);
    pLayout5->addWidget(pItem57);
    pLayout5->addWidget(pItem58);

    ColorItem *pItem61 = new ColorItem(QColor(85, 0, 255));
    ColorItem *pItem62 = new ColorItem(QColor(255, 0, 255));
    ColorItem *pItem63 = new ColorItem(QColor(85, 85, 255));
    ColorItem *pItem64 = new ColorItem(QColor(255, 85, 255));
    ColorItem *pItem65 = new ColorItem(QColor(85, 170, 255));
    ColorItem *pItem66 = new ColorItem(QColor(255, 170, 255));
    ColorItem *pItem67 = new ColorItem(QColor(85, 255, 255));
    ColorItem *pItem68 = new ColorItem(QColor(255, 255, 255));
    QHBoxLayout *pLayout6 = new QHBoxLayout;
    pLayout6->setMargin(0);
    pLayout6->setSpacing(4);
    pLayout6->addWidget(pItem61);
    pLayout6->addWidget(pItem62);
    pLayout6->addWidget(pItem63);
    pLayout6->addWidget(pItem64);
    pLayout6->addWidget(pItem65);
    pLayout6->addWidget(pItem66);
    pLayout6->addWidget(pItem67);
    pLayout6->addWidget(pItem68);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->setMargin(0);
    pVLayout->setSpacing(4);
    pVLayout->addLayout(pLayout1);
    pVLayout->addLayout(pLayout2);
    pVLayout->addLayout(pLayout3);
    pVLayout->addLayout(pLayout4);
    pVLayout->addLayout(pLayout5);
    pVLayout->addLayout(pLayout6);

    connect(pItem11, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem12, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem13, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem14, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem15, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem16, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem17, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem18, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));

    connect(pItem21, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem22, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem23, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem24, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem25, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem26, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem27, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem28, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));

    connect(pItem31, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem32, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem33, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem34, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem35, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem36, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem37, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem38, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));

    connect(pItem41, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem42, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem43, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem44, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem45, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem46, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem47, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem48, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));

    connect(pItem51, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem52, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem53, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem54, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem55, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem56, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem57, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem58, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));

    connect(pItem61, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem62, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem63, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem64, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem65, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem66, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem67, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
    connect(pItem68, SIGNAL(itemClicked(QColor)), this, SIGNAL(colorItemSelected(QColor)));
}
