#include <stdio.h>

unsigned int count(int N)
{
    int result = 0;
    while (N > 0)
    {
        result += N % 2;
        N /= 2;
    }

    return result;
}

int main()
{
    printf("%d\n", count(15));
    return 0;
}