#include <QtWidgets>
#include "ui_emaildialog.h"
#include "emaildialog.h"
#include "colorwidget.h"
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QTextEdit>
#include <QFile>


#include <client_net.h>
#include <identify.h>
#include <mail.h>


#include <vector>
EmailDialog::EmailDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::emaildialog)
{

    //   ui->setupUi(this);

    m_initIndent = 20.0;
    m_addIndent = 30.0;

    this->setWindowIcon(QIcon(tr(":/image/email.png")));
    this->setWindowFlags(Qt::Window);
    //  this->setGeometry(0,0,1000,500);
    this->setMinimumSize(300, 300);

    initToolBar();

}

EmailDialog::~EmailDialog()
{
    delete ui;
}

void EmailDialog::initToolBar()
{
    m_layout = new QVBoxLayout(this);



    //收件人
    m_recv = new QLabel();
    m_recv->setText("收件人");
    m_recv->setFixedSize(QSize(50, 23));
    m_addr = new QLineEdit();
    m_addr->setFixedHeight(23);
    //  m_addr->setMinimumWidth(300);
    m_hlayout = new QHBoxLayout();
    m_hlayout->addWidget(m_recv);
    m_hlayout->addWidget(m_addr);


    //主题
    m_topic = new QLabel();
    m_topic->setText("主题");
    m_topic->setFixedSize(QSize(50, 23));
    m_topictext = new QLineEdit();
    m_topictext->setFixedHeight(23);
    m_topiclayout = new QHBoxLayout();
    m_topiclayout->addWidget(m_topic);
    m_topiclayout->addWidget(m_topictext);


    //CC
    m_CC = new QLabel();
    m_CC->setText("抄送");
    m_CC->setFixedSize(QSize(50, 23));
    m_CCtext = new QLineEdit();
    m_CCtext->setFixedHeight(23);
    m_CClayout = new QHBoxLayout();
    m_CClayout->addWidget(m_CC);
    m_CClayout->addWidget(m_CCtext);
    m_CC->setVisible(false);
    m_CCtext->setVisible(false);


    //BCC
    m_BCC = new QLabel();
    m_BCC->setText("密送");
    m_BCC->setFixedSize(QSize(50, 23));
    m_BCCtext = new QLineEdit();
    m_BCCtext->setFixedHeight(23);
    m_BCClayout = new QHBoxLayout();
    m_BCClayout->addWidget(m_BCC);
    m_BCClayout->addWidget(m_BCCtext);
    m_BCC->setVisible(false);
    m_BCCtext->setVisible(false);





    m_toolBar = new QToolBar();
    m_toolBar->setFixedHeight(30);
    //m_textEdit=ui->ui_textEdit;
    m_textEdit = new QTextEdit();
    //字体、字号
    m_fontFamilyBox = new QFontComboBox(this);
    m_fontFamilyBox->setFontFilters(QFontComboBox::ScalableFonts);
    m_fontFamilyBox->setFixedWidth(80);
    m_fontFamilyBox->setFixedHeight(23);
    m_fontFamilyBox->setEditable(false);
    connect(m_fontFamilyBox, SIGNAL(activated(QString)), this, SLOT(onFontFamily(QString)), Qt::UniqueConnection);
    m_toolBar->addWidget(m_fontFamilyBox);

    m_fontSizeBox = new QComboBox(this);
    m_fontSizeBox->setMinimumWidth(50);
    m_fontSizeBox->setFixedHeight(23);
    m_fontSizeBox->setEditable(false);
    m_fontSizeBox->setStyleSheet("QComboBox QAbstractItemView{outline:0px;}");
    QFontDatabase fontDb;
    foreach(int size, fontDb.standardSizes()) {
        m_fontSizeBox->addItem(QString::number(size));
    }
    m_fontSizeBox->setCurrentText(tr("9"));
    connect(m_fontSizeBox, SIGNAL(activated(QString)), this, SLOT(onFontSize(QString)), Qt::UniqueConnection);
    m_toolBar->addWidget(m_fontSizeBox);

    //加粗、斜体、下划线、删除线、字体颜色
    m_boldBtn = new QToolButton(this);
    m_boldBtn->setIcon(QIcon(tr(":/image/bold.png")));
    connect(m_boldBtn, SIGNAL(clicked()), this, SLOT(onBold()), Qt::UniqueConnection);
    m_toolBar->addWidget(m_boldBtn);

    m_italicBtn = new QToolButton(this);
    m_italicBtn->setIcon(QIcon(tr(":/image/italic.png")));
    connect(m_italicBtn, SIGNAL(clicked()), this, SLOT(onItalic()), Qt::UniqueConnection);
    m_toolBar->addWidget(m_italicBtn);

    m_underLineBtn = new QToolButton(this);
    m_underLineBtn->setIcon(QIcon(tr(":/image/under.png")));
    connect(m_underLineBtn, SIGNAL(clicked()), this, SLOT(onUnderLine()), Qt::UniqueConnection);
    m_toolBar->addWidget(m_underLineBtn);

    m_strikeoutBtn = new QToolButton(this);
    m_strikeoutBtn->setIcon(QIcon(tr(":/image/strikeout.png")));
    connect(m_strikeoutBtn, SIGNAL(clicked()), this, SLOT(onStrikeout()), Qt::UniqueConnection);
    m_toolBar->addWidget(m_strikeoutBtn);

    QMenu* fontColorMenu = new QMenu(this);
    m_fontColorBtn = new QToolButton(this);
    m_fontColorBtn->setIcon(QIcon(tr(":/image/fontcolor.png")));
    m_fontColorBtn->setPopupMode(QToolButton::MenuButtonPopup);
    m_fontColorBtn->setMenu(fontColorMenu);
    m_toolBar->addWidget(m_fontColorBtn);

    ColorWidget* colorWidget = new ColorWidget();
    QWidgetAction* actFontColor = new QWidgetAction(fontColorMenu);
    actFontColor->setDefaultWidget(colorWidget);
    fontColorMenu->addAction(actFontColor);
    connect(colorWidget, SIGNAL(colorItemSelected(QColor)), this, SLOT(onFontColor(QColor)), Qt::UniqueConnection);

    //文本对齐方式<左对齐、居中对齐、右对齐、两端对齐>
    m_actLeftAlign = new QAction(QIcon(tr(":/image/left.png")), tr(""), this);
    m_actLeftAlign->setToolTip(tr("左对齐"));
    connect(m_actLeftAlign, SIGNAL(triggered()), this, SLOT(onLeftAlign()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actLeftAlign);

    m_actCenterAlign = new QAction(QIcon(tr(":/image/center.png")), tr(""), this);
    m_actCenterAlign->setToolTip(tr("居中对齐"));
    connect(m_actCenterAlign, SIGNAL(triggered()), this, SLOT(onCenterAlign()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actCenterAlign);

    m_actRightAlign = new QAction(QIcon(tr(":/image/right.png")), tr(""), this);
    m_actRightAlign->setToolTip(tr("右对齐"));
    connect(m_actRightAlign, SIGNAL(triggered()), this, SLOT(onRightAlign()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actRightAlign);

    m_actJustifyAlign = new QAction(QIcon(":/image/justify.png"), tr(""), this);
    m_actJustifyAlign->setToolTip(tr("两端对齐"));
    connect(m_actJustifyAlign, SIGNAL(triggered()), this, SLOT(onJustifyAlign()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actJustifyAlign);

    m_toolBar->addSeparator();

    //行间距
    QMenu* lineSpaceMenu = new QMenu(this);
    connect(lineSpaceMenu, SIGNAL(aboutToShow()), this, SLOT(onLineSpaceAboutToShow()), Qt::UniqueConnection);
    connect(lineSpaceMenu, SIGNAL(triggered(QAction*)), this, SLOT(onLineSpaceTriggered(QAction*)), Qt::UniqueConnection);
    m_actLineSpace = new QAction(QIcon(tr(":/image/linespace.png")), tr(""), this);
    m_actLineSpace->setToolTip(tr("行间距"));
    m_actLineSpace->setMenu(lineSpaceMenu);
    connect(m_actLineSpace, SIGNAL(triggered()), this, SLOT(onLineSpace()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actLineSpace);

    QStringList lineSpaceItems;
    lineSpaceItems << "0.0" << "1.0" << "1.5" << "2.0" << "2.5" << "3.0" << "3.5";
    for (int i = 0; i < lineSpaceItems.size(); i++) {
        QAction* actLineSpace = new QAction(lineSpaceItems.at(i), lineSpaceMenu);
        actLineSpace->setData(QVariant::fromValue(i));  //设置action在menu中的索引
        lineSpaceMenu->addAction(actLineSpace);
    }

    //项目符号
    QMenu* bulletsMenu = new QMenu(this);
    connect(bulletsMenu, SIGNAL(aboutToShow()), this, SLOT(onBulletsAboutToShow()), Qt::UniqueConnection);
    connect(bulletsMenu, SIGNAL(triggered(QAction*)), this, SLOT(onBulletsTriggered(QAction*)), Qt::UniqueConnection);
    m_actBullets = new QAction(QIcon(tr(":/image/bullets.png")), tr(""), this);
    m_actBullets->setToolTip(tr("项目符号"));
    m_actBullets->setMenu(bulletsMenu);
    connect(m_actBullets, SIGNAL(triggered()), this, SLOT(onBullets()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actBullets);

    QStringList bulletsItems;
    bulletsItems << "QTextListFormat::ListStyleUndefined"
        << "QTextListFormat::ListDisc" << "QTextListFormat::ListCircle" << "QTextListFormat::ListSquare" << "QTextListFormat::ListDecimal"
        << "QTextListFormat::ListLowerAlpha" << "QTextListFormat::ListUpperAlpha" << "QTextListFormat::ListLowerRoman" << "QTextListFormat::ListUpperRoman";
    for (int i = 0; i < bulletsItems.size(); i++) {
        QAction* actBullets = new QAction(bulletsItems.at(i), bulletsMenu);
        actBullets->setData(QVariant::fromValue(i));
        bulletsMenu->addAction(actBullets);
    }

    m_toolBar->addSeparator();

    //减少缩进量
    m_actDecIndent = new QAction(QIcon(tr(":/image/decindent.png")), tr(""), this);
    m_actDecIndent->setToolTip(tr("减少缩进量"));
    connect(m_actDecIndent, SIGNAL(triggered()), this, SLOT(onDecIndent()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actDecIndent);

    //增加缩进量
    m_actIncIndent = new QAction(QIcon(tr(":/image/incindent.png")), tr(""), this);
    m_actIncIndent->setToolTip(tr("增加缩进量"));
    connect(m_actIncIndent, SIGNAL(triggered()), this, SLOT(onIncIndent()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actIncIndent);

    m_toolBar->addSeparator();

    //插入日期
    QMenu* insertDateMenu = new QMenu(this);
    m_actInsertDate = new QAction(QIcon(tr(":/image/date.png")), tr(""), this);
    m_actInsertDate->setToolTip(tr("插入日期"));
    m_actInsertDate->setMenu(insertDateMenu);
    connect(m_actInsertDate, SIGNAL(triggered()), this, SLOT(onInsertDate()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actInsertDate);

    QCalendarWidget* calendarWidget = new QCalendarWidget();
    calendarWidget->setGridVisible(true);
    connect(calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(onCalendarWidget(QDate)), Qt::UniqueConnection);
    QWidgetAction* actInsertDate = new QWidgetAction(insertDateMenu);
    actInsertDate->setDefaultWidget(calendarWidget);
    insertDateMenu->addAction(actInsertDate);

    //插入图片
    m_actInsertImage = new QAction(QIcon(tr(":/image/image.png")), tr(""), this);
    m_actInsertImage->setToolTip(tr("插入图片"));
    connect(m_actInsertImage, SIGNAL(triggered()), this, SLOT(onInsertImage()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actInsertImage);

    //插入表格
    m_actInsertTable = new QAction(QIcon(tr(":/image/table.png")), tr(""), this);
    m_actInsertTable->setToolTip(tr("插入表格"));
    connect(m_actInsertTable, SIGNAL(triggered()), this, SLOT(onInsertTable()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actInsertTable);

    //***********加入附件
    m_actInsertFile = new QAction(QIcon(tr(":/res/file.png")), tr(""), this);
    m_actInsertFile->setToolTip(tr("插入附件"));
    connect(m_actInsertFile, SIGNAL(triggered()), this, SLOT(onInsertFile()), Qt::UniqueConnection);
    m_toolBar->addAction(m_actInsertFile);


    QPushButton* m_forward;
    // connect(m_send, SIGNAL(clicked(int)), this, SLOT(onForward()));
    QPushButton* m_cancel;
    //  connect(m_send, SIGNAL(clicked(int)), this, SLOT(onCancel()));

    m_send = new QPushButton();
    m_send->setText("发送");
    connect(m_send, SIGNAL(clicked()), this, SLOT(onSend()), Qt::UniqueConnection);

    m_save = new QPushButton();
    m_save->setText("保存");
    connect(m_save, SIGNAL(clicked()), this, SLOT(onSave()), Qt::UniqueConnection);

    m_addfile = new QPushButton();
    connect(m_addfile, SIGNAL(clicked()), this, SLOT(onAddFile()), Qt::UniqueConnection);

    m_modeCC = new QCheckBox();
    m_modeCC->setText("抄送");
    connect(m_modeCC, SIGNAL(stateChanged(int)), this, SLOT(onCheckCC()), Qt::UniqueConnection);

    m_modeBCC = new QCheckBox();
    m_modeBCC->setText("密送");
    connect(m_modeBCC, SIGNAL(stateChanged(int)), this, SLOT(onCheckBCC()), Qt::UniqueConnection);


    m_layout->addLayout(m_hlayout);
    m_layout->addLayout(m_topiclayout);
    m_layout->addLayout(m_CClayout);
    m_layout->addLayout(m_BCClayout);

    m_layout->addWidget(m_toolBar);
    m_layout->addWidget(m_textEdit);

    //****文件图形
    toolBar_fileup = new QToolBar();
    m_layout->addWidget(toolBar_fileup);

    m_layout->addWidget(m_modeCC);
    m_layout->addWidget(m_modeBCC);

    //*****发送
    m_layout->addWidget(m_send);
    m_layout->addWidget(m_save);




}




//*************


void EmailDialog::onFontFamily(const QString& str)
{
    QTextCharFormat charFmt;
    charFmt.setFontFamily(str);

    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onFontSize(const QString& str)
{
    QTextCharFormat charFmt;
    charFmt.setFontPointSize(str.toDouble());

    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onBold()
{
    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat charFmt = cursor.charFormat();
    if (charFmt.font().bold()) {
        charFmt.setFontWeight(QFont::Normal);
    }
    else {
        charFmt.setFontWeight(QFont::Bold);
    }

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onItalic()
{
    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat charFmt = cursor.charFormat();
    if (charFmt.font().italic()) {
        charFmt.setFontItalic(false);
    }
    else {
        charFmt.setFontItalic(true);
    }

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onUnderLine()
{
    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat charFmt = cursor.charFormat();
    if (charFmt.font().underline()) {
        charFmt.setFontUnderline(false);
    }
    else {
        charFmt.setFontUnderline(true);
    }

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onStrikeout()
{
    QTextCursor cursor = m_textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    QTextCharFormat charFmt = cursor.charFormat();
    if (charFmt.font().strikeOut()) {
        charFmt.setFontStrikeOut(false);
    }
    else {
        charFmt.setFontStrikeOut(true);
    }

    cursor.mergeCharFormat(charFmt);
    m_textEdit->mergeCurrentCharFormat(charFmt);
}

void EmailDialog::onFontColor(const QColor& color)
{
    QString str = m_textEdit->toPlainText();
    if (str.isEmpty())
        return;

    m_textEdit->setTextColor(color);
    m_fontColorBtn->menu()->close();
}

void EmailDialog::onLeftAlign()
{
    m_textEdit->setAlignment(Qt::AlignLeft);
}

void EmailDialog::onCenterAlign()
{
    m_textEdit->setAlignment(Qt::AlignCenter);
}

void EmailDialog::onRightAlign()
{
    m_textEdit->setAlignment(Qt::AlignRight);
}

void EmailDialog::onJustifyAlign()
{
    m_textEdit->setAlignment(Qt::AlignJustify);
}

void EmailDialog::onLineSpaceAboutToShow()
{
    QMenu* lineSpaceMenu = qobject_cast<QMenu*>(sender());
    QList<QAction*> actions = lineSpaceMenu->actions();

    QTextCursor cursor = m_textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();
    qreal margin = blockFmt.bottomMargin();

    QVector<qreal> lineSpaces;
    lineSpaces << 0.0 << 10.0 << 15.0 << 20.0 << 25.0 << 30.0 << 35.0;
    for (int i = 0; i < lineSpaces.size(); i++) {
        if (margin == lineSpaces.at(i)) {
            actions[i]->setChecked(true);
        }
        else {
            actions[i]->setChecked(false);
        }
    }
}

void EmailDialog::onLineSpaceTriggered(QAction* action)
{
    action->setCheckable(true);  //先设置为可选状态，再设置为已选状态

    QTextCursor cursor = m_textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    QVector<qreal> lineSpaces;
    lineSpaces << 0.0 << 10.0 << 15.0 << 20.0 << 25.0 << 30.0 << 35.0;

    blockFmt.setBottomMargin(lineSpaces.at(action->data().toInt()));
    cursor.setBlockFormat(blockFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onLineSpace()
{
    QMenu* lineSpaceMenu = m_actLineSpace->menu();
    lineSpaceMenu->exec(QCursor::pos());
}

void EmailDialog::onBulletsAboutToShow()
{
    QMenu* bulletsMenu = qobject_cast<QMenu*>(sender());
    QList<QAction*> actions = bulletsMenu->actions();

    QTextCursor cursor = m_textEdit->textCursor();
    QTextListFormat listFmt;
    if (cursor.currentList()) {
        listFmt = cursor.currentList()->format();

        QVector<QTextListFormat::Style> styles;
        styles << QTextListFormat::ListStyleUndefined
            << QTextListFormat::ListDisc << QTextListFormat::ListCircle << QTextListFormat::ListSquare << QTextListFormat::ListDecimal
            << QTextListFormat::ListLowerAlpha << QTextListFormat::ListUpperAlpha << QTextListFormat::ListLowerRoman << QTextListFormat::ListUpperRoman;
        for (int i = 0; i < styles.size(); i++) {
            if (listFmt.style() == styles.at(i)) {
                actions[i]->setChecked(true);
            }
            else {
                actions[i]->setChecked(false);
            }
        }
    }
}

void EmailDialog::onBulletsTriggered(QAction* action)
{
    action->setCheckable(true);

    QTextCursor cursor = m_textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();
    blockFmt.setIndent(0);
    blockFmt.setLeftMargin(m_initIndent);

    QVector<QTextListFormat::Style> styles;
    styles << QTextListFormat::ListStyleUndefined
        << QTextListFormat::ListDisc << QTextListFormat::ListCircle << QTextListFormat::ListSquare << QTextListFormat::ListDecimal
        << QTextListFormat::ListLowerAlpha << QTextListFormat::ListUpperAlpha << QTextListFormat::ListLowerRoman << QTextListFormat::ListUpperRoman;

    QTextListFormat listFmt;
    listFmt.setIndent(0);
    listFmt.setStyle(styles.at(action->data().toInt()));

    cursor.setBlockFormat(blockFmt);
    cursor.createList(listFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onBullets()
{
    QMenu* bulletsMenu = m_actBullets->menu();
    bulletsMenu->exec(QCursor::pos());
}

void EmailDialog::onDecIndent()
{
    //减少缩进
    QTextCursor cursor = m_textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();
    qreal margin = blockFmt.leftMargin();
    if (margin == m_initIndent)
        return;
    blockFmt.setLeftMargin(margin - m_addIndent);
    cursor.setBlockFormat(blockFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onIncIndent()
{
    //增加缩进
    QTextCursor cursor = m_textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();
    qreal margin = blockFmt.leftMargin();
    blockFmt.setLeftMargin(margin + m_addIndent);
    cursor.setBlockFormat(blockFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onInsertDate()
{
    QTextCursor cursor = m_textEdit->textCursor();
    QTextCharFormat charFmt = cursor.charFormat();
    cursor.insertText(QDate::currentDate().toString("yyyy-MM-dd"), charFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onCalendarWidget(const QDate& date)
{
    QTextCursor cursor = m_textEdit->textCursor();
    QTextCharFormat charFmt = cursor.charFormat();
    cursor.insertText(date.toString("yyyy-MM-dd"), charFmt);
    m_textEdit->setTextCursor(cursor);

    m_actInsertDate->menu()->hide();
}

void EmailDialog::onInsertImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("插入图片"), tr(""), "PNG(*.png)\n JPEG(*.jpg)\n BMP(*.bmp)");
    if (fileName.isEmpty())
        return;

    QImage image = QImageReader(fileName).read();
    QTextImageFormat imageFmt;
    imageFmt.setName(fileName);
    imageFmt.setWidth(image.width());
    imageFmt.setHeight(image.height());

    QTextCursor cursor = m_textEdit->textCursor();
    cursor.insertImage(imageFmt);  //可以直接插入图片
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onInsertTable()
{
    QTextTableFormat tableFmt;
    tableFmt.setCellPadding(10);
    tableFmt.setCellSpacing(2);

    QTextCursor cursor = m_textEdit->textCursor();
    cursor.insertTable(5, 5, tableFmt);
    m_textEdit->setTextCursor(cursor);
}

void EmailDialog::onCheckCC()
{
    if (m_modeCC->checkState() == Qt::Checked)
    {
        if (m_modeBCC->checkState() == Qt::Checked) {
            m_modeBCC->setCheckState(Qt::Unchecked);
            m_BCC->setVisible(false);
            m_BCCtext->setVisible(false);
        }
        m_CC->setVisible(true);
        m_CCtext->setVisible(true);
    }
    else if (m_modeCC->checkState() == Qt::Unchecked)
    {
        m_CC->setVisible(false);
        m_CCtext->setVisible(false);
    }
}


void EmailDialog::onCheckBCC()
{
    if (m_modeBCC->checkState() == Qt::Checked)
    {
        if (m_modeCC->checkState() == Qt::Checked)
        {
            m_modeCC->setCheckState(Qt::Unchecked);
            m_CC->setVisible(false);
            m_CCtext->setVisible(false);
        }
        m_BCC->setVisible(true);
        m_BCCtext->setVisible(true);
    }
    else if (m_modeBCC->checkState() == Qt::Unchecked)
    {
        m_BCC->setVisible(false);
        m_BCCtext->setVisible(false);

    }
}
QString usrname = "";
int File_Flag = 0;
void EmailDialog::receiveData(QString Data)
{
    usrname = Data;
    qDebug() << "usrname sent ::" << usrname << endl;
}

void EmailDialog::onSend()
{
    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock("192.168.43.38", 5000, addr, AF_INET);//2
    qDebug() << "connet success" << endl;
    /*********connecting*********/

    std::vector<std::string> file_addr;
    if (File_Flag == 1)
    {
        file_addr.push_back(file_path.toStdString() + "/" + file_name.toStdString());
    }
    Mail_send t_send;
    QString send_info = m_addr->text() + "/" + usrname + "/" + m_topictext->text();
    QByteArray info_send;
    info_send.append(send_info);
    QString m_text = m_textEdit->toHtml();
    QByteArray m_TEXT;
    m_TEXT.append(m_text);
    t_send.to_send_mail(sock, info_send.data(), m_TEXT.data(), 0, &file_addr);
    /*********disconnecting*********/

    m_addr->clear();
    m_topictext->clear();
    m_textEdit->clear();
    client_base::close_sock(sock);
}

void EmailDialog::onSave()
{
    /*********connecting*********/
    sockaddr_in addr;//1
    qDebug() << "conneting" << endl;
    int sock = client_base::run_sock("192.168.43.38", 5000, addr, AF_INET);//2
    qDebug() << "connet success" << endl;
    /*********connecting*********/

    std::vector<std::string> file_addr;
    Mail_send t_send;
    QString send_info = m_addr->text() + "/" + usrname + "/" + m_topic->text();
    QByteArray info_send;
    info_send.append(send_info);
    QString m_text = m_textEdit->toHtml();
    QByteArray m_TEXT;
    m_TEXT.append(m_text);
    t_send.to_send_mail(sock, info_send.data(), m_TEXT.data(), 1, &file_addr);
    QApplication::setQuitOnLastWindowClosed(false);
    QMessageBox::warning(NULL, "", "发送成功！",
        QMessageBox::Yes | QMessageBox::Yes);
    /*********disconnecting*********/
    client_base::close_sock(sock);
    m_addr->clear();
    m_topictext->clear();
    m_textEdit->clear();
}

void EmailDialog::onAddFile()
{

}


//*****删除文件
void EmailDialog::fileup_delete()
{
    QApplication::setQuitOnLastWindowClosed(false);
    QMessageBox messageBox(QMessageBox::NoIcon,
        "提示", "确定要删除附件吗?",
        QMessageBox::Yes | QMessageBox::No, NULL);

    int result = messageBox.exec();


    File_Flag = 0;
    switch (result)
    {
    case QMessageBox::Yes:
        QApplication::setQuitOnLastWindowClosed(false);
        file_name.clear();//文件名
        EmailDialog::file_path.clear();
        toolBar_fileup->removeAction(fileup);
        QMessageBox::information(NULL, "提示", "已删除", QMessageBox::Yes | QMessageBox::Yes);
        break;
    case QMessageBox::No:
        qDebug() << "NO";
        break;
    default:
        break;
    }


}




//*******文件
void EmailDialog::onInsertFile()
{
    QString file_full;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this, tr("加入附件"), tr(""), "");


    fi = QFileInfo(file_full);
    file_name = fi.fileName();//文件名
    EmailDialog::file_path = fi.absolutePath();//文件路径

    if (file_full.isEmpty())
    {
        QMessageBox::information(NULL, "错误", "未加入文件!!", QMessageBox::Yes | QMessageBox::Yes);//报错 可省去
        return;
    }



    QByteArray QBfile_name;
    QBfile_name.append(file_name);

    const char* char_file_name = QBfile_name.data();



    fileup = new QAction(QIcon(tr(":/res/fileup.png")), tr(""), this);

    fileup->setToolTip(tr(char_file_name));

    connect(fileup, SIGNAL(triggered()), this, SLOT(fileup_delete()), Qt::UniqueConnection);

    toolBar_fileup->addAction(fileup);

    File_Flag = 1;


}
