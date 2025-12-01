// 使用最朴素的字符列表
#include <stdio.h>
#include <string.h>

void getNext(char *P, int *next, int m) {
    int i = 0, j = -1;
    next[0] = -1;

    while (i < m - 1) {
        if (j == -1 || P[i] == P[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];  // 回溯
        }
    }
}

int KMP(char *S, char *P, int *next) {
    int i = 0, j = 0;
    int n = strlen(S);
    int m = strlen(P);

    int count = 1;
    while (i < n && j < m) {
        if (j == -1 || S[i] == P[j]) {
            i++;
            j++;
        } else {
            j = next[j];  // 模式串右移，主串不回溯
        }
        printf("count %d  ", count);
        printf("i=%d, j=%d", i, j);
        printf("\n");
        count++;
    }

    if (j == m) {
        return i - m;  // 匹配成功，返回起始位置
    } else {
        return -1;  // 匹配失败
    }
}

int main() {
    char p1[20] = "ababcabcacbab";
    char p2[20] = "abcac";
    int next[5];
    getNext(p2, next, 5);
    printf("%d", KMP(p1, p2, next));
    return 0;
}