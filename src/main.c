#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define QUARANTINE_PATH "./quarantine/"

typedef enum file_error_msg {
    FILE_OK,
    FILE_ERR_OPEN,
    FILE_ERR_WRITE,
    FILE_ERR_DELETE,
    FILE_ERR_NULL,
    FILEPATH_ERR_EMPTY
} FileError;


/*
Was wenn src_path MAX size?
Was wenn dest_path zu gorß wird?

Was wenn snprinf fails?
Was wenn strings NULL?
Was wenn src_path is 0?

Feedback wenn datei schon da
*/

FileError move_file(const char src_path[]) 
{
    if (src_path == NULL) return FILE_ERR_NULL;
    if (strlen(src_path) == 0) return FILEPATH_ERR_EMPTY;

    // Cuts filename from source path
    const char *filename = strrchr(src_path, '/'); // strrchr points to the adress of the last char of '/' and returns the adress of this char
    filename = filename ? filename + 1 : src_path; // If filename not Null = filename + 1 (+ 1 because: sample.txt and not /sample.txt)
    printf("Filename: %s \n", filename);
    
    char src_file[256];
    strcpy(src_file, filename);

    // Concorate destination path
    char dest_path[512];      // Buffer for the dest path
    snprintf(dest_path, sizeof(dest_path), "%s%s", QUARANTINE_PATH, src_file); // Concorate string in des_path with QURANTINE_PATH with source file 
    printf("dest_path: %s \n", dest_path);

    // Opens stream to source file in read binary mode (return NULL if fail). Returns a pointer to a FILE strcut
    FILE *src = fopen(src_path, "rb");      
    if (!src) {
        perror("ERROR: Cannot open fource file");
        fclose(src);
        return FILE_ERR_OPEN;
    }

    // Opens stream to source file in write binary mode 
    FILE *dst = fopen(dest_path, "wb");
    if (!dst) {
        perror("Fehler beim Öffnen der Zieldatei");
        return FILE_ERR_OPEN;
    }

    char buffer[1024];
    size_t bytes;

    // Writes data from src into dst
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if(fwrite(buffer, 1, bytes, dst) <= 0) {
            return FILE_ERR_WRITE;
        } 
    }

    fclose(src);
    fclose(dst);

    // Delete original
    if (remove(src_path) != 0) {
        perror("Fehler beim Löschen der Originaldatei");
        return FILE_ERR_DELETE;
    }

    return FILE_OK;
}

int main() {

    if (mkdir(QUARANTINE_PATH, 0755) != 0 && errno != EEXIST) {
        perror("Fehler beim Erstellen des Quarantine-Ordners");
    }

    move_file("/mnt/e/CodingProjects/Git_Repos/C/Vault/target.txt");
    return 0;
}