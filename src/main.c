#include <stdio.h>
#include <string.h>

#define QURANTINE_PATH "quarantine"

void move_file(const char src_path[]) 
{
    // Cuts filename from source path
    char *filename = strrchr(src_path, '/'); // strrchr points to the adress of the last char of '/' and returns the adress of this char
    filename = filename ? filename + 1 : src_path; // If filename not Null then filename + 1 (+ 1 because: sample.txt and not /sample.txt)
    
    char src_file[256];
    strcpy(src_file, filename);

    // Concorate destination path
    char dest_path[512];      // Buffer for the dest path
    snprintf(dest_path, sizeof(dest_path), "%s/%s", QURANTINE_PATH, src_file); // Concorate string in des_path with QURANTINE_PATH with source file 

    FILE *src = fopen(src_path, "rb");      // Opens source file in read binary mode (return NULL if fail)
    if (!src) {
        perror("ERROR: Cannot open fource file");
        return;
    }

    FILE *dst = fopen(QURANTINE_PATH + "/" + src_file, "wb");
}

int main() {



    printf("Hello SafeVault");    
    return 0;
}