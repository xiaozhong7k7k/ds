#include<stdio.h>
#include<stdlib.h>

int  *array_create(int n);
void  array_destroy(int *a); 

int main()
{
    int n;
    scanf("%d",&n);

    int *p = array_create(n);

    array_destroy(p);
    p = NULL;

    return 0;
}

int  *array_create(int n)
{
    if(n == 0){ return NULL;}
    int *p = (int*)malloc(sizeof(int)*n);
    for(int i = 0 ; i<n ; i++){
        p[i] = i+1;
        printf("%d ",p[i]);
    }
    return p;
}

void array_destroy(int *a)
{
    free(a);//用一级指针，让用户自己置空
}