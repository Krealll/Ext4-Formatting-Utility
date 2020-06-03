#include <FormatManager.h>


void FormatManager::setFileSystem(FileSystem &value)
{
    fileSystem = value;
}

bool FormatManager::getFlagFormat() const
{
    return flagFormat;
}

void FormatManager::setFlagFormat(bool value)
{
    flagFormat = value;
}

bool FormatManager::initialiseData()                            //initializinf file system and allocating
{                                                               //and marking it super_dirty in order to write it on device at the end
        fileSystem.sb.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
        fileSystem.error = ext2fs_initialize("/dev/sdc1", EXT2_FLAG_PRINT_PROGRESS, &fileSystem.sb,
                       unix_io_manager, &fileSystem.fs);
        if(this->fileSystem.error){
            return false;
        }
        ext2fs_mark_super_dirty(fileSystem.fs);
        return true;
}

bool FormatManager::manageTables()                              //allocating space for inode table and bitmaps
{
        fileSystem.error = ext2fs_allocate_tables(fileSystem.fs);
        if(this->fileSystem.error){
            return false;
        }
        blk64_t	blk;
        dgrp_t	i;
        int	num;
        for (i = 0; i < fileSystem.fs->group_desc_count; i++) {
            blk = ext2fs_inode_table_loc(fileSystem.fs, i);
                                                                // solution for overflow error
            num = ext2fs_div_ceil((fileSystem.fs->super->s_inodes_per_group -
                               ext2fs_bg_itable_unused(fileSystem.fs, i)) *
                              EXT2_INODE_SIZE(fileSystem.fs->super),
                              EXT2_BLOCK_SIZE(fileSystem.fs->super));
            ext2fs_bg_flags_set(fileSystem.fs, i, EXT2_BG_INODE_ZEROED);
            ext2fs_zero_blocks2(fileSystem.fs, blk, num, &blk, &num);
        }
                                                                // in case when fs has cmus feature
                                                                // it's necessary to write zeroed inodes
        if (ext2fs_has_feature_metadata_csum(fileSystem.fs->super))
        {
            ext2_ino_t ino;
            ext2_inode *inode;
            fileSystem.error = ext2fs_get_memzero(EXT2_INODE_SIZE(fileSystem.fs->super), &inode);
            if(this->fileSystem.error){
                return false;
            }
            for (ino = 1; ino < EXT2_FIRST_INO(fileSystem.fs->super); ino++) {
                fileSystem.error = ext2fs_write_inode_full(fileSystem.fs, ino, inode,
                                EXT2_INODE_SIZE(fileSystem.fs->super));
            }
            ext2fs_free_mem(&inode);

        }
        return true;
}

bool FormatManager::createDirectories()                             //creating root and lost+found directories
{
    ext2_inode inode;
    ext2_ino_t ino;
    const char *name = "lost+found";
    unsigned int lpf_size = 0;

    fileSystem.error = ext2fs_mkdir(fileSystem.fs, EXT2_ROOT_INO, EXT2_ROOT_INO, 0);
    if(this->fileSystem.error){
      return false;
    }
    if (getuid()) {
      fileSystem.error = ext2fs_read_inode(fileSystem.fs, EXT2_ROOT_INO, &inode);
      if(this->fileSystem.error){
          return false;
      }
      inode.i_uid = getuid();
      if (inode.i_uid)
          inode.i_gid = getgid();

    }                                                               //Lost+found creation
    fileSystem.fs->umask = 077;                                     // setting dir permissions
    fileSystem.error = ext2fs_mkdir(fileSystem.fs, EXT2_ROOT_INO, 0, name);
    if(this->fileSystem.error){
      return false;
    }
    fileSystem.error = ext2fs_lookup(fileSystem.fs, EXT2_ROOT_INO, name, strlen(name), 0, &ino);
    if(this->fileSystem.error){
      return false;
    }
    for (int i=1; i < EXT2_NDIR_BLOCKS; i++) {
      if ((lpf_size += fileSystem.fs->blocksize) >= 16*1024 &&
          lpf_size >= 2 * fileSystem.fs->blocksize)
          break;
      fileSystem.error = ext2fs_expand_dir(fileSystem.fs, ino);
      if(this->fileSystem.error){
          return false;
      }
    }
    return true;
}

bool FormatManager::writeReservedInodes()
{
    ext2_ino_t	inodeR;
    for (inodeR = EXT2_ROOT_INO + 1; inodeR < EXT2_FIRST_INODE(fileSystem.fs->super); inodeR++)
        ext2fs_inode_alloc_stats2(fileSystem.fs, inodeR, +1, 0);
    ext2fs_mark_ib_dirty(fileSystem.fs);
    return true;
}

bool FormatManager::endFormatting()                                 // fflushing filesystem information
{                                                                   //to the device and closing filesystem
    ext2fs_flush(fileSystem.fs);
    ext2fs_close_free(&fileSystem.fs);
    return true;
}

