/* ============================================================
 * B 关 W2-1：双链表（带头结点、非循环）—— 插入 / 删除
 *
 * 【表示法：带头结点、非循环】
 *   head 是头结点（哨兵），不存数据，head->prev == NULL
 *   head->next 是首元节点；head->next == NULL 表示空表
 *   尾节点的 next == NULL
 *   所有函数都假设传入的 head 非 NULL
 *   每个节点必须 prev / next 双向一致（测试会从两头各走一遍）
 *
 * 【下标范围】
 *   下面写的 len 只是"当前数据节点个数"的简写，**函数里没有这个参数**，
 *   也不会传进来；判断越界只能靠走指针：
 *     insert：从 head 出发向后走 idx 步，半路 prev 变成 NULL = idx 太大
 *     erase ：从 head->next 出发向后走 idx 步，走到 NULL  = idx 太大
 *   dlist_insert(head, idx, val)：0 <= idx <= len，idx == len 表示尾插
 *   dlist_erase (head, idx, out)：0 <= idx <  len
 *
 * 【返回值】
 *   insert：成功 0；idx 越界或 malloc 失败 -1
 *   erase ：成功 0；idx 越界 -1
 *           被删的值写入 *out（out 可以为 NULL，表示不关心）
 *   find  ：找到返回节点地址；找不到或空表返回 NULL；有重复返回第一个
 *   destroy：void，连头结点一起释放
 *
 * 【所有权】
 *   节点由 insert malloc；由 erase / destroy free
 *   测试自己建表、自己释放，不依赖你的 destroy（destroy 单独测）
 *
 * 规则：只填 4 个函数体，测试不要改。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {
    int val;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* ---------- 1. 插入 ----------
 * 在 idx 位置插入 val（idx == len 时尾插）
 * 思路提醒：新节点的 prev / next 先接好，再动老节点
 */
int dlist_insert(DNode *head, int idx, int val)
{
    // DNode *temp = head->next;
    //temp->prev = head;

    // for(int i = 0; i<idx && temp != NULL; i++){
    //         temp = temp->next;
    // }

    // if(temp == NULL){ return -1; }
    //错误原因 ( 边界条件 )：temp 从首元节点开始走，把「第 idx 个节点」当成插入点。当 idx == len 时，
    //这个节点并不存在，于是空表和尾插都被当成越界，直接 return -1
    if(idx<0){ return -1; }
    DNode *prev = head;

    for(int i = 0; i<idx && prev != NULL; i++){
         prev = prev->next;
    }

    if(prev == NULL){ return -1; }

    DNode *p = (DNode*)malloc(sizeof(DNode));
    if(p == NULL){ return -1; }

    p->val = val;
    p->prev = prev;
    p->next = prev->next;
    if (prev->next != NULL) { prev->next->prev = p; }
    prev->next = p;

    // p->val = val;
    // p->prev = temp->prev;
    // p->next = temp;
    // temp->prev->next = p;
    // temp->prev = p;

    return 0;
}

/* ---------- 2. 删除 ----------
 * 删除 idx 位置的节点，值通过 *out 带回（out 可为 NULL）
 * 注意：被删的可能是尾节点，它的 next 是 NULL
 */
int dlist_erase(DNode *head, int idx, int *out)
{
    if(idx<0){ return -1; }
    DNode *cur = head->next;
    //cur->prev = head; 不需要

    for(int i = 0; i<idx && cur != NULL; i++){
            cur = cur->next;
    }

    if(cur == NULL){ return -1; }

    if(out != NULL){ *out = cur->val; }
    //*out = cur->val;
    if(cur->next == NULL){ 
        cur->prev->next = NULL;
        free(cur);
    }else{
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        free(cur);
    }

    return 0;
}

/* ---------- 3. 查找 ----------
 * 返回第一个值为 val 的节点；找不到 / 空表返回 NULL
 */
DNode *dlist_find(DNode *head, int val)
{
    //DNode *cur = (DNode*)malloc(sizeof(DNode));
    //if(cur == NULL){ return ?}
    DNode *cur = head->next;
    //cur->prev = head;
    while(cur != NULL && cur->val != val){
            cur = cur->next;
    }

    if(cur == NULL){ return NULL; }
    else{ return cur; }
}

/* ---------- 4. 销毁 ----------
 * 释放所有节点，包括头结点
 */
void dlist_destroy(DNode *head)
{
    // DNode *cur = head->next;
    // cur->prev = head; 不需要

    // if(cur == NULL){ free(head); return ;}
    // while(cur != NULL && cur->next != NULL){
    //     free(cur->prev);
    //     cur = cur->next;
    // }
    // free(cur);
    while(head != NULL){
        DNode *next = head->next;
        free(head);
        head = next;
    }
}

/* ==================== 测试（不要改） ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

#define MAXN 1000

/* 测试自己建表：带头结点，数据来自数组；n=0 只有头结点 */
static DNode *make_dlist(const int *a, int n) {
    DNode *head = (DNode*)malloc(sizeof(DNode));
    head->val = 0;
    head->prev = NULL;
    head->next = NULL;
    DNode *tail = head;
    for (int i = 0; i < n; i++) {
        DNode *p = (DNode*)malloc(sizeof(DNode));
        p->val = a[i];
        p->prev = tail;
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return head;
}

/* 从两头各走一遍，验证 prev / next 双向一致 */
static int links_ok(DNode *head) {
    if (!head) return 0;
    if (head->prev != NULL) return 0;
    DNode *p = head->next, *prev = head;
    int n = 0;
    while (p) {
        if (++n > MAXN) return 0;          /* 成环了 */
        if (p->prev != prev) return 0;     /* 正向走时 prev 指错 */
        prev = p;
        p = p->next;
    }
    DNode *q = prev;                       /* 从尾节点倒着走回 head */
    int m = 0;
    while (q && q != head) {
        if (++m > MAXN) return 0;
        q = q->prev;
    }
    return q == head;
}

static int data_ok(DNode *head, const int *a, int n) {
    DNode *cur = head ? head->next : NULL;
    for (int i = 0; i < n; i++) {
        if (!cur || cur->val != a[i]) return 0;
        cur = cur->next;
    }
    return cur == NULL;
}

/* 结构 + 数据一起验 */
static int ok(DNode *head, const int *a, int n) {
    return links_ok(head) && data_ok(head, a, n);
}

/* 测试自己释放，不依赖学生写的 destroy；带步数上限，坏表也不会死循环 */
static void free_all(DNode *head) {
    int n = 0;
    while (head && n++ < MAXN) {
        DNode *next = head->next;
        free(head);
        head = next;
    }
}

static void test_insert(void) {
    printf("[1] dlist_insert\n");
    DNode *L = make_dlist(NULL, 0);
    int e1[] = {1};
    CHECK(dlist_insert(L, 0, 1) == 0, "空表 idx=0：返回 0");
    CHECK(ok(L, e1, 1), "空表插入后：表 = 1，prev/next 双向都接对");

    int e2[] = {1, 2};
    CHECK(dlist_insert(L, 1, 2) == 0, "尾插 idx=1（len）：返回 0");
    CHECK(ok(L, e2, 2), "尾插后：表 = 1,2，尾节点 next 是 NULL");

    int e3[] = {0, 1, 2};
    CHECK(dlist_insert(L, 0, 0) == 0, "头插 idx=0：返回 0");
    CHECK(ok(L, e3, 3), "头插后：表 = 0,1,2，head->next->prev == head");

    int e4[] = {0, 1, 9, 2};
    CHECK(dlist_insert(L, 2, 9) == 0, "中间插 idx=2：返回 0");
    CHECK(ok(L, e4, 4), "中间插入后：表 = 0,1,9,2，9 的 prev 指向 1");

    CHECK(dlist_insert(L, 5, 99) == -1, "idx=5 > len=4：返回 -1");
    CHECK(dlist_insert(L, -1, 99) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e4, 4), "越界插入后：表没有被改动");
    free_all(L);
}

static void test_erase(void) {
    printf("[2] dlist_erase\n");
    int a[] = {10, 20, 30, 40};
    DNode *L = make_dlist(a, 4);
    int out = -1;

    int e1[] = {20, 30, 40};
    CHECK(dlist_erase(L, 0, &out) == 0 && out == 10, "删头 idx=0：返回 0，*out = 10");
    CHECK(ok(L, e1, 3), "删头后：表 = 20,30,40，新首元的 prev 是 head");

    int e2[] = {20, 30};
    out = -1;
    CHECK(dlist_erase(L, 2, &out) == 0 && out == 40, "删尾 idx=2：返回 0，*out = 40（尾节点 next 为 NULL）");
    CHECK(ok(L, e2, 2), "删尾后：表 = 20,30，新尾节点 next 是 NULL");

    int e3[] = {20};
    out = -1;
    CHECK(dlist_erase(L, 1, &out) == 0 && out == 30, "再删尾（只剩 2 个数据节点）");
    CHECK(ok(L, e3, 1), "删后：表 = 20");

    CHECK(dlist_erase(L, 1, &out) == -1, "idx=1 = len：返回 -1");
    CHECK(dlist_erase(L, -1, &out) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e3, 1), "越界删除后：表没有被改动");

    out = -1;
    CHECK(dlist_erase(L, 0, &out) == 0 && out == 20, "删掉最后一个数据节点");
    CHECK(ok(L, NULL, 0), "删空后：head->next == NULL");
    CHECK(dlist_erase(L, 0, &out) == -1, "空表删除：返回 -1");
    free_all(L);

    /* out 传 NULL 也要能用 */
    int b[] = {7, 8};
    L = make_dlist(b, 2);
    int e5[] = {8};
    CHECK(dlist_erase(L, 0, NULL) == 0, "out 传 NULL：只删不取值，返回 0");
    CHECK(ok(L, e5, 1), "out 传 NULL 删除后：表 = 8");
    free_all(L);
}

static void test_find(void) {
    printf("[3] dlist_find\n");
    int v[] = {5, 7, 7, 9};
    DNode *L = make_dlist(v, 4);

    DNode *p = dlist_find(L, 7);
    CHECK(p != NULL && p->val == 7, "找 7：找到节点");
    CHECK(p != NULL && p->prev != NULL && p->prev->val == 5, "有重复时返回第一个 7（它的 prev 是 5）");

    p = dlist_find(L, 5);
    CHECK(p != NULL && p->prev == L, "找 5：返回首元节点，prev 是 head");

    p = dlist_find(L, 9);
    CHECK(p != NULL && p->next == NULL, "找 9：返回尾节点");

    CHECK(dlist_find(L, 1) == NULL, "找 1：返回 NULL");

    DNode *E = make_dlist(NULL, 0);
    CHECK(dlist_find(E, 7) == NULL, "空表：返回 NULL");
    free_all(E);
    free_all(L);
}

static void test_destroy(void) {
    printf("[4] dlist_destroy\n");
    int a[] = {1, 2, 3};
    DNode *L = make_dlist(a, 3);
    dlist_destroy(L);
    CHECK(1, "3 个数据节点 + 头结点全部释放（看 valgrind）");

    DNode *E = make_dlist(NULL, 0);
    dlist_destroy(E);
    CHECK(1, "只有头结点的空表也能释放（看 valgrind）");
}

int main(void) {
    printf("=== B 关 W2-1：双链表 插入 / 删除 ===\n\n");
    test_insert();
    test_erase();
    test_find();
    test_destroy();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
