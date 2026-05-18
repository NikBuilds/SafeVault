#ifndef SAFEVALUT_H
#define SAFEVALUT_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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
    FILEPATH_ERR_EMPTY,
    FILEPATH_ERR_LENGTH,
    FILENAME_ERR_LENGTH,
    FILE_ERR_INVALID
} FileError;

// File handling function (fileHandling.c)
FileError init_file(FileInfo *info, const char src_path[]);
FileError move_file(FileInfo *fileInfo);
FileError copy_file(FileInfo *fileInfo);
FileError delete_file(FileInfo *fileInfo);

// Command Dispatch Pattern 
typedef struct {
    const char *name;
    const char *description;
    int (*execute)(int argc, char *argv[]);
} Command;

// Befehl-Funktionen deklarieren
int cmd_move(int argc, char *argv[]);
int cmd_copy(int argc, char *argv[]);
int cmd_delete(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);


typedef enum permission_error_msg {
    PERMISSION_SET,
    PERMISSION_ERR_DEFAULT,
    PERMISSION_ERR_WRITE,
    PERMISSION_ERR_READ,
    PERMISSION_ERR_EXEC
} PermissionError;

// Quarantine folder
int setup_quarantine_folder(const char *path);

// Permissions
static inline PermissionError default_permission(FileInfo *info)
{
    if (chmod(info->dest_path, 0000) != 0) {
        perror("Failed to set default permission");
        return PERMISSION_ERR_DEFAULT;
    }
    printf("Permission set to default!\n");
    return PERMISSION_SET;
}

#endif