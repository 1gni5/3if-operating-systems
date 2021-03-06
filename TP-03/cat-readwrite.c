#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void cat_file(char* filename)
{
    // Ouvre le fichier
    int fd = open(filename, O_RDONLY);

    // Vérifie que le fichier est ouvert
    assert(fd != -1);

    // Réserver la mémoire pour un buffer de lecture
    char buffer; // Réserve un seul charactère sinon contenu manquant.

    // Parcours l'ensemble du fichier
    while (read(fd, &buffer, 1) != 0)
    { write(1, &buffer, 1); }

    // Ferme le fichier
    close(fd);
}

int main(int argc, char* argv[])
{
    // Parcours tout les fichiers passé en paramètre
    for(int i=1; i<argc ; i++)
    { cat_file(argv[i]); }    

    return 0;
}
