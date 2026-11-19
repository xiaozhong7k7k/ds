/* ============================================================
 * A 关加练卷（4 题，约 40 分钟）
 * 规则：只填下面 5 个函数的函数体，main 和测试部分不要改。
 *       递归那两题禁止用循环。
 *       改完一题就 ./check.sh，看哪几段是 ✅。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{   int val;
    struct Node *next;
} Node;

/* ---------- 1. 递归求和（禁止用循环） ---------- */
int sum_list(Node *head)
{
    if(head == NULL){ return 0; }
    return head->val + sum_list(head->next);
}

/* ---------- 2. 递归反转单链表（禁止用循环） ----------
 * 返回反转后的新头指针。不许 free 任何节点。
 */
Node *reverse_rec(Node *head)
{
    
    
}

/* ---------- 3. 二维数组：一整块连续内存 ----------
 * 第一步 malloc rows 个 int* ——行指针数组
 * 第二步 malloc 一块 rows*cols 个 int ——数据全在这一块里
 * 让 m[i] 指向块内第 i 行的开头，然后填 m[i][j] = i*cols + j
 * 任何一步失败：已申请的释放干净，返回 NULL
 */
int **grid_create(int rows, int cols)
{
    
    return NULL;
}

/* ---------- 4. 释放：只有两次 free（数据块 + 行指针数组） ---------- */
void grid_free(int **m)
{
    
}

/* ---------- 5. 转置 ----------
 * 返回 cols 行、rows 列的新矩阵，t[i][j] = m[j][i]
 * 必须用 grid_create 建结果（测试会用 grid_free 释放它）
 */
int **transpose(int **m, int rows, int cols)
{
    
    return NULL;
}

/* ==================== 以下不要改 ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

static Node *build_list(const int *a, int n) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *nd = malloc(sizeof(Node));
        if (!nd) { while (head) { Node *t = head->next; free(head); head = t; } return NULL; }
        nd->val = a[i];
        nd->next = NULL;
        if (!head) head = nd; else tail->next = nd;
        tail = nd;
    }
    return head;
}

static void free_list(Node *h) {
    while (h) { Node *t = h->next; free(h); h = t; }
}

static int list_eq(Node *h, const int *a, int n) {
    int i = 0;
    while (h && i < n) {
        if (h->val != a[i]) return 0;
        h = h->next;
        i++;
    }
    return (i == n && h == NULL);
}

static void test_sum(void) {
    printf("[1] sum_list（递归，不许用循环）\n");
    CHECK(sum_list(NULL) == 0, "空链表 -> 0");
    int a1[] = {5};
    Node *h = build_list(a1, 1);
    CHECK(sum_list(h) == 5, "{5} -> 5");
    free_list(h);
    int a2[] = {1, 2, 3, 4, 5};
    h = build_list(a2, 5);
    CHECK(sum_list(h) == 15, "{1,2,3,4,5} -> 15");
    free_list(h);
    int a3[] = {-1, 2, -3};
    h = build_list(a3, 3);
    CHECK(sum_list(h) == -2, "{-1,2,-3} -> -2");
    free_list(h);
}

static void test_rev(void) {
    printf("[2] reverse_rec（递归，不许用循环）\n");
    CHECK(reverse_rec(NULL) == NULL, "空链表 -> NULL");

    int a1[] = {7}, e1[] = {7};
    Node *h = build_list(a1, 1);
    h = reverse_rec(h);
    CHECK(list_eq(h, e1, 1), "{7} -> {7}");
    free_list(h);

    int a3[] = {1, 2, 3}, e3[] = {3, 2, 1};
    h = build_list(a3, 3);
    h = reverse_rec(h);
    CHECK(list_eq(h, e3, 3), "{1,2,3} -> {3,2,1}");
    free_list(h);

    int a5[] = {1, 2, 3, 4, 5}, e5[] = {5, 4, 3, 2, 1};
    h = build_list(a5, 5);
    h = reverse_rec(h);
    CHECK(list_eq(h, e5, 5), "{1..5} -> {5..1}");
    if (h) {
        int n = 0;
        Node *p = h;
        while (p && n < 20) { p = p->next; n++; }
        if (p == NULL && n == 5) { CHECK(1, "反转后没有成环"); free_list(h); }
        else CHECK(0, "反转后成环了（老头的 next 没置 NULL）");
    }
}

static void test_grid(void) {
    printf("[3][4] grid_create / grid_free（一整块内存）\n");
    int rows = 3, cols = 4;
    int **m = grid_create(rows, cols);
    CHECK(m != NULL, "3x4 返回非 NULL");
    if (m) {
        int ok = 1;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (m[i][j] != i * cols + j) ok = 0;
        CHECK(ok, "每个格子都等于 i*cols + j");
        CHECK(m[1] == m[0] + cols && m[2] == m[1] + cols, "三行在同一块连续内存里");
        m[1][2] = 99;
        CHECK(m[1][2] == 99, "写进去能读出来");
        grid_free(m);
    }
    int **one = grid_create(1, 1);
    CHECK(one != NULL && one[0][0] == 0, "1x1 边界");
    if (one) grid_free(one);
}

static void test_transpose(void) {
    printf("[5] transpose\n");
    int rows = 2, cols = 3;
    int **m = grid_create(rows, cols);
    int **t = NULL;
    if (m) t = transpose(m, rows, cols);
    CHECK(t != NULL, "2x3 转置返回非 NULL");
    if (m && t) {
        int ok = 1;
        for (int i = 0; i < cols; i++)
            for (int j = 0; j < rows; j++)
                if (t[i][j] != m[j][i]) ok = 0;
        CHECK(ok, "是 3x2，且 t[i][j] == m[j][i]");
        CHECK(t[0][1] == 3, "m[1][0] 跑到 t[0][1]（值 3）");
    }
    if (t) grid_free(t);
    if (m) grid_free(m);

    int **r1 = grid_create(1, 4);
    int **t1 = NULL;
    if (r1) t1 = transpose(r1, 1, 4);
    CHECK(t1 != NULL && t1[3][0] == 3, "1x4 -> 4x1");
    if (t1) grid_free(t1);
    if (r1) grid_free(r1);
}

int main(void) {
    printf("=== A 关加练卷 ===\n\n");
    test_sum();
    test_rev();
    test_grid();
    test_transpose();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
