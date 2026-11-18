#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

Node *build(int n);          // 尾插建表：1..n，返回头指针
void  print(Node *head);     // 1 -> 2 -> ... -> n -> NULL
void  destroy(Node *head);   // 释放全部节点

int main(void)
{
    for (int n = 0; n <= 5; n++) {
        Node *head = build(n);
        printf("[n=%d] ", n);
        print(head);
        destroy(head);
    }
    return 0;
}

Node *build(int n)
{
    (void)n;
    // TODO: 你来写。提示：
    //   1. head 和 tail 一开始都是 NULL
    //   2. 空表时：head = tail = 新节点
    //      非空时：tail->next = 新节点; tail = 新节点
    //   3. 每个新节点的 next 必须是 NULL（想想为什么）
    return NULL;
}

void print(Node *head)
{
    (void)head;
    // TODO: 从 head 一路走到 NULL，按 "1 -> 2 -> 3 -> NULL" 的格式打印
}

void destroy(Node *head)
{
    (void)head;
    // TODO: 释放所有节点。
    // 注意：free 掉当前节点后，就再也读不到它的 next 了。
}