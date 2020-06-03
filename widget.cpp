#include "widget.h"
#include "ui_widget.h"
#include"FileSystem.h"
#include <QMessageBox>
#include "FormattingThread.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textEdit->hide();
    ui->progressBar->hide();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()                            // slot that manages file system info
{
    if(fs->getPath()==""){
        fs->info();
    }
    QMessageBox::information(this,"File system information",this->fs->getInformation());
}

void Widget::on_pushButton_2_clicked()                           // slot that manages formatting button
{
    switch(QMessageBox::question(this,"WARNING","Be sure to unmount your device before fromatting",QMessageBox::Cancel|QMessageBox::Ok))
    {
    case QMessageBox::Cancel:
        fs->info();
        return;
    case QMessageBox::Ok:
        if(this->fs->getPath()==""){
            QMessageBox::information(this,"Warning","Input proper device path");
            return;
        }                                                       // If user confirmed, that device is unmounted
        ui->textEdit->show();                                   // formatting thread starts and connects
        ui->progressBar->show();                                // to info recieving slot: update
        this->setFixedHeight(300);
        this->fs->processSize();
        this->fm->setFileSystem(*fs);
        this->fm->setFlagFormat(true);
        ui->comboBox->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->lineEdit->setDisabled(true);
        ui->textEdit->clear();
        ui->textEdit->setDisabled(true);
        ui->progressBar->setValue(0);
        fThread.setThreadFormatManager(*this->fm);
        fThread.start();
        if(fThread.flagCreated==0){
            connect(&fThread,SIGNAL(send(QString)),this,SLOT(update(QString)));
            fThread.flagCreated=1;
        }
    default:
        return;
    }
}

void Widget::on_lineEdit_editingFinished()                          // slot that manages path to device
{
       this->fs->setPath(ui->lineEdit->displayText().toStdString().c_str());
       if(!this->fs->initializeFs()){
           this->fs->setCurrentSize(0);
           this->fs->setPath("");
           fs->info();
           return;
       }
       fs->info();
}

void Widget::on_comboBox_currentIndexChanged(int index)             // slot that manages block size choice
{
    this->fs->setBlockSize(index);
}

void Widget::on_pushButton_3_clicked()                              // slot that manages program info
{
    QMessageBox::information(this,"Program information"
                             , "Ext 4 formatting utility\nVersion 1.0"
                               "\nSupport: https://github.com/Krealll");
}

void Widget::closeEvent(QCloseEvent* event)                         // slot that disable closing the program while formatting
{
    if(!fm->getFlagFormat()||fThread.threadFormatManager.getFlagFormat()==0){
        event->accept();
        return;
    }
    event->ignore();
}

void Widget::update(QString info)                                   // slot that recieves string from formatting thread
{
    ui->textEdit->append(info);
    ui->progressBar->setValue(ui->progressBar->value()+12);
    if(info=="-Formatting ended"){
        ui->progressBar->setValue(ui->progressBar->value()+10);
        usleep(500000);
        ui->progressBar->setValue(ui->progressBar->value()+10);
        usleep(300000);
        ui->progressBar->setValue(ui->progressBar->value()+10);
        usleep(100000);
        ui->progressBar->setValue(ui->progressBar->value()+10);
        ui->lineEdit->setDisabled(false);
        ui->comboBox->setDisabled(false);
        ui->pushButton_2->setDisabled(false);

    }
}
