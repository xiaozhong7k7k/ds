/* ============================================================
 * A 关重测卷（40 分钟）
 * 规则：只填下面 4 个函数的函数体，main 和测试部分不要改。
 *       不许查答案，卡住 10 分钟先跳过，最后回来。
 *       改完一题就 ./check.sh，看 4 段里哪几段是 ✅。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node { 
    int val;
    struct Node *next;
} Node;

/* ---------- 1. 二维数组：每行单独 malloc ----------
 * matrix_create：申请 rows 行、每行 cols 个 int，并令 m[i][j] = i*cols + j
 *               任何一步 malloc 失败：把已经申请的全部释放干净，返回 NULL
 * matrix_free  ：释放全部内存（rows 行 + 行指针数组）
 */
int **matrix_create(int rows, int cols)
{
        //int count = 0;
        //int (*m)[cols] = (int*)malloc(sizeof(int(*)[cols])*cols*rows);
        //if(m == NULL){ 
        //   free(m);
        //    m = NULL;
        //    return NULL;
        //}
    int **m = (int**)malloc(sizeof(int*)*rows);
    if(m == NULL){ return NULL; }
    for(int i = 0; i<rows ; i++){
        m[i] = (int*)malloc(sizeof(int*)*cols);
        if(m[i] == NULL){
            for(int cnt = 0; cnt<i ; cnt++){
                free(m[cnt]);
            }
            free(m);
            return NULL;
        }
        for(int j = 0; j<cols ; j++){
            m[i][j] = i*cols + j;
        }
    }
    return m;
}

void matrix_free(int **m, int rows)
{
    for(int i = 0; i<rows ; i++){
        free(m[i]);
    }
    free(m);
}

/* ---------- 2. 链表删除：删除所有值为 val 的节点 ----------
 * 返回新的头指针（头节点可能被删）。空链表返回 NULL。
 * 要求：只遍历一遍；删掉的节点必须 free。
 */
Node *remove_all(Node *head, int val)
{
    while (head != NULL && head->val == val) {//判断头部，循环防接连删除
            Node *old = head;
            head = head->next;
            free(old);
    }
    if(head == NULL){ return NULL; }
    Node *cur = head;
while (cur != NULL && cur->next != NULL) {//链表删除不用for，如果cur为NULL，再进行cur->next，会崩
        if (cur->next != NULL && cur->next->val == val) {//也是防止对空指针解引用
            Node *temp = cur->next;
            cur->next = temp->next;
            free(temp);
        } else {
        cur = cur->next;
    }
}
    return head;
}

/* ---------- 3. 递归统计链表节点数（禁止用循环） ---------- */
int count(Node *head)
{
    if(head == NULL){ return 0 ; }
    return 1+count(head->next);
}

/* ---------- 4. my_strcmp ----------
 * 相等返回 0；a<b 返回负数；a>b 返回正数。
 * 按「第一个不同字符的差」返回即可。空串要能正确处理。
 */
int my_strcmp(const char *a, const char *b)
{
    //int idx = 0;
	//while(a[idx]==b[idx]&&a[idx]!='\0'&&b[idx]!='\0') { idx++; }
	//return a[idx] - b[idx];
	while(*a!='\0'&&*b!='\0'&&*a == *b){
        a++;
        b++;
    }
	return *a - *b;
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

static void test_matrix(void) {
    printf("[1] matrix_create / matrix_free\n");
    int rows = 3, cols = 4;
    int **m = matrix_create(rows, cols);
    CHECK(m != NULL, "3x4 返回非 NULL");
    if (m) {
        int ok = 1;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (m[i][j] != i * cols + j) ok = 0;
        CHECK(ok, "每个格子都等于 i*cols + j");
        matrix_free(m, rows);
    }
    int **one = matrix_create(1, 1);
    CHECK(one != NULL && one[0][0] == 0, "1x1 边界");
    if (one) matrix_free(one, 1);
}

static void test_remove(void) {
    printf("[2] remove_all\n");
    int a1[] = {1, 2, 2, 3, 2}, e1[] = {1, 3};
    Node *h = build_list(a1, 5);
    h = remove_all(h, 2);
    CHECK(list_eq(h, e1, 2), "{1,2,2,3,2} 删 2 -> {1,3}");
    free_list(h);

    int a2[] = {2, 2, 2};
    h = build_list(a2, 3);
    h = remove_all(h, 2);
    CHECK(h == NULL, "{2,2,2} 删 2 -> 空");
    free_list(h);

    int a3[] = {2, 1, 2}, e3[] = {1};
    h = build_list(a3, 3);
    h = remove_all(h, 2);
    CHECK(list_eq(h, e3, 1), "{2,1,2} 删 2 -> {1}（删头节点）");
    free_list(h);

    int a4[] = {1, 3, 4}, e4[] = {1, 3, 4};
    h = build_list(a4, 3);
    h = remove_all(h, 9);
    CHECK(list_eq(h, e4, 3), "没有 val 时链表不变");
    free_list(h);

    h = remove_all(NULL, 5);
    CHECK(h == NULL, "空链表删 -> NULL");
}

static void test_count(void) {
    printf("[3] count（递归，不许用循环）\n");
    CHECK(count(NULL) == 0, "空链表 -> 0");
    int a1[] = {7};
    Node *h = build_list(a1, 1);
    CHECK(count(h) == 1, "一个节点 -> 1");
    free_list(h);
    int a5[] = {1, 2, 3, 4, 5};
    h = build_list(a5, 5);
    CHECK(count(h) == 5, "五个节点 -> 5");
    free_list(h);
}

static void test_cmp(void) {
    printf("[4] my_strcmp\n");
    CHECK(my_strcmp("abc", "abc") == 0, "\"abc\" vs \"abc\" -> 0");
    CHECK(my_strcmp("abc", "abd") < 0, "\"abc\" vs \"abd\" -> 负");
    CHECK(my_strcmp("abd", "abc") > 0, "\"abd\" vs \"abc\" -> 正");
    CHECK(my_strcmp("abc", "abcd") < 0, "前缀短的那个更小");
    CHECK(my_strcmp("abcd", "abc") > 0, "前缀长的那个更大");
    CHECK(my_strcmp("", "") == 0, "空串 vs 空串 -> 0");
    CHECK(my_strcmp("", "a") < 0, "空串 < \"a\"");
    CHECK(my_strcmp("a", "") > 0, "\"a\" > 空串");
}

int main(void) {
    printf("=== A 关重测卷 ===\n\n");
    test_matrix();
    test_remove();
    test_count();
    test_cmp();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("四题全过，可以进 B 关。\n");
    return fails == 0 ? 0 : 1;
}
