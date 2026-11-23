/* ============================================================
 * B 关 W1-2：单链表基础（尾插 / 头插 / 查找 / 插入 / 删除）
 *
 * 【表示法约定：带头结点】
 *   head 是头结点（哨兵），始终存在，不存有效数据
 *   head->next 是首元节点；head->next == NULL 表示空表
 *   build_tail(0) / build_head(0) 返回只有头结点的表，不是 NULL
 *   长度不数头结点；find 从 head->next 开始比较
 *   destroy 连头结点一起释放
 *
 * 规则：只填 8 个函数体，main 和测试部分不要改。
 *       每填完一题敲 ./check.sh，看哪一段是 ✅。
 * ============================================================ */

/*笔记：1.头指针 ≠ 头结点。约定带头结点后，空表是 head != NULL && head->next == NULL，不是 head == NULL。
2.malloc 不清零，新节点的 next 是随机值。所以不能 malloc 一个节点当游标，需要初始化赋值为，head或其他节点
3.新增节点必须写进前驱的 next 字段。p = prev->next(交换后就对); 只改栈上的局部指针，新节点没人指向，等于没接上，还泄漏。栈上指针在函数结束后成野指针
4.删除节点不需要 malloc。只要让前驱跨过目标（prev->next = t->next），再 free(t)；断链只动已有节点的指针。
5.遍历从头结点下一个开始（head->next），头结点的 val 没意义，读它就是未初始化内存。
*
6.build_head 里 if(p == NULL){ return NULL; } 会泄漏已经建好的整条链表和头结点。应先把已建的销毁，再返回 NULL。
7.build_tail 里 if(p == NULL){ return head; } 返回的是半截链表，调用方分不清“成功但短”还是“失败”。同样应销毁后返回 NULL。
8.list_length 和 list_print 都没判 head == NULL，传空指针会崩。测试没覆盖，但接口上该防。
*
重点：
9.要新增一个节点、并且函数返回后它还得活着 → malloc（链表节点、树节点、返回给调用方的数组）
10.只是借已有节点的地址来走/来存 → 栈上指针就够（cur、prev、next、temp）
11.大小运行时才知道 → malloc（n 个 int 的数组）；栈数组大小必须编译期定死
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

/* ---------- 1. 尾插建表 ----------
 * 建 1..n，返回头结点；n <= 0 也返回只有头结点的表
 * 要求：用一个 tail 指针，整体 O(n)
 */
Node *list_build_tail(int n)
{
//旧写法：Node *head = (Node*)malloc(sizeof(Node));
//        if(head == NULL){ return NULL; }
//        if(n<=0) { return NULL};
//错误：函数要返回的是一个头结点，如果malloc成功了却返回NULL，那么已经申请的头指针就会导致内存泄露，而且忘记对头指针的next初始化
    Node *head = (Node*)malloc(sizeof(Node));
    if(head == NULL){ return NULL; }
    head->next = NULL;
    if(n <= 0){ return head; }

    Node *tail = head;

    for(int i = 0; i<n ; i++){
        Node *p = (Node*)malloc(sizeof(Node));
        if(p == NULL){ return head; }
        p->next = NULL;
        p->val = i+1;

        // if(head->next == NULL){
        //     head->next = p;
        //     tail = p;
        // }else{}  多余
            tail->next = p;
            tail = p;
    }
    return head;
}

/* ---------- 2. 头插建表 ----------
 * 建 1..n，结果顺序是 n..1；n <= 0 返回只有头结点的表
 */
Node *list_build_head(int n)
{
    Node *head = (Node*)malloc(sizeof(Node));
    if(head == NULL){ return NULL; }
    head->next = NULL;
    if(n <= 0){ return head; }

    for(int i = 0; i<n ; i++){
        Node *p = (Node*)malloc(sizeof(Node));
        if(p == NULL){ return NULL; };
        p->val = i+1;
        p->next = head->next;
        head->next = p;
    }

    return head;
}

/* ---------- 3. 求长度 ---------- */
int list_length(Node *head)
{
    Node *cur = head;
    int i = 0;
    while(cur->next != NULL){
        cur = cur->next;
        i++;
    }
    return i;
}

/* ---------- 4. 查找 ----------
 * 找到返回值所在的第一个节点，找不到返回 NULL
 * 从 head->next 开始找；头结点的 val 没意义，不要拿来比
 * 注意：先判 cur != NULL，再读 cur->val
 */
Node *list_find(Node *head, int v)
{
    if(head == NULL){ return NULL; }
    Node *cur = head->next;

    while(cur != NULL && cur->val != v){
        cur = cur->next;
    }

    return cur;
}

/* ---------- 5. 在下标 idx 处插入 ----------
 * 合法范围：0 <= idx <= 长度（等于长度就是尾插）
 * 带头结点：idx=0 就是插在 head->next 前面，头结点本身不动
 * 成功返回 0；idx 非法或 malloc 失败返回 -1
 */
int list_insert(Node **head, int idx, int v)
{
    int len = list_length(*head);
    if( idx<0 || idx>len){ return -1; }
    Node *prev = (*head);

    for(int i = 0; i<idx ; i++){
        prev = prev->next;
    }
//原来的p指的是栈上内存
//旧写法：p->next = prev->next->next;
//p = prev->next;      /* 这行只是让局部变量 p 指向别人 */

//错误：p = prev->next; 没有链接任何东西，只是把 p 这个指针改指向原来的后继节点；
//刚 malloc 的那个节点从此没人指向，泄漏了（valgrind 那 4 个 16 字节就是它）。
//然后 p->val = v 覆盖的是已有节点的值，所以表里数据没多出来
//新增要 malloc 并把地址写进链；删除只需栈上指针做断链，堆上的目标节点交给 free。
    Node *p = (Node*)malloc(sizeof(Node));
    if(p == NULL){ return -1; }
    p->next = prev->next;
    prev->next = p;
    p->val = v;

    return 0;
}

/* ---------- 6. 删除下标 idx 处的节点 ----------
 * 合法范围：0 <= idx < 长度
 * 成功返回 0；idx 非法返回 -1
 * 释放前先存 next，再 free
 */
int list_erase(Node **head, int idx)
{
    int len = list_length(*head);
    if(idx < 0 || idx >= len){ return -1; }

    Node *cur = (*head);

    int i = 0;
    for(i = 0; i<idx ; i++){
        cur = cur->next;
    }

    // if(i == 0){
    //     cur = (*head)->next;
    //     (*head)->next = (*head)->next->next;
    //     free(cur);
    //     cur = NULL;
    // }else{}  多余
        Node *temp = cur->next;
        cur->next = cur->next->next;
        free(temp);

    return 0;
}

/* ---------- 7. 打印（自己调试用，测试不检查） ----------
 * 1 -> 2 -> 3 -> NULL（末尾换行）
 */
void list_print(Node *head)
{
    if(head == NULL){ return -1; }
    Node *cur = head->next;

    while(cur != NULL){
        printf("%d -> ",cur->val);
        cur = cur->next;
    }
    printf("NULL\n");
}

/* ---------- 8. 销毁 ----------
 * 连头结点一起释放；head 为 NULL 时也要安全返回
 */
void list_destroy(Node *head)
{
    Node *cur = head;
    while(cur != NULL){
        Node *temp = cur->next;
        free(cur);
        cur = temp;
    }
}

/* ==================== 以下测试不要改 ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

/* 跳过带头结点，只比数据节点；不用学生写的 list_length */
static int eq(Node *h, const int *a, int n) {
    Node *cur = (h == NULL) ? NULL : h->next;
    for (int i = 0; i < n; i++) {
        if (!cur || cur->val != a[i]) return 0;
        cur = cur->next;
    }
    return cur == NULL;
}

static void test_build(void) {
    printf("[1] build_tail / build_head / length\n");
    Node *L = list_build_tail(0);
    CHECK(L != NULL && L->next == NULL, "build_tail(0)：只有头结点，next 为 NULL");
    if (!L) { CHECK(0, "空表没有头结点，后面跳过"); return; }
    list_destroy(L);

    L = list_build_tail(1);
    int a1[] = {1};
    CHECK(eq(L, a1, 1), "build_tail(1) = 1");
    CHECK(list_length(L) == 1, "length(1 个) = 1");
    list_destroy(L);

    L = list_build_tail(5);
    int a5[] = {1, 2, 3, 4, 5};
    CHECK(eq(L, a5, 5), "build_tail(5) = 1..5");
    CHECK(list_length(L) == 5, "length(5 个) = 5");
    list_destroy(L);

    L = list_build_head(5);
    int h5[] = {5, 4, 3, 2, 1};
    CHECK(eq(L, h5, 5), "build_head(5) = 5..1");
    list_destroy(L);

    L = list_build_head(0);
    CHECK(L != NULL && L->next == NULL, "build_head(0)：只有头结点");
    list_destroy(L);
}

static void test_find(void) {
    printf("[2] list_find\n");
    Node *L = list_build_tail(5);
    if (!L || L->next == NULL) { CHECK(0, "建表失败，find 跳过"); list_destroy(L); return; }

    Node *p = list_find(L, 1);
    CHECK(p != NULL && p->val == 1, "find(1) 命中首元");
    p = list_find(L, 5);
    CHECK(p != NULL && p->val == 5, "find(5) 命中尾节点");
    p = list_find(L, 3);
    CHECK(p != NULL && p->val == 3, "find(3) 命中中间");
    p = list_find(L, 99);
    CHECK(p == NULL, "find(99) 返回 NULL");
    list_destroy(L);
}

static void test_insert(void) {
    printf("[3] list_insert\n");
    Node *L = list_build_tail(3);       /* 1 2 3 */
    if (!L || L->next == NULL) { CHECK(0, "建表失败，insert 跳过"); list_destroy(L); return; }

    CHECK(list_insert(&L, 0, 100) == 0, "插到 idx=0 返回 0");
    int e1[] = {100, 1, 2, 3};
    CHECK(eq(L, e1, 4), "头插后 {100,1,2,3}");

    CHECK(list_insert(&L, 4, 7) == 0, "插到 idx=4（尾）返回 0");
    int e2[] = {100, 1, 2, 3, 7};
    CHECK(eq(L, e2, 5), "尾插后 {100,1,2,3,7}");

    CHECK(list_insert(&L, 2, 30) == 0, "插到 idx=2 返回 0");
    int e3[] = {100, 1, 30, 2, 3, 7};
    CHECK(eq(L, e3, 6), "中间插入后 {100,1,30,2,3,7}");

    CHECK(list_insert(&L, 99, 0) == -1, "idx 超范围 -> -1");
    CHECK(eq(L, e3, 6), "非法 idx 不改表");
    CHECK(list_insert(&L, -1, 0) == -1, "idx 为负 -> -1");
    list_destroy(L);
}

static void test_erase(void) {
    printf("[4] list_erase\n");
    Node *L = list_build_tail(5);       /* 1 2 3 4 5 */
    if (!L || L->next == NULL) { CHECK(0, "建表失败，erase 跳过"); list_destroy(L); return; }

    CHECK(list_erase(&L, 1) == 0, "删 idx=1 返回 0");
    int e1[] = {1, 3, 4, 5};
    CHECK(eq(L, e1, 4), "删后 {1,3,4,5}");

    CHECK(list_erase(&L, 0) == 0, "删首元返回 0");
    int e2[] = {3, 4, 5};
    CHECK(eq(L, e2, 3), "删首元后 {3,4,5}");

    CHECK(list_erase(&L, 2) == 0, "删尾返回 0");
    int e3[] = {3, 4};
    CHECK(eq(L, e3, 2), "删尾后 {3,4}");

    CHECK(list_erase(&L, 2) == -1, "idx 超范围 -> -1");
    CHECK(eq(L, e3, 2), "非法 idx 不改表");
    CHECK(list_erase(&L, -1) == -1, "idx 为负 -> -1");

    CHECK(list_erase(&L, 0) == 0 && list_erase(&L, 0) == 0, "删空两个数据节点");
    CHECK(L != NULL && L->next == NULL, "删空后只剩头结点");
    CHECK(list_erase(&L, 0) == -1, "空表再删 -> -1");
    list_destroy(L);
}

static void test_destroy(void) {
    printf("[5] list_destroy\n");
    Node *L = list_build_tail(1000);
    list_destroy(L);
    CHECK(1, "destroy 1000 个节点没崩（valgrind 看头结点和数据节点是否都释放）");
}

int main(void) {
    printf("=== B 关 W1-2：单链表基础（带头结点）===\n\n");
    test_build();
    test_find();
    test_insert();
    test_erase();
    test_destroy();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
