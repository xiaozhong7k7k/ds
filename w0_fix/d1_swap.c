#include<stdio.h>

struct Point { int x, y; };

void swap_point(struct Point *a, struct Point *b);

int main()
{
    struct Point a = { 1,2 };
    struct Point b = { 3,4 };

    swap_point(&a,&b);
    printf("%d %d %d %d",a.x,a.y,b.x,b.y);

    return 0;
}

void swap_point(struct Point *a, struct Point *b)
{
    struct Point temp = *a;
    *a = *b;
    *b = temp;
}