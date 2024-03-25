
#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QDialog>
#include <QFontComboBox>
#include <QFont>
#include <QLabel>
#include <QVBoxLayout>

class QToolBar;
class QFontComboBox;
class QToolButton;

class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;
class QTextEdit;

QT_BEGIN_NAMESPACE
namespace Ui { class emaildialog; }
QT_END_NAMESPACE

class EmailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailDialog(QWidget* parent = 0);
    ~EmailDialog();

    void initToolBar();
    void initEmailDialog();
    QTextEdit* getTextEdit();

    bool checkIsEmpty();

signals:
    void sendData(QString data);

private slots:
    void receiveData(QString data);

    void onFontFamily(const QString& str);
    void onFontSize(const QString& str);
    void onBold();
    void onItalic();
    void onUnderLine();
    void onStrikeout();
    void onFontColor(const QColor& color);
    void onLeftAlign();
    void onCenterAlign();
    void onRightAlign();
    void onJustifyAlign();
    void onLineSpaceAboutToShow();
    void onLineSpaceTriggered(QAction* action);
    void onLineSpace();
    void onBulletsAboutToShow();
    void onBulletsTriggered(QAction* action);
    void onBullets();
    void onDecIndent();
    void onIncIndent();
    void onInsertDate();
    void onCalendarWidget(const QDate& date);
    void onInsertImage();
    void onInsertTable();

    void onCheckCC();
    void onCheckBCC();
    void onSend();
    void onSave();
    void onAddFile();
    // void Forward();
    // void Cancel();
     //****文件
    void onInsertFile();
    void fileup_delete();

private:
    qreal          m_initIndent;
    qreal          m_addIndent;

    QLabel* m_recv;
    QLineEdit* m_addr;

    QLabel* m_topic;
    QLineEdit* m_topictext;

    QLabel* m_CC;
    QLineEdit* m_CCtext;

    QLabel* m_BCC;
    QLineEdit* m_BCCtext;

    QPushButton* m_send;
    QPushButton* m_save;
    QPushButton* m_addfile;
    QPushButton* m_forward;
    QPushButton* m_cancel;

    QCheckBox* m_modeCC;
    QCheckBox* m_modeBCC;


    QHBoxLayout* m_CClayout;
    QHBoxLayout* m_BCClayout;
    QHBoxLayout* m_topiclayout;
    QHBoxLayout* m_hlayout;
    QVBoxLayout* m_layout;
    QToolBar* m_toolBar;

    QToolBar* toolBar_fileup;
    QAction* m_actInsertFile;
    QAction* fileup;

    QFontComboBox* m_fontFamilyBox;
    QComboBox* m_fontSizeBox;
    QToolButton* m_boldBtn;
    QToolButton* m_italicBtn;
    QToolButton* m_underLineBtn;
    QToolButton* m_strikeoutBtn;
    QToolButton* m_fontColorBtn;
    QAction* m_actLeftAlign;
    QAction* m_actCenterAlign;
    QAction* m_actRightAlign;
    QAction* m_actJustifyAlign;
    QAction* m_actLineSpace;
    QAction* m_actBullets;
    QAction* m_actDecIndent;
    QAction* m_actIncIndent;
    QAction* m_actInsertDate;
    QAction* m_actInsertImage;
    QAction* m_actInsertTable;
    QAction* m_actInsertHyperlink;

    QLabel* m_userLabel;
    QLineEdit* m_userEdit;
    QLabel* m_passwordLabel;
    QLineEdit* m_passwordEdit;
    QLabel* m_serverLabel;
    QComboBox* m_serverBox;
    QLabel* m_portLabel;
    QComboBox* m_portBox;
    QCheckBox* m_sslCheck;
    QLabel* m_sslLabel;
    QPushButton* m_sendBtn;
    QPushButton* m_closeBtn;
    QLabel* m_senderLabel;
    QLineEdit* m_senderEdit;
    QLabel* m_receiverLabel;
    QLineEdit* m_receiverEdit;
    QLabel* m_titleLabel;
    QLineEdit* m_titleEdit;
    QLabel* m_attachmentLabel;
    QLineEdit* m_attachmentEdit;
    QPushButton* m_browseBtn;
    QLabel* m_textLabel;
    QTextEdit* m_textEdit;

private:
    QString sender_;
    QString title;
    QString text;
    QString file_path;
    QString file_name;
private:
    Ui::emaildialog* ui;

};

#endif // EMAILDIALOG_H
