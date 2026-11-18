#include<stdio.h>

void reverse(char *s, int left, int right)
{
    if(left>=right){ return ;}
    char x = s[left];
    s[left] = s[right];
    s[right] = x;
    reverse(s,left+1,right-1);
}

int main(void)
{
    char s[] = {"hello"};
    reverse(s,0,4);
    printf("%s ",s);

    return 0;
}