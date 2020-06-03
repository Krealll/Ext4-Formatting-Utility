#pragma once
#include "FormatManager.h"
#include "FileSystem.h"
#include "FormattingThread.h"
#include<ext2fs/ext2fs.h>
#include<ext2fs/ext2_err.h>
#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();                   // slot that manages file system info

    void on_pushButton_2_clicked();                 // slot that manages formatting button

    void on_lineEdit_editingFinished();             // slot that manages path to device

                                                    // slot that manages block size choice
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_3_clicked();                 // slot that manages program info

    void closeEvent(QCloseEvent*);                  // slot that disable closing the program while formatting
    void update(QString info);                      // slot that recieves string from formatting thread

private:
    Ui::Widget *ui;
    FormatManager* fm = new FormatManager();
    FileSystem* fs = new FileSystem();
    FormattingThread fThread;

};
