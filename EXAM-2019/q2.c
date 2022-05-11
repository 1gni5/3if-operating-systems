#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void chaine(unsigned int N)
{
    printf("N = %d, pid : %d\n", N, getpid());

    if (N >= 1 && fork() == 0)
    {
        chaine(N-1);
    }
}

int main()
{
    chaine(5);
    return 0;
}