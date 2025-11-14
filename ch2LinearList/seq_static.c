// 实现顺序表的 创建，增删改查
#include <stdio.h>
#define MaxSize 10
typedef struct SeqList {
    int data[MaxSize];
    int length;
} SeqList;

int InitList(SeqList *L) {
    L->length = 0;
    for (int i = 0; i < MaxSize; i++) {
        L->data[i] = 0;
    }
    return 1;
}

// 插入第 index 个位置，占据数组的 index-1 位置
int InsertElem(SeqList *L, int index, int data) {
    if (index < 1 || index > L->length + 1) {
        return 0;
    }
    if (L->length == MaxSize) {
        return 0;
    }

    // index及其后面的元素需要变动
    // index ... length 一共变动 length-index+1 次
    // 这里的考点一般是考 i > what, 设这里的未知数为 x
    // 那么有 length - x (代码循环体循环次数) =
    // length - index + 1 （从index到length的元素个数，即后移元素数）
    for (int i = L->length; i > index - 1; i--) {
        L->data[i] = L->data[i - 1];
    }
    L->data[index - 1] = data;
    L->length++;
    return 1;
}

// 删除第 index 个元素
int DropElem(SeqList *L, int index) {
    if (index < 1 || index > L->length) {
        return 0;
    }
    for (int i = index - 1; i < L->length - 1; i++) {
        L->data[i] = L->data[i + 1];
    }
    L->length--;
    return 1;
}

int GetElem(SeqList L, int index, int *data) {
    if (index < 1 || index > L.length) {
        return 0;
    }
    *data = L.data[index - 1];
    return 1;
}

// 按值查找，并返回其位置（返回第一个）
int LocateElem(SeqList L, int data, int *index) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == data) {
            *index = i + 1;
            return 1;
        }
    }
    *index = -1;
    return 0;
}

int UpdateElem(SeqList *L, int index, int data) {
    if (index < 1 || index > L->length) {
        return 0;
    }
    L->data[index - 1] = data;
    return 1;
}

void PrintList(SeqList L) {
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.data[i]);
    }
    printf("\n");
}

void ClearList(SeqList *L) { L->length = 0; }

// 反转顺序表
int ReverseList(SeqList *L) {
    int start = 0;
    int end = L->length - 1;
    while (end > start) {
        int temp = L->data[start];
        L->data[start] = L->data[end];
        L->data[end] = temp;
        start++;
        end--;
    }
    return 1;
}

// 删除指定范围内的元素
int DeleteRange(SeqList *L, int min, int max) {
    int k = 0;
    for (int i = 0; i < L->length; i++) {
        if (L->data[i] < min || L->data[i] > max) {
            L->data[k] = L->data[i];
            k++;
        }
    }
    L->length = k;
    return 1;
}

// 可能还会考有序列表的去重
// 以及两个有序列表的合并

int main() {
    SeqList L;
    InitList(&L);

    for (int i = 0; i < 6; i++) {
        int flag = InsertElem(&L, i + 1, i + 2);
        if (!flag) {
            printf("插入失败");
            break;
        }
    }
    PrintList(L);
    DropElem(&L, 5);
    PrintList(L);

    int data;
    GetElem(L, 2, &data);
    printf("第二个元素为：%d\n", data);

    int index;
    LocateElem(L, 3, &index);
    printf("值为3的第一个元素的位置: %d\n", index);

    UpdateElem(&L, 4, 88);
    PrintList(L);

    ReverseList(&L);
    PrintList(L);

    DeleteRange(&L, 2, 4);
    PrintList(L);
}