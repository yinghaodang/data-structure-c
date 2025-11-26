#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 字符串段栈结构
typedef struct {
    char **segments;  // 存储路径段的指针数组
    int top;
    int capacity;
} SegmentStack;

SegmentStack *InitSegmentStack() {
    SegmentStack *s = (SegmentStack *)malloc(sizeof(SegmentStack));
    s->capacity = 100;  // 初始容量100个路径段，按需扩展
    s->segments = (char **)malloc(s->capacity * sizeof(char *));
    s->top = -1;
    return s;
}

void Push(SegmentStack *s, char *segment) {
    if (s->top + 1 >= s->capacity) {
        s->capacity *= 2;
        s->segments =
            (char **)realloc(s->segments, s->capacity * sizeof(char *));
    }
    s->segments[++s->top] = segment;
}

void Pop(SegmentStack *s) {
    if (s->top >= 0) {
        free(s->segments[s->top--]);
    }
}

// char *simplifyPath(char *path) {
//     SegmentStack *stack = InitSegmentStack();
//     int len = strlen(path);

//     // 解析路径段
//     int i = 0;
//     while (i < len) {
//         // 跳过连续的 '/'
//         while (i < len && path[i] == '/') {
//             i++;
//         }

//         if (i >= len) break;

//         // 提取一个路径段
//         int start = i;
//         while (i < len && path[i] != '/') {
//             i++;
//         }

//         int segLen = i - start;
//         char *segment = (char *)malloc((segLen + 1) * sizeof(char));
//         strncpy(segment, path + start, segLen);
//         segment[segLen] = '\0';

//         // 处理路径段
//         if (strcmp(segment, "..") == 0) {
//             // 返回上级目录
//             Pop(stack);
//             free(segment);
//         } else if (strcmp(segment, ".") == 0) {
//             // 当前目录，忽略
//             free(segment);
//         } else {
//             // 普通目录名
//             Push(stack, segment);
//         }
//     }

//     // 构建结果字符串
//     if (stack->top < 0) {
//         // 空栈，返回根目录
//         char *result = (char *)malloc(2 * sizeof(char));
//         strcpy(result, "/");
//         free(stack->segments);
//         free(stack);
//         return result;
//     }

//     // 计算结果长度
//     int resultLen = 0;
//     for (int j = 0; j <= stack->top; j++) {
//         resultLen += strlen(stack->segments[j]) + 1;  // +1 for '/'
//     }

//     // 构建结果
//     char *result = (char *)malloc((resultLen + 1) * sizeof(char));
//     result[0] = '\0';
//     for (int j = 0; j <= stack->top; j++) {
//         strcat(result, "/");
//         strcat(result, stack->segments[j]);
//         free(stack->segments[j]);
//     }

//     free(stack->segments);
//     free(stack);
//     return result;
// }

char *simplifyPath(char *path) {
    int len = strlen(path);
    char *stack = (char *)malloc((len + 1) * sizeof(char));
    int top = 0;

    stack[top++] = '/';  // 根目录始终存在

    int i = 0;
    while (path[i] != '\0') {
        // 跳过所有 '/'
        while (path[i] == '/') {
            i++;
        }

        if (path[i] == '\0') break;

        // 提取路径段
        int start = i;
        while (path[i] != '\0' && path[i] != '/') {
            i++;
        }
        int segLen = i - start;

        // 处理路径段
        if (segLen == 2 && path[start] == '.' && path[start + 1] == '.') {
            // ".." 返回上级
            if (top > 1) {  // 不能回退到根目录之前
                top--;      // 移除最后的 '/'
                while (top > 1 && stack[top - 1] != '/') {
                    top--;  // 移除目录名
                }
            }
        } else if (segLen == 1 && path[start] == '.') {
            // "." 忽略
            continue;
        } else {
            // 普通目录
            if (top > 1) {  // 不是根目录后的第一个，需要加 '/'
                stack[top++] = '/';
            }
            for (int j = 0; j < segLen; j++) {
                stack[top++] = path[start + j];
            }
        }
    }

    // 处理尾部 '/'
    if (top > 1 && stack[top - 1] == '/') {
        top--;
    }

    // 空路径返回 '/'
    if (top == 0) {
        top = 1;
    }

    stack[top] = '\0';
    return stack;
}

int main() {
    printf("%s\n", simplifyPath("/home/"));
    printf("%s\n", simplifyPath("/home//foo/"));
    printf("%s\n", simplifyPath("/home/user/Documents/../Pictures"));
    printf("%s\n", simplifyPath("/../"));
    printf("%s\n", simplifyPath("/.../a/../b/c/../d/./"));
    printf("%s\n", simplifyPath("/a//b////c/d//././/.."));
    printf("%s\n", simplifyPath("/hello../world"));
    printf("%s\n", simplifyPath("/...../../home"));
    return 0;
}