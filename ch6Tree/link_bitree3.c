// 二叉链表, 复习三种非递归遍历, 讨论时间复杂度
// 非递归实现二叉树线索化
// 前中后序线索二叉树的遍历
// 命名贴近教材
// 思来想去, 二叉树会额外创建一个头节点没啥软用, 硬要有用只有先序线索找前驱,
// 后序线索找后继. 但是实用性不大, 因此代码实现的二叉链表依旧是不含头节点的.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BiTNode {
    char data;
    struct BiTNode* lchild;
    struct BiTNode* rchild;
} BiTNode, *BiTree;
// 教材中的名称
typedef enum { Link, Thread } ThrTag;  // Link(0): 指针; Thread(1): 线索
typedef struct BiThrNode {
    char data;
    struct BiThrNode *lchild, *rchild;  // 可以并排写一起, 但是 * 号不能丢
    ThrTag LTag, RTag;
} BiThrNode, *BiThrTree;

// ==================== 复习二叉树创建, 遍历 ====================
BiThrNode* preThrNode = NULL;  // 全局变量, 辅助线索化, 指向指针刚刚访问的节点
int offset = 0;                // 全局变量, 辅助创建二叉树, 指示先序序列的偏移量

BiTree BuildBiTree(char* p) {
    if (p[offset] == '#' || p[offset] == '\0') {
        return NULL;
    }
    BiTNode* root = (BiTNode*)malloc(sizeof(BiTNode));
    root->data = p[offset];
    ++offset;
    root->lchild = BuildBiTree(p);
    ++offset;
    root->rchild = BuildBiTree(p);
    return root;
}

// 最标准的遍历, 相当于数组中的for循环, 必须标准化
void PreTrav(BiTree tree) {
    BiTNode* p = tree;
    BiTNode* Stack[20];  // 辅助栈, 不考虑太大的树
    int top = -1;        // 栈指针
    while (top >= 0 || p != NULL) {
        if (p != NULL) {
            printf("%c ", p->data);
            Stack[++top] = p;
            p = p->lchild;
        } else {
            p = Stack[top--];
            p = p->rchild;
        }
    }
}

void InTrav(BiTree tree) {
    BiTNode* p = tree;
    BiTNode* Stack[20];  // 辅助栈, 不考虑太大的树
    int top = -1;        // 栈指针
    while (top >= 0 || p != NULL) {
        if (p != NULL) {
            Stack[++top] = p;
            p = p->lchild;
        } else {
            p = Stack[top--];
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}

void PostTrav(BiTree tree) {
    BiTNode* p = tree;
    BiTNode* Stack[20];   // 辅助栈, 不考虑太大的树
    int top = -1;         // 栈指针
    BiTNode* pre = NULL;  // 辅助指针, 用于标记
    while (top >= 0 || p != NULL) {
        if (p != NULL) {
            Stack[++top] = p;
            p = p->lchild;
        } else {
            p = Stack[top];  // 后序遍历不能此时出栈
            if (p->rchild != NULL && pre != p->rchild) {
                p = p->rchild;
                pre = p;  // 标记已经访问过该节点了
            } else {
                top--;
                printf("%c ", p->data);  // 右子树已经访问过了
                // 不更新 pre 会导致困在 F-G-D 中出不来, 因为 pre 始终是G
                // B 的右孩子D 始终和 pre(G) 不一致, p 会不断跳到 D
                pre = p;
                p = NULL;  // 不重置 p 会导致困在 F 中无法出来, 一直打印F
            }
        }
    }
}

// ==================== 将二叉树转换为线索二叉树 ==================
// 将二叉树复制为线索二叉树
BiThrTree CopyToBiThrTree(BiTree bitree) {
    if (bitree == NULL) return NULL;
    BiThrNode* node = (BiThrNode*)malloc(sizeof(BiThrNode));
    node->data = bitree->data;
    node->lchild = CopyToBiThrTree(bitree->lchild);
    node->rchild = CopyToBiThrTree(bitree->rchild);
    node->LTag = (node->lchild == NULL) ? Thread : Link;
    node->RTag = (node->rchild == NULL) ? Thread : Link;
    return node;
}

// 创建中序线索二叉树
BiThrTree InThread(BiThrTree tree) {
    BiThrNode* p = tree;
    BiThrNode* Stack[20];  // 辅助栈, 不考虑太大的树
    int top = -1;          // 栈指针
    // 创建根节点
    while (top >= 0 || p != NULL) {
        if (p != NULL) {
            Stack[++top] = p;
            p = p->lchild;
        } else {
            p = Stack[top--];
            if (p->lchild == NULL) {
                p->lchild = preThrNode;
                p->LTag = Thread;
            }
            if (preThrNode != NULL && preThrNode->rchild == NULL) {
                preThrNode->rchild = p;
                preThrNode->RTag = Thread;
            }
            preThrNode = p;
            p = p->rchild;
        }
    }
    return tree;
}

BiThrTree InThread2(BiThrTree tree) {
    if (tree == NULL) {
        return NULL;
    }
    tree->lchild = InThread2(tree->lchild);
    if (tree->lchild == NULL) {
        tree->lchild = preThrNode;
        tree->LTag = Thread;
    }
    if (preThrNode != NULL && preThrNode->rchild == NULL) {
        preThrNode->rchild = tree;
        preThrNode->RTag = Thread;
    }
    preThrNode = tree;
    tree->rchild = InThread2(tree->rchild);
    return tree;
}

// 遍历中序线索二叉树
void TravThrTree(BiThrTree thrtree) {
    BiThrNode* p = thrtree;
    while (p != NULL) {
        // 从根节点移动到中序遍历第一个节点
        while (p->LTag == Link) {
            p = p->lchild;
        }
        printf("%c ", p->data);
        while (p->RTag == 1 && p->rchild != NULL) {
            p = p->rchild;
            printf("%c ", p->data);
        }
        p = p->rchild;
    }
}

// ==================== 垂直打印树 ====================
#define MAX_HEIGHT 10
#define MAX_WIDTH 256

// 将节点填充到二维数组中
// row: 当前行（节点行，连接线在 row+1）
// left, right: 当前子树可用的水平范围
void FillTreeArray(BiTree root, char arr[][MAX_WIDTH], int row, int left,
                   int right) {
    if (root == NULL) return;

    int mid = (left + right) / 2;
    arr[row][mid] = root->data;  // 放置节点

    if (root->lchild) {
        // 左连接线：从 mid-1 向左画到子节点位置
        int leftChildPos = (left + mid - 1) / 2;
        arr[row + 1][mid - 1] = '/';
        // 画水平延伸线（可选，让树更清晰）
        for (int i = mid - 2; i > leftChildPos; i--) {
            arr[row + 1][i] = '_';
        }
        FillTreeArray(root->lchild, arr, row + 2, left, mid - 1);
    }

    if (root->rchild) {
        // 右连接线：从 mid+1 向右画到子节点位置
        int rightChildPos = (mid + 1 + right) / 2;
        arr[row + 1][mid + 1] = '\\';
        // 画水平延伸线（可选）
        for (int i = mid + 2; i < rightChildPos; i++) {
            arr[row + 1][i] = '_';
        }
        FillTreeArray(root->rchild, arr, row + 2, mid + 1, right);
    }
}

// 获取树的高度
int GetHeight(BiTree root) {
    if (root == NULL) return 0;
    int left = GetHeight(root->lchild);
    int right = GetHeight(root->rchild);
    return (left > right ? left : right) + 1;
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
    offset = 0;  // 重置全局变量
    char* PreString = "AB#DF##G##CE#H###";
    BiTree tree = BuildBiTree(PreString);
    PrintTree(tree);
    printf("非递归中序遍历复习:\n");
    InTrav(tree);
    printf("\n非递归后序遍历复习:\n");
    PostTrav(tree);

    // 非递归线索二叉树
    printf("\n中序线索二叉树遍历:\n");
    BiThrTree thrtree = CopyToBiThrTree(tree);
    preThrNode = NULL;  // 重置全局变量
    InThread(thrtree);
    TravThrTree(thrtree);

    // 递归线索二叉树
    printf("\n中序线索二叉树遍历:\n");
    thrtree = CopyToBiThrTree(tree);
    preThrNode = NULL;  // 重置全局变量
    InThread2(thrtree);
    TravThrTree(thrtree);
}