#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR 0
#define OK 1
#define OVERFLOW -2
#define INIT_SIZE 100
typedef int Status;
typedef struct {
    int *elem;
    int maxSize;
    int length;
} SqList;

bool IsEqual(int a, int b) { return a == b; }

SqList *InitList2() {
    SqList *L = (SqList *)malloc(sizeof(SqList));
    if (!L) exit(OVERFLOW);
    L->elem = (int *)malloc(sizeof(int) * INIT_SIZE);
    if (!L->elem) exit(OVERFLOW);
    L->maxSize = INIT_SIZE;
    L->length = 0;
    return L;
}

Status InitList(SqList *L) {
    L->elem = (int *)malloc(sizeof(int) * INIT_SIZE);
    if (!L->elem) exit(OVERFLOW);
    L->maxSize = INIT_SIZE;
    L->length = 0;
    return OK;
}

Status InsertList(SqList *L, int i, int e) {
    if (i < 1 || i > L->length + 1) return ERROR;
    if (L->length >= L->maxSize) return ERROR;
    for (int j = L->length; j >= i; j--) {
        L->elem[j] = L->elem[j - 1];
    }
    L->elem[i - 1] = e;
    L->length++;
    return OK;
}

Status GetElem(SqList L, int i, int *e) {
    if (i < 1 || i > L.length) return ERROR;
    *e = L.elem[i - 1];
    return OK;
}

Status ListLength(SqList L) { return L.length; }

Status ListEmpty(SqList L) { return L.length == 0 ? OK : ERROR; }

Status ClearList(SqList *L) {
    L->length = 0;
    return OK;
}

Status CopyList(SqList *L1, SqList *L2) {
    L2->elem = (int *)malloc(sizeof(int) * L1->maxSize);
    if (!L2->elem) exit(OVERFLOW);
    L2->maxSize = L1->maxSize;
    L2->length = L1->length;
    for (int i = 0; i < L1->length; i++) {
        L2->elem[i] = L1->elem[i];
    }
    return OK;
}

Status CopyList2(SqList *L1, SqList *L2) {
    L2->elem = (int *)malloc(sizeof(int) * L1->maxSize);
    if (!L2->elem) exit(OVERFLOW);
    L2->maxSize = L1->maxSize;
    L2->length = L1->length;
    int *l1 = L1->elem;
    int *l2 = L2->elem;
    while (l1 <= l1 + L1->length) {
        *l2++ = *l1++;
    }
    return OK;
}

Status CopyList3(SqList *L1, SqList *L2) {
    L2->elem = (int *)malloc(sizeof(int) * L1->maxSize);
    if (!L2->elem) exit(OVERFLOW);
    L2->maxSize = L1->maxSize;
    L2->length = L1->length;
    memcpy(L2->elem, L1->elem, L1->length * sizeof(int));
    return OK;
}

Status LocateElem(SqList *L, int e, bool (*equal)(int, int)) {
    for (int i = 0; i < L->length; i++) {
        if (equal(L->elem[i], e)) return i + 1;
    }
    return ERROR;
}

Status DestroyList(SqList *L) {
    free(L->elem);
    L->elem = NULL;
    L->maxSize = 0;
    L->length = 0;
    return OK;
}

void PrintList(SqList *L) {
    for (int i = 0; i < L->length; i++) {
        printf("%d ", L->elem[i]);
    }
    printf("\n");
}

int main() {
    SqList L;
    InitList(&L);
    printf("List initialized\n");

    for (int i = 1; i <= 10; i++) {
        InsertList(&L, i, i);
        printf("Inserted %d at position %d\n", i, i);
    }

    printf("Locate elem: %d\n", LocateElem(&L, 5, IsEqual));

    DestroyList(&L);
    printf("List destroyed\n");

    return 0;
}