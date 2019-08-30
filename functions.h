#ifndef   FUNCTIONS       //如果没有定义这个宏
#define   FUNCTIONS       //定义这个宏

#include <string>
#include "mainwindow.h"

using namespace std;

/* 行链表插入新结点 (插入到startPtr的next上) */
extern void LinkInsert(PtrToNode startPtr);
/* 释放行链表 (从startPtr开始(包括startPtr)，endPtr结束(不包括endPtr)) */
extern void LinkDelete(Link headPtr, PtrToNode startPtr, PtrToNode endPtr);

/* 堆变长，每次增加HEAPLENGTH个字符长度 */
extern void LengthenHeap(PtrToNode currentPtr, int n);
/* 行拷贝 拷贝currentPtr1的i1start(包括)到i1end(包括)，到currentPtr2的i2start(包括)开始 */
extern void RowCopy(PtrToNode currentPtr1, PtrToNode currentPtr2, int i1start, int i1end, int i2start);

/* position是行链表最后一个字符返回1，否则返回0 */
extern bool IsLinkEnd(struct Position position);
/* 将position1在headPtr中的位置 变成 position2在cancelPtr中的位置 */
extern void PosToPos(Link headPtr1, const struct Position position1, Link headPtr2, struct Position &position2);

/* 将数据结构转换成Qstring形式并输出 */
extern void LinkToQstring(Link headPtr, Text *text);
/* 将数据结构中position的位置转换成int型的行和列，然后调用函数输出光标 */
extern void PositionRowColumn(Link headPtr, const struct Position position, Cursor *cursor);
/* 将数据结构中position1和position2的位置转换成int型的行和列, 然后调用函数输出阴影 */
extern void PositionShadow(Link headPtr, const struct Position position1, const struct Position position2, Text *text);

#endif
