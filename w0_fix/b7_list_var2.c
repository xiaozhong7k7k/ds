/* ============================================================
 * B 关 W2-4：链表变形 ③ 判环 + 环入口  ④ 删倒数第 k 个  ⑤ 有序去重
 *
 * 【表示法：单链表、不带头结点】
 *   head 就是首元节点；head == NULL 表示空表
 *   尾节点的 next == NULL（有环的表除外）
 *   "环"由测试自己造：把某个尾节点的 next 指回表里某个节点
 *
 * 【下标 / 长度】
 *   下面写的 len 只是"当前节点个数"的简写，函数里没有这个参数
 *   k 从 1 开始数：k == 1 是最后一个节点，k == len 是首元节点
 *
 * 【返回值】
 *   has_cycle(head)              ：有环 1，无环 0；空表算无环
 *   cycle_entry(head)            ：返回环的入口节点；无环 / 空表返回 NULL
 *   remove_nth_from_end(head, k) ：返回删除后的新头指针（可能变成 NULL，
 *                                  也可能不再是原来的 head）；k <= 0
 *                                  或 k > len 时一个都不删，原样返回 head
 *   remove_dups_sorted(head)     ：表按非降序排好，每个值只保留第一个；
 *                                  返回新头指针（可能变，也可能不变）
 *
 * 【所有权】
 *   节点全部由测试 malloc；被删掉的节点由你的函数 free
 *   返回的新头指针必须仍然是可遍历的链表，测试拿它继续用
 *
 * 【这四题的共同点，也是唯一要练的东西】
 *   ①③④ 是「快慢指针」：一个走一步、一个走两步
 *   ⑤ 是「在一条链上做删除」，删的时候必须握着**前一个**节点
 *
 * 注意别踩的老坑（你自己在 b3 踩过）：两条指针同时前进的那一行，
 * 相等分支里只让一边走，别两边都走。
 *
 * 规则：只填 4 个函数体，测试不要改。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

/* ---------- 1. 判环 ----------
 * 有环返回 1，无环返回 0
 * 提示：slow 走 1 步、fast 走 2 步；fast 先撞到 NULL 就是无环，
 *       某一刻 slow == fast 就是有环
 */
int has_cycle(Node *head)
{
    (void)head;
    /* TODO */
    return 0;
}

/* ---------- 2. 环入口 ----------
 * 返回环的入口节点；无环 / 空表返回 NULL
 * 提示：先用快慢指针在环内相遇；相遇之后把一个指针放回 head，
 *       两个都改成一次走一步，再相遇的地方就是入口
 */
Node *cycle_entry(Node *head)
{
    (void)head;
    /* TODO */
    return NULL;
}

/* ---------- 3. 删倒数第 k 个 ----------
 * 返回新头指针；k <= 0 或 k > len 时原样返回
 * 提示：fast 先从 head 走 k 步，然后 fast 和 cur 一起走，
 *       fast 撞 NULL 时 cur 就停在倒数第 k 个上
 * 坑：这样走下来 prev 可能是 NULL，说明删的是首元节点，要改 head
 */
Node *remove_nth_from_end(Node *head, int k)
{
    (void)head; (void)k;
    /* TODO */
    return head;
}

/* ---------- 4. 有序链表去重 ----------
 * 非降序链表，每个值只留第一个；返回新头指针
 * 提示：比较 cur 和 cur->next，相等就删掉 cur->next 并接上；
 *       不相等才让 cur 前进
 * 坑：相等分支里 cur 不能前进，否则连着三个相同值会漏
 */
Node *remove_dups_sorted(Node *head)
{
    (void)head;
    /* TODO */
    return head;
}

/* ==================== 测试（不要改） ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

#define MAXN 1000

/* 尾插建表，不带头结点；n=0 返回 NULL */
static Node *make_list(const int *a, int n) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->val = a[i];
        p->next = NULL;
        if (!head) head = p;
        else tail->next = p;
        tail = p;
    }
    return head;
}

/* 第 i 个节点（0 起）；走不到就返回 NULL */
static Node *nth(Node *head, int i) {
    while (i-- > 0 && head) head = head->next;
    return head;
}

/* 值、长度、结尾是 NULL，三样都对才算对；带步数上限防环 */
static int list_eq(Node *head, const int *a, int n) {
    for (int i = 0; i < n; i++) {
        if (!head || head->val != a[i]) return 0;
        head = head->next;
    }
    return head == NULL;
}

/* 测试自己释放；带步数上限，坏表也不会死循环 */
static void free_list(Node *head) {
    int n = 0;
    while (head && n++ < MAXN) {
        Node *nx = head->next;
        free(head);
        head = nx;
    }
}

/* 有环的表先把尾巴切断再释放，否则走不完一圈 */
static void cut_and_free(Node *head, int len) {
    if (len > 0) {
        Node *t = nth(head, len - 1);
        if (t) t->next = NULL;
    }
    free_list(head);
}

static void test_has_cycle(void) {
    printf("[1] has_cycle\n");
    CHECK(has_cycle(NULL) == 0, "空表：无环");

    int a1[] = {1};
    Node *L = make_list(a1, 1);
    CHECK(has_cycle(L) == 0, "单个节点、next 是 NULL：无环");
    free_list(L);

    int a5[] = {1, 2, 3, 4, 5};
    L = make_list(a5, 5);
    CHECK(has_cycle(L) == 0, "5 个节点正常结尾：无环");
    free_list(L);

    int a2[] = {1, 2};
    L = make_list(a2, 2);
    nth(L, 1)->next = L;
    CHECK(has_cycle(L) == 1, "2 个节点，尾指回头：有环");
    cut_and_free(L, 2);

    int a4[] = {1, 2, 3, 4};
    L = make_list(a4, 4);
    nth(L, 3)->next = L;
    CHECK(has_cycle(L) == 1, "4 个节点，尾指回 head（整表成环）：有环");
    cut_and_free(L, 4);

    L = make_list(a5, 5);
    nth(L, 4)->next = nth(L, 1);
    CHECK(has_cycle(L) == 1, "5 个节点，尾指回第 2 个：有环");
    cut_and_free(L, 5);

    L = make_list(a1, 1);
    L->next = L;
    CHECK(has_cycle(L) == 1, "单个节点指向自己：有环");
    cut_and_free(L, 1);
}

static void test_cycle_entry(void) {
    printf("[2] cycle_entry\n");
    CHECK(cycle_entry(NULL) == NULL, "空表：返回 NULL");

    int a5[] = {1, 2, 3, 4, 5};
    Node *L = make_list(a5, 5);
    CHECK(cycle_entry(L) == NULL, "5 个节点正常结尾：返回 NULL");
    free_list(L);

    int a1[] = {1};
    L = make_list(a1, 1);
    CHECK(cycle_entry(L) == NULL, "单个节点 next 是 NULL：返回 NULL");
    free_list(L);

    int a4[] = {1, 2, 3, 4};
    L = make_list(a4, 4);
    nth(L, 3)->next = L;
    CHECK(cycle_entry(L) == L, "整表成环：入口就是 head");
    cut_and_free(L, 4);

    L = make_list(a5, 5);
    Node *e = nth(L, 1);
    nth(L, 4)->next = e;
    CHECK(cycle_entry(L) == e, "入口在第 2 个节点：返回它");
    cut_and_free(L, 5);

    L = make_list(a1, 1);
    L->next = L;
    CHECK(cycle_entry(L) == L, "单节点自环：入口是它自己");
    cut_and_free(L, 1);

    L = make_list(a4, 4);
    Node *t = nth(L, 3);
    t->next = t;
    CHECK(cycle_entry(L) == t, "入口是尾节点（尾巴指向自己）：返回尾节点");
    cut_and_free(L, 4);
}

static void test_remove_nth(void) {
    printf("[3] remove_nth_from_end\n");
    CHECK(remove_nth_from_end(NULL, 1) == NULL, "空表 k=1：返回 NULL");

    int a1[] = {1};
    Node *L = make_list(a1, 1);
    CHECK(remove_nth_from_end(L, 1) == NULL, "只有一个节点 k=1：删空，返回 NULL");

    L = make_list(a1, 1);
    CHECK(list_eq(remove_nth_from_end(L, 2), a1, 1), "只有一个节点 k=2（>len）：原样返回");
    free_list(L);

    int a5[] = {1, 2, 3, 4, 5};
    int e1[] = {1, 2, 3, 4};
    L = make_list(a5, 5);
    Node *R = remove_nth_from_end(L, 1);
    CHECK(list_eq(R, e1, 4), "1,2,3,4,5 k=1：删尾，得到 1,2,3,4");
    free_list(R);

    int e2[] = {2, 3, 4, 5};
    L = make_list(a5, 5);
    R = remove_nth_from_end(L, 5);
    CHECK(list_eq(R, e2, 4), "k=5（= len）：删头，得到 2,3,4,5");
    free_list(R);

    int e3[] = {1, 2, 4, 5};
    L = make_list(a5, 5);
    R = remove_nth_from_end(L, 3);
    CHECK(list_eq(R, e3, 4), "k=3：删中间的 3，得到 1,2,4,5");
    free_list(R);

    L = make_list(a5, 5);
    R = remove_nth_from_end(L, 6);
    CHECK(list_eq(R, a5, 5), "k=6（> len）：一个都不删");
    free_list(R);

    L = make_list(a5, 5);
    R = remove_nth_from_end(L, 0);
    CHECK(list_eq(R, a5, 5), "k=0：非法，原样返回");
    free_list(R);

    int a2[] = {1, 2};
    int e4[] = {2};
    L = make_list(a2, 2);
    R = remove_nth_from_end(L, 2);
    CHECK(list_eq(R, e4, 1), "1,2 k=2：删头，只剩 2");
    free_list(R);

    int e5[] = {1};
    L = make_list(a2, 2);
    R = remove_nth_from_end(L, 1);
    CHECK(list_eq(R, e5, 1) && R->next == NULL,
          "1,2 k=1：删尾，剩 1 且它的 next 是 NULL");
    free_list(R);
}

static void test_remove_dups(void) {
    printf("[4] remove_dups_sorted\n");
    CHECK(remove_dups_sorted(NULL) == NULL, "空表：返回 NULL");

    int a1[] = {1};
    Node *L = make_list(a1, 1);
    CHECK(list_eq(remove_dups_sorted(L), a1, 1), "单个节点：原样");
    free_list(L);

    int a7[] = {1, 1, 2, 3, 3, 3, 4};
    int e1[] = {1, 2, 3, 4};
    L = make_list(a7, 7);
    CHECK(list_eq(remove_dups_sorted(L), e1, 4), "1,1,2,3,3,3,4 → 1,2,3,4");
    free_list(L);

    int a3[] = {1, 1, 1};
    int e2[] = {1};
    L = make_list(a3, 3);
    CHECK(list_eq(remove_dups_sorted(L), e2, 1), "1,1,1 → 1（连着三个不能漏）");
    free_list(L);

    int a6[] = {1, 2, 3};
    L = make_list(a6, 3);
    CHECK(list_eq(remove_dups_sorted(L), a6, 3), "没有重复：一个都不动");
    free_list(L);

    int a4[] = {1, 1, 2, 2};
    int e3[] = {1, 2};
    L = make_list(a4, 4);
    CHECK(list_eq(remove_dups_sorted(L), e3, 2), "1,1,2,2 → 1,2");
    free_list(L);

    int a2[] = {1, 2, 2};
    int e4[] = {1, 2};
    L = make_list(a2, 3);
    CHECK(list_eq(remove_dups_sorted(L), e4, 2), "重复在尾部 1,2,2 → 1,2");
    free_list(L);

    int a5[] = {1, 1, 2};
    int e5[] = {1, 2};
    L = make_list(a5, 3);
    CHECK(list_eq(remove_dups_sorted(L), e5, 2), "重复在头部 1,1,2 → 1,2");
    free_list(L);

    int a8[] = {2, 2, 3, 3, 3};
    int e6[] = {2, 3};
    L = make_list(a8, 5);
    CHECK(list_eq(remove_dups_sorted(L), e6, 2), "2,2,3,3,3 → 2,3");
    free_list(L);
}

int main(void) {
    printf("=== B 关 W2-4：链表变形 ③判环 ④删倒数第 k ⑤有序去重 ===\n\n");
    test_has_cycle();
    test_cycle_entry();
    test_remove_nth();
    test_remove_dups();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
