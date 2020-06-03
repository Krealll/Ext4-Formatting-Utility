#pragma once
#include <iostream>
#include<ext2fs/ext2fs.h>
#include<ext2fs/ext2_err.h>
#include<QString>

class FileSystem{
    private:
       QString path;
       long currentSize=0;              //Size of file system
       QString information;             //String for file system information
    public:
        ext2_filsys fs;                 // stucture that represent file system
        ext2_super_block sb;            // stucture that represent superblock of file system
        errcode_t error;
        FileSystem(){
            path="";
            this->sb.s_log_block_size=0;
        }
        void setBlockSize(int size);
        void setBlockCount(int count);
        QString getPath() const;
        void setPath(QString pathToFs);
        long getCurrentSize() const;
        void setCurrentSize(long value);
        QString getInformation() const;
        void setInformation(const QString &value);

        bool openFs();                  // method opens that file system
        void processSize();             // setting block size and block count
        bool initializeFs();            //setting size of current file system
        QString info();                 // method that show program info
};
