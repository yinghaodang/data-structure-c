#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    int *base;
    int *top;
    int stackSize;
} SqStack, *PSqStack;

SqStack *InitStack(int stackSize) {
    SqStack *S = (SqStack *)malloc(sizeof(SqStack));
    S->base = (int *)calloc(1, stackSize * sizeof(int));
    S->top = S->base;
    S->stackSize = stackSize;
    return S;
}

bool StackEmpty(SqStack S) { return S.top == S.base; }

bool Push(SqStack *S, int e) {
    if (S->top - S->base >= S->stackSize) return false;
    *S->top++ = e;
    return true;
}

bool Pop(SqStack *S, int *e) {
    if (S->top == S->base) return false;
    *e = *--S->top;
    return true;
}

bool GetTop(SqStack S, int *e) {
    if (S.top == S.base) return false;
    *e = *(S.top - 1);
    return true;
}

bool DestroyStack(SqStack *S) {
    free(S->base);
    S->base = NULL;
    S->top = NULL;
    S->stackSize = 0;
    return true;
}

void Conversion(unsigned int N, int radix) {
    SqStack *S;
    S = InitStack(100);
    while (N) {
        Push(S, N % radix);
        N = N / radix;
    }
    while (!StackEmpty(*S)) {
        int e;
        Pop(S, &e);
        char c = e < 10 ? '0' + e : 'a' + e - 10;
        printf("%c", c);
    }
    printf("\n");
    DestroyStack(S);
}

int main() {
    Conversion(35, 36);
    return 0;
}