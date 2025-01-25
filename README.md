# Huffman_Tree

对字符串进行Huffman编码，得到WPL和各字符编码：

（1）DEAEDDEDBCEEECECDEEDDBEEECEEDEE
https://github.com/zhuang3346/Huffman_Tree/blob/main/huffman1.png

（2）EEBEACCCCAADACCBCCCCECCCEEECCCE
https://github.com/zhuang3346/Huffman_Tree/blob/main/huffman2.png

---

### 思考题

1. 当Huffman树的叶结点为n时，整个生成树的结点总数为何是2n-1？
答：构造哈夫曼树时，每次从剩余的节点中选取两个权值最小的节点进行合并，形成一个新节点。每次合并会减少两个节点，并增加一个新节点（中间节点）。经过n-1次合并后，合并结束，所有节点合并成了一棵完整的哈夫曼树。此时生成了中间结点个数为n-1，而叶子结点个数为n，则总结点数应该为2n-1。
   
2. 该代码可以对大写字母的字符串进行huffman编码，如果要读小写字母字符串进行编码，应该如何修改？如果想要对同时具有大写和小写字母的字符串都进行编码呢（写出思路即可）？
答：增加对小写字母的判断，将arr_big[27]修改为arr_big[53]。在统计字符出现次数时用选择结构语句判断是小写还是大写字母，分别进行判断，并得出uniqueChars[]数组，最终仍然将uniqueChars[]数组按照ASCII表的顺序进行排列。

3. 使用画图工具画出上述两个Huffman生成树的生成过程
答：这里仅提供实例生成过程分析：https://github.com/zhuang3346/Huffman_Tree/blob/main/huffman_generate.jpg
