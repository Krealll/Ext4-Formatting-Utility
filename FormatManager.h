#pragma once
#include <iostream>
#include<unistd.h>
#include<ext2fs/ext2fs.h>
#include "FileSystem.h"


class FormatManager {
    FileSystem fileSystem;
    bool flagFormat =0;             //flag that indicates beginning of formatting
    public:
        FormatManager(){}

        bool initialiseData();      //initializinf file system and allocating
                                    //and marking it super_dirty in order to write it on device at the end

        bool manageTables();        //allocating space for inode table and bitmaps
        bool createDirectories();   //creating root and lost+found directories
        bool writeReservedInodes();
        bool endFormatting();       // fflushing filesystem information to the device and closing filesystem

        void setFileSystem(FileSystem &value);
        bool getFlagFormat() const;
        void setFlagFormat(bool value);
};
