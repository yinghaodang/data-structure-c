// 留到以后删掉练习吧, Claude写的还是有错的
// Claude出手了, 这个比较消耗时间
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define Maxsize 100
#define INFEASIBLE -1
#define OVERSTACK -2

// Char类型栈 - 用于存储操作符
typedef struct CharStackNode {
    int top;
    char *data;
} CharStackNode, *CharStack;

// Int类型栈 - 用于存储操作数
typedef struct IntStackNode {
    int top;
    int *data;
} IntStackNode, *IntStack;

// 辅助函数
bool isOperand(char c) { return isdigit(c); }

bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

// 比较操作符优先级,返回true代表前者比后者优先级更高或相等
// 返回为0代表前者比后者优先级更低
// c1 只能是 + - * /
// c2 可能是 + - * / ( $
// c2 为$ 标记c2是空的字符, c1优先级更高
// ( 的优先级高于一切
bool comparePrecedence(char c1, char c2) {
    if (c2 == '(') {
        return false;
    } else if (c2 == '$') {
        return true;
    }

    bool b1 = c1 == '*' || c1 == '/';
    bool b2 = c1 == '+' || c1 == '-';
    bool b3 = c2 == '*' || c2 == '/';
    bool b4 = c2 == '+' || c2 == '-';
    if (b1) {
        return true;
    } else if (b2 && b3) {
        return false;
    } else if (b2 && b4) {
        return false;
    }
    printf("比较优先级参数不合法c1: %c,c2: %c ", c1, c2);
    exit(INFEASIBLE);
}

int evalSimple(int pre_value, char operator, int post_value) {
    if (operator== '+') {
        return pre_value + post_value;
    } else if (operator== '-') {
        return pre_value - post_value;
    } else if (operator== '*') {
        return pre_value * post_value;
    } else if (operator== '/' && post_value != 0) {
        return pre_value / post_value;
    }
    printf("简单运算参数不合法pre_value: %d, operator: %c, post_value: %d",
           pre_value, operator, post_value);
    exit(INFEASIBLE);
}

// ========== Char栈操作 ==========
CharStack InitCharStack() {
    CharStack S = (CharStack)malloc(sizeof(CharStackNode));
    if (!S) {
        printf("内存分配失败\n");
        exit(OVERSTACK);
    }
    S->data = (char *)malloc(sizeof(char) * Maxsize);
    if (!S->data) {
        printf("内存分配失败\n");
        exit(OVERSTACK);
    }
    S->top = -1;
    return S;
}

CharStack PushChar(CharStack S, char c) {
    if (S->top >= Maxsize - 1) {
        return NULL;
    }
    S->data[++S->top] = c;
    return S;
}

char PopChar(CharStack S) {
    if (S->top <= -1) {
        printf("在空栈上pop\n");
        exit(INFEASIBLE);
    }
    return S->data[S->top--];
}

char GetChar(CharStack S) {
    if (S->top <= -1) {
        return '$';  // 标记空栈
    }
    return S->data[S->top];
}

// ========== Int栈操作 ==========
IntStack InitIntStack() {
    IntStack S = (IntStack)malloc(sizeof(IntStackNode));
    if (!S) {
        printf("内存分配失败\n");
        exit(OVERSTACK);
    }
    S->data = (int *)malloc(sizeof(int) * Maxsize);
    if (!S->data) {
        printf("内存分配失败\n");
        exit(OVERSTACK);
    }
    S->top = -1;
    return S;
}

IntStack PushInt(IntStack S, int value) {
    if (S->top >= Maxsize - 1) {
        return NULL;
    }
    S->data[++S->top] = value;
    return S;
}

int PopInt(IntStack S) {
    if (S->top <= -1) {
        printf("在空栈上pop\n");
        exit(INFEASIBLE);
    }
    return S->data[S->top--];
}

int GetInt(IntStack S) {
    if (S->top <= -1) {
        return 0;  // 标记空栈
    }
    return S->data[S->top];
}

int evalInfix(char *p) {
    if (p == NULL) {
        printf("传入空指针");
        exit(INFEASIBLE);
    }

    int i = 0;  // 标记字符串位置
    CharStack operator_stack = InitCharStack();
    IntStack operand_stack = InitIntStack();
    while (p[i] != '\0') {
        if (isOperator(p[i])) {
            // 比较当前操作符 和 操作符栈栈顶操作符 的优先级
            // 如果当前操作符优先级小于等于栈顶操作符
            while (GetChar(operator_stack) != '(' &&
                   !comparePrecedence(p[i], GetChar(operator_stack))) {  // 这个while其实只会执行一层
                char operator= PopChar(operator_stack);
                int post_value = PopInt(operand_stack);
                int pre_value = PopInt(operand_stack);
                PushInt(operand_stack,
                        evalSimple(pre_value, operator, post_value));
            }
            PushChar(operator_stack, p[i]);
        } else if (isOperand(p[i])) {
            PushInt(operand_stack, p[i] - '0');  // 将字符转换为int值
        } else if (p[i] == '(') {
            PushChar(operator_stack, p[i]);
        } else {  // 理论上只有可能是 ')'
            while (GetChar(operator_stack) != '(') {
                char operator= PopChar(operator_stack);
                int post_value = PopInt(operand_stack);
                int pre_value = PopInt(operand_stack);
                PushInt(operand_stack,
                        evalSimple(pre_value, operator, post_value));
            }
            PopChar(operator_stack);
        }
        i++;
    }

    while (operator_stack->top >= 0) {
        char operator= PopChar(operator_stack);
        int post_value = PopInt(operand_stack);
        int pre_value = PopInt(operand_stack);
        PushInt(operand_stack, evalSimple(pre_value, operator, post_value));
    }
    return GetInt(operand_stack);
}

int main() {
    char *p = "0-3*4-5/1";
    printf("%d", evalInfix(p));
}