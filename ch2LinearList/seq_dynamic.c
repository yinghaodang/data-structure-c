#include <stdio.h>
#include <stdlib.h>
typedef struct DynamicSeqList {
    int *data;
    int capacity;
    int length;
} DynamicSeqList;

int InitList(DynamicSeqList *L, int capacity) {
    if (capacity < 1) {
        return 0;
    }
    L->capacity = capacity;
    L->data = (int *)malloc(sizeof(int) * capacity);
    if (L->data == NULL) {
        return 0;
    }
    L->length = 0;
    return 1;
}

int ExpandList(DynamicSeqList *L, int capacity) {
    if (capacity < L->length) {
        return 0;  // 会丢失数据，不允许
    }
    if (capacity == L->capacity) {
        return 1;
    }

    int *temp = (int *)realloc(L->data, sizeof(int) * capacity);
    if (temp == NULL) {
        return 0;  // 分配失败
    }

    L->data = temp;
    L->capacity = capacity;
    return 1;
}

void PrintList(DynamicSeqList L) {
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

void DestroyList(DynamicSeqList *L) {
    free(L->data);
    L->data = NULL;
    L->capacity = 0;
    L->length = 0;
}

int main() {
    DynamicSeqList L;
    InitList(&L, 10);

    for (int i = 0; i < 6; i++) {
        L.data[i] = i + 1;
        L.length++;
    }

    ExpandList(&L, 20);
    for (int i = 6; i < 15; i++) {
        L.data[i] = i + 1;
        L.length++;
    }

    PrintList(L);
    DestroyList(&L);
}