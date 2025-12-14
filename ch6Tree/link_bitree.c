// 创建节点上存储 字符 的二叉链表
// 二叉树的遍历
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct BiTNode {
    char data;
    struct BiTNode* left;
    struct BiTNode* right;
} BiTNode, *BiTree;

BiTree InitBiTree() {
    BiTree root = (BiTree)malloc(sizeof(BiTNode));
    root->data = '\0';
    root->left = NULL;
    root->right = NULL;
    return root;
}

BiTree MakeBiTree(char c, BiTree left, BiTree right) {
    BiTree root = InitBiTree();
    root->data = c;
    root->left = left;
    root->right = right;
    return root;
}

// 替换左子树, 并返回原来的左子树
BiTree ReplaceLeft(BiTree* root, BiTree newleft) {
    BiTree left = (*root)->left;
    (*root)->left = newleft;
    return left;
}

// 替换右子树, 并返回原来的右子树
BiTree ReplaceRight(BiTree* root, BiTree newright) {
    BiTree right = (*root)->right;
    (*root)->right = newright;
    return right;
}

void PrintElem(char p) { printf("%c", p); }

// 书本上的写法, 传入的是函数指针
// void (*visit)(char) 是函数指针类型, visit是参数名
// 它指向一个 返回值为void, 参数为char 的函数
void PreOrderTravBook(BiTree root, void (*visit)(char)) {
    if (root == NULL) {
        return;
    }
    visit(root->data);  // 通过函数指针调用函数
    PreOrderTravBook(root->left, visit);
    PreOrderTravBook(root->right, visit);
}

void PreOrderTrav(BiTree root) {
    if (root == NULL) {
        return;
    }
    printf("%c ", root->data);
    PreOrderTrav(root->left);
    PreOrderTrav(root->right);
}

void InOrderTrav(BiTree root) {
    if (root == NULL) {
        return;
    }
    InOrderTrav(root->left);
    printf("%c ", root->data);
    InOrderTrav(root->right);
}

void PostOrderTrav(BiTree root) {
    if (root == NULL) {
        return;
    }
    PostOrderTrav(root->left);
    PostOrderTrav(root->right);
    printf("%c ", root->data);
}

// 使用栈进行先序遍历
// n 为栈容量
void PreOrderTravNoRec(BiTree root, int n) {
    BiTNode* stack[n];
    int top = -1;
    stack[++top] = root;  // 先将根节点入栈

    while (top >= 0) {
        BiTNode* p = stack[top--];
        if (p == NULL) {
            continue;
        }
        printf("%c ", p->data);

        // 先压右子树，再压左子树（保证左子树先被访问）
        if (p->right != NULL) {
            stack[++top] = p->right;
        }
        if (p->left != NULL) {
            stack[++top] = p->left;
        }
    }
}

// 经典教科书写法
void PreOrderTravNoRecBook(BiTree root, int n) {
    BiTNode* stack[n];
    int top = -1;
    BiTNode* p = root;

    while (p != NULL || top >= 0) {
        while (p != NULL) {
            printf("%c ", p->data);  // 访问当前节点
            stack[++top] = p;        // 入栈（保存，以便后续访问右子树）
            p = p->left;             // 沿左子树向下
        }
        // 左子树走完了，弹栈，转向右子树
        if (top >= 0) {
            p = stack[top--];
            p = p->right;
        }
    }
}

void InOrderTravNoRec(BiTree root, int n) {
    BiTNode* stack[n];
    int top = -1;       // 栈顶指针，-1 表示空栈
    BiTNode* p = root;  // 工作指针

    while (p != NULL || top >= 0) {
        if (p != NULL) {
            stack[++top] = p;  // 当前节点入栈
            p = p->left;       // 继续向左走
        } else {
            p = stack[top--];        // 弹出栈顶节点
            printf("%c ", p->data);  // 访问节点
            p = p->right;            // 转向右子树
        }
    }
}

// 非标准化写法
// void InOrderTravNoRec(BiTree root, int n) {
//     BiTNode* stack[n];
//     stack[0] = root;
//     int top = 0;  // 栈指针
//     bool flag = true;
//     while (top >= 0) {  // 栈非空的时候
//         BiTNode* temp = stack[top];
//         if (temp->left && flag) {
//             stack[++top] = temp->left;  // 左节点入栈
//         } else {
//             flag = false;
//             temp = stack[top--];
//             printf("%c ", temp->data);
//             if (temp->right != NULL) {
//                 stack[++top] = temp->right;
//                 flag = true;
//             }
//         }
//     }
// }

void InOrderMorris(BiTree root) {
    BiTNode* cur = root;
    BiTNode* pre;

    while (cur != NULL) {
        if (cur->left == NULL) {
            printf("%c ", cur->data);  // 访问节点
            cur = cur->right;
        } else {
            // 找到左子树的最右节点（前驱）
            pre = cur->left;
            while (pre->right != NULL && pre->right != cur) {
                pre = pre->right;
            }

            if (pre->right == NULL) {
                pre->right = cur;  // 建立线索
                cur = cur->left;
            } else {
                pre->right = NULL;  // 恢复树结构
                printf("%c ", cur->data);
                cur = cur->right;
            }
        }
    }
}

void PostOrderTravNoRec(BiTree root, int n) {
    BiTNode* stack[n];
    BiTNode* p = root;
    BiTNode* r = NULL;  // 记录最近访问过的节点
    int top = -1;

    while (top >= 0 || p != NULL) {
        if (p != NULL) {
            stack[++top] = p;
            p = p->left;  // 一直向左走
        } else {
            p = stack[top];  // 读栈顶（不弹出）
            if (p->right != NULL && p->right != r) {
                p = p->right;  // 右子树存在且未访问，转向右子树
            } else {
                top--;  // 弹栈
                printf("%c ", p->data);
                r = p;     // 记录刚访问的节点
                p = NULL;  // 重置 p，继续处理栈顶
            }
        }
    }
}

// 辅助函数
BiTree BuildBiTreeHelper(char* str, int len, int i) {
    // 先检查索引是否越界, 再访问数组
    // 不能使用 str[i] == '\0' 因为i的变化不是逐个增加的
    if (i >= len) {
        return NULL;
    }

    BiTree root = InitBiTree();
    root->data = str[i];
    root->left = BuildBiTreeHelper(str, len, 2 * i + 1);
    root->right = BuildBiTreeHelper(str, len, 2 * i + 2);

    return root;
}

// 递归方法使用字符串构建完全二叉树
BiTree BuildBiTreeRec(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return BuildBiTreeHelper(str, i, 0);
}

// 书本中构造树的方法
// 正确的实现：传递 i 的指针
BiTree CreateBiTree(char* str, int* i) {
    BiTree root;
    if (str[*i] == ' ' || str[*i] == '\0') {
        (*i)++;  // 跳过空格，移动索引
        root = NULL;
    } else {
        root = InitBiTree();
        root->data = str[(*i)++];            // 读取当前字符，然后 i++
        root->left = CreateBiTree(str, i);   // i 是指针，递归会修改它
        root->right = CreateBiTree(str, i);  // 继续从更新后的位置读取
    }
    return root;
}

// 获取数的节点个数
int GetNodes(BiTree root) {
    if (root == NULL) {
        return 0;
    }
    if (!root->left && !root->right) {
        return 1;
    }
    return GetNodes(root->left) + GetNodes(root->right) + 1;
}

// 获取树的高度
int GetHeight(BiTree root) {
    if (root == NULL) return 0;
    int left = GetHeight(root->left);
    int right = GetHeight(root->right);
    return (left > right ? left : right) + 1;
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

// 简洁版：横向打印（右子树在上）
void PrintTreeHorizontal(BiTree tree, int level, int indent, char dir) {
    if (tree == NULL) return;

    PrintTreeHorizontal(tree->right, level + 1, indent + 4, '/');

    for (int i = 0; i < indent; i++) printf(" ");
    if (level > 0) printf("%c---", dir);
    printf("%c\n", tree->data);

    PrintTreeHorizontal(tree->left, level + 1, indent + 4, '\\');
}

void FreeBiTree(BiTree root) {
    if (root == NULL) {
        return;
    }
    FreeBiTree(root->left);
    FreeBiTree(root->right);
    free(root);
}

int main() {
    char s1[] = "ABCDEFGH";
    BiTree tree = BuildBiTreeRec(s1);
    PrintTree(tree);

    PreOrderTrav(tree);
    printf("\n");
    InOrderTrav(tree);
    printf("\n");
    PostOrderTrav(tree);
    printf("\n");

    printf("\n非递归排序实现:\n");
    PreOrderTravNoRec(tree, 10);
    printf("\n");
    InOrderTravNoRec(tree, 10);
    printf("\n");
    PostOrderTravNoRec(tree, 10);
    printf("\n");

    printf("\nMorris中序遍历实现:\n");
    InOrderMorris(tree);
    printf("\n");

    printf("树的节点个数: %d\n", GetNodes(tree));

    FreeBiTree(tree);

    char s2[] = "ABC  DE G  F   ";  // 先序遍历字符串
    // 调用方式
    int index = 0;
    BiTree tree2 = CreateBiTree(s2, &index);
    PrintTree(tree2);
    FreeBiTree(tree2);
}