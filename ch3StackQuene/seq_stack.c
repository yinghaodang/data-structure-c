// 栈中存储字符数组(字符串)
// 查找操作如果不返回字符数组,那么就要用二级指针,注意!
#include <stdio.h>
#define Maxsize 10
typedef struct SqStack {
    int top;
    char* data[Maxsize];
} SqStack;

int InitStack(SqStack* S) {
    S->top = -1;
    return 1;
}

int PushElem(SqStack* S, char* p) {
    if (S->top == Maxsize - 1) {
        return 0;
    }
    S->data[++S->top] = p;
    return 1;
}

char* PopElem(SqStack* S) {
    if (S->top == -1) {
        return NULL;
    }
    return S->data[S->top--];
}

char* GetPop(SqStack S) {
    if (S.top == -1) {
        return NULL;
    }
    return S.data[S.top];
}

int ClearStack(SqStack* S) {
    S->top = -1;
    return 1;
}

void PrintStack(SqStack* S) {
    while (S->top > -1) {
        printf("%s ", S->data[S->top]);
        printf("====");
        S->top--;
    }
    printf("\n");
}

int main() {
    SqStack S;
    InitStack(&S);

    char p[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    PushElem(&S, p);
    char q[] = "Hi";
    PushElem(&S, q);
    PrintStack(&S);

    printf("%s\n", q);
    printf("%s\n", p);
}