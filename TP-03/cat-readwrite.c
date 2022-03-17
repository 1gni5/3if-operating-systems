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
    int fp = open(filename, O_RDWR);

    // Vérifie que le fichier est ouvert
    if (fp == -1)
    {
        printf("Une erreur est survenue.\n");
        exit(1);
    }

    // Réserver la mémoire pour un buffer de lecture
    char buffer;

    while (read(fp, &buffer, 1) != 0)
    {
        printf("%c", buffer);
    }

    printf("\n");


    // Ferme le fichier
    close(fp);
}

int main(int argc, char* argv[])
{
    for(int i=1; i<argc ; i++)
    {
        printf("%s\n",argv[i]);
        cat_file(argv[i]);
    }    

    return 0;
}
