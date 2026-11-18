#include<stdio.h>

int sum_of(int *a, int n)
{
    if(n == 1){ return a[0]; }
    int sum = a[n-1];
    sum += sum_of(a,n-1);
    return sum;
}

int main()
{
    //int a[5] = {4,2,5,8,6};
    int a[1] = {3};
    int sum = sum_of(a,1);
    printf("%d\n",sum);

    return 0;
}