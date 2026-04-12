#ifndef SAFEVALUT_H
#define SAFEVALUT_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define QUARANTINE_PATH "./quarantine/"

typedef struct {
    char src_file[256];
    char dest_path[512];
    char src_path[1024];
    unsigned long file_id;
    size_t size;
} FileInfo;

typedef enum file_error_msg {
    FILE_OK,
    FILE_ERR_OPEN,
    FILE_ERR_WRITE,
    FILE_ERR_DELETE,
    FILE_ERR_NULL,
    FILEPATH_ERR_EMPTY
} FileError;

// File handling function (fileHandling.c)
FileError init_file(FileInfo *info, const char src_path[]);
FileError move_file(FileInfo *fileInfo);
FileError copy_file(FileInfo *fileInfo);
FileError delete_file(FileInfo *fileInfo);

#endif