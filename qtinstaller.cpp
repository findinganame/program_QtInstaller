#include "qtinstaller.h"
#include "ui_qtinstaller.h"
#include<iostream>
#include<filesystem>
namespace fs = std::filesystem;
#include<QDebug>
#include <QClipboard>
#include <QDir>
#include<QFile>
#include <QSettings>
#include <QThread>
#include <QProcess>
#include<QMessageBox>
#define print qDebug()<<

QtInstaller::QtInstaller(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtInstaller)
{
    ui->setupUi(this);
    releasePath=QDir::currentPath();//保存当前路径
    prepareDir();//新建文件夹，并复制exe文件
    listDir();//显示当前目录所有文件夹
    ui->btn_run->setDefaultAction(ui->actRun);
    ui->btn_exit->setDefaultAction(ui->actExit);
}
QtInstaller::~QtInstaller()
{
    delete ui;
}

void QtInstaller::prepareDir(){//新建文件夹，并复制exe文件

    QDir dir("release");
    for(auto& file:dir.entryList()){if(file.contains(".exe")){exeName=file;}}//获取程序文件名
    exename=exeName.split(".")[0];//获取程序名，不包含后缀名.exe    
    QDir dir2(releasePath);
    if (dir2.exists(exename)){fs::remove_all(exename.toStdString());}    //文件夹已存在则删除
    dir2.mkdir(exename);//新建文件夹，名字与程序同名
    QFile::copy("release/"+exeName,exename+"/"+exeName);
    //创建cmd命令
    strCmd="windeployqt "+QDir::toNativeSeparators(dir.currentPath()+QDir::separator()+exename+QDir::separator()+exeName);
}


void QtInstaller::on_btn1_clicked(){
    QDir::setCurrent("D:\\Qt\\5.12.3\\mingw73_64\\bin");
    std::system(strCmd.toStdString().c_str());
    QDir::setCurrent(releasePath);
    writeFlag("1");
}

void QtInstaller::on_btn2_clicked(){
    QDir::setCurrent("D:\\Qt\\6.4.2\\mingw_64\\bin");
    std::system(strCmd.toStdString().c_str());
    QDir::setCurrent(releasePath);
    writeFlag("2");
}

void QtInstaller::on_btn3_clicked(){
    QDir::setCurrent("C:\\Qt\\5.12.3\\mingw73_64\\bin");
    std::system(strCmd.toStdString().c_str());
    QDir::setCurrent(releasePath);
    writeFlag("3");
}

void QtInstaller::on_btn4_clicked(){
    QDir::setCurrent("C:\\Qt\\6.4.2\\mingw_64\\bin");
    std::system(strCmd.toStdString().c_str());
    QDir::setCurrent(releasePath);
    writeFlag("4");
}

void QtInstaller::on_btn6_clicked(){
    QDir::setCurrent("D:\\Qt\\6.4.2\\mingw_64\\bin");
    std::system(strCmd.toStdString().c_str());
    QDir::setCurrent(releasePath);
    writeFlag("6");
}



void QtInstaller::writeFlag(QString flag){//写flag文件
    QFile file("flag");
    file.open(QIODevice::WriteOnly);
    file.write(flag.toUtf8().data() );
    file.close();
}

void QtInstaller::listDir(){//显示所有文件夹
    QDir dir(releasePath);
    for (QString& str:dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)){ ui->listWidget->addItem(str);}
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,this,&QtInstaller::slot_itemDoubleClicked);
}

void QtInstaller::on_actRun_triggered(){//运行上一次环境
    QFile flag("flag");
    if (flag.open(QFile::ReadOnly)){
        switch (flag.readAll().toInt())
        {
        case 1:
            on_btn1_clicked();
            break;
        case 2:
            on_btn2_clicked();
            break;
        case 3:
            on_btn3_clicked();
            break;
        case 4:
            on_btn4_clicked();
            break;
        case 5:
            break;
        case 6:
            on_btn6_clicked();
            break;
        default:
            break;
        }
    }
    flag.close();
    return;

}

void QtInstaller::slot_itemDoubleClicked(QListWidgetItem *item)//双击复制文件
{
    fs::copy(item->text().toStdString(), exeName.split(".")[0].toStdString()+"/"+item->text().toStdString(), std::filesystem::copy_options::recursive);
    strLabel_copied.append(item->text()+"\n");
    ui->label_copied->setText(strLabel_copied);
}



void QtInstaller::on_btnRename_clicked(){//重命名exe文件和文件夹
    QString newName=ui->lineEdit_rename->text();
    QDir dir;
    dir.rename(exename,newName);
    QFile::rename(newName+"/"+exeName,newName+"/"+newName+".exe");
}


void QtInstaller::on_btnRenameZip_clicked(){//重命名，再压缩
    if(ui->lineEdit_rename->text().isEmpty()){
        //lineEdit_rename为空，直接压缩
        QString str="Rar.exe a "+exename+".zip "+exename;
        std::system(str.toStdString().c_str());
    }else{
        //lineEdit_rename不为空，先重命名，再压缩
        QString newName=ui->lineEdit_rename->text();
        QDir dir;
        dir.rename(exename,newName);
        QFile::rename(newName+"/"+exeName,newName+"/"+newName+".exe");
        QString zipName=newName+".zip";
        QProcess* p=new QProcess(this);
        p->start("Rar.exe",QStringList()<<"a"<<zipName<<newName);
        connect(p,&QProcess::finished,this,[=](int exitCode){QMessageBox::information(this,"压缩完成","压缩完成         \n\n\n");p->deleteLater();});
    }
}

