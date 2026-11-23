/* ============================================================
 * B 关 W1-3：链表变形（一）—— 合并有序链表 / 快慢指针找中点
 *
 * 【表示法约定：带头结点】
 *   head 是头结点（哨兵），head->next 是首元节点
 *   head->next == NULL 表示空表
 *   所有函数都假设传入的 head 非 NULL
 *
 * list_merge：
 *   两条有序表（都带头结点），归并成一条新的有序表
 *   返回新头结点；**函数内 free 掉 a、b 两个头结点**
 *   数据节点直接重接，不复制值、不新建数据节点
 *
 * list_middle：
 *   返回中间节点；偶数个返回靠后的那个（n=4 返回第 3 个）
 *   空表返回 NULL；只能用快慢指针，不许先求长度
 *
 * 规则：只填 2 个函数体，测试不要改。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

/* ---------- 1. 合并两个有序链表 ----------
 * a、b 都带头结点且升序；返回新头结点（也带头结点）
 * 不新建数据节点：把 a、b 的数据节点按序重接
 * 结束时 free(a)、free(b) 两个头结点
 */
Node *list_merge(Node *a, Node *b)
{
    // if(a->next == NULL){ free(a); }
    // if(b->next == NULL){ free(b); }
    //错了：这里free完后面会对悬空指针解引用

    Node *c = (Node*)malloc(sizeof(Node));
    if(c == NULL){ return NULL; }

    c->next = a->next;
    Node *pc = c;
    Node *pa = a->next;
    Node *pb = b->next;
    while(pa != NULL && pb != NULL){
        if(pa->val <= pb->val){ 
            pc->next = pa;
            pa = pa->next;
        }else{
            pc->next = pb;
            pb = pb->next;
        }

        pc = pc->next;
    }

    if(pa != NULL){ pc->next = pa; }
    if(pb != NULL){ pc->next = pb; }

    free(a);
    free(b);

    return c;
}

/* ---------- 2. 快慢指针找中点 ----------
 * 快指针一次走 2 步，慢指针一次走 1 步
 * 偶数个返回靠后的那个；空表返回 NULL
 */
Node *list_middle(Node *head)
{
    Node *fast = head->next;
    Node *slow = head->next;

    if(head->next == NULL){ return NULL; }

    while(fast != NULL && fast->next != NULL){
        fast = fast->next;
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

/* ==================== 测试（不要改） ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

/* 测试自己建表：带头结点，数据来自数组；n=0 只有头结点 */
static Node *make_list(const int *a, int n) {
    Node *head = (Node*)malloc(sizeof(Node));
    head->next = NULL;
    Node *tail = head;
    for (int i = 0; i < n; i++) {
        Node *p = (Node*)malloc(sizeof(Node));
        p->val = a[i];
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return head;
}

/* 测试自己释放，不依赖学生写的 destroy */
static void free_all(Node *head) {
    while (head) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

/* 跳过带头结点，比对数据 */
static int eq_data(Node *h, const int *a, int n) {
    Node *cur = (h == NULL) ? NULL : h->next;
    for (int i = 0; i < n; i++) {
        if (!cur || cur->val != a[i]) return 0;
        cur = cur->next;
    }
    return cur == NULL;
}

static void test_merge(void) {
    printf("[1] list_merge\n");
    int a1[] = {1, 3, 5}, b1[] = {2, 4, 6};
    int e1[] = {1, 2, 3, 4, 5, 6};
    Node *m = list_merge(make_list(a1, 3), make_list(b1, 3));
    CHECK(eq_data(m, e1, 6), "等长交叉：{1,3,5} + {2,4,6} = 1..6");
    free_all(m);

    int a2[] = {1, 2, 3};
    int e2[] = {1, 2, 3};
    m = list_merge(make_list(a2, 3), make_list(NULL, 0));
    CHECK(eq_data(m, e2, 3), "b 为空：原样返回 a 的数据");
    free_all(m);

    int b3[] = {4, 5};
    int e3[] = {4, 5};
    m = list_merge(make_list(NULL, 0), make_list(b3, 2));
    CHECK(eq_data(m, e3, 2), "a 为空：原样返回 b 的数据");
    free_all(m);

    int a4[] = {1, 3, 3, 7}, b4[] = {3, 4};
    int e4[] = {1, 3, 3, 3, 4, 7};
    m = list_merge(make_list(a4, 4), make_list(b4, 2));
    CHECK(eq_data(m, e4, 6), "有重复值：{1,3,3,7} + {3,4} = 1,3,3,3,4,7");
    free_all(m);

    m = list_merge(make_list(NULL, 0), make_list(NULL, 0));
    CHECK(m != NULL && m->next == NULL, "两个空表：只剩一个头结点");
    free_all(m);
}

static void test_middle(void) {
    printf("[2] list_middle\n");
    int v[] = {1, 2, 3, 4, 5};
    struct { int n, expect; } cases[] = {
        {0, 0}, {1, 1}, {2, 2}, {3, 2}, {4, 3}, {5, 3}
    };
    for (int i = 0; i < 6; i++) {
        int n = cases[i].n;
        Node *L = make_list(v, n);
        Node *mid = list_middle(L);
        char msg[64];
        if (n == 0) {
            CHECK(mid == NULL, "空表：返回 NULL");
        } else {
            snprintf(msg, sizeof msg, "n=%d：中间节点是 %d（偶数取靠后）", n, cases[i].expect);
            CHECK(mid != NULL && mid->val == cases[i].expect, msg);
        }
        free_all(L);
    }
}

int main(void) {
    printf("=== B 关 W1-3：链表变形（一）===\n\n");
    test_merge();
    test_middle();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
