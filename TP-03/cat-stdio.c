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
    FILE *fp = fopen(filename, "r");

    // Vérifie que le fichier est ouvert
    if (fp == NULL)
    {
        printf("Une erreur est survenue.\n");
        exit(1);
    }

    // Parcours le fichier
    char ch;
    while((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }

    // Ferme le fichier
    fclose(fp);
}

int main(int argc, char* argv[])
{
    // Parcours chaque nom de fichiers 
    for(int i=1; i<argc ; i++)
    {
        cat_file(argv[i]);
    }    

    return 0;
}
