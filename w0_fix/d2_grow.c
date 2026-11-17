#include<stdio.h>
#include<stdlib.h>

int *grow(int *old, int old_n, int new_n);

int main()
{
    int *old = (int*)malloc(sizeof(int)*10);
    for(int i = 0; i<10 ; i++){
        old[i] = i+1;
        printf("%d ",old[i]);
    }
    old = grow(old,10,20);
    old = grow(old,20,40);
    old = grow(old,40,80);
    free(old);
    old = NULL;

    return 0;
}

int *grow(int *old, int old_n, int new_n)
{
    int *new = (int*)malloc(sizeof(int)*new_n);
    if(new ==NULL){ return NULL; }
    for(int i = 0 ; i<old_n ; i++){
        new[i] = old[i];
    }
    free(old);//用户自己置空
    return new;
}
