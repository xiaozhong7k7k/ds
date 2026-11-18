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
int  length(Node *head);//检测链表长度
Node *find(Node *head, int val);//链表的查找

int main()
{
    int n;
    scanf("%d",&n);

    Node *a = build(n);//测试尾插法
    print(a);

    Node *b = build_head(n);//测试头插法
    print(b);
    destroy(b);

    printf("length = %d\n", length(a));//测试量长度的函数
    printf("length(NULL) = %d\n", length(NULL));

    Node *p = find(a, 3);
    printf("find(3) = %d\n", p ? p->val : -1);//测试链表查找

    p = find(a, 99);
    printf("find(99) = %s\n", p ? "found" : "NULL");

    destroy(a);

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

int  length(Node *head)
{
    if(head == NULL){ return 0; }
    int count = 0;//记录链表长度
    Node *cur = head;
    for( count = 0; cur != NULL ; cur = cur->next, count++ ){ ; }//遍历链表求长度
    return count;
}

Node *find(Node *head, int val)
{
    if(head == NULL){ return NULL; }
    Node *cur = head;
    for( ; cur->val != val ; cur = cur->next){//找到就凭循环条件退出，此时的cur就是目标节点
        if(cur == NULL){//找不到就凭借if退出，返回的cur此时就是NULL
            break;
        }
    }
    return cur;
}
