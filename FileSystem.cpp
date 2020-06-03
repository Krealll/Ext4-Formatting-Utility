#include "FileSystem.h"

QString FileSystem::getPath() const
{
    return path;
}

void FileSystem::setPath(QString pathToFs)
{
    path = pathToFs;
}

long FileSystem::getCurrentSize() const
{
    return currentSize;
}

void FileSystem::setCurrentSize(long value)
{
    currentSize = value;
}

QString FileSystem::getInformation() const
{
    return information;
}

void FileSystem::setInformation(const QString &value)
{
    information = value;
}

void FileSystem::setBlockSize(int size)
{
    this->sb.s_log_block_size=size;
}

void FileSystem::setBlockCount(int count)
{
    this->sb.s_blocks_count=count;
}

bool FileSystem::openFs()                           // method opens that file system
{
    initialize_ext2_error_table();
    error = ext2fs_open(this->path.toStdString().c_str(),EXT2_FLAG_RW, 0,4096,unix_io_manager,&fs);
    if(error){
        if(error==EXT2_ET_UNEXPECTED_BLOCK_SIZE){
            error = ext2fs_open(this->path.toStdString().c_str(),EXT2_FLAG_RW, 0,2048,unix_io_manager,&fs);
            if(error){
                if(error==EXT2_ET_UNEXPECTED_BLOCK_SIZE){
                    error = ext2fs_open(this->path.toStdString().c_str(),EXT2_FLAG_RW, 0,1024,unix_io_manager,&fs);
                    if(error){
                        return false;
                    }
                }
                else{
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    return true;
}

void FileSystem::processSize()                          // setting block size and block count
{
    int tempSize, tempCount;
    tempSize =1024<<this->sb.s_log_block_size;
    tempCount=this->currentSize/tempSize;
    memset(&this->sb,0,sizeof (this->sb));
    setBlockCount(tempCount);
    setBlockSize(tempSize>>11);
}

QString FileSystem::info()                              // method that show program info
{
    if(this->currentSize==0){                           // first time case
       information="Input proper device path to get information";
       return information;
    }
    if(!openFs()){
        information="Error: path not found or device is not supported";
        return information;
    }
    information="";
    information.append("Device name: ");
    information.append(this->fs->device_name);
    information.append("\n");
    information.append("Block size: ");
    information.append(QString().number(this->fs->blocksize));
    information.append("\n");
    information.append("Block count: ");
    information.append(QString().number(this->fs->super->s_blocks_count));
    information.append("\n");
    information.append("First data blok: ");
    information.append(QString().number(this->fs->super->s_first_data_block));
    information.append("\n");
    information.append("Free blocks count: ");
    information.append(QString().number(this->fs->super->s_free_blocks_count));
    information.append("\n");
    ext2fs_free(fs);
    return information;
}

bool FileSystem::initializeFs()                         //setting size of current file system
{
    if(openFs()){
        this->sb.s_log_block_size=0;
        currentSize=this->fs->super->s_blocks_count;
        currentSize*=this->fs->blocksize;
        ext2fs_close(fs);
        return true;
    }
    else{
        return false;
    }
}
