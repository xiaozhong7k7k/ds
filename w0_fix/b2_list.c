/* ============================================================
 * B 关 W1-2：单链表基础（尾插 / 头插 / 查找 / 插入 / 删除）
 * 规则：只填 8 个函数体，main 和测试部分不要改。
 *       每填完一题敲 ./check.sh，看哪一段是 ✅。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

/* ---------- 1. 尾插建表 ----------
 * 建 1..n，返回头指针；n <= 0 返回 NULL
 * 要求：用一个 tail 指针，整体 O(n)
 */
Node *list_build_tail(int n) {
    /* TODO */
    (void)n;
    return NULL;
}

/* ---------- 2. 头插建表 ----------
 * 建 1..n，结果顺序是 n..1；n <= 0 返回 NULL
 */
Node *list_build_head(int n) {
    /* TODO */
    (void)n;
    return NULL;
}

/* ---------- 3. 求长度 ---------- */
int list_length(Node *head) {
    /* TODO */
    (void)head;
    return 0;
}

/* ---------- 4. 查找 ----------
 * 找到返回值所在的第一个节点，找不到返回 NULL
 * 注意：先判 cur != NULL，再读 cur->val
 */
Node *list_find(Node *head, int v) {
    /* TODO */
    (void)head; (void)v;
    return NULL;
}

/* ---------- 5. 在下标 idx 处插入 ----------
 * 合法范围：0 <= idx <= 长度（等于长度就是尾插）
 * 用二级指针改头：*head 可能变
 * 成功返回 0；idx 非法或 malloc 失败返回 -1
 * 插头要单独处理（没有「前一个节点」）
 */
int list_insert(Node **head, int idx, int v) {
    /* TODO */
    (void)head; (void)idx; (void)v;
    return -1;
}

/* ---------- 6. 删除下标 idx 处的节点 ----------
 * 合法范围：0 <= idx < 长度
 * 成功返回 0；idx 非法返回 -1
 * 释放前先存 next，再 free
 */
int list_erase(Node **head, int idx) {
    /* TODO */
    (void)head; (void)idx;
    return -1;
}

/* ---------- 7. 打印（自己调试用，测试不检查） ----------
 * 1 -> 2 -> 3 -> NULL（末尾换行）
 */
void list_print(Node *head) {
    /* TODO */
    (void)head;
}

/* ---------- 8. 销毁 ----------
 * 全部释放；head 为 NULL 时也要安全返回
 */
void list_destroy(Node *head) {
    /* TODO */
    (void)head;
}

/* ==================== 以下不要改 ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

/* 直接遍历判断，不用学生写的 list_length */
static int eq(Node *h, const int *a, int n) {
    for (int i = 0; i < n; i++) {
        if (!h || h->val != a[i]) return 0;
        h = h->next;
    }
    return h == NULL;
}

static void test_build(void) {
    printf("[1] build_tail / build_head / length\n");
    Node *L = list_build_tail(0);
    CHECK(L == NULL, "build_tail(0) 返回 NULL");
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

    CHECK(list_build_head(0) == NULL, "build_head(0) 返回 NULL");
}

static void test_find(void) {
    printf("[2] list_find\n");
    Node *L = list_build_tail(5);
    if (!L) { CHECK(0, "建表失败，find 跳过"); return; }

    Node *p = list_find(L, 1);
    CHECK(p != NULL && p->val == 1, "find(1) 命中头节点");
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
    if (!L) { CHECK(0, "建表失败，insert 跳过"); return; }

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
    if (!L) { CHECK(0, "建表失败，erase 跳过"); return; }

    CHECK(list_erase(&L, 1) == 0, "删 idx=1 返回 0");
    int e1[] = {1, 3, 4, 5};
    CHECK(eq(L, e1, 4), "删后 {1,3,4,5}");

    CHECK(list_erase(&L, 0) == 0, "删头返回 0");
    int e2[] = {3, 4, 5};
    CHECK(eq(L, e2, 3), "删头后 {3,4,5}");

    CHECK(list_erase(&L, 2) == 0, "删尾返回 0");
    int e3[] = {3, 4};
    CHECK(eq(L, e3, 2), "删尾后 {3,4}");

    CHECK(list_erase(&L, 2) == -1, "idx 超范围 -> -1");
    CHECK(eq(L, e3, 2), "非法 idx 不改表");
    CHECK(list_erase(&L, -1) == -1, "idx 为负 -> -1");

    CHECK(list_erase(&L, 0) == 0 && list_erase(&L, 0) == 0, "删空两个节点");
    CHECK(L == NULL, "删空后头指针为 NULL");
    CHECK(list_erase(&L, 0) == -1, "空表再删 -> -1");
    list_destroy(L);
}

static void test_destroy(void) {
    printf("[5] list_destroy\n");
    Node *L = list_build_tail(1000);
    list_destroy(L);
    CHECK(1, "destroy 1000 个节点没崩（valgrind 看是否泄漏）");
}

int main(void) {
    printf("=== B 关 W1-2：单链表基础 ===\n\n");
    test_build();
    test_find();
    test_insert();
    test_erase();
    test_destroy();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
