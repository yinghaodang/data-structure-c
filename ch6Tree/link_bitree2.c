// 先复习二叉树的创建和遍历, 再学习线索化
// 创建节点上存储 字符 的线索二叉链表
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct BiTNode {
    char data;
    struct BiTNode* left;
    struct BiTNode* right;
} BiTNode, *BiTree;
typedef struct BiThreadTNode {
    char data;
    struct BiThreadTNode* left;
    struct BiThreadTNode* right;
    int ltag, rtag;
} BiThreadTNode, *BiThreadTree;

// ==================== 构造二叉树 ====================
BiTree InitBiTree() {
    BiTree root = (BiTree)malloc(sizeof(BiTNode));
    root->data = '\0';
    root->left = NULL;
    root->right = NULL;
    return root;
}

// 书本中的实现, 使用了全局变量 i
int i = 0;
BiTree CreateBiTreeBook(char* str) {
    if (str[i] == ' ') {
        return NULL;
    }
    BiTree tree = InitBiTree();
    tree->data = str[i++];
    tree->left = CreateBiTreeBook(str);
    i++;
    tree->right = CreateBiTreeBook(str);
    return tree;
}

BiTree CreateBiTree(char* str, int* i) {
    if (str[*i] == ' ') {
        return NULL;
    }
    BiTree tree = InitBiTree();
    tree->data = str[(*i)++];
    tree->left = CreateBiTree(str, i);
    (*i)++;
    tree->right = CreateBiTree(str, i);
    return tree;
}

int j = 14;  // 后序遍历字符串的长度
BiTree CreateBiTreePost(char* str) {
    if (str[j] == ' ') {
        return NULL;
    }
    BiTree tree = InitBiTree();
    tree->data = str[j--];
    tree->right = CreateBiTreePost(str);
    j--;
    tree->left = CreateBiTreePost(str);
    return tree;
}

BiTree CreateBiTreeNoRec(char* str) {
    if (str == NULL || str[0] == ' ') {
        return NULL;
    }

    // 定义栈元素结构：保存节点和处理状态
    typedef struct {
        BiTree node;
        int state;  // 0: 下一步处理左子树, 1: 下一步处理右子树
    } StackItem;

    StackItem stack[100];  // 栈
    int top = -1;          // 栈顶指针
    int i = 0;             // 字符串索引

    // 创建根节点
    BiTree root = InitBiTree();
    root->data = str[i++];

    // 根节点入栈，状态为0（待处理左子树）
    stack[++top] = (StackItem){root, 0};

    while (top >= 0 && str[i] != '\0') {
        if (stack[top].state == 0) {
            // 当前状态：处理左子树
            stack[top].state = 1;  // 更新状态，下次处理右子树

            if (str[i] == ' ') {
                // 空节点
                stack[top].node->left = NULL;
                i++;
            } else {
                // 创建左孩子并入栈
                BiTree leftChild = InitBiTree();
                leftChild->data = str[i++];
                stack[top].node->left = leftChild;
                stack[++top] = (StackItem){leftChild, 0};
            }
        } else {
            // 当前状态：处理右子树
            BiTree parent = stack[top].node;
            top--;  // 当前节点处理完毕，出栈

            if (str[i] == ' ') {
                // 空节点
                parent->right = NULL;
                i++;
            } else {
                // 创建右孩子并入栈
                BiTree rightChild = InitBiTree();
                rightChild->data = str[i++];
                parent->right = rightChild;
                stack[++top] = (StackItem){rightChild, 0};
            }
        }
    }

    return root;
}

// ==================== 遍历二叉树 ====================
// 递归先序遍历
void PreTrav(BiTree tree) {
    if (tree == NULL) {
        return;
    }
    printf("%c ", tree->data);
    PreTrav(tree->left);
    PreTrav(tree->right);
}

// 非递归中序遍历
void InTravNoRec(BiTree tree) {
    BiTNode* stack[20];
    int top = -1;
    BiTNode* p = tree;

    while (p != NULL || top >= 0) {
        if (p != NULL) {
            stack[++top] = p;  // 非递归先序遍历在这里打印
            p = p->left;
        } else {
            p = stack[top--];
            printf("%c ", p->data);
            p = p->right;
        }
    }
}

// 非递归后序遍历
void PostTravNoRec(BiTree tree) {
    BiTNode* stack[20];
    int top = -1;       // 栈指针
    BiTNode* r = NULL;  // 标记已经访问过的右子树
    BiTNode* p = tree;  // 树指针

    while (p != NULL || top >= 0) {
        if (p != NULL) {
            stack[++top] = p;
            p = p->left;
        } else {
            p = stack[top];
            if (p->right && p->right != r) {
                p = p->right;
            } else {
                printf("%c ", p->data);
                top--;
                r = p;
                p = NULL;
            }
        }
    }
}

// ==================== 递归获取节点的前驱 ====================
char c = 'E';  // 寻找节点E的前驱
BiTNode* pre = NULL;
void GetPrede(BiTNode* p) {
    if (p->data == c) {
        if (pre != NULL) {
            printf("E节点前驱是%c\n", pre->data);
            return;
        } else {
            printf("E节点前驱是#\n");
            return;
        }
    } else {
        pre = p;
    }
}

void PreGetPrede(BiTree tree, void (*visit)(BiTNode*)) {
    if (tree == NULL) {
        return;
    }
    visit(tree);
    PreGetPrede(tree->left, visit);
    PreGetPrede(tree->right, visit);
}

void InGetPrede(BiTree tree, void (*visit)(BiTNode*)) {
    if (tree == NULL) {
        return;
    }
    InGetPrede(tree->left, visit);
    visit(tree);
    InGetPrede(tree->right, visit);
}

void PostGetPrede(BiTree tree, void (*visit)(BiTNode*)) {
    if (tree == NULL) {
        return;
    }
    PostGetPrede(tree->left, visit);
    PostGetPrede(tree->right, visit);
    visit(tree);
}

// 使用栈获取中序前驱
char InGetPredeNoRec(BiTree tree, char c) {
    BiTNode* stack[20];
    int top = -1;
    BiTNode* p = tree;
    BiTNode* pre = NULL;  // p的前驱节点

    while (p != NULL || top >= 0) {
        if (p != NULL) {
            stack[++top] = p;
            p = p->left;
        } else {
            p = stack[top--];
            if (p->data == c) {
                if (pre != NULL) {
                    return pre->data;
                } else {
                    return '#';  // 代表输入的是中序序列的首字符
                }
            }
            pre = p;
            p = p->right;
        }
    }
    return '$';  // 代表输入的字符没有节点匹配上
}

// ==================== 构造线索二叉树 ================
BiThreadTree BuildTreadBiTree(BiTree tree) {
    if (tree == NULL) {
        return NULL;
    }
    BiThreadTree root = (BiThreadTree)malloc(sizeof(BiThreadTNode));
    root->ltag = 0;
    root->rtag = 0;
    root->data = tree->data;
    root->left = BuildTreadBiTree(tree->left);
    root->right = BuildTreadBiTree(tree->right);
    return root;
}

BiThreadTNode* thread_pre = NULL;
void Tread(BiThreadTNode* p) {
    if (p->left == NULL) {
        p->left = thread_pre;
        p->ltag = 1;
    }
    if (thread_pre != NULL && thread_pre->right == NULL) {
        thread_pre->right = p;
        thread_pre->rtag = 1;
    }
    thread_pre = p;
}

void Transform(BiThreadTree tree, void (*visit)(BiThreadTNode* p)) {
    if (tree == NULL) {
        return;
    }
    Transform(tree->left, visit);
    visit(tree);
    Transform(tree->right, visit);
}

void CreateInThread(BiThreadTree tree) {
    thread_pre = NULL;  // 重置全局变量
    if (tree != NULL) {
        Transform(tree, Tread);
        // 处理最后一个节点的后继
        if (thread_pre->right == NULL) {
            thread_pre->rtag = 1;
        }
    }
}

BiThreadTNode* InGetPredecessor(BiThreadTNode* p) {
    if (p == NULL) {
        return NULL;
    }
    if (p->ltag == 1) {
        return p->left;
    }
    if (p->left == NULL) {
        return NULL;
    }
    p = p->left;
    while (p->rtag == 0) {
        p = p->right;
    }
    return p;
}

BiThreadTNode* InGetSuccessor(BiThreadTNode* p) {
    if (p == NULL) {
        return NULL;
    }
    if (p->rtag == 1) {
        return p->right;
    }
    if (p->right == NULL) {
        return NULL;
    }
    p = p->right;
    while (p->ltag == 0) {
        p = p->left;
    }
    return p;
}

void PrintInOrder(BiThreadTree p) {
    if (p == NULL) return;

    // 这里 p 是根节点
    while (p->ltag == 0) {
        p = p->left;
    }
    while (p != NULL) {
        printf("%c ", p->data);
        p = InGetSuccessor(p);
    }
}

// ==================== 垂直打印树 ====================
#define MAX_HEIGHT 10
#define MAX_WIDTH 256

// 获取树的高度
int GetHeight(BiTree root) {
    if (root == NULL) return 0;
    int left = GetHeight(root->left);
    int right = GetHeight(root->right);
    return (left > right ? left : right) + 1;
}

// 将节点填充到二维数组中
// row: 当前行（节点行，连接线在 row+1）
// left, right: 当前子树可用的水平范围
void FillTreeArray(BiTree root, char arr[][MAX_WIDTH], int row, int left,
                   int right) {
    if (root == NULL) return;

    int mid = (left + right) / 2;
    arr[row][mid] = root->data;  // 放置节点

    if (root->left) {
        // 左连接线：从 mid-1 向左画到子节点位置
        int leftChildPos = (left + mid - 1) / 2;
        arr[row + 1][mid - 1] = '/';
        // 画水平延伸线（可选，让树更清晰）
        for (int i = mid - 2; i > leftChildPos; i--) {
            arr[row + 1][i] = '_';
        }
        FillTreeArray(root->left, arr, row + 2, left, mid - 1);
    }

    if (root->right) {
        // 右连接线：从 mid+1 向右画到子节点位置
        int rightChildPos = (mid + 1 + right) / 2;
        arr[row + 1][mid + 1] = '\\';
        // 画水平延伸线（可选）
        for (int i = mid + 2; i < rightChildPos; i++) {
            arr[row + 1][i] = '_';
        }
        FillTreeArray(root->right, arr, row + 2, mid + 1, right);
    }
}

void PrintTree(BiTree tree) {
    if (tree == NULL) {
        printf("(空树)\n");
        return;
    }

    int height = GetHeight(tree);
    int rows = height * 2 - 1;      // 节点行 + 连接线行
    int width = (1 << height) * 2;  // 2^height * 2，足够宽

    if (width > MAX_WIDTH) width = MAX_WIDTH;

    // 创建并初始化二维数组
    char arr[MAX_HEIGHT * 2][MAX_WIDTH];
    for (int i = 0; i < rows; i++) {
        memset(arr[i], ' ', width);
        arr[i][width - 1] = '\0';
    }

    // 填充树到数组
    FillTreeArray(tree, arr, 0, 0, width - 2);

    // 打印每一行，去除尾部空格
    for (int i = 0; i < rows; i++) {
        int len = width - 2;
        while (len >= 0 && arr[i][len] == ' ') len--;
        if (len >= 0) {
            arr[i][len + 1] = '\0';
            printf("%s\n", arr[i]);
        }
    }
}

int main() {
    int m = 0;
    BiTree tree = CreateBiTree("ABC  DE G  F   ", &m);
    PrintTree(tree);

    BiTree tree2 = CreateBiTreePost("  C   GE  FDB A");
    PrintTree(tree2);

    BiTree tree3 = CreateBiTreeNoRec("ABC  DE G  F   ");
    PrintTree(tree3);

    printf("先序遍历: ");
    PreTrav(tree3);
    printf("\n");

    printf("中序遍历: ");
    InTravNoRec(tree3);
    printf("\n");

    printf("后序遍历: ");
    PostTravNoRec(tree3);
    printf("\n");

    printf("\n非递归寻找中序遍历前驱: \n");
    printf("E节点前驱是%c\n", InGetPredeNoRec(tree3, 'E'));
    printf("C节点前驱是%c\n", InGetPredeNoRec(tree3, 'C'));
    printf("B节点前驱是%c\n", InGetPredeNoRec(tree3, 'B'));
    printf("A节点前驱是%c\n", InGetPredeNoRec(tree3, 'A'));
    printf("V节点前驱是%c\n", InGetPredeNoRec(tree3, 'V'));

    printf("\n递归寻找先序遍历前驱: \n");
    PreGetPrede(tree3, GetPrede);

    printf("\n递归寻找中序遍历前驱: \n");
    InGetPrede(tree3, GetPrede);

    printf("\n递归寻找后序遍历前驱: \n");
    PostGetPrede(tree3, GetPrede);

    // 测试线索二叉树
    BiThreadTree thread_tree3 = BuildTreadBiTree(tree3);
    CreateInThread(thread_tree3);
    printf("\n中序线索二叉树遍历:\n");
    PrintInOrder(thread_tree3);
}