#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    int sexe; // 1=Garcon, 2=Fille
    char prenom[25];
    int annee; // 1900..2020
    int nombre; // d'enfants nés cette année avec ce prénom
} tuple;

void afficher_tuple(tuple t)
{
    printf("[%d, %s, %d, %d]\n", t.sexe, t.prenom, t.annee, t.nombre);
}

bool tuple_valide(tuple t)
{
    return !(t.sexe == 0);
}

int main(int argc, char **argv)
{
    // Vérifie le nombre d'arguments
    if (argc < 2) {
        printf("%s: usage %s filename\n", argv[0], argv[0]);
        exit(1);
    }

    printf("Ouverture du fichier...\n");
    int fd = open(argv[1], O_RDONLY);

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
    tuple* file_content = (tuple*)mmap(NULL, filesize, PROT_READ, MAP_FILE|MAP_SHARED, fd, 0);

    size_t nb_tuple = filesize / sizeof(tuple);
    printf("Nombre de tuple: %zu\n", nb_tuple);

    tuple row; size_t nb_valid = 0;
    for (size_t i = 0; i < nb_tuple; i++)
    {
        row = file_content[i];
        if (tuple_valide(row))
        {
            nb_valid += 1;
            afficher_tuple(row);
        }
    }

    printf("Nombre de tuple valides : %zu\n", nb_valid);
    
    
    // Libère la mémoire et le fichier
    munmap(file_content, filesize);
    close(fd);

    return 0;
}
