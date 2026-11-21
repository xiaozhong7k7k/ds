/* ============================================================
 * B 关 W1-1：顺序表（动态数组 + 插入 / 删除 / 移动次数）
 * 规则：只填 6 个函数体，main 和测试部分不要改。
 *       每填完一题敲 ./check.sh，看哪一段是 ✅。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;      /* 数据区 */
    int  size;      /* 现在有几个元素 */
    int  cap;       /* 数据区现在能装几个 */
} SeqList;

/* ---------- 1. 创建 ----------
 * 申请结构体 + cap 个 int 的数据区，size=0
 * 任何一步失败：已申请的释放干净，返回 NULL
 */


 //注意事项：申请空数据区的特别情况与申请失败的区别
 //申请失败后的每一步回滚free
SeqList *seq_create(int cap)
{
    SeqList *p = (SeqList*)malloc(sizeof(SeqList));
    if(p == NULL){
         return NULL;
    }
    if(cap == 0){//如果用户想建一个空数据区，防止返回NULL让别人以为申请失败
        p->data = NULL;
    }else{
        p->data = (int*)malloc(sizeof(int)*cap);
        if(p->data == NULL){
            free(p->data);
            p->data = NULL;
            free(p);
            p = NULL;
            return NULL;
        }
    }
    p->size = 0;
    p->cap = cap;
    return p;
}

/* ---------- 2. 尾部插入 ----------
 * 满了就扩容成原来的 2 倍（cap 为 0 时按 1 起步）
 * 成功返回 0，失败返回 -1
 */


//注意事项：不要一开始就改动L的cap，先用newcap存起来
//线性表申请内存的每一步判空和申请失败
//往数据区填新数据的时候如果要扩容，记得拷贝老内存
//灵活运用L->size，运用时分清楚下标和总个数的区别
int seq_push_back(SeqList *L, int v)
{
    if(L == NULL){ return -1; }

    //判断是否扩容
    if(L->size == L->cap){
        int newcap;
        if(L->cap == 0){
            newcap = 1;
        }else{
            newcap = L->cap*2;
        }

        int *new_data = (int*)malloc(sizeof(int)*newcap);
        if(new_data == NULL){ return -1; }

        for(int i = 0; i<L->size ; i++){
            new_data[i] = L->data[i];
        }

        free(L->data);
        L->data = new_data;
        L->cap = newcap;
    }

    L->data[L->size] = v;
    L->size++;

    return 0;
}

/* ---------- 3. 在下标 idx 处插入 ----------
 * 合法范围：0 <= idx <= size（等于 size 就是尾插）
 * 后面的元素整体往后挪一格，返回「往后挪了几个元素」
 * idx 非法返回 -1；扩容失败返回 -1
 */


 //注意事项：扩容后要改变L->cap的大小，否则下次调用会在if那里出错导致内存出问题
 //扩容直接翻倍，节省时间，用空间换时间
 //扩容步骤是先申请一片新的内存，然后free老内存，再让原来指着老内存的指针指向新内存
 //增删改一定要记得改L->size
int seq_insert(SeqList *L, int idx, int v)
{
    if( idx<0 || idx>L->size){ return -1; }

    if(L->size == L->cap){
        int *newdata = (int*)malloc(sizeof(int)*(L->cap*2));
        if(newdata == NULL){ return -1; }

        for(int i = 0; i<L->size; i++){
            newdata[i] = L->data[i];
        }
        free(L->data);
        L->data = newdata;
        L->cap *= 2;
    }

    int count = 0;
    for(int i = L->size; i>idx; i--){
        L->data[i] = L->data[i-1];
        count++;
    }

    L->data[idx] = v;
    L->size++;

    return count;
}

/* ---------- 4. 删除下标 idx 处的元素 ----------
 * 合法范围：0 <= idx < size
 * 后面的元素整体往前挪一格，返回「往前挪了几个元素」
 * idx 非法返回 -1
 */


 //注意事项：注意for循环里的判断条件，增删直接用新数据覆盖老数据也可以得到删除的效果
int seq_erase(SeqList *L, int idx)
{
    if(idx<0 || idx>=L->size){ return -1; }

    int count = 0;
    for(int i = idx; i<L->size-1 ; i++){
        L->data[i] = L->data[i+1];
        count++;
    }
    
    L->size--;

    return count;
}

/* ---------- 5. 查找 ----------
 * 找到返回下标，找不到返回 -1
 */
int seq_find(SeqList *L, int v)
{
    for(int i = 0; i<L->size ; i++){
        if(L->data[i] == v){
            return i;
        }
    }

    return -1;
}

/* ---------- 6. 销毁 ----------
 * 先 free 数据区，再 free 结构体本身
 */

 //释放结构体的内存时，遵循先里后外的规则
void seq_destroy(SeqList *L)
{
    free(L->data);
    L->data = NULL;
    free(L);
}

/* ==================== 以下不要改 ==================== */

static int fails = 0;
#define CHECK(cond, msg) do { \
    if (cond) printf("  \xE2\x9C\x85 %s\n", msg); \
    else { printf("  \xE2\x9D\x8C %s\n", msg); fails++; } \
} while (0)

static int eq(SeqList *L, const int *a, int n) {
    if (!L || L->size != n) return 0;
    for (int i = 0; i < n; i++)
        if (L->data[i] != a[i]) return 0;
    return 1;
}

static void test_basic(void) {
    printf("[1] seq_create / seq_push_back / seq_find / seq_destroy\n");
    SeqList *L = seq_create(4);
    CHECK(L != NULL && L->size == 0 && L->cap == 4, "create(4)：size=0，cap=4");
    if (!L) return;
    for (int i = 1; i <= 5; i++) seq_push_back(L, i);
    CHECK(L->size == 5, "push 5 次后 size=5");
    CHECK(L->cap == 8, "第 5 次插入触发扩容：cap 4 -> 8");
    CHECK(L->data[0] == 1 && L->data[4] == 5, "数据顺序对：1..5");
    CHECK(seq_find(L, 1) == 0 && seq_find(L, 5) == 4, "find(1)=0，find(5)=4");
    CHECK(seq_find(L, 99) == -1, "find(99)=-1");

    for (int i = 6; i <= 1000; i++) seq_push_back(L, i);
    CHECK(L->size == 1000, "push 到 1000 个，size=1000");
    CHECK(L->cap == 1024, "按倍数扩容：cap = 1024（不是 1000 也不是几千）");
    seq_destroy(L);

    SeqList *E = seq_create(2);
    CHECK(E && E->size == 0 && seq_find(E, 7) == -1, "空表 find 返回 -1");
    seq_destroy(E);
}

static void test_insert(void) {
    printf("[2] seq_insert 与移动次数\n");
    SeqList *L = seq_create(8);
    if (!L) { CHECK(0, "create 失败，后面跳过"); return; }
    for (int i = 1; i <= 5; i++) seq_push_back(L, i);   /* 1 2 3 4 5 */

    int moved = seq_insert(L, 0, 100);                  /* 头插 */
    int e1[] = {100, 1, 2, 3, 4, 5};
    CHECK(moved == 5, "5 个元素里插到最前：挪 5 个");
    CHECK(eq(L, e1, 6), "头插后是 {100,1,2,3,4,5}");

    int e2[] = {100, 1, 2, 3, 4, 5, 7};
    CHECK(seq_insert(L, 6, 7) == 0, "插到末尾：挪 0 个");
    CHECK(eq(L, e2, 7), "尾插后是 {100,1,2,3,4,5,7}");

    int e3[] = {100, 1, 2, 30, 3, 4, 5, 7};
    CHECK(seq_insert(L, 3, 30) == 4, "插到下标 3（后面有 4 个）：挪 4 个");
    CHECK(eq(L, e3, 8), "结果 {100,1,2,30,3,4,5,7}");

    CHECK(seq_insert(L, 99, 0) == -1, "idx 超范围 -> -1");
    CHECK(seq_insert(L, -1, 0) == -1, "idx 为负 -> -1");
    CHECK(seq_insert(L, L->size, 8) == 0, "idx == size 合法（尾插）");
    seq_destroy(L);
}

static void test_head_cost(void) {
    printf("[3] 头插 n 次的总移动次数\n");
    SeqList *L = seq_create(4);
    if (!L) { CHECK(0, "create 失败，后面跳过"); return; }
    int total = 0;
    for (int i = 0; i < 100; i++) total += seq_insert(L, 0, i);
    CHECK(total == 4950, "头插 100 次，总移动 = 0+1+...+99 = 4950");
    CHECK(L->size == 100 && L->data[0] == 99, "头插后 size=100，第一个是最后插的 99");
    seq_destroy(L);
}

static void test_erase(void) {
    printf("[4] seq_erase 与移动次数\n");
    SeqList *L = seq_create(8);
    if (!L) { CHECK(0, "create 失败，后面跳过"); return; }
    for (int i = 1; i <= 5; i++) seq_push_back(L, i);   /* 1 2 3 4 5 */

    int e1[] = {1, 3, 4, 5};
    CHECK(seq_erase(L, 1) == 3, "删下标 1：后面 3 个往前挪");
    CHECK(eq(L, e1, 4), "结果 {1,3,4,5}");

    int e2[] = {3, 4, 5};
    CHECK(seq_erase(L, 0) == 3, "删头：后面 3 个往前挪");
    CHECK(eq(L, e2, 3), "结果 {3,4,5}");

    CHECK(seq_erase(L, 2) == 0, "删最后一个：挪 0 个");
    CHECK(seq_erase(L, 5) == -1, "idx 超范围 -> -1");
    CHECK(seq_erase(L, -1) == -1, "idx 为负 -> -1");
    CHECK(L->size == 2, "删完 size=2");

    seq_erase(L, 0);
    seq_erase(L, 0);
    CHECK(L->size == 0 && seq_erase(L, 0) == -1, "删空后再删 -> -1");
    seq_destroy(L);
}

int main(void) {
    printf("=== B 关 W1-1：顺序表 ===\n\n");
    test_basic();
    test_insert();
    test_head_cost();
    test_erase();
    printf("\n=== 失败项：%d ===\n", fails);
    if (fails == 0) printf("全过。\n");
    return fails == 0 ? 0 : 1;
}
