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
    FILE *fp = fopen(filename, "r");

    // Vérifie que le fichier est ouvert
    if (fp == NULL)
    {
        printf("Une erreur est survenue.\n");
        exit(1);
    }

    char c;
    // Parcours le fichier et affiche son contenu
    while((c = fgetc(fp)) != EOF)
    { printf("%c", c); }

    // Ferme le fichier
    fclose(fp);
}

int main(int argc, char* argv[])
{
    // Parcours chaque fichiers passé en paramètre
    for(int i=1; i<argc ; i++)
    { cat_file(argv[i]); }    

    return 0;
}
