/* ============================================================
 * B 关 W2-3：双向循环链表（带头结点）—— 插入 / 删除
 *
 * 【表示法：带头结点、双向循环】
 *   head 是头结点（哨兵），不存数据
 *   空表：head->next == head 且 head->prev == head
 *   尾节点的 next 指向 head；head->prev 指向尾节点
 *   每个节点都有 prev / next，并且必须"对得上"：
 *       任一节点 p 满足 p->next->prev == p
 *   测试会从两头各走一遍，有一边没接上就算错
 *
 *   注意：这张表既没有 NULL 结尾，也没有"尾节点的 next 是 NULL"，
 *         所有"到头了"的判断都是"回到 head"
 *
 * 【下标范围】
 *   下面写的 len 只是"当前数据节点个数"的简写，函数里没有这个参数，
 *   也不会传进来；判断越界只能靠走指针：
 *     insert：从 head 出发向后走 idx 步，半路走回 head = idx 太大
 *     erase ：从 head 出发向后走 idx 步，停在要删节点的前一个；
 *             它的 next 是 head 说明 idx 太大
 *   dc_insert(head, idx, val)：0 <= idx <= len，idx == len 表示尾插
 *   dc_erase (head, idx, out)：0 <= idx <  len, idx == len 表示越界
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
 * 【这次和 b4 / b5 的唯一区别，也是唯一难点】
 *   插入要改 4 根指针，删除要改 2 根，顺序错了就断链或者死循环：
 *
 *     插入（在 prev 后面插 p）：        删除（删 cur）：
 *       p->prev = prev;                   prev->next = cur->next;
 *       p->next = prev->next;             cur->next->prev = prev;
 *       prev->next->prev = p;             free(cur);
 *       prev->next = p;
 *
 *   记法：新节点的两根先接好；老节点的两根后动；
 *         老节点里"后面那根"(next->prev) 先改，"前面那根"(prev->next) 最后改。
 *
 * 规则：只填 4 个函数体，测试不要改。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct DCNode {
    int val;
    struct DCNode *prev;
    struct DCNode *next;
} DCNode;

/* ---------- 1. 插入 ----------
 * 在 idx 位置插入 val（idx == len 时尾插）
 * 思路：从 head 走 idx 步找到"插在谁后面"；走回 head 说明 idx 太大。
 *       走 idx == len 步时正好停在尾节点，所以尾插不需要特判。
 * 注意：这个函数要改 4 根指针，顺序见文件头
 */
int dc_insert(DCNode *head, int idx, int val)
{
    if(idx < 0){ return -1; }
    DCNode *prev = head;
    for(int i = 0 ; i<idx ; i++){
        prev = prev->next;
        if(prev == head){ return -1; }
    }

    DCNode *p = (DCNode*)malloc(sizeof(DCNode));
    if(p == NULL){ return -1; }

    p->val = val;
    p->next = prev->next;
    p->prev = prev;
    prev->next->prev = p;
    prev->next = p;
    
    return 0;
}

/* ---------- 2. 删除 ----------
 * 删除 idx 位置的节点，值通过 *out 带回（out 可为 NULL）
 * 注意：被删节点两边都要重新接上；删完别忘了 free
 *       删的是尾节点时，它的 next 是 head，照样成立，不用特判
 */
int dc_erase(DCNode *head, int idx, int *out)
{
    if(idx < 0){ return -1; }

    DCNode *prev = head;
    for(int i = 0; i<idx ; i++){
        prev = prev->next;
        if(prev == head){ return -1; }
    }

    DCNode *cur = prev->next;
    if(cur == head){ return -1; }

    if(out != NULL){ *out = cur->val; }

    prev->next = cur->next;
    cur->next->prev = prev;
    free(cur);
    cur = NULL;

    return 0;
}

/* ---------- 3. 查找 ----------
 * 返回第一个值为 val 的节点；找不到 / 空表返回 NULL
 * 注意：结束条件是"回到 head"，不是 NULL
 */
DCNode *dc_find(DCNode *head, int val)
{
    // DCNode *prev = head;
    // while(prev->next != head && prev->next->val != val){
    //     prev = prev->next;
    // }

    // DCNode *cur = prev->next;
    // if(cur == head){ return NULL; }


    //减少冗余，直接从head->next开始起步就好
    DCNode *cur = head->next;
    while(cur != head && cur->val != val){
        cur = cur->next;
    }

    if(cur == head){ return NULL; }

    return cur;
}

/* ---------- 4. 销毁 ----------
 * 释放所有节点，包括头结点
 * 注意：一圈走完要停，别绕圈跑；先存 next 再 free
 */
void dc_destroy(DCNode *head)
{
    DCNode *cur = head->next;
    while(cur != head){
        DCNode *next = cur->next;
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

/* 测试自己建表：带头结点、双向循环；n=0 只有头结点 */
static DCNode *make_dclist(const int *a, int n) {
    DCNode *head = (DCNode *)malloc(sizeof(DCNode));
    head->val = 0;
    head->prev = head;
    head->next = head;
    DCNode *tail = head;
    for (int i = 0; i < n; i++) {
        DCNode *p = (DCNode *)malloc(sizeof(DCNode));
        p->val = a[i];
        p->prev = tail;
        p->next = head;
        tail->next = p;
        head->prev = p;
        tail = p;
    }
    return head;
}

/* 正反两个方向都要对得上：
 *   正向走一圈必须正好回到 head，中间不能出现 NULL，也不能绕不完
 *   每一步 p->next->prev == p
 *   最后 head->prev 必须指回尾节点
 */
static int links_ok(DCNode *head) {
    if (!head) return 0;
    DCNode *p = head->next, *back = head;
    int n = 0;
    while (p != head) {
        if (!p) return 0;
        if (p->prev != back) return 0;
        back = p;
        if (++n > MAXN) return 0;
        p = p->next;
    }
    if (head->prev != back) return 0;
    return 1;
}

static int data_ok(DCNode *head, const int *a, int n) {
    DCNode *cur = head ? head->next : NULL;
    for (int i = 0; i < n; i++) {
        if (!cur || cur == head || cur->val != a[i]) return 0;
        cur = cur->next;
    }
    return cur == head;
}

static int ok(DCNode *head, const int *a, int n) {
    return links_ok(head) && data_ok(head, a, n);
}

/* 测试自己释放；带步数上限，坏表也不会死循环 */
static void free_all(DCNode *head) {
    if (!head) return;
    DCNode *p = head->next;
    int n = 0;
    while (p != head && p && n++ < MAXN) {
        DCNode *next = p->next;
        free(p);
        p = next;
    }
    free(head);
}

static void test_insert(void) {
    printf("[1] dc_insert\n");
    DCNode *L = make_dclist(NULL, 0);
    int e1[] = {1};
    CHECK(dc_insert(L, 0, 1) == 0, "空表 idx=0：返回 0");
    CHECK(ok(L, e1, 1) && L->next->prev == L && L->next->next == L,
          "空表插入后：表 = 1，且 1 的 prev / next 都指回 head");

    int e2[] = {1, 2};
    CHECK(dc_insert(L, 1, 2) == 0, "尾插 idx=1（len）：返回 0");
    CHECK(ok(L, e2, 2) && L->prev->val == 2 && L->prev->prev->val == 1,
          "尾插后：表 = 1,2；head->prev 指到新的尾节点 2");

    int e3[] = {0, 1, 2};
    CHECK(dc_insert(L, 0, 0) == 0, "头插 idx=0：返回 0");
    CHECK(ok(L, e3, 3) && L->next->val == 0 && L->next->prev == L,
          "头插后：表 = 0,1,2；新首元节点的 prev 是 head");

    int e4[] = {0, 1, 9, 2};
    CHECK(dc_insert(L, 2, 9) == 0, "中间插 idx=2：返回 0");
    CHECK(ok(L, e4, 4), "中间插入后：表 = 0,1,9,2，正反两个方向都一致");

    CHECK(dc_insert(L, 5, 99) == -1, "idx=5 > len=4：返回 -1");
    CHECK(dc_insert(L, -1, 99) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e4, 4), "越界插入后：表没有被改动，也没有断链");
    free_all(L);
}

static void test_erase(void) {
    printf("[2] dc_erase\n");
    int a[] = {10, 20, 30, 40};
    DCNode *L = make_dclist(a, 4);
    int out = -1;

    int e1[] = {20, 30, 40};
    CHECK(dc_erase(L, 0, &out) == 0 && out == 10, "删头 idx=0：返回 0，*out = 10");
    CHECK(ok(L, e1, 3) && L->next->prev == L,
          "删头后：表 = 20,30,40；新首元节点的 prev 是 head");

    int e2[] = {20, 30};
    out = -1;
    CHECK(dc_erase(L, 2, &out) == 0 && out == 40, "删尾 idx=2：返回 0，*out = 40");
    CHECK(ok(L, e2, 2) && L->prev->val == 30,
          "删尾后：表 = 20,30；head->prev 回到 30");

    int e3[] = {20};
    out = -1;
    CHECK(dc_erase(L, 1, &out) == 0 && out == 30, "再删尾（只剩 2 个数据节点）");
    CHECK(ok(L, e3, 1), "删后：表 = 20");

    CHECK(dc_erase(L, 1, &out) == -1, "idx=1 = len：返回 -1");
    CHECK(dc_erase(L, -1, &out) == -1, "idx=-1：返回 -1");
    CHECK(ok(L, e3, 1), "越界删除后：表没有被改动");

    out = -1;
    CHECK(dc_erase(L, 0, &out) == 0 && out == 20, "删掉最后一个数据节点");
    CHECK(ok(L, NULL, 0) && L->next == L && L->prev == L,
          "删空后：head->next 和 head->prev 都是 head");
    CHECK(dc_erase(L, 0, &out) == -1, "空表删除：返回 -1");
    free_all(L);

    int b[] = {7, 8};
    L = make_dclist(b, 2);
    int e5[] = {8};
    CHECK(dc_erase(L, 0, NULL) == 0, "out 传 NULL：只删不取值，返回 0");
    CHECK(ok(L, e5, 1), "out 传 NULL 删除后：表 = 8");
    free_all(L);
}

static void test_find(void) {
    printf("[3] dc_find\n");
    int v[] = {5, 7, 7, 9};
    DCNode *L = make_dclist(v, 4);

    DCNode *p = dc_find(L, 7);
    CHECK(p != NULL && p->val == 7, "找 7：找到节点");
    CHECK(p != NULL && p == L->next->next, "有重复时返回第一个 7（第 2 个节点）");

    p = dc_find(L, 5);
    CHECK(p != NULL && p == L->next, "找 5：返回首元节点");

    p = dc_find(L, 9);
    CHECK(p != NULL && p->next == L, "找 9：返回尾节点，它的 next 是 head");

    CHECK(dc_find(L, 1) == NULL, "找 1：返回 NULL");

    DCNode *E = make_dclist(NULL, 0);
    CHECK(dc_find(E, 7) == NULL, "空表：返回 NULL（不会绕圈跑）");
    free_all(E);
    free_all(L);
}

static void test_destroy(void) {
    printf("[4] dc_destroy\n");
    int a[] = {1, 2, 3};
    DCNode *L = make_dclist(a, 3);
    dc_destroy(L);
    CHECK(1, "3 个数据节点 + 头结点全部释放（看 valgrind，而且不能卡死）");

    DCNode *E = make_dclist(NULL, 0);
    dc_destroy(E);
    CHECK(1, "只有头结点的空表也能释放");
}

int main(void) {
    printf("=== B 关 W2-3：双向循环链表 插入 / 删除 ===\n\n");
    test_insert();
    test_erase();
    test_find();
    test_destroy();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
