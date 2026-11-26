/* ============================================================
 * B 关 W2-2：循环单链表（带头结点）—— 插入 / 删除
 *
 * 【表示法：带头结点、单向循环】
 *   head 是头结点（哨兵），不存数据
 *   head->next 是首元节点；head->next == head 表示空表
 *   尾节点的 next 指向 head（不是 NULL）
 *   所有函数都假设传入的 head 非 NULL
 *
 *   注意：这张表没有 NULL 结尾，遍历时"走回 head"就是走完一圈
 *
 * 【下标范围】
 *   下面写的 len 只是"当前数据节点个数"的简写，函数里没有这个参数：
 *     insert：从 head 出发向后走 idx 步，中途走回 head = idx 太大
 *     erase ：从 head 出发向后走 idx 步，停在第 idx 个节点的前一个；
 *             它的 next 是 head 说明 idx 太大
 *   clist_insert(head, idx, val)：0 <= idx <= len，idx == len 表示尾插
 *   clist_erase (head, idx, out)：0 <= idx <  len
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

typedef struct CNode {
    int val;
    struct CNode *next;
} CNode;

/* ---------- 1. 插入 ----------
 * 在 idx 位置插入 val（idx == len 时尾插）
 */
int clist_insert(CNode *head, int idx, int val)
{
    if(idx<0){ return -1; }
    // CNode *prev = (CNode*)malloc(sizeof(CNode));
    // prev = head;
    //这么写，每次都会泄露一个CNode的内存大小，每次插入应该都是用普通指针遍历，找到目标之后再malloc一个内存写值插入

    // for(int i = 0 ; i<idx && prev->next != head ; i++){
    //     prev = prev->next;
    // }
    //if(prev->next == head){ return -1; }
    //这么写循环条件，就无法进行尾插，直接把if写进循环里

    CNode *prev = head;
    for(int i = 0 ; i<idx ; i++){
        prev = prev->next;
        if(prev == head){ return -1; }
    }

    CNode *p = (CNode*)malloc(sizeof(CNode));
    if(p == NULL){ return -1; }

    p->val = val;
    // prev->next = prev;
    // p->next = prev->next->next;
    //这么写就逻辑混乱了，p的next指向了p自己
    p->next = prev->next;
    prev->next = p;

    return 0;
}

/* ---------- 2. 删除 ----------
 * 删除 idx 位置的节点，值通过 *out 带回（out 可为 NULL）
 * 注意：尾节点的 next 是 head，不是 NULL
 */
int clist_erase(CNode *head, int idx, int *out)//idx的合理范围应该是 0 <= idx <len
{
    if(idx<0){ return -1; }
    // CNode *dele_prev = head;//等于头节点还是首元节点只是因边界（如尾插）而异，循环的条件不会因此改变

    // for(int i = 0; i<idx && prev->next->next != head; i++){
    //     dele_prev = dele_prev->next;        //没找到的退出条件不一定要写到循环条件里，还可以在循环里开个if
    // }

    // if(out != NULL){ *out = dele_prev->next->val; }
    // free(dele_prev->next);
    // dele_prev->next = dele_prev->next->next;
    // dele_prev = NULL;

    CNode *prev = head;

    for(int i =0 ; i<idx ; i++){
        prev = prev->next;
        if(prev == head){ return -1; }//没有考虑到idx等于len的情况，这个特殊点导致测试出错了
    }

    CNode *cur = prev->next;
    if(cur == head){ return -1; }//如果这是idx等于len的情况，我就是读头节点的值，再删除头节点，链表就找不到了
    if(out != NULL){ *out = cur->val; }

    prev->next = cur->next;
    free(cur);
    return 0;
}

/* ---------- 3. 查找 ----------
 * 返回第一个值为 val 的节点；找不到 / 空表返回 NULL
 */
CNode *clist_find(CNode *head, int val)
{
    if(head->next == head){ return NULL; }
    CNode *find = head->next;

    while(find != head && find->val != val ){ find = find->next; }

    if(find == head){ return NULL; }
    return find;
}

/* ---------- 4. 销毁 ----------
 * 释放所有节点，包括头结点
 * 注意：一圈走完要停，别绕圈跑
 */
void clist_destroy(CNode *head)
{
    CNode *cur = head->next;
    while(cur != head){
        CNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(head);
}

/* ==================== 测试（不要改） ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

#define MAXN 1000

/* 测试自己建表：带头结点、单向循环；n=0 只有头结点 */
static CNode *make_clist(const int *a, int n) {
    CNode *head = (CNode*)malloc(sizeof(CNode));
    head->val = 0;
    head->next = head;
    CNode *tail = head;
    for (int i = 0; i < n; i++) {
        CNode *p = (CNode*)malloc(sizeof(CNode));
        p->val = a[i];
        p->next = head;
        tail->next = p;
        tail = p;
    }
    return head;
}

/* 走一圈：必须正好回到 head，中间不能出现 NULL，也不能绕不完 */
static int links_ok(CNode *head) {
    if (!head) return 0;
    CNode *p = head->next;
    int n = 0;
    while (p != head) {
        if (!p) return 0;
        if (++n > MAXN) return 0;
        p = p->next;
    }
    return 1;
}

static int data_ok(CNode *head, const int *a, int n) {
    CNode *cur = head ? head->next : NULL;
    for (int i = 0; i < n; i++) {
        if (!cur || cur == head || cur->val != a[i]) return 0;
        cur = cur->next;
    }
    return cur == head;
}

static int ok(CNode *head, const int *a, int n) {
    return links_ok(head) && data_ok(head, a, n);
}

/* 测试自己释放；带步数上限，坏表也不会死循环 */
static void free_all(CNode *head) {
    if (!head) return;
    CNode *p = head->next;
    int n = 0;
    while (p != head && p && n++ < MAXN) {
        CNode *next = p->next;
        free(p);
        p = next;
    }
    free(head);
}

static void test_insert(void) {
    printf("[1] clist_insert\n");
    CNode *L = make_clist(NULL, 0);
    int e1[] = {1};
    CHECK(clist_insert(L, 0, 1) == 0, "空表 idx=0：返回 0");
    CHECK(ok(L, e1, 1), "空表插入后：表 = 1，且 1->next 指回 head");

    int e2[] = {1, 2};
    CHECK(clist_insert(L, 1, 2) == 0, "尾插 idx=1（len）：返回 0");
    CHECK(ok(L, e2, 2), "尾插后：表 = 1,2，尾节点 2->next 是 head 不是 NULL");

    int e3[] = {0, 1, 2};
    CHECK(clist_insert(L, 0, 0) == 0, "头插 idx=0：返回 0");
    CHECK(ok(L, e3, 3), "头插后：表 = 0,1,2");

    int e4[] = {0, 1, 9, 2};
    CHECK(clist_insert(L, 2, 9) == 0, "中间插 idx=2：返回 0");
    CHECK(ok(L, e4, 4), "中间插入后：表 = 0,1,9,2");

    CHECK(clist_insert(L, 5, 99) == -1, "idx=5 > len=4：返回 -1");
    CHECK(clist_insert(L, -1, 99) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e4, 4), "越界插入后：表没有被改动，也没有断圈");
    free_all(L);
}

static void test_erase(void) {
    printf("[2] clist_erase\n");
    int a[] = {10, 20, 30, 40};
    CNode *L = make_clist(a, 4);
    int out = -1;

    int e1[] = {20, 30, 40};
    CHECK(clist_erase(L, 0, &out) == 0 && out == 10, "删头 idx=0：返回 0，*out = 10");
    CHECK(ok(L, e1, 3), "删头后：表 = 20,30,40，圈还是连着的");

    int e2[] = {20, 30};
    out = -1;
    CHECK(clist_erase(L, 2, &out) == 0 && out == 40, "删尾 idx=2：返回 0，*out = 40（尾节点 next 是 head）");
    CHECK(ok(L, e2, 2), "删尾后：表 = 20,30，尾节点 30->next 是 head");

    int e3[] = {20};
    out = -1;
    CHECK(clist_erase(L, 1, &out) == 0 && out == 30, "再删尾（只剩 2 个数据节点）");
    CHECK(ok(L, e3, 1), "删后：表 = 20");

    CHECK(clist_erase(L, 1, &out) == -1, "idx=1 = len：返回 -1");
    CHECK(clist_erase(L, -1, &out) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e3, 1), "越界删除后：表没有被改动");

    out = -1;
    CHECK(clist_erase(L, 0, &out) == 0 && out == 20, "删掉最后一个数据节点");
    CHECK(ok(L, NULL, 0), "删空后：head->next == head");
    CHECK(clist_erase(L, 0, &out) == -1, "空表删除：返回 -1");
    free_all(L);

    int b[] = {7, 8};
    L = make_clist(b, 2);
    int e5[] = {8};
    CHECK(clist_erase(L, 0, NULL) == 0, "out 传 NULL：只删不取值，返回 0");
    CHECK(ok(L, e5, 1), "out 传 NULL 删除后：表 = 8");
    free_all(L);
}

static void test_find(void) {
    printf("[3] clist_find\n");
    int v[] = {5, 7, 7, 9};
    CNode *L = make_clist(v, 4);

    CNode *p = clist_find(L, 7);
    CHECK(p != NULL && p->val == 7, "找 7：找到节点");
    CHECK(p != NULL && p == L->next->next, "有重复时返回第一个 7（第 2 个节点）");

    p = clist_find(L, 5);
    CHECK(p != NULL && p == L->next, "找 5：返回首元节点");

    p = clist_find(L, 9);
    CHECK(p != NULL && p->next == L, "找 9：返回尾节点，它的 next 是 head");

    CHECK(clist_find(L, 1) == NULL, "找 1：返回 NULL");

    CNode *E = make_clist(NULL, 0);
    CHECK(clist_find(E, 7) == NULL, "空表：返回 NULL（不会绕圈跑）");
    free_all(E);
    free_all(L);
}

static void test_destroy(void) {
    printf("[4] clist_destroy\n");
    int a[] = {1, 2, 3};
    CNode *L = make_clist(a, 3);
    clist_destroy(L);
    CHECK(1, "3 个数据节点 + 头结点全部释放（看 valgrind，而且不能卡死）");

    CNode *E = make_clist(NULL, 0);
    clist_destroy(E);
    CHECK(1, "只有头结点的空表也能释放");
}

int main(void) {
    printf("=== B 关 W2-2：循环单链表 插入 / 删除 ===\n\n");
    test_insert();
    test_erase();
    test_find();
    test_destroy();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
