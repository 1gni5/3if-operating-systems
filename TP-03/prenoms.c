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
#include <string.h>
#include <assert.h>

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
    // Si le sexe est à 0 le tuple est invalide
    return !(t.sexe == 0);
}

char* camel_case(char* str) // Oui c'est écrit en snake_case :p
{
    bool wordBegin = true;
    char* reader = str;
    
    // Parcours la chaine
    while(*reader != '\0')
    {
        // Début de mot, rien à faire
        if (wordBegin) 
        { 
            wordBegin = false;
        }

        // À l'intérieur, passage en minuscule
        else if (isalpha(*reader)) 
        {
            *reader = tolower(*reader);
            wordBegin = false;

        } 

        // Séparateur, nouveau mot
        else if (*reader == '-')
        {
            wordBegin = true;
        }

        // Avance la tête de lecture
        reader++;
    }

    return str;
}


// Requêtes sur la base de données

// Requête: prénom le plus long de la base
char* query_prenom_le_plus_long(tuple* db, int nb_tuple)
{
    // Initalisation de la réponse
    char* answer = db[0].prenom;
    size_t max = strlen(answer), tmp;

    // Parcours des tuples
    for (size_t i = 0; i < nb_tuple; i++)
    {
        tuple* row = &db[i];

        // Passe les tuples invalides
        if (!tuple_valide(*row)) {continue;}

        // Nouvelle réponse trouvée
        if ((tmp = strlen(row->prenom)) > max)
        {
            answer = row->prenom;
            max = tmp;
        }
    }

    return answer;
}

// Requête: Prénom le plus populaire en xxxx
int query_prenom_le_plus_populaire_en_annee(tuple* db, int nb_tuple, char* prenom)
{
    // Initialisation de la réponse
    int occurences = -1, year = -1;

    // Parcours les tuples
    for (size_t i = 0; i < nb_tuple; i++)
    {
        tuple* row = &db[i];

        // Passe les tuples invalides
        if (!tuple_valide(*row)) {continue;}

        // Nouvelle réponse trouvée
        if (row->nombre > occurences && strcmp(row->prenom, prenom))
        {
            occurences = row->nombre;
            year = row->annee;
        }
    }

    return year;
}

// Requête: applique la transformation CamelCase à tout les prénoms
void query_apply_camel_case(tuple* db, int nb_tuple)
{
    // Parcours les tuples de la base
    for (size_t i = 0; i < nb_tuple; i++)
    { 
        tuple* row = &db[i];

        // Passe les tuples invalides
        if (!tuple_valide(*row)) {continue;}

        // Passe en CamelCase
        camel_case(row->prenom);
    }
}

int main(int argc, char **argv)
{
    // Vérifie le nombre d'arguments
    if (argc < 2) {
        printf("%s: usage %s filename\n", argv[0], argv[0]);
        exit(1);
    }

    // Ouverture du fichier
    int fd = open(argv[1], O_RDWR);

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
    tuple* file_content = (tuple*)mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);

    // Test si l'allocation s'est bien déroulée
    assert(file_content != MAP_FAILED);
    
    // Calcul le nombre de tuple dans le fichier
    size_t nb_tuple = filesize / sizeof(tuple);
    printf("Nombre total de tuple: %zu\n", nb_tuple);

    // Parcours tout les tuples
    tuple row; size_t nb_valid = 0;
    for (size_t i = 0; i < nb_tuple; i++)
    {
        row = file_content[i];

        // Compte le nombre de tuple valide
        if (tuple_valide(row)) {nb_valid ++;}
    }

    printf("Nombre de tuple valides : %zu\n", nb_valid);

    // Variable des requêtes
    char* prenom = "JULES";

    // Exécute les requêtes
    printf("Le prénom le plus long de la base est %s\n",
        query_prenom_le_plus_long(file_content, nb_tuple));
    printf("Le prénom %s était le plus populaire en %d.\n", prenom,
        query_prenom_le_plus_populaire_en_annee(
            file_content, nb_tuple, prenom
        )
    );
    // À décommenter pour activer la modification
    // query_apply_camel_case(file_content, nb_tuple);
    
    // Affiche le contenu de la base
    // for (size_t i = 0; i < nb_tuple; i++)
    // {
    //     tuple row = file_content[i];
    //     // Affiche les tuples valides
    //     if (tuple_valide(row)) {afficher_tuple(row);}
    // }
    
    // Libère la mémoire et le fichier
    munmap(file_content, filesize);
    close(fd);

    return 0;
}
