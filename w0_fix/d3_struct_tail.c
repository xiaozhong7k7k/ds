#include<stdio.h>
#include<stdlib.h>

typedef struct Node { 
    int val;
    struct Node *next;
 } Node;

// typedef struct MemoryPool{
//     Node *head;
//     Node *tail;
// }MemoryPool;

Node *build(int n);// 尾插，1..n
void  print(Node *head);// 1 -> 2 -> ... -> n
void  destroy(Node *head);//释放链表

int main()
{
    int n;
    scanf("%d",n);

    Node *head = build(5);
    print(head);
    destroy(head);

    return 0;
}

Node *build(int n)
{
    Node *head = NULL;
    Node *tail = head;
    for(int i = 0; i<n ; i++){
    Node *p = (Node*)malloc(sizeof(Node));
    if(p == NULL){
        printf("内存分配失败");
        return NULL;
    }
    if(head == NULL){
        head = tail = p;
    }else{
        tail->next = p;
        tail = p;
    }
    tail->next = NULL;
    tail->val = i+1;
    }
    return head;
}

void  print(Node *head)
{
    Node *cur = head;
    for( cur = head ; cur!= NULL ; cur = cur->next){
        printf("%d ",cur->val);
    }
}

void  destroy(Node *head)
{
    Node *temp = head;
    for( temp = head ; temp!=NULL ; temp = temp->next){
        free(temp);
    }
}