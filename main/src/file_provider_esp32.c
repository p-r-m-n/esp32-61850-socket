#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "esp_vfs.h"
//#include "unity.h"

#include "hal_filesystem.h"
#include "lib_memory.h"

struct sDirectoryHandle {
    DIR* handle;
};


FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    FileHandle newHandle = NULL;

    if (readWrite)
        newHandle = (FileHandle) fopen(fileName, "w");
    else
        newHandle = (FileHandle) fopen(fileName, "r");

    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return fread(buffer, maxSize, 1, (FILE*) handle);
}

int
FileSystem_writeFile(FileHandle handle, uint8_t* buffer, int size)
{
    return fwrite(buffer, size, 1, (FILE*) handle);
}

void
FileSystem_closeFile(FileHandle handle)
{
    fclose((FILE*) handle);
}

bool
FileSystem_deleteFile(char* filename)
{
    if (remove(filename) == 0)
        return true;
    else
        return false;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
    if (rename(oldFilename, newFilename) == 0)
        return true;
    else
        return false;
}


bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    struct stat fileStats;

    if (stat(filename, &fileStats) == -1)
        return false;

    if (lastModificationTimestamp != NULL)
        *lastModificationTimestamp = (uint64_t) (fileStats.st_mtime) * 1000LL;
        // does not work on older systems --> *lastModificationTimestamp = (uint64_t) (fileStats.st_ctim.tv_sec) * 1000LL;

    if (fileSize != NULL)
        *fileSize = fileStats.st_size;

    return true;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    DIR* dirHandle = opendir(directoryName);

    DirectoryHandle handle = NULL;

    if (dirHandle != NULL) {
        handle = (DirectoryHandle) GLOBAL_MALLOC(sizeof(struct sDirectoryHandle));
        handle->handle = dirHandle;
    }

    return handle;
}

char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    struct dirent* dir;

    dir = readdir(directory->handle);

    if (dir != NULL) {
        if (dir->d_name[0] == '.')
            return FileSystem_readDirectory(directory, isDirectory);
        else {
            if (isDirectory != NULL) {
                if (dir->d_type == DT_DIR)
                    *isDirectory = true;
                else
                    *isDirectory = false;
            }

            return dir->d_name;
        }
    }
    else
        return NULL;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
    closedir(directory->handle);
    GLOBAL_FREEMEM(directory);
}

