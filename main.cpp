#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 查找结点 i 的父结点,通过递归得到结点到根的长度
int findParent(int i, int huffman[][4], int n);
// 根据初始权重构建哈夫曼树
void huffmanTree(int w[], int huffman[][4], int n);
// 寻找权重最小的两个节点
void findMin(int ii[], int huffman[][4], int n);
// 对每个叶节点进行哈夫曼编码
void HuffmanCode(int i, int huffman[][4], int n, char uniqueChars[]);
// 交换两个 char 型数据
void inplace_swap(char* x, char* y);
// 按字母表排序字母【自己补充】
void sort_letter(char uniqueChars[], int count);

int main()
{
    // 对输入的字符串进行记录，对字符的总个数 count 以及各个字符的出现次数 arr_small[] 进行统计
    char str[100];
    printf("Please Enter the string\n");
    scanf("%s", str);

    int arr_big[27] = { 0 }; // 各个字符的出现次数
    int count = 0;         // 字符的总个数
    char uniqueChars[27];  // 用于记录唯一的字符
    for (int i = 0; i < strlen(str); i++)   // 遍历输入字符串中的所有字符
    {
        for (int j = 0; j < 27; j++)        // 遍历26个大写字母
        {
            if (str[i] == (char)(0x41 + j))
            {
                arr_big[j] += 1;
                if (arr_big[j] == 1)
                {
                    uniqueChars[count] = (char)(0x41 + j); // 记录唯一字符
                    count++; // 记录一共出现了几种不同的字母
                }
                break;
            }
        }
    }
    printf("The letter count is %d\n", count); // 打印输入字符串中不同字母的种类数

    sort_letter(uniqueChars, count);    // 按字母表顺序重排uniqueChars[]数组元素【自己补充】

    // 通过 arr_big 对输入字符串中出现的字母次数进行统计，放入数组 arr_small 中
    int count2 = 0;       // 字符串中出现的字符总个数
    int arr_small[count]; // 统计输入字符串中字母的出现次数
    for (int i = 0; i < 27; i++)
    {
        if (arr_big[i] > 0) {
            arr_small[count2++] = arr_big[i]; // 将每个字符的出现次数存入 arr_small
        }
    }

    // 定义 int 型二维数组，数组长度 [*] 为哈夫曼树的节点个数
    // c[*][0] 存放的是该节点的 [父节点的位序]，c[*][1] 为该节点的 [左子树节点的位序]
    // c[*][2] 存放该节点的 [右子树节点的位序]，c[*][3] 为该节点的权值
    int huffman[2 * count - 1][4]; // 前 n 个为叶结点，后 n-1 个为中间结点

    // 根据初始权重数组 arr_small 和字符个数构建哈夫曼树
    huffmanTree(arr_small, huffman, count);

    // 计算 Huffman 生成树的总长度 WPL
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        int length = 0;
        if (huffman[i][1] == -1 && huffman[i][2] == -1) // 判断是否为叶结点
        {
            length = findParent(i, huffman, count); // 得到各个叶结点的带权路径长度
            sum += huffman[i][3] * length;         // 各个叶结点带权路径长度累加得到总长度 WPL
        }
    }
    printf("The tree's WPL is %d\n", sum);

    // Huffman 编码
    for (int i = 0; i < count; i++)
    {
        HuffmanCode(i, huffman, count, uniqueChars);
    }

    return 0;
}

// 子函数——构建哈夫曼树
void huffmanTree(int w[], int huffman[][4], int n)
{
    // 结点初始化
    for (int i = 0; i < 2 * n - 1; i++)
    {
        huffman[i][0] = -1;
        huffman[i][1] = -1;
        huffman[i][2] = -1;
        huffman[i][3] = -1;
    }
    /*将初始权重赋值到前 n 个结点中*/
    for (int i = 0; i < n; i++)
    {
        huffman[i][3] = w[i];
    }
    // 每次抽出两个权重最小的结点进行合并，直到最终产生根结点
    for (int i = n; i < 2 * n - 1; i++)
    {
        int i1, i2; // 权重最小的两个结点
        int ii[2];
        // 找出两个权重最小的结点
        findMin(ii, huffman, n);
        i1 = ii[0];
        i2 = ii[1];
        // 合并 i1、i2 结点，更新结点信息
        huffman[i][1] = i1; // 左孩子
        huffman[i][2] = i2; // 右孩子
        huffman[i][3] = huffman[i1][3] + huffman[i2][3]; // 合并权重
        huffman[i1][0] = i; // 更新父节点
        huffman[i2][0] = i;
    }
}

// 子函数——找出两个权重最小的结点
void findMin(int ii[], int huffman[][4], int n)
{
    // 找出第一个权重最小的结点
    int min = 9999999;
    for (int i = 0; i < 2 * n - 1; i++)
    {
        if (huffman[i][3] == -1 && huffman[i][0] == -1) /*从第一个i向下检索，直到权值为-1的i，即结束的i点还未被赋予权值，退出循环*/
        {
            break;
        }
        if (huffman[i][3] < min && huffman[i][0] == -1) /*未被选中过且权重最小*/
        {
            min = huffman[i][3];
            ii[0] = i;
        }
    }

    // 找出第二个权重最小的结点
    min = 9999999;
    for (int i = 0; i < 2 * n - 1; i++)
    {
        if (huffman[i][3] == -1 && huffman[i][0] == -1)
        {
            break;
        }
        if (huffman[i][3] < min &&  huffman[i][0] == -1 && i != ii[0])
        {
            min = huffman[i][3];
            ii[1] = i;
        }
    }
}

// 子函数——查找结点 i 的父结点,得到结点到根的长度
int findParent(int i, int huffman[][4], int n)
{
    int length = 0;
    if (huffman[i][0] == -1) // 根节点时结束递归
    {
        return 0;
    }
    length += (findParent(huffman[i][0], huffman, n) + 1); // 递归得到路径长度
    return length;
}

// 子函数——对每个叶节点进行哈夫曼编码并进行打印
void HuffmanCode(int i, int huffman[][4], int n, char uniqueChars[])
{
    char code[30];  // char 数组填充编码
    int current = i;  // 定义当前访问的结点
    int father = huffman[i][0]; // 定义当前结点的父节点
    int start = 0;    // 每次编码的位置，初始为编码倒数位置
    while (father != -1)
    {
        if (huffman[father][1] == current)
        {
            code[start++] = '0'; // 子结点是父结点的左子树，编码为 0
        }
        else {
            code[start++] = '1'; // 子结点是父结点的右子树，编码为 1
        }
        current = father;       // 更新当前结点
        father = huffman[current][0]; // 更新父节点
    }
    code[start] = '\0'; // 编码结束符

    // 将 char 数组中的元素头尾两端进行对调
    for (int first = 0, last = start - 1; first < last; first++, last--)
    {
        inplace_swap(&code[first], &code[last]); // 对调数组内部元素
    }

    printf("%c Huffman code:  %s\n", uniqueChars[i], code); // 打印字符的 Huffman 编码【修改为使用uniqueChars数组来映射字符】
}

// 子函数——交换两个 char 型数据（使用了布尔运算），可自己另外用可读性较好的方法重新实现改函数【本处利用指针进行互换操作】
void inplace_swap(char* x, char* y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

// 按字母表排序字母【自己补充，用于uniqueChars[]数组的重排序】
void sort_letter(char uniqueChars[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (uniqueChars[j] > uniqueChars[j + 1])
            {
                char temp = uniqueChars[j];
                uniqueChars[j] = uniqueChars[j + 1];
                uniqueChars[j + 1] = temp;
            }
        }
    }
}
