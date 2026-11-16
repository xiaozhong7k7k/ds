#include<stdio.h>

void minmax(int a[],int *min,int *max,int n)
{
    *min = a[0];
    *max = a[0];
    for(int i = 0 ; i<n-1 ; i++ ){
        if(a[i]<*min){
            *min = a[i];
        }
        if(a[i]>*max){
            *max = a[i];
        }
    }
}

int main()
{
    int min;
    int max;
    int a[5] = { 9,2,3,7,5 };
    minmax(a,&min,&max,sizeof(a)/sizeof(a[0]));

    printf("%d %d",min,max);
    return 0;
}