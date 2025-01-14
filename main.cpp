#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���ҽ�� i �ĸ����,ͨ���ݹ�õ���㵽���ĳ���
int findParent(int i, int huffman[][4], int n);
// ���ݳ�ʼȨ�ع�����������
void huffmanTree(int w[], int huffman[][4], int n);
// Ѱ��Ȩ����С�������ڵ�
void findMin(int ii[], int huffman[][4], int n);
// ��ÿ��Ҷ�ڵ���й���������
void HuffmanCode(int i, int huffman[][4], int n, char uniqueChars[]);
// �������� char ������
void inplace_swap(char* x, char* y);
// ����ĸ��������ĸ���Լ����䡿
void sort_letter(char uniqueChars[], int count);

int main()
{
    // ��������ַ������м�¼�����ַ����ܸ��� count �Լ������ַ��ĳ��ִ��� arr_small[] ����ͳ��
    char str[100];
    printf("Please Enter the string\n");
    scanf("%s", str);

    int arr_big[27] = { 0 }; // �����ַ��ĳ��ִ���
    int count = 0;         // �ַ����ܸ���
    char uniqueChars[27];  // ���ڼ�¼Ψһ���ַ�
    for (int i = 0; i < strlen(str); i++)   // ���������ַ����е������ַ�
    {
        for (int j = 0; j < 27; j++)        // ����26����д��ĸ
        {
            if (str[i] == (char)(0x41 + j))
            {
                arr_big[j] += 1;
                if (arr_big[j] == 1)
                {
                    uniqueChars[count] = (char)(0x41 + j); // ��¼Ψһ�ַ�
                    count++; // ��¼һ�������˼��ֲ�ͬ����ĸ
                }
                break;
            }
        }
    }
    printf("The letter count is %d\n", count); // ��ӡ�����ַ����в�ͬ��ĸ��������

    sort_letter(uniqueChars, count);    // ����ĸ��˳������uniqueChars[]����Ԫ�ء��Լ����䡿

    // ͨ�� arr_big �������ַ����г��ֵ���ĸ��������ͳ�ƣ��������� arr_small ��
    int count2 = 0;       // �ַ����г��ֵ��ַ��ܸ���
    int arr_small[count]; // ͳ�������ַ�������ĸ�ĳ��ִ���
    for (int i = 0; i < 27; i++)
    {
        if (arr_big[i] > 0) {
            arr_small[count2++] = arr_big[i]; // ��ÿ���ַ��ĳ��ִ������� arr_small
        }
    }

    // ���� int �Ͷ�ά���飬���鳤�� [*] Ϊ���������Ľڵ����
    // c[*][0] ��ŵ��Ǹýڵ�� [���ڵ��λ��]��c[*][1] Ϊ�ýڵ�� [�������ڵ��λ��]
    // c[*][2] ��Ÿýڵ�� [�������ڵ��λ��]��c[*][3] Ϊ�ýڵ��Ȩֵ
    int huffman[2 * count - 1][4]; // ǰ n ��ΪҶ��㣬�� n-1 ��Ϊ�м���

    // ���ݳ�ʼȨ������ arr_small ���ַ�����������������
    huffmanTree(arr_small, huffman, count);

    // ���� Huffman ���������ܳ��� WPL
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        int length = 0;
        if (huffman[i][1] == -1 && huffman[i][2] == -1) // �ж��Ƿ�ΪҶ���
        {
            length = findParent(i, huffman, count); // �õ�����Ҷ���Ĵ�Ȩ·������
            sum += huffman[i][3] * length;         // ����Ҷ����Ȩ·�������ۼӵõ��ܳ��� WPL
        }
    }
    printf("The tree's WPL is %d\n", sum);

    // Huffman ����
    for (int i = 0; i < count; i++)
    {
        HuffmanCode(i, huffman, count, uniqueChars);
    }

    return 0;
}

// �Ӻ�������������������
void huffmanTree(int w[], int huffman[][4], int n)
{
    // ����ʼ��
    for (int i = 0; i < 2 * n - 1; i++)
    {
        huffman[i][0] = -1;
        huffman[i][1] = -1;
        huffman[i][2] = -1;
        huffman[i][3] = -1;
    }
    /*����ʼȨ�ظ�ֵ��ǰ n �������*/
    for (int i = 0; i < n; i++)
    {
        huffman[i][3] = w[i];
    }
    // ÿ�γ������Ȩ����С�Ľ����кϲ���ֱ�����ղ��������
    for (int i = n; i < 2 * n - 1; i++)
    {
        int i1, i2; // Ȩ����С���������
        int ii[2];
        // �ҳ�����Ȩ����С�Ľ��
        findMin(ii, huffman, n);
        i1 = ii[0];
        i2 = ii[1];
        // �ϲ� i1��i2 ��㣬���½����Ϣ
        huffman[i][1] = i1; // ����
        huffman[i][2] = i2; // �Һ���
        huffman[i][3] = huffman[i1][3] + huffman[i2][3]; // �ϲ�Ȩ��
        huffman[i1][0] = i; // ���¸��ڵ�
        huffman[i2][0] = i;
    }
}

// �Ӻ��������ҳ�����Ȩ����С�Ľ��
void findMin(int ii[], int huffman[][4], int n)
{
    // �ҳ���һ��Ȩ����С�Ľ��
    int min = 9999999;
    for (int i = 0; i < 2 * n - 1; i++)
    {
        if (huffman[i][3] == -1 && huffman[i][0] == -1) /*�ӵ�һ��i���¼�����ֱ��ȨֵΪ-1��i����������i�㻹δ������Ȩֵ���˳�ѭ��*/
        {
            break;
        }
        if (huffman[i][3] < min && huffman[i][0] == -1) /*δ��ѡ�й���Ȩ����С*/
        {
            min = huffman[i][3];
            ii[0] = i;
        }
    }

    // �ҳ��ڶ���Ȩ����С�Ľ��
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

// �Ӻ����������ҽ�� i �ĸ����,�õ���㵽���ĳ���
int findParent(int i, int huffman[][4], int n)
{
    int length = 0;
    if (huffman[i][0] == -1) // ���ڵ�ʱ�����ݹ�
    {
        return 0;
    }
    length += (findParent(huffman[i][0], huffman, n) + 1); // �ݹ�õ�·������
    return length;
}

// �Ӻ���������ÿ��Ҷ�ڵ���й��������벢���д�ӡ
void HuffmanCode(int i, int huffman[][4], int n, char uniqueChars[])
{
    char code[30];  // char ����������
    int current = i;  // ���嵱ǰ���ʵĽ��
    int father = huffman[i][0]; // ���嵱ǰ���ĸ��ڵ�
    int start = 0;    // ÿ�α����λ�ã���ʼΪ���뵹��λ��
    while (father != -1)
    {
        if (huffman[father][1] == current)
        {
            code[start++] = '0'; // �ӽ���Ǹ�����������������Ϊ 0
        }
        else {
            code[start++] = '1'; // �ӽ���Ǹ�����������������Ϊ 1
        }
        current = father;       // ���µ�ǰ���
        father = huffman[current][0]; // ���¸��ڵ�
    }
    code[start] = '\0'; // ���������

    // �� char �����е�Ԫ��ͷβ���˽��жԵ�
    for (int first = 0, last = start - 1; first < last; first++, last--)
    {
        inplace_swap(&code[first], &code[last]); // �Ե������ڲ�Ԫ��
    }

    printf("%c Huffman code:  %s\n", uniqueChars[i], code); // ��ӡ�ַ��� Huffman ���롾�޸�Ϊʹ��uniqueChars������ӳ���ַ���
}

// �Ӻ��������������� char �����ݣ�ʹ���˲������㣩�����Լ������ÿɶ��ԽϺõķ�������ʵ�ָĺ�������������ָ����л���������
void inplace_swap(char* x, char* y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

// ����ĸ��������ĸ���Լ����䣬����uniqueChars[]�����������
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
