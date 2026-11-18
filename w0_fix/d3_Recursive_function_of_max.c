//终止条件：只剩一个元素时，最大值就是它。
//缩小规模：前 n 个元素的最大值，跟前 n-1 个元素的最大值有关。
//合并：拿 a[n-1] 和子问题的返回值比。
#include<stdio.h>

int max_of(int *a,int n)
{
    if(n == 1){ return a[0]; }
    int m = max_of(a,n-1);
    return a[n-1] > m ? a[n-1] : m;
}

int main()
{
    //int a[3] = {9,1,2};
    //int a[3] = {-2,-5,-9};
    int a[1] = {7};
    int max = max_of(a,1);
    printf("%d\n",max);

    return 0;
}