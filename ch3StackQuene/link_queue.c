#include <stdio.h>
#include <stdlib.h>
typedef struct LNode {
    char *data;
    struct LNode *next;
} LNode;
typedef struct {
    LNode *front, *rear;
} LinkQueue;

int InitQueue(LinkQueue *Q) {
    Q->front = NULL;
    Q->rear = NULL;
    return 1;
}

int EnQueue(LinkQueue *Q, char *p) {
    LNode *newNode = (LNode *)malloc(sizeof(LNode));
    if (newNode == NULL) {
        return 0;
    }
    newNode->data = p;
    newNode->next = NULL;

    if (Q->front == NULL) {
        Q->front = newNode;
        Q->rear = newNode;
        return 1;
    }

    Q->rear->next = newNode;
    Q->rear = newNode;
    return 1;
}

// 返回出队的元素,依赖外部回收内存
char *DeQueue(LinkQueue *Q) {
    if (Q->front == NULL) {
        return NULL;
    }

    LNode *temp = Q->front;
    char *p = temp->data;

    if (Q->front == Q->rear) {
        Q->front = NULL;
        Q->rear = NULL;
    } else {
        Q->front = Q->front->next;
    }

    free(temp);  // 释放节点内存
    return p;
}

char *GetHead(LinkQueue Q) {
    if (Q.front == NULL) {
        return NULL;
    }
    return Q.front->data;
}

// 练习二级指针
int GetHead2(LinkQueue Q, char **p) {
    if (Q.front == NULL) {
        return 0;
    }
    *p = Q.front->data;
    return 1;
}

void PrintQueue(LinkQueue *Q) {
    if (Q->front == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    LNode *front = Q->front;
    LNode *rear = Q->rear;
    while (front != rear) {
        printf("%s ", front->data);
        front = front->next;
    }
    printf("%s\n", front->data);
}

int DestroyQueue(LinkQueue *Q) {
    while (Q->front != Q->rear) {
        LNode *temp = Q->front;
        Q->front = Q->front->next;
        free(temp);
    }
    if (Q->front != NULL) {
        free(Q->front);
    }
    return 1;
}

int main() {
    LinkQueue Q;
    InitQueue(&Q);

    printf("===入队===\n");
    EnQueue(&Q, "Hello");
    EnQueue(&Q, "Hi");
    PrintQueue(&Q);

    printf("===出队===\n");
    char *p = DeQueue(&Q);
    printf("%s\n", p);

    p = DeQueue(&Q);
    printf("%s\n", p);

    p = DeQueue(&Q);
    if (p == NULL) {
        printf("p is NULL.\n");
    }
    PrintQueue(&Q);

    p = GetHead(Q);
    if (p == NULL) {
        printf("p is NULL.\n");
    }

    EnQueue(&Q, "Fine");
    GetHead2(Q, &p);
    printf("%s", p);
    DestroyQueue(&Q);
}