#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode* next;
};

typedef struct ListNode ListNode;
typedef struct {
    ListNode** data;  // 栈中存放ListNode的地址，data是栈空间的起始地址
    int top;
    int maxsize;
} Stack;

Stack* InitStack(int n) {
    Stack* S = (Stack*)malloc(sizeof(Stack));
    S->data = (ListNode**)malloc(n * sizeof(ListNode*));
    S->top = -1;
    S->maxsize = n;
    return S;
}

Stack* PushItem(Stack* S, ListNode* Node) {
    if (S->top >= S->maxsize - 1) {
        printf("Stack Overflow");
        exit(1);
    }
    S->data[++S->top] = Node;
    return S;
}

ListNode* PopItem(Stack* S) {
    if (S->top < 0) {
        printf("Pop Empty Stack");
        exit(1);
    }
    return S->data[S->top--];
}

ListNode* GetPop(Stack* S) {
    if (S->top < 0) {
        return NULL;
    }
    return S->data[S->top];
}

void PrintList(ListNode* head) {
    if (head == NULL) {
        printf("Print NULL List");
        exit(1);
    }
    while (head != NULL) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

void reorderList(struct ListNode* head) {
    // 第一次遍历：统计实际长度
    ListNode* tail = head;
    int length = 0;
    while (tail != NULL) {
        tail = tail->next;
        length++;
    }

    // 按实际长度分配栈空间
    Stack* S = InitStack(length);

    // 第二次遍历：入栈
    tail = head;
    while (tail != NULL) {
        PushItem(S, tail);
        tail = tail->next;
    }

    // 重排序逻辑
    ListNode* head2 = head;
    for (int i = 0; i < length / 2; i++) {
        ListNode* temp = PopItem(S);
        temp->next = head2->next;
        head2->next = temp;
        head2 = temp->next;
    }
    head2->next = NULL;

    // 释放栈内存
    free(S->data);
    free(S);
}

int main() {
    ListNode* node1 = (ListNode*)malloc(sizeof(ListNode));
    ListNode* node2 = (ListNode*)malloc(sizeof(ListNode));
    ListNode* node3 = (ListNode*)malloc(sizeof(ListNode));
    ListNode* node4 = (ListNode*)malloc(sizeof(ListNode));
    ListNode* node5 = (ListNode*)malloc(sizeof(ListNode));
    node1->next = node2;
    node1->val = 1;
    node2->next = node3;
    node2->val = 2;
    node3->next = node4;
    node3->val = 3;
    node4->next = node5;
    node4->val = 4;
    node5->next = NULL;
    node5->val = 5;

    PrintList(node1);
    reorderList(node1);
    PrintList(node1);
}