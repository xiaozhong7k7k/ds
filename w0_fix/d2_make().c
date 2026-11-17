#include<stdio.h>
#include<stdlib.h>

int *make(void) {
    int *buf = (int*)malloc(sizeof(int)*100);
    if(buf == NULL){ return NULL; }
    for (int i = 0; i < 100; i++) buf[i] = i;
    return buf;
}
int main(void) {
    int *p = make();
    printf("%d\n", p[10]);
    free(p);
    p = NULL;
    return 0;
}