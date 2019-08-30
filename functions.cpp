/*
* �����������
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

/* ����������½��
 * ���뵽startPtr��next��
 */
extern void LinkInsert(PtrToNode startPtr)
{
    PtrToNode tempPtr = startPtr->next;

    startPtr->next = new Node; //����һ���½��
    startPtr->next->heap = new Heap[HEAPLENGTH];
    startPtr->next->heapSize = 1;
    startPtr->next->strRear = -1;

    startPtr->next->next = tempPtr;
}

/* �ͷ�������
 * ��startPtr��ʼ(����startPtr)��endPtr����(������endPtr)
 */
extern void LinkDelete(Link headPtr, PtrToNode startPtr, PtrToNode endPtr)
{
    PtrToNode currentPtr = startPtr;
    PtrToNode tempPtr = NULL;

    PtrToNode prePtr = headPtr;
    for (;prePtr->next != startPtr;prePtr = prePtr->next);//�ҵ�ɾ����ʼ�е���һ��
    prePtr->next = endPtr;

    while (currentPtr != endPtr) {
        delete []currentPtr->heap;
        tempPtr = currentPtr;
        currentPtr = currentPtr->next;
        delete tempPtr;
    }
}

/* �ѱ䳤��ÿ������HEAPLENGTH���ַ����� */
extern void LengthenHeap(PtrToNode currentPtr, int n)
{
    currentPtr->heapSize++;
    PtrToHeap newHeap = new Heap[currentPtr->heapSize * HEAPLENGTH];
    memcpy(newHeap, currentPtr->heap, n);
    delete []currentPtr->heap;
    currentPtr->heap = newHeap;
}

/* �п��� ����currentPtr1��i1start(����)��i1end(����)����currentPtr2��i2start(����)��ʼ */
extern void RowCopy(PtrToNode currentPtr1, PtrToNode currentPtr2, int i1start, int i1end, int i2start)
{
    if (i2start > currentPtr2->heapSize*HEAPLENGTH - 1) //�����Ȳ���������һ�������Ķ�
        LengthenHeap(currentPtr2, i2start);//��ʱi2startλ��û����
    *(currentPtr2->heap + i2start) = *(currentPtr1->heap + i1start);

    for (int i = i1start + 1;i <= i1end;i++) {
        if (i2start == currentPtr2->heapSize*HEAPLENGTH - 1) //�����Ȳ���������һ�������Ķ�
            LengthenHeap(currentPtr2, i2start + 1);//��ʱi2startλ���ж���
        i2start++;
        *(currentPtr2->heap + i2start) = *(currentPtr1->heap + i);
    }
    currentPtr2->strRear = i2start;
}

/* position�����������һ���ַ�����1�����򷵻�0 */
extern bool IsLinkEnd(struct Position position)
{
    if (!position.linkPos->next) {//��������һ��
        if (position.strPos == position.linkPos->strRear)
            return 1;
    }
    return 0;
}

/* ��position1��headPtr�е�λ�� ��� position2��cancelPtr�е�λ�� */
extern void PosToPos(Link headPtr1, const struct Position position1, Link headPtr2, struct Position &position2)
{
    PtrToNode currentPtr = headPtr1->next;
    position2.linkPos = headPtr2;
    for (;currentPtr != position1.linkPos;currentPtr = currentPtr->next)
        position2.linkPos = position2.linkPos->next;
    position2.linkPos = position2.linkPos->next;

    position2.strPos = position1.strPos;
}

/* �����ݽṹת����Qstring��ʽ����� */
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

/* �����ݽṹ��position��λ��ת����int�͵��к��У�Ȼ����ú��������� */
extern void PositionRowColumn(Link headPtr, const struct Position position, Cursor *cursor)
{
    int row = -1, column = -1;

    PtrToNode currentPtr = headPtr->next;
    for (;currentPtr != position.linkPos;currentPtr = currentPtr->next)
        row++;
    row++;
    column = position.strPos + 1;

    cursor->setCursor(row,column);//����QT�����꺯��
}

/* �����ݽṹ��position1��position2��λ��ת����int�͵��к���, Ȼ����ú��������Ӱ */
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
