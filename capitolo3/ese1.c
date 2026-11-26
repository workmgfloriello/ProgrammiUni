#include <stdio.h>

int main()
{
    printf("A\tA+2\tA+4\tA+6\n");
    int i = 1;

    while(i<=15)
    {
        if (i % 3 == 0)
        {
            printf("%d\t%d\t%d\t%d\n", i, i + 2, i + 4, i + 6);
        }
        i++;
    }
}