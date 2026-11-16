#include<stdio.h>
#include<stdlib.h>

void new_array(int **p, int n)
{
    *p = (int*)malloc(n*sizeof(int));
    int i = 0;
    for( i = 0 ; i<n ; i++){
        (*p)[i] = i+1;
    }
}

int main()
{
    int *p = NULL;
    int n = 5;
    new_array(&p,n);

    for(int i = 0 ; i<n ; i++){
        printf("%d ",p[i]);
    }
    free(p);
    return 0;
}