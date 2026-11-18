#include<stdio.h>
#include<stdlib.h>

typedef struct Node{ 
    int val;
    struct Node *next;
 } Node;

// typedef struct MemoryPool{
//     Node *head;
//     Node *tail;
// }MemoryPool;

Node *build(int n);// 尾插，1..n
Node *build_head(int n);   // 头插建 1..n
void  print(Node *head);// 1 -> 2 -> ... -> n
void  destroy(Node *head);//释放链表

int main()
{
    int n;
    scanf("%d",&n);

    Node *a = build(n);
    print(a);
    destroy(a);

    Node *b = build_head(n);
    print(b);
    destroy(b);

    return 0;
}

Node *build(int n)//用栈上的指针接着malloc出的内存没问题，只要在换作用域的时候拿新的指针接住就行
{
    Node *head = NULL;
    Node *tail = head;
    for(int i = 0; i<n ; i++){
    Node *p = (Node*)malloc(sizeof(Node));
    if(p == NULL){
        printf("内存分配失败");//防呆
        return NULL;
    }
    if(head == NULL){
        head = tail = p;
    }else{
        tail->next = p;//尾插
        tail = p;
    }
    tail->next = NULL;//初始化
    tail->val = i+1;
    }
    return head;
}

Node *build_head(int n)
{
    Node *head = NULL;
    for(int i = 0 ; i<n ; i++){
        Node *p = (Node*)malloc(sizeof(Node));
        if(p == NULL){ return NULL; }
        p->val = i+1;
        p->next = head;
        head = p;
    }
    return head;
}

void  print(Node *head)
{
    Node *cur = head;
    for( cur = head ; cur!= NULL ; cur = cur->next){
        printf("%d -> ",cur->val);//链表这么打印
    }
    printf("NULL\n");
}

void  destroy(Node *head)
{
    Node *temp = head;
    while(temp != NULL){
        Node *next = temp->next;//如果用temp = temp->next，就相当于再次调用了free完的悬空指针
        free(temp);
        temp = next;
    }
}