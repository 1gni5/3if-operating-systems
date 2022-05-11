#include <stdio.h>
#include <unistd.h>

int main()
{
    int x = 30;

    if (fork() == 0)
    {
        x = x + 5;
        printf("C = %d, D = %p\n", x, &x);
    }
    else
    {
        x = x - 5;
        printf("A = %d, B = %p\n", x, &x);
    }

    return 0;
}