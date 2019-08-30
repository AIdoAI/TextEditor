#ifndef ENTITLE
#define ENTITLE

struct Node;
typedef struct Node* PtrToNode; 	//指向链表结点的指针
typedef char Heap;					//堆中的元素类型
typedef Heap* PtrToHeap; 			//指向堆的指针
typedef PtrToNode Link;				//链表头指针
typedef PtrToHeap ElemOfLink;		//链表中的元素类型

#define HEAPLENGTH 3				//一个子堆的大小

/* 链表结点的定义 */
struct Node {
    PtrToNode next;
    PtrToHeap heap;					//堆
    int heapSize;					//子堆个数
    int strRear;					//堆末尾
};
/* 当前光标所在位置 */
struct Position {
    PtrToNode linkPos;
    int strPos;					    //当前光标所对应的堆中数组元素下标
};

#endif // !ENTITLE
