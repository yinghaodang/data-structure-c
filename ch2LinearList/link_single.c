// 不带头结点的版本
#include <stdio.h>
#include <stdlib.h>
typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

int InitList(LinkList *L) {
    *L = NULL;
    return 1;
}

int InsertElem(LinkList *L, int index, int data) {
    if (index < 1 || *L == NULL) {
        return 0;
    }

    LNode *newNode = (LNode *)malloc(sizeof(LNode));
    if (newNode == NULL) {
        return 0;
    }
    newNode->data = data;
    newNode->next = NULL;  // 可省略，但是防止初始化的时候尾节点的next是野指针

    if (index == 1) {
        newNode->next = *L;
        *L = newNode;
        return 1;
    }

    LinkList p = *L;
    // 插入到第 index 位置，指针需要移动到 index-1 位置，需要移动 index-2 次
    for (int i = 0; i < index - 2; i++) {
        if (p->next == NULL) {
            free(newNode);
            return 0;
        }
        p = p->next;
    }

    newNode->next = p->next;
    p->next = newNode;
    return 1;
}

int DropElem(LinkList *L, int index) {
    if (index < 1 || *L == NULL) {
        return 0;
    }

    if (index == 1) {
        LinkList temp = *L;
        *L = (*L)->next;
        free(temp);
        return 1;
    }

    LinkList p = *L;
    // 找到前驱节点
    for (int i = 0; i < index - 2; i++) {
        if (p->next == NULL) {
            return 0;
        }
        p = p->next;
    }
    LinkList temp = p->next;
    p->next = p->next->next;
    free(temp);
    return 1;
}

int DropElem2(LinkList *L, int index) {
    if (index < 1 || *L == NULL) {
        return 0;
    }

    if (index == 1) {
        LinkList temp = *L;
        *L = (*L)->next;
        free(temp);
        return 1;
    }

    LinkList p = *L;
    // 找到前驱节点
    for (int i = 0; i < index - 2 && p->next != NULL ; i++) {
        p = p->next;
    }
    if (p->next == NULL) {
        return 0;
    }
    LinkList temp = p->next;
    p->next = p->next->next;
    free(temp);
    return 1;
}

int UpdateElem(LinkList L, int index, int data) {
    if (index < 1 || L == NULL) {
        return 0;
    }
    for (int i = 0; i < index - 1; i++) {
        if (L->next == NULL) {
            return 0;
        }
        L = L->next;
    }
    L->data = data;
    return 1;
}

int GetElem(LinkList L, int index) {
    if (index < 1 || L == NULL) {
        return -1;
    }
    for (int i = 0; i < index - 1; i++) {
        if (L->next == NULL) {
            return 0;
        }
        L = L->next;
    }
    return L->data;
}

// 按值查找，返回第一个元素下标
int LocateElem(LinkList L, int data) {
    int index = 1;
    while (L != NULL) {
        if (L->data == data) {
            return index;
        }
        index++;
        L = L->next;
    }
    return -1;
}

void PrintList(LinkList L) {
    while (L != NULL) {
        printf("%d ", L->data);
        L = L->next;
    }
    printf("\n");
}

int main() {
    LinkList L;
    InitList(&L);

    LinkList p = L;
    for (int i = 0; i < 8; i++) {
        LNode *newNode = (LNode *)malloc(sizeof(LNode));
        if (newNode == NULL) {
            return 0;
        }
        newNode->data = i + 1;
        newNode->next = NULL;
        if (p == NULL) {
            p = newNode;
            L = newNode;
        } else {
            p->next = newNode;
            p = p->next;
        }
    }

    PrintList(L);

    // 插入表头表尾
    InsertElem(&L, 1, 88);
    PrintList(L);
    InsertElem(&L, 10, 88);
    PrintList(L);

    // 删除表头表尾
    DropElem(&L, 1);
    PrintList(L);
    DropElem(&L, 9);
    PrintList(L);

    UpdateElem(L, 1, 8);
    PrintList(L);

    printf("%d \n", GetElem(L, 5));
    printf("%d \n", LocateElem(L, 5));
}


// AI建议

// 1. 统一遍历模式
// // 模式1：找到第 index 个节点
// LinkList p = L;
// for (int i = 1; i < index && p != NULL; i++) {
//     p = p->next;
// }
// if (p == NULL) return 0;
// // 现在 p 指向第 index 个节点

// // 模式2：找到第 index-1 个节点（前驱）
// LinkList p = L;
// for (int i = 1; i < index - 1 && p != NULL; i++) {
//     p = p->next;
// }
// if (p == NULL || p->next == NULL) return 0;
// // 现在 p 指向第 index-1 个节点

// 2. 带头节点优化插入删除首个节点的逻辑

// 3. 合法性检查循序 (1).index范围 (2).链表非空 (3).循环体循环次数

// -------- //

// 经过我的思考，还是我自己的实现更好，但是得注意
// 在寻找 index-1 节点过程中 if (p == NULL || p->next == NULL) return 0; 
// 是一个比较阴险的考点，考试过程中时间充裕必须再次检查首尾节点的边界情况。

// 我的实现
// LinkList p = L;
// // 循环 index-1 次
// for (int i = 1; i < index; i++) {
//     if (p->next == NULL) return 0;
//     p = p->next;
// }