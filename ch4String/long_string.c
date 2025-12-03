#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct LongString {
    char *p;
    int length;
    int size;
} LongString, *String;

String InitString(int size) {
    String s = (String)malloc(sizeof(LongString));
    s->p = (char *)malloc(sizeof(char) * size);
    s->size = size;
    s->length = 0;
    return s;
}

String InitFromArray(int size, char *array) {
    String s = (String)malloc(sizeof(LongString));
    s->p = (char *)malloc(sizeof(char) * size);
    s->size = size;
    int i = 0;
    while (array[i] != '\0' && i < size) {
        s->p[i] = array[i];
        i++;
    }
    s->length = i;
    return s;
}

void LegalStr(String s) {
    if (!s) {
        printf("字符串 空指针错误");
        exit(1);
    }
}

void FreeString(String s) {
    LegalStr(s);
    free(s->p);
    free(s);
}

// 如果 s1 大返回1, 小返回-1, 相等返回 0
int CompareString(String s1, String s2) {
    LegalStr(s1);
    LegalStr(s2);
    int i = 0;
    while (i < s1->length && i < s2->length) {
        if (s1->p[i] > s2->p[i]) {
            return 1;
        } else if (s1->p[i] < s2->p[i]) {
            return -1;
        }
        i++;
    }
    if (s1->length > s2->length) {
        return 1;
    } else if (s1->length < s2->length) {
        return -1;
    } else {
        return 0;
    }
}

String CopyString(String s) {
    LegalStr(s);
    String newS = InitString(s->size);
    newS->length = s->length;
    for (int i = 0; i < s->length; i++) {
        newS->p[i] = s->p[i];
    }
    return newS;
}

String Concat(String s1, String s2) {
    LegalStr(s1);
    LegalStr(s2);
    if (s1->length + s2->length > s1->size) {
        s1->p = realloc(s1->p, s1->length + s2->length + 5);
        s1->size = s1->length + s2->length + 5;
    }
    for (int i = s1->length; i < s1->length + s2->length; i++) {
        s1->p[i] = s2->p[i - s1->length];
    }
    s1->length = s1->length + s2->length;
    return s1;
}

// 将 s2 覆盖从 index 开始的 s1串
String AssignString(String s1, String s2, int index) {
    LegalStr(s1);
    LegalStr(s2);
    if (index < 0 || index > s1->length) {
        printf("Assign参数不合法");
        exit(1);
    }
    if (s2->length + index > s1->size) {
        s1->p = realloc(s1->p, s2->length + index + 5);
        s1->size = s2->length + index + 5;
    }
    for (int i = index; i < s2->length + index; i++) {
        s1->p[i] = s2->p[i - index];
    }
    s1->length = s2->length + index;
    return s1;
}

// 将 s2 从 s1 的 index 位置开始插入
String InsertString(String s1, String s2, int index) {
    LegalStr(s1);
    LegalStr(s2);
    if (index < 0 || index > s1->length) {
        printf("Insert参数不合法");
        exit(1);
    }
    if (s2->length + s1->length > s1->size) {
        s1->p = realloc(s1->p, s2->length + s1->length + 5);
        s1->size = s1->length + s2->length + 5;
    }
    char p[s1->length - index];
    for (int i = 0; i < s1->length - index; i++) {
        p[i] = s1->p[index + i];
    }
    for (int i = index; i < s2->length + index; i++) {
        s1->p[i] = s2->p[i - index];
    }
    for (int i = index + s2->length; i < s2->length + s1->length; i++) {
        s1->p[i] = p[i - index - s2->length];
    }
    s1->length = s1->length + s2->length;
    return s1;
}

// 字符串从0开始计数, 子串包含start但是不包含end
String SubString(String s, int start, int end) {
    LegalStr(s);
    if (start < 0 || end > s->length || start >= end) {
        printf("截取子串参数不合法");
        exit(1);
    }
    String newS = InitString(s->size);
    for (int i = start; i < end; i++) {
        newS->p[i - start] = s->p[i];
    }
    newS->length = end - start;
    return newS;
}

bool CharExist(char c, char *s) {
    int i = 0;
    while (s[i] != '\0') {
        if (c == s[i]) {
            return true;
        }
        i++;
    }
    return false;
}

// 从字符串的左右两边开始移除在 t 中存在的字符
String TrimString(String s, char *t) {
    LegalStr(s);
    int left = 0, right = s->length - 1;

    // 从左边开始移除
    while (left <= right && CharExist(s->p[left], t)) {
        left++;
    }
    // 从右边开始移除
    while (right >= left && CharExist(s->p[right], t)) {
        right--;
    }

    int newLen = right - left + 1;
    for (int j = 0; j < newLen; j++) {
        s->p[j] = s->p[left + j];
    }
    s->length = newLen;
    return s;
}

// 返回模式串开始匹配的位置 (字符串从0开始计数)
int LocateString(String s, String p) {
    LegalStr(s);
    LegalStr(p);
    if (s->length < p->length) {
        return -1;
    }

    int j = 0, i = 0, count = 0;
    while (j < p->length) {
        if (s->p[i] == p->p[j]) {
            i++;
            j++;
        } else {
            j = 0;
            count++;
            i = count;
        }
    }
    if (j >= p->length) {
        return count;
    }
    return -1;  // 没找到返回 -1
}

int GetNext(String p, int *next) {
    LegalStr(p);
    if (!next) {
        exit(1);
    }
    int i = 0, j = -1;
    next[0] = -1;
    while (i < p->length) {
        if (j == -1 || p->p[i] == p->p[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }

    // 优化的next数组
    for (int i = 0; i < p->length; i++) {
        if (p->p[i] == p->p[next[i]]) {
            next[i] = next[next[i]];
        }
    }

    return 1;
}

int LocateStringKMP(String s, String p, int *next) {
    int i = 0, j = 0;
    while (j < p->length && i < s->length) {
        // printf("i=%d, j=%d\n", i, j);
        if (j == -1 || s->p[i] == p->p[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    if (j >= p->length) {
        return i - j;
    } else {
        return -1;
    }
}

String ClearString(String s) {
    LegalStr(s);
    free(s->p);
    s->length = 0;
    s->p = NULL;
    s->size = 0;
    return s;
}

String ReSetString(String s, char *array) {
    LegalStr(s);
    int i = 0;
    while (array[i] != '\0' && i < s->size) {
        s->p[i] = array[i];
        i++;
    }
    s->length = i;
    return s;
}

void PrintString(String s) {
    LegalStr(s);
    for (int i = 0; i < s->length; i++) {
        printf("%c", s->p[i]);
    }
    printf("\n");
}

int main() {
    String s = InitFromArray(20, "acabaabaabcacaabc");
    String p = InitFromArray(10, "abaabcac");

    int next[p->length];
    GetNext(p, next);
    printf("打印Next数组:\n");
    for (int i = 0; i < p->length; i++) {
        printf("%2c ", p->p[i]);
    }
    printf("\n");
    for (int i = 0; i < p->length; i++) {
        printf("%2d ", next[i] + 1);
    }
    printf("\n");

    printf("朴素定位法定位结果: %d\n", LocateString(s, p));
    printf("KMP定位法定位结果: %d\n", LocateStringKMP(s, p, next));

    PrintString(p);
    PrintString(SubString(p, 0, p->length));
    printf("字符串比较结果: %d\n",
           CompareString(p, SubString(p, 0, p->length)));
    FreeString(s);
    FreeString(p);

    String s1 = InitFromArray(10, "Hello, ");
    String s2 = InitFromArray(10, "World!");

    PrintString(Concat(s1, s2));  // 会修改s1的值
    ReSetString(s1, "Hello, ");

    PrintString(AssignString(s1, s2, s1->length));  // 会修改s1的值
    ReSetString(s1, "Hello, ");

    PrintString(InsertString(s1, s2, s1->length));  // 会修改s1的值
    ReSetString(s1, "Hello, ");

    FreeString(s1);
    FreeString(s2);

    String s3 = InitFromArray(30, "b aHello, ab");
    PrintString(TrimString(s3, "a b"));
    ReSetString(s3, "b aHello, ab");
    FreeString(s3);
}