#include "../include/safeVault.h"

int main() {

    if (mkdir(QUARANTINE_PATH, 0755) != 0 && errno != EEXIST) {
        perror("Fehler beim Erstellen des Quarantine-Ordners");
    }
    
    FileInfo fileMove;
    FileInfo fileCopy;
    
    init_file(&fileMove, "/mnt/e/CodingProjects/Git_Repos/C/Vault/targetMove.txt");
    init_file(&fileCopy, "/mnt/e/CodingProjects/Git_Repos/C/Vault/targetCopy.txt");

    copy_file(&fileCopy);
    move_file(&fileMove);
    
    return 0;
}