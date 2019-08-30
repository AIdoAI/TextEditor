/*
 * �����
 *
 * qreamly
 * 2018.5.21
*/

#include <iostream>
#include "trunk_operation.h"
#include "single_operation.h"
#include "functions.h"

using namespace std;

/* �Ƚ���������ǰ�� (0����) */
bool IsPositionEqual(const struct Position position1, const struct Position position2)
{
    return (position1.linkPos == position2.linkPos && position1.strPos == position2.strPos);
}

/* ����� */
void Trunk::TrunkCut(Link headPtr, Link destinationPtr, struct Position &position1, struct Position &position2)
{
    if (!IsPositionEqual(position1, position2)) { //����겻����
        Trunk trunk;
        trunk.TrunkCopy(destinationPtr, position1, position2);
        trunk.TrunkDelete(headPtr, position1, position2);
    }
}

/* �鿽�� ������destinationPtr�� */
void Trunk::TrunkCopy(Link destinationPtr, struct Position position1, struct Position position2)
{
    if (IsPositionEqual(position1, position2))//����겻����
        return;

    LinkDelete(destinationPtr, destinationPtr->next,NULL);//���ԭ������
    destinationPtr->next = NULL;

    PtrToNode currentPtr1 = position1.linkPos;
    PtrToNode currentPtr2 = destinationPtr;

    //������һ����������У���һ�����֮����ַ�
    LinkInsert(currentPtr2);
    currentPtr2 = currentPtr2->next;
    if (position1.strPos != currentPtr1->strRear) {
        if(position1.linkPos!=position2.linkPos) //���������겻��ͬһ��
            RowCopy(currentPtr1, currentPtr2, position1.strPos + 1, currentPtr1->strRear, 0);
        else {
            RowCopy(currentPtr1, currentPtr2, position1.strPos + 1, position2.strPos, 0);
            return;
        }
    }

    //������һ����������к͵ڶ������������֮����ַ�
    for (currentPtr1 = currentPtr1->next;currentPtr1 != position2.linkPos;currentPtr1 = currentPtr1->next) {
        LinkInsert(currentPtr2);
        currentPtr2 = currentPtr2->next;
        RowCopy(currentPtr1, currentPtr2, 0, currentPtr1->strRear, 0);
    }

    //�����ڶ�����������У��ڶ������֮ǰ���ַ�
    LinkInsert(currentPtr2);
    currentPtr2 = currentPtr2->next;
    RowCopy(currentPtr1, currentPtr2, 0, position2.strPos, 0);
}

/* ��ɾ��
* AAAXXXXXXXX
* XXX
* XXXXXBBB
*
* AAABBB
*/
void Trunk::TrunkDelete(Link headPtr, struct Position &position1, struct Position &position2)
{
    //�ڶ�����������У��ڶ������֮����ַ�����������һ����������У���һ�����֮��
    if (position2.strPos != position2.linkPos->strRear)
        RowCopy(position2.linkPos, position1.linkPos, position2.strPos+1, position2.linkPos->strRear, position1.strPos + 1);
    else
        position1.linkPos->strRear = position1.strPos;//���µ�һ��������������һ���ַ�����λ��

    //ɾ����һ����������к͵ڶ������������֮��(�����ڶ������������)���ַ�
    if (position1.linkPos != position2.linkPos)
        LinkDelete(headPtr, position1.linkPos->next, position2.linkPos->next);

    position2 = position1;
}

/* ��ճ��
 * AAA����꣩BBB
 * XXXXXXXX
 * XXX
 * XXXXX
 *
 * AAAXXXXXXXX
 * XXX
 * XXXXX����꣩BBB
*/
void Trunk::TrunkPaste(Link sourcePtr, struct Position &position)
{
    if (!sourcePtr->next)
        return;

    PtrToNode currentPtr1 = position.linkPos;
    PtrToNode currentPtr2 = sourcePtr;

    //(1)����������У��������λ��֮��������ַ���sourcePtr���һ�� �ϳ�һ�� ���洢���µ�ָ����
    Link newPtr = new Node;//ע�⣺newPtr��ͷָ�룬���Ժϳɺ�����д����newPtrָ��ĵ�һ�����
    newPtr->next = NULL;
    LinkInsert(newPtr);

    for (;currentPtr2->next;currentPtr2 = currentPtr2->next);//�ҵ�sourcePtr���һ��
    if (currentPtr2->strRear != -1)
        RowCopy(currentPtr2, newPtr->next, 0, currentPtr2->strRear, 0);
    if (position.strPos != currentPtr1->strRear)
        RowCopy(currentPtr1, newPtr->next, position.strPos + 1, currentPtr1->strRear, newPtr->next->strRear + 1);

    //(2)ճ��sourcePtr�еĵ�һ�е����֮��
    currentPtr2 = sourcePtr->next;
    if (currentPtr2->next) {//sourcePtr����������
        if (currentPtr2->strRear != -1)
            RowCopy(currentPtr2, currentPtr1, 0, currentPtr2->strRear, position.strPos + 1);

        //(3)ճ��sourcePtr����һ�С����һ�����ʣ���У���newPtrָ�������
        //�Ƚ�sourcePtr����һ�С����һ�����ʣ���п�����newPtr��
        currentPtr1 = newPtr;
        for (currentPtr2 = currentPtr2->next;currentPtr2->next;currentPtr1 = currentPtr1->next, currentPtr2 = currentPtr2->next) {
            LinkInsert(currentPtr1);
            currentPtr1 = currentPtr1->next;
            RowCopy(currentPtr2, currentPtr1, 0, currentPtr2->strRear, 0);
        }
        //�ٽ�newPtr���뵽ԭ��������
        currentPtr1 = position.linkPos;
        currentPtr2 = newPtr->next;
        PtrToNode tempPtr = currentPtr1->next;
        currentPtr1->next = currentPtr2;
        for (;currentPtr2->next;currentPtr2 = currentPtr2->next);//�ҵ�newPtr�����һ��
        currentPtr2->next = tempPtr;
        delete newPtr;

        //���ĵ�ǰ�������λ��
        position.linkPos = currentPtr2;//��newPtr�����һ��
        for (currentPtr2 = sourcePtr;currentPtr2->next;currentPtr2 = currentPtr2->next);//�ҵ�sourcePtr�����һ��
        position.strPos = currentPtr2->strRear;
    }
    else{//sourcePtrֻ��һ��
        if (newPtr->next->strRear != -1)
            RowCopy(newPtr->next, position.linkPos, 0, newPtr->next->strRear, position.strPos + 1);
        LinkDelete(newPtr, newPtr->next, NULL);
        delete newPtr;

        //���ĵ�ǰ�������λ��
        position.strPos += sourcePtr->next->strRear + 1;
    }
}

/* ȫѡ */
void Trunk::SelectAll(Link headPtr, struct Position &position1, struct Position &position2)
{
    Single single;

    position2 = position1 = { headPtr->next, -1 };
    while (position2.linkPos->next)
        single.Direction(1, position2, headPtr);
    while (!IsLinkEnd(position2))
        single.Direction(3, position2, headPtr);
}
