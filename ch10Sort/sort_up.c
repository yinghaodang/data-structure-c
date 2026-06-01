#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode* next;
} LNode, *LinkList;

int InitList(LinkList* L, int* A, int len) {
    LNode* head = (LNode*)malloc(sizeof(LNode));
    *L = head;
    LNode* p = head;
    for (int i = 1; i <= len; i++) {
        LNode* newNode = (LNode*)malloc(sizeof(LNode));
        p->next = newNode;
        newNode->data = A[i];
        p = newNode;
    }
    p->next = NULL;
    return 1;
}

int Binary_Search(int* table, int key, int len) {
    int low = 0;
    int high = len - 1;
    while (high >= low) {
        int mid = (high + low) / 2;
        if (table[mid] == key) {
            return mid;
        } else if (table[high] > mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void InSertSort(int* A, int n) {
    int i, j;
    for (i = 2; i <= n; i++) {
        if (A[i] < A[i - 1]) {
            A[0] = A[i];
            for (j = i - 1; A[j] > A[0]; j--) {
                A[j + 1] = A[j];
            }
            A[j + 1] = A[0];
        }
    }
}

void BubbleSort(int* A, int n) {
    for (int j = 1; j < n; j++) {
        int flag = 0;
        for (int i = n; i > j; i--) {
            if (A[i] < A[i - 1]) {
                flag = 1;
                int temp = A[i];
                A[i] = A[i - 1];
                A[i - 1] = temp;
            }
        }
        if (flag == 0) {
            return;
        }
    }
}

void SelectSort(int* A, int n) {
    for (int i = 1; i <= n; i++) {
        A[0] = i;  // 存储最小元素所在的位置
        for (int j = i + 1; j < n + 1; j++) {
            if (A[j] < A[A[0]]) {
                A[0] = j;
            }
        }
        if (A[0] != i) {
            int temp = A[A[0]];
            A[A[0]] = A[i];
            A[i] = temp;
        }
    }
}

void ShellSort(int* A, int n) {
    int dk, i, j;
    for (dk = n / 2; dk >= 1; dk = dk / 2) {
        for (i = dk + 1; i <= n; i++) {
            if (A[i] < A[i - dk]) {
                A[0] = A[i];
                for (j = i - dk; j > 0 && A[j] > A[0]; j = j - dk) {
                    A[j + dk] = A[j];
                }
                A[j + dk] = A[0];
            }
        }
    }
}

int Partition(int* A, int low, int high) {
    int pivot = A[low];
    while (high > low) {
        while (low < high && A[high] >= pivot) {
            high--;
        }
        A[low] = A[high];
        while (low < high && A[low] <= pivot) {
            low++;
        }
        A[high] = A[low];
    }
    A[low] = pivot;
    return low;
}

void QuickSort(int* A, int low, int high) {
    if (high > low) {
        int pivot = Partition(A, low, high);
        QuickSort(A, low, pivot - 1);
        QuickSort(A, pivot + 1, high);
    }
}

void HeapAdjust(int* A, int k, int len) {
    // 调整以k为根节点的子树(根节点下坠)
    A[0] = A[k];
    for (int i = 2 * k; i <= len; i = 2 * i) {
        if (i < len && A[i] < A[i + 1]) {
            i++;
        }
        if (A[0] >= A[i]) {
            break;
        } else {
            A[k] = A[i];
            k = i;
        }
    }
    A[k] = A[0];
}

void BuildMaxHeap(int* A, int len) {
    for (int i = len / 2; i > 0; i--) {
        HeapAdjust(A, i, len);
    }
}

void HeapSort(int* A, int len) {
    // 最大值必定是堆顶
    BuildMaxHeap(A, len);
    for (int i = 1; i < len + 1; i++) {
        int temp = A[1];
        A[1] = A[len + 1 - i];
        A[len + 1 - i] = temp;
        HeapAdjust(A, 1, len - i);
    }
}

int B[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
void MergeSeq(int* A, int low, int mid, int high) {
    for (int i = low; i <= high; i++) {
        B[i] = A[i];
    }
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
        if (B[i] <= B[j]) {
            A[k++] = B[i++];
        } else {
            A[k++] = B[j++];
        }
    }
    while (i <= mid) {
        A[k++] = B[i++];
    }
    while (j <= high) {
        A[k++] = B[j++];
    }
}

void MergeSortSeq(int* A, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSortSeq(A, low, mid);
        MergeSortSeq(A, mid + 1, high);
        MergeSeq(A, low, mid, high);
    }
}

LNode* findMiddle(LinkList L1) {
    if (L1->next == NULL || L1->next->next == NULL) {
        return L1->next;
    }
    LNode* fast = L1->next;
    LNode* slow = L1->next;
    while (fast->next != NULL && fast->next->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}

LinkList Merge(LinkList L1, LinkList L2) {
    if (L1 == NULL) return L2;
    if (L2 == NULL) return L1;

    LNode* dummy = L1;
    LNode* tail = L1;
    L1 = L1->next;
    L2 = L2->next;
    while (L1 != NULL && L2 != NULL) {
        if (L1->data <= L2->data) {
            tail->next = L1;
            L1 = L1->next;
        } else {
            tail->next = L2;
            L2 = L2->next;
        }
        tail = tail->next;
    }
    if (L1 != NULL) {
        tail->next = L1;
    } else {
        tail->next = L2;
    }
    return dummy;
}

void MergeSort(LinkList L) {
    if (L == NULL || L->next == NULL || L->next->next == NULL) {
        return;
    }
    LNode* mid = findMiddle(L);

    LinkList M = (LinkList)malloc(sizeof(LNode));
    M->next = mid->next;
    mid->next = NULL;
    MergeSort(L);
    MergeSort(M);
    LinkList sorted = Merge(L, M);
    L->next = sorted->next;

    free(M);
}

void PrintList(int* List, int n) {
    for (int i = 1; i <= n; i++) {
        printf("%d ", List[i]);
    }
    printf("\n");
}

void PrintLinkList(LinkList L) {
    while (L->next != NULL) {
        printf("%d ", L->next->data);
        L = L->next;
    }
}

int main() {
    // int data[10] = {10, 20, 30, 70, 90};
    int A[10] = {0, 49, 38, 65, 97, 76, 13, 27, 49};
    // InSertSort(A, 8);
    // BubbleSort(A, 8);
    // SelectSort(A, 8);
    // ShellSort(A, 8);
    // QuickSort(A, 1, 8);
    // HeapSort(A, 8);

    MergeSortSeq(A, 1, 8);
    // LinkList L;
    // InitList(&L, A, 8);
    // MergeSort(L);
    // PrintLinkList(L);
    PrintList(A, 8);
    // printf("%d", Binary_Search(data, 30, 5));
}