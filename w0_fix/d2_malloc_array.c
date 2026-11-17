#include<stdio.h>
#include<stdlib.h>

int  *array_create(int n);
void  array_destroy(int *a); 

int main(void)
{
    int n;
    scanf("%d",&n);
    if(n <= 0){ return 0; }

    int *p = array_create(n);
    if(p != NULL ){
        for(int i = 0 ; i<n ; i++){
            printf("%d ",p[i]);
        }
        printf("\n");
    }else {
        fprintf(stderr,"内存分配失败: n=%d\n",n);
        return 1;
    }

    array_destroy(p);
    p = NULL;

    return 0;
}

int  *array_create(int n)
{
    int *p = (int*)malloc(sizeof(int)*n);
    if(p == NULL){ return NULL; }
    for(int i = 0 ; i<n ; i++){
        p[i] = i+1;
    }
    return p;
}

void array_destroy(int *a)
{
    free(a);//用一级指针，让用户自己置空
}
