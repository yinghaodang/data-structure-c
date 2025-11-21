// 书本中的实现 from Claude
#include <ctype.h>
#include <stdio.h>

#define MAXSIZE 100
#define OPSetSize 7
char OPSet[OPSetSize] = {'+', '-', '*', '/', '(', ')', '#'};
char Prior[7][7] = {
    {'>', '>', '<', '<', '<', '>', '>'}, {'>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '>', '>'}, {'>', '>', '>', '>', '<', '>', '>'},
    {'<', '<', '<', '<', '<', '=', ' '}, {'>', '>', '>', '>', ' ', '>', '>'},
    {'<', '<', '<', '<', '<', ' ', '='},
};

// 字符栈（操作符栈）
typedef struct {
    char data[MAXSIZE];
    int top;
} CharStack;

// 浮点数栈（操作数栈）
typedef struct {
    float data[MAXSIZE];
    int top;
} FloatStack;

void InitCharStack(CharStack *S) { S->top = -1; }

void InitFloatStack(FloatStack *S) { S->top = -1; }

void PushChar(CharStack *S, char c) { S->data[++S->top] = c; }

void PushFloat(FloatStack *S, float f) { S->data[++S->top] = f; }

char PopChar(CharStack *S) { return S->data[S->top--]; }

float PopFloat(FloatStack *S) { return S->data[S->top--]; }

char GetTopChar(CharStack *S) { return S->data[S->top]; }

float Operate(float a, char theta, float b) {
    if (theta == '+') {
        return a + b;
    } else if (theta == '-') {
        return a - b;
    } else if (theta == '*') {
        return a * b;
    } else if (theta == '/') {
        return a / b;
    }
    return 0;
}

int InOPSet(char c) {
    for (int i = 0; i < OPSetSize; i++) {
        if (c == OPSet[i]) {
            return 1;
        }
    }
    return 0;
}

int OpOrd(char op) {
    for (int i = 0; i < OPSetSize; i++) {
        if (op == OPSet[i]) {
            return i;
        }
    }
    return 0;
}

char precede(char op1, char op2) { return Prior[OpOrd(op1)][OpOrd(op2)]; }

float EvaluateExpression(char *expression) {
    CharStack OPTR;   // 操作符栈
    FloatStack OPND;  // 操作数栈
    InitCharStack(&OPTR);
    InitFloatStack(&OPND);

    PushChar(&OPTR, '#');  // 表达式起始符号
    char *p = expression;
    char c = *p;

    while (c != '#' || GetTopChar(&OPTR) != '#') {
        // 判断 '-' 是负号还是减号：如果前面是操作符、左括号、'#' 或表达式开始，则是负号
        int is_negative_sign = (c == '-' && (p == expression || *(p-1) == '(' || 
                                *(p-1) == '#' || *(p-1) == '+' || *(p-1) == '-' || 
                                *(p-1) == '*' || *(p-1) == '/'));
        
        if (isdigit(c) || is_negative_sign) {
            // 读取完整的数字（支持整数、小数和负数）
            float num = 0;
            int is_negative = 0;
            
            // 处理负号
            if (c == '-') {
                is_negative = 1;
                c = *(++p);
            }
            
            // 读取整数部分
            while (isdigit(c)) {
                num = num * 10 + (c - '0');
                c = *(++p);
            }
            // 读取小数部分
            if (c == '.') {
                c = *(++p);
                float decimal = 0.1;
                while (isdigit(c)) {
                    num = num + (c - '0') * decimal;
                    decimal *= 0.1;
                    c = *(++p);
                }
            }
            
            // 应用负号
            if (is_negative) {
                num = -num;
            }
            
            PushFloat(&OPND, num);
        } else if (InOPSet(c)) {
            // 是操作符
            char topOp = GetTopChar(&OPTR);
            char priority = precede(topOp, c);

            if (priority == '<') {
                // 栈顶优先级低，当前操作符入栈
                PushChar(&OPTR, c);
                c = *(++p);
            } else if (priority == '>') {
                // 栈顶优先级高，弹出栈顶操作符进行运算
                char theta = PopChar(&OPTR);
                float b = PopFloat(&OPND);
                float a = PopFloat(&OPND);
                PushFloat(&OPND, Operate(a, theta, b));
            } else if (priority == '=') {
                // 优先级相等（括号匹配），弹出栈顶
                PopChar(&OPTR);
                c = *(++p);
            }
        } else {
            // 跳过空格等其他字符
            c = *(++p);
        }
    }

    return PopFloat(&OPND);
}

int main() {
    char expression1[] = "3+4*2#";
    printf("3+4*2 = %.2f\n", EvaluateExpression(expression1));

    char expression2[] = "(3+4)*2#";
    printf("(3+4)*2 = %.2f\n", EvaluateExpression(expression2));

    char expression3[] = "10+20/2-5*3#";
    printf("10+20/2-5*3 = %.2f\n", EvaluateExpression(expression3));

    // 测试小数
    char expression4[] = "3.5+2.5*4#";
    printf("3.5+2.5*4 = %.2f\n", EvaluateExpression(expression4));
    printf("Expected: %.2f\n", 3.5 + 2.5 * 4);

    char expression5[] = "10.25/2.5+1.5#";
    printf("10.25/2.5+1.5 = %.2f\n", EvaluateExpression(expression5));
    printf("Expected: %.2f\n", 10.25 / 2.5 + 1.5);

    char expression6[] = "(1.5+2.5)*3.2#";
    printf("(1.5+2.5)*3.2 = %.2f\n", EvaluateExpression(expression6));
    printf("Expected: %.2f\n", (1.5 + 2.5) * 3.2);

    char expression7[] = "-10+20.0/2-5.0/3+(-6)*2#";
    printf("-10+20.0/2-5.0/3+(-6)*2 = %.2f\n", EvaluateExpression(expression7));
    printf("Expected: %.2f\n\n", -10 + 20.0 / 2 - 5.0 / 3 + (-6) * 2);

    // 更多负数测试
    char expression8[] = "-5*-3#";
    printf("-5*-3 = %.2f\n", EvaluateExpression(expression8));
    printf("Expected: %.2f\n\n", -5.0 * -3.0);
    
    char expression9[] = "(-2.5+3.5)*(-4)#";
    printf("(-2.5+3.5)*(-4) = %.2f\n", EvaluateExpression(expression9));
    printf("Expected: %.2f\n\n", (-2.5 + 3.5) * (-4));
    
    char expression10[] = "-1.5--2.5#";
    printf("-1.5--2.5 = %.2f\n", EvaluateExpression(expression10));
    printf("Expected: %.2f\n", -1.5 - (-2.5));

    return 0;
}