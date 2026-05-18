#include "../include/safeVault.h"

/*
TODO: 

Was wenn src_path MAX size?
Was wenn dest_path zu gorß wird?

Was wenn snprinf fails?
Was wenn strings NULL?
Was wenn src_path is 0?

Feedback wenn datei schon da in quarantäne
*/

static inline unsigned long filehash_djb2(const char str[]) 
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    return hash;
}

FileError init_file(FileInfo *fileInfo, const char src_path[]) 
{
    if (src_path == NULL) return FILE_ERR_NULL;
    if (strlen(src_path) == 0) return FILEPATH_ERR_EMPTY;

    // SICHER: Längenprüfung vor strcpy
    if (strlen(src_path) >= sizeof(fileInfo->src_path)) {
        return FILEPATH_ERR_LENGTH;  // Pfad zu lang
    }
    strcpy(fileInfo->src_path, src_path); // Copy's src_path in file struct

    // Cuts filename from source path
    // strrchr points to the adress of the last char of '/' and returns the adress of this char
    // If filename not Null = filename + 1 (+ 1 because: sample.txt and not /sample.txt)
    const char *filename = strrchr(src_path, '/'); 
    filename = filename ? filename + 1 : src_path; 
    
    // SICHER: Längenkontrolle für Dateinamen
    if (strlen(filename) >= sizeof(fileInfo->src_file)) {
        return FILENAME_ERR_LENGTH;  // Dateiname zu lang
    }
    strcpy(fileInfo->src_file, filename);

    // Concorate destination path (des_path) with QURANTINE_PATH with source file 
    if (snprintf(fileInfo->dest_path, sizeof(fileInfo->dest_path), 
                "%s%s", QUARANTINE_PATH, fileInfo->src_file) < 0) { 
        return FILE_ERR_INVALID; 
    }

    fileInfo->file_id = filehash_djb2(src_path);
    printf("File: %s (%lu)\n", fileInfo->src_file, fileInfo->file_id);
    
    return FILE_OK;
}

FileError move_file(FileInfo *fileInfo) 
{
    // Opens stream to source file in read binary mode (return NULL if fail). Returns a pointer to a FILE strcut
    FILE *src = fopen(fileInfo->src_path, "rb");      
    if (!src) {
        perror("Cannot open fource file");
        return FILE_ERR_OPEN;
    }

    // Opens stream to source file in write binary mode 
    FILE *dst = fopen(fileInfo->dest_path, "wb");
    if (!dst) {
        perror("Fehler beim Öffnen der Zieldatei");
        fclose(src);
        return FILE_ERR_OPEN;
    }

    char buffer[1024];  // Ein temporärer Zwischenspeicher (1024 Bytes = 1 KB). Daten werden hier kurz „geparkt", bevor sie weitergegeben werden.
    size_t bytes;       // Speichert, wie viele Bytes fread tatsächlich gelesen hat.

    /* Writes data from src into dst:
        - fread(buffer, 1, sizeof(buffer), src) -> Liest (fread = file reade) bis zu 1024 Bytes aus src in den buffer. 1 gibt an wie groß ist ein einzelnes Element in Bytes ist (1x char). (Bei Dateiende oder Fehler: gibt 0 zurück -> Schleife endet)
        - fwrite(buffer, 1, bytes, dst) -> Schreibt genau so viele size_t bytes aus dem Inhalt von buffer in dst (nicht immer 1024 – der letzte Block ist oft kleiner).
    */
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t written = fwrite(buffer, 1, bytes, dst);
        if (written < bytes) {
            fclose(src); 
            fclose(dst);
            return FILE_ERR_WRITE;
        }
    }

    fclose(src);
    fclose(dst);

    // Delete original
    if (remove(fileInfo->src_path) != 0) {
        perror("Fehler beim Löschen der Originaldatei");
        return FILE_ERR_DELETE;
    }

    return FILE_OK;
}

FileError copy_file(FileInfo *fileInfo) 
{
    // Opens stream to source file in read binary mode (return NULL if fail). Returns a pointer to a FILE strcut
    FILE *src = fopen(fileInfo->src_path, "rb");      
    if (!src) {
        perror("Cannot open fource file");
        return FILE_ERR_OPEN;
    }

    // Opens stream to source file in write binary mode 
    FILE *dst = fopen(fileInfo->dest_path, "wb");
    if (!dst) {
        perror("Fehler beim Öffnen der Zieldatei");
        fclose(src);
        return FILE_ERR_OPEN;
    }

    char buffer[1024];
    size_t bytes;

    // Writes data from src into dst
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t written = fwrite(buffer, 1, bytes, dst);
        if (written < bytes) {
            fclose(src);
            fclose(dst);
            return FILE_ERR_WRITE;
        }
    }

    fclose(src);
    fclose(dst);

    return FILE_OK;
}

FileError delete_file(FileInfo *fileInfo) 
{
    // Delete file in quarantine
    if (remove(fileInfo->dest_path) != 0) {
        perror("Fehler beim Löschen der Originaldatei");
        return FILE_ERR_DELETE;
    }
    
    printf("Deleted file: %s \n", fileInfo->src_file);
    return FILE_OK;
}