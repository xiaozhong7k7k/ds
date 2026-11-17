#include<stdio.h>
#include<string.h>

int  my_strlen(const char *s)
{
    int idx = 0;
    while(*s++ != '\0'){
        idx++;
    }
    return idx;
}

void my_strcpy(char *dst, const char *src)
{
    while(*dst++ = *src++) ;//复制到‘\0’循环结束，赋值表达式的结果是赋进去的值
}

int main()
{
    char a[10] = "hello";
    char b[] = "world";

    my_strcpy(b,a);
    printf("%d ",my_strlen(a));
    printf("%s\n",b);

    return 0;
}