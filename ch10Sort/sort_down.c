// 复习, 采用降序排序
#include <stdio.h>

void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 直接插入排序
void InSertSort(int *A, int len) {
    int i, j;
    for (i = 2; i <= len; i++) {
        A[0] = A[i];
        for (j = i; j > 1; j--) {
            if (A[j - 1] < A[0]) {
                A[j] = A[j - 1];
            } else {
                break;
            }
        }
        A[j] = A[0];
    }
}

// 希尔排序
void ShellSort(int *A, int len) {
    for (int d = len / 2; d >= 1; d = d / 2) {
        int i, j;
        for (i = 1 + d; i <= len; i++) {
            A[0] = A[i];
            for (j = i; j > d; j = j - d) {
                if (A[0] > A[j - d]) {
                    A[j] = A[j - d];
                } else {
                    break;
                }
            }
            A[j] = A[0];
        }
    }
}

// 冒泡排序
void BubbleSort(int *A, int len) {
    for (int i = 1; i < len; i++) {
        int flag = 1;
        for (int j = len; j > i; j--) {
            if (A[j] > A[j - 1]) {
                flag = 0;
                Swap(&A[j], &A[j - 1]);
            }
        }
        if (flag == 1) {
            break;
        }
    }
}

// 返回枢轴在一趟快排后的位置
int Portition(int *A, int low, int high) {
    int povit = A[low];
    while (low < high) {
        while (low < high && A[high] <= povit) {
            high--;
        }
        A[low] = A[high];
        while (low < high && A[low] >= povit) {
            low++;
        }
        A[high] = A[low];
    }
    A[low] = povit;
    return low;
}

// 快速排序
void QuickSort(int *A, int low, int high) {
    if (low < high) {
        int povit = Portition(A, low, high);
        QuickSort(A, povit + 1, high);
        QuickSort(A, low, povit - 1);
    }
}

// 简单选择排序
void SelectSort(int *A, int len) {
    for (int i = 1; i <= len; i++) {
        int max = i;
        for (int j = i; j <= len; j++) {
            if (A[j] > A[max]) {
                max = j;
            }
        }
        Swap(&A[i], &A[max]);
    }
}

// 调整以 k 为根的子树
void HeapAdjust(int *A, int k, int len) {
    int i = 2 * k;
    while (i <= len) {
        if (i < len && A[i] > A[i + 1]) {
            i++;
        }
        if (A[k] < A[i]) {
            break;
        }
        Swap(&A[k], &A[i]);
        k = i;
        i = 2 * k;
    }
}

// 创建小根堆
void BuildHeap(int *A, int len) {
    for (int i = len / 2; i >= 1; i--) {
        HeapAdjust(A, i, len);
    }
}

// 堆排序
void HeapSort(int *A, int len) {
    BuildHeap(A, 8);
    for (int i = 1; i < len; i++) {
        Swap(&A[1], &A[len - i + 1]);
        HeapAdjust(A, 1, len - i);
    }
}

// 合并有序数组
int B[10];
void Merge(int *A, int low, int mid, int high) {
    for (int m = low; m <= high; m++) {
        B[m] = A[m];
    }
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
        if (B[i] >= B[j]) {
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

// 归并排序
void MergeSort(int *A, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(A, mid + 1, high);
        MergeSort(A, low, mid);
        Merge(A, low, mid, high);
    }
}

void PrintList(int A[], int len) {
    for (int i = 1; i <= len; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    int A[10] = {0, 49, 38, 65, 97, 76, 13, 27, 49};
    // InSertSort(A, 8);
    // ShellSort(A, 8);
    // BubbleSort(A, 8);
    // QuickSort(A, 1, 8);
    // SelectSort(A, 8);
    // HeapSort(A, 8);
    MergeSort(A, 1, 8);
    PrintList(A, 8);
}
