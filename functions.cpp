/*
* 外调函数集合
*
* qreamly
* 2018.5.21
*/

#include <iostream>
#include <string>
#include "entitle.h"

#include <QDebug>
#include <qstring.h>
#include "mainwindow.h"
#include "text.h"

using namespace std;

/* 行链表插入新结点
 * 插入到startPtr的next上
 */
extern void LinkInsert(PtrToNode startPtr)
{
    PtrToNode tempPtr = startPtr->next;

    startPtr->next = new Node; //插入一个新结点
    startPtr->next->heap = new Heap[HEAPLENGTH];
    startPtr->next->heapSize = 1;
    startPtr->next->strRear = -1;

    startPtr->next->next = tempPtr;
}

/* 释放行链表
 * 从startPtr开始(包括startPtr)，endPtr结束(不包括endPtr)
 */
extern void LinkDelete(Link headPtr, PtrToNode startPtr, PtrToNode endPtr)
{
    PtrToNode currentPtr = startPtr;
    PtrToNode tempPtr = NULL;

    PtrToNode prePtr = headPtr;
    for (;prePtr->next != startPtr;prePtr = prePtr->next);//找到删除开始行的上一行
    prePtr->next = endPtr;

    while (currentPtr != endPtr) {
        delete []currentPtr->heap;
        tempPtr = currentPtr;
        currentPtr = currentPtr->next;
        delete tempPtr;
    }
}

/* 堆变长，每次增加HEAPLENGTH个字符长度 */
extern void LengthenHeap(PtrToNode currentPtr, int n)
{
    currentPtr->heapSize++;
    PtrToHeap newHeap = new Heap[currentPtr->heapSize * HEAPLENGTH];
    memcpy(newHeap, currentPtr->heap, n);
    delete []currentPtr->heap;
    currentPtr->heap = newHeap;
}

/* 行拷贝 拷贝currentPtr1的i1start(包括)到i1end(包括)，到currentPtr2的i2start(包括)开始 */
extern void RowCopy(PtrToNode currentPtr1, PtrToNode currentPtr2, int i1start, int i1end, int i2start)
{
    if (i2start > currentPtr2->heapSize*HEAPLENGTH - 1) //若长度不够，申请一个更长的堆
        LengthenHeap(currentPtr2, i2start);//此时i2start位置没东西
    *(currentPtr2->heap + i2start) = *(currentPtr1->heap + i1start);

    for (int i = i1start + 1;i <= i1end;i++) {
        if (i2start == currentPtr2->heapSize*HEAPLENGTH - 1) //若长度不够，申请一个更长的堆
            LengthenHeap(currentPtr2, i2start + 1);//此时i2start位置有东西
        i2start++;
        *(currentPtr2->heap + i2start) = *(currentPtr1->heap + i);
    }
    currentPtr2->strRear = i2start;
}

/* position是行链表最后一个字符返回1，否则返回0 */
extern bool IsLinkEnd(struct Position position)
{
    if (!position.linkPos->next) {//如果是最后一行
        if (position.strPos == position.linkPos->strRear)
            return 1;
    }
    return 0;
}

/* 将position1在headPtr中的位置 变成 position2在cancelPtr中的位置 */
extern void PosToPos(Link headPtr1, const struct Position position1, Link headPtr2, struct Position &position2)
{
    PtrToNode currentPtr = headPtr1->next;
    position2.linkPos = headPtr2;
    for (;currentPtr != position1.linkPos;currentPtr = currentPtr->next)
        position2.linkPos = position2.linkPos->next;
    position2.linkPos = position2.linkPos->next;

    position2.strPos = position1.strPos;
}

/* 将数据结构转换成Qstring形式并输出 */
extern void LinkToQstring(Link headPtr, Text *text)
{
    vector<QString> qstring;
    string str = "";
    QString qstr = "";
    PtrToNode currentPtr = headPtr->next;

    for (;currentPtr;currentPtr = currentPtr->next, str="") {
        for (int i = 0;i <= currentPtr->strRear;i++)
            str += currentPtr->heap[i];
        //cout << str << endl;
        qstr = QString::fromStdString(str);
        qstring.push_back(qstr);
    }
    text->getString(qstring);
}

/* 将数据结构中position的位置转换成int型的行和列，然后调用函数输出光标 */
extern void PositionRowColumn(Link headPtr, const struct Position position, Cursor *cursor)
{
    int row = -1, column = -1;

    PtrToNode currentPtr = headPtr->next;
    for (;currentPtr != position.linkPos;currentPtr = currentPtr->next)
        row++;
    row++;
    column = position.strPos + 1;

    cursor->setCursor(row,column);//调用QT输出光标函数
}

/* 将数据结构中position1和position2的位置转换成int型的行和列, 然后调用函数输出阴影 */
extern void PositionShadow(Link headPtr, const struct Position position1, const struct Position position2, Text *text)
{
    cout <<"void PositionShadow("<<endl;
    int row1 = -1, column1 = -1;
    int row2 = -1, column2 = -1;

    PtrToNode currentPtr = headPtr->next;
    for (;currentPtr != position1.linkPos;currentPtr = currentPtr->next)
        row1++;
    row1++;
    column1 = position1.strPos + 1;
    currentPtr = headPtr->next;
    for (;currentPtr != position2.linkPos;currentPtr = currentPtr->next)
        row2++;
    row2++;
    column2 = position2.strPos + 1;

    cout <<"text->drawShadow(row1,column1,row2,column2);"<<endl;

    text->drawShadow(row1,column1,row2,column2);
}
