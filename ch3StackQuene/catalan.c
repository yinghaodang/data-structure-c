// a1,a2,a3...an n个元素按顺序入栈
// 1. 有多少种出栈顺序？
// 2. 如何判断一串序列是否是合法的出栈序列？
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int catalan(int n) {
    if (n <= 1) return 1;

    // 使用递推公式: C(n) = C(n-1) * 2(2n-1) / (n+1)
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result = result * 2 * (2 * i - 1) / (i + 1);
    }
    return (int)result;
}

// 方法1：检查231模式（arr[i] > arr[k] > arr[j]，其中 i < j < k）
// 如果存在这样的三元组，说明arr[i]先出栈，arr[k]后出栈，
// 但arr[j] < arr[k] < arr[i]，说明arr[j]应该在arr[i]和arr[k]之间出栈，
// 但实际arr[j]最晚出栈，矛盾！
// 时间复杂度 O(n³)
int islegal_v1(int *arr, int length) {
    for (int i = 0; i < length - 2; i++) {
        for (int j = i + 1; j < length - 1; j++) {
            for (int k = j + 1; k < length; k++) {
                // 检查231模式：arr[i] > arr[k] > arr[j]
                if (arr[i] > arr[k] && arr[k] > arr[j]) {
                    printf("序列 ");
                    for (int l = 0; l < length; l++) {
                        printf("%d ", arr[l]);
                    }
                    printf("不合法（存在231模式: arr[%d]=%d > arr[%d]=%d > arr[%d]=%d）\n", 
                           i, arr[i], k, arr[k], j, arr[j]);
                    return 0;  // 不合法
                }
            }
        }
    }
    printf("序列 ");
    for (int l = 0; l < length; l++) {
        printf("%d ", arr[l]);
    }
    printf("合法\n");
    return 1;  // 合法
}

// 方法2：使用栈模拟入栈出栈过程
// 时间复杂度 O(n)，空间复杂度 O(n)
int islegal(int *arr, int length) {
    int *stack = (int *)malloc(length * sizeof(int));
    int top = -1;
    int next = 1;  // 下一个要入栈的元素（假设元素是1到length）
    
    for (int i = 0; i < length; i++) {
        // 将小于等于 arr[i] 的所有元素入栈
        while (next <= arr[i]) {
            stack[++top] = next++;
        }
        
        // 检查栈顶是否是当前要出栈的元素
        if (top == -1 || stack[top] != arr[i]) {
            printf("序列 ");
            for (int l = 0; l < length; l++) {
                printf("%d ", arr[l]);
            }
            printf("不合法\n");
            free(stack);
            return 0;
        }
        
        // 出栈
        top--;
    }
    
    printf("序列 ");
    for (int l = 0; l < length; l++) {
        printf("%d ", arr[l]);
    }
    printf("合法\n");
    free(stack);
    return 1;
}

int main() {
    printf("卡特兰数 C(5) = %d\n\n", catalan(5));

    // 测试不同的出栈序列
    printf("=== 测试出栈序列合法性 ===\n");
    
    int arr1[] = {1, 2, 3};
    islegal(arr1, 3);
    
    int arr2[] = {3, 2, 1};
    islegal(arr2, 3);
    
    int arr3[] = {3, 1, 2};  // 不合法序列
    islegal(arr3, 3);
    
    int arr4[] = {2, 3, 1};
    islegal(arr4, 3);
    
    int arr5[] = {1, 3, 2};
    islegal(arr5, 3);
    
    printf("\n=== 使用方法1验证 {3, 1, 2} ===\n");
    islegal_v1(arr3, 3);
}