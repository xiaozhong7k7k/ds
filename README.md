# 数据结构练习（C 语言）

大一起步的数据结构手写实现，配合 LeetCode 刷题，目标就业。

## 目录

- `w0_fix/` — C 基础补漏：指针、内存管理、链表、递归
- `w1_list/` — 线性表（顺序表、单链表、双链表）
- `w2_stack_queue/` — 栈与队列
- `w3_tree/` — 二叉树、堆、BST
- `w4_graph/` — 图的遍历与最短路径
- `w5_sort/` — 排序与查找

## 环境

- WSL2 + Ubuntu 26.04
- gcc 15.2.0 / gdb 17.1 / valgrind 3.26.0

## 编译运行

```bash
gcc -Wall -Wextra -g w0_fix/d1_swap.c -o bin/d1 && ./bin/d1
