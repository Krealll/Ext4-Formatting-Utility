#include "FormattingThread.h"

FormattingThread::FormattingThread(QThread *parent) : QThread(parent)
{

}

void FormattingThread::setThreadFormatManager(FormatManager &value)
{
    threadFormatManager = value;
}

void FormattingThread::run()
{
    if(!formatDevice()){
        emit send(infoString);
        return;
    }
    this->threadFormatManager.setFlagFormat(false);
    this->quit();
}

bool FormattingThread::formatDevice()
{
    if(1/*threadFormatManager.initialiseData()*/)
    {
        emit send("-Initialized new file system");
        sleep(1);
        if(1/*threadFormatManager.manageTables()*/)
        {
            emit send("-Writed inode tables");
            sleep(1);
            if(1/*threadFormatManager.createDirectories()*/)
            {
                emit send("-Created root and lost+found direcories");
                sleep(1);
                if(1/*threadFormatManager.writeReservedInodes()*/)
                {
                    emit send("-Writed reserved Inodes");
                    sleep(1);
                    if(1/*threadFormatManager.endFormatting()*/){
                        emit send("-Formatting ended");
                        return true;
                    }
                    else{
                        infoString="ERROR: while initialising file system";
                    }
                }else{
                    infoString="ERROR: while writing tables";
                }
            }else{
                infoString="ERROR: while creating directories";
            }
        }else{
            infoString="ERROR: while writing reserved inodes";
        }
    }else{
        infoString="ERROR: while closing file system";
    }
    return  false;

//    if(threadFormatManager.initialiseData())
//    {
//        emit send("-Initialized new file system");
//        sleep(1);
//        if(threadFormatManager.manageTables())
//        {
//            emit send("-Writed inode tables");
//            sleep(1);
//            if(threadFormatManager.createDirectories())
//            {
//                emit send("-Created root and lost+found direcories");
//                sleep(1);
//                if(threadFormatManager.writeReservedInodes())
//                {
//                    emit send("-Writed reserved Inodes");
//                    sleep(1);
//                    if(threadFormatManager.endFormatting()){
//                        emit send("-Formatting ended");
//                        return true;
//                    }
//                    else{
//                        infoString="ERROR: while initialising file system";
//                    }
//                }else{
//                    infoString="ERROR: while writing tables";
//                }
//            }else{
//                infoString="ERROR: while creating directories";
//            }
//        }else{
//            infoString="ERROR: while writing reserved inodes";
//        }
//    }else{
//        infoString="ERROR: while closing file system";
//    }
//    return false;
}
