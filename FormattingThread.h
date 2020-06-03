#ifndef FORMATTINGTHREAD_H
#define FORMATTINGTHREAD_H
#include "FormatManager.h"
#include <QObject>
#include<QThread>
class FormattingThread : public QThread             //custom thread class that manages formatting
{
    Q_OBJECT
public:
    FormatManager threadFormatManager;
    explicit FormattingThread(QThread *parent = nullptr);
    void setThreadFormatManager(FormatManager &value);
    bool flagCreated=0;                             // flag that indicates that thread was created
                                                    //and doesn't need to connect slots again
protected:
    QString infoString;                             // string for sending info into main thread
    void run();
    bool formatDevice();                            //main method of program
signals:
    void send(QString);                             // method that will send info about formatting process

signals:

};

#endif // FORMATTINGTHREAD_H
