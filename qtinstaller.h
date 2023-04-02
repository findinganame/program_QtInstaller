#ifndef QTINSTALLER_H
#define QTINSTALLER_H

#include <QWidget>
#include <QListWidgetItem>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class QtInstaller; }
QT_END_NAMESPACE

class QtInstaller : public QWidget
{
    Q_OBJECT

public:
    QtInstaller(QWidget *parent = nullptr);
    ~QtInstaller();

private slots:
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn6_clicked();

private slots:
    void on_actRun_triggered();
    void slot_itemDoubleClicked(QListWidgetItem *item);
    void on_btnRename_clicked();
    void on_btnRenameZip_clicked();

private:
    Ui::QtInstaller *ui;
    QString exeName;//程序名，包含.exe
    QString exename;//程序名，不包含后缀名.exe
    QString releasePath;
    QString strCmd;
    QString strLabel_copied="已复制：\n";
    void prepareDir();
    void removeFiles();
    void writeFlag(QString flag);
    void listDir();

};
#endif // QTINSTALLER_H
