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
    int fd = open(filename, O_RDWR);


    // Vérifie que le fichier est ouvert
    if (fd == -1)
    {
        printf("Une erreur est survenue.\n");
        exit(1);
    }

    // Récupère la taille du fichier
    struct stat buf; fstat(fd, &buf);
    size_t filesize = buf.st_size;

    // Map le contenu du fichier en mémoire
    char* file_content = (char*)mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);

    // Affiche le contenu du fichier
    write(1, file_content, filesize);

    // Supprime la zone de mémoire
    munmap(file_content, filesize);

    // Ferme le fichier
    close(fd);
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
