#include "../include/safeVault.h"

int setup_quarantine_folder(const char *path) 
{
    // Verzeichnis erstellen mit sofort restriktiven Rechten
    if (mkdir(path, 0700) != 0) {
        perror("mkdir");
        return -1;
    }

    // Besitzer auf Root (oder Daemon-User) setzen
    if (chown(path, 0, 0) != 0) {
        perror("chown");
        return -1;
    }

    // Rechte nochmal explizit setzen (umask umgehen)
    if (chmod(path, 0700) != 0) {
        perror("chmod");
        return -1;
    }

    return 0;
}