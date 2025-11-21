// 使用顺序栈实现 符号匹配
#include <stdbool.h>
#include <stdio.h>

#define MAX_STACK_SIZE 100

// 辅助函数,判断两边是否是匹配的
bool isMatchingPair(char left, char right) {
    return (left == '(' && right == ')') || (left == '[' && right == ']') ||
           (left == '{' && right == '}');
}

void BracketMatch(char *p) {
    char stack[MAX_STACK_SIZE];
    int i = 0;
    int top = -1;  // 栈顶索引

    while (p[i] != '\0') {
        if (p[i] == '{' || p[i] == '[' || p[i] == '(') {
            // 检查栈是否溢出
            if (top >= MAX_STACK_SIZE - 1) {
                printf("栈溢出,表达式过长\n");
                return;
            }
            stack[++top] = p[i];
        }
        // 右括号匹配
        else if (p[i] == '}' || p[i] == ')' || p[i] == ']') {
            // 检查栈是否为空
            if (top == -1) {
                printf("符号不匹配: 右括号 '%c' 没有与之匹配的左括号.", p[i]);
                return;
            }
            // 检查是否匹配
            if (!isMatchingPair(stack[top], p[i])) {
                printf("符号不匹配：'%c' 与 '%c' 不匹配\n", stack[top], p[i]);
                return;
            }
            top--;  // 匹配成功，出栈
        }
        i++;
    }

    // 检查栈是否为空
    if (top == -1) {
        printf("符号匹配\n");
    } else {
        printf("符号不匹配：有 %d 个左括号未闭合\n", top + 1);
    }
}

int main() {
    BracketMatch("{(a+b)*c}[]");
    BracketMatch("(a+b))");  // 测试多余的右括号
    BracketMatch("((a+b)");  // 测试多余的左括号
    BracketMatch("{(a+b]");  // 测试不匹配的括号
    return 0;
}