#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("A\n");
    execl("./rebours", "./rebours", "5", NULL); /* Remplace le processus */
    printf("A\n"); /* Cette ligne ne sera jamais exécutée */
    return 0;
}