/*
 * 块操作
 *
 * qreamly
 * 2018.5.21
*/

#include <iostream>
#include "trunk_operation.h"
#include "single_operation.h"
#include "functions.h"

using namespace std;

/* 比较两个光标的前后 (0不等) */
bool IsPositionEqual(const struct Position position1, const struct Position position2)
{
    return (position1.linkPos == position2.linkPos && position1.strPos == position2.strPos);
}

/* 块剪切 */
void Trunk::TrunkCut(Link headPtr, Link destinationPtr, struct Position &position1, struct Position &position2)
{
    if (!IsPositionEqual(position1, position2)) { //单光标不剪切
        Trunk trunk;
        trunk.TrunkCopy(destinationPtr, position1, position2);
        trunk.TrunkDelete(headPtr, position1, position2);
    }
}

/* 块拷贝 拷贝到destinationPtr中 */
void Trunk::TrunkCopy(Link destinationPtr, struct Position position1, struct Position position2)
{
    if (IsPositionEqual(position1, position2))//单光标不拷贝
        return;

    LinkDelete(destinationPtr, destinationPtr->next,NULL);//清空原缓冲区
    destinationPtr->next = NULL;

    PtrToNode currentPtr1 = position1.linkPos;
    PtrToNode currentPtr2 = destinationPtr;

    //拷贝第一个光标所在行，第一个光标之后的字符
    LinkInsert(currentPtr2);
    currentPtr2 = currentPtr2->next;
    if (position1.strPos != currentPtr1->strRear) {
        if(position1.linkPos!=position2.linkPos) //如果两个光标不在同一行
            RowCopy(currentPtr1, currentPtr2, position1.strPos + 1, currentPtr1->strRear, 0);
        else {
            RowCopy(currentPtr1, currentPtr2, position1.strPos + 1, position2.strPos, 0);
            return;
        }
    }

    //拷贝第一个光标所在行和第二个光标所在行之间的字符
    for (currentPtr1 = currentPtr1->next;currentPtr1 != position2.linkPos;currentPtr1 = currentPtr1->next) {
        LinkInsert(currentPtr2);
        currentPtr2 = currentPtr2->next;
        RowCopy(currentPtr1, currentPtr2, 0, currentPtr1->strRear, 0);
    }

    //拷贝第二个光标所在行，第二个光标之前的字符
    LinkInsert(currentPtr2);
    currentPtr2 = currentPtr2->next;
    RowCopy(currentPtr1, currentPtr2, 0, position2.strPos, 0);
}

/* 块删除
* AAAXXXXXXXX
* XXX
* XXXXXBBB
*
* AAABBB
*/
void Trunk::TrunkDelete(Link headPtr, struct Position &position1, struct Position &position2)
{
    //第二个光标所在行，第二个光标之后的字符，拷贝到第一个光标所在行，第一个光标之后
    if (position2.strPos != position2.linkPos->strRear)
        RowCopy(position2.linkPos, position1.linkPos, position2.strPos+1, position2.linkPos->strRear, position1.strPos + 1);
    else
        position1.linkPos->strRear = position1.strPos;//更新第一个光标所在行最后一个字符所在位置

    //删除第一个光标所在行和第二个光标所在行之间(包括第二个光标所在行)的字符
    if (position1.linkPos != position2.linkPos)
        LinkDelete(headPtr, position1.linkPos->next, position2.linkPos->next);

    position2 = position1;
}

/* 块粘贴
 * AAA（光标）BBB
 * XXXXXXXX
 * XXX
 * XXXXX
 *
 * AAAXXXXXXXX
 * XXX
 * XXXXX（光标）BBB
*/
void Trunk::TrunkPaste(Link sourcePtr, struct Position &position)
{
    if (!sourcePtr->next)
        return;

    PtrToNode currentPtr1 = position.linkPos;
    PtrToNode currentPtr2 = sourcePtr;

    //(1)光标所在行中，光标所在位置之后的所有字符与sourcePtr最后一行 合成一行 并存储到新的指针中
    Link newPtr = new Node;//注意：newPtr是头指针，所以合成后的新行存放在newPtr指向的第一个结点
    newPtr->next = NULL;
    LinkInsert(newPtr);

    for (;currentPtr2->next;currentPtr2 = currentPtr2->next);//找到sourcePtr最后一行
    if (currentPtr2->strRear != -1)
        RowCopy(currentPtr2, newPtr->next, 0, currentPtr2->strRear, 0);
    if (position.strPos != currentPtr1->strRear)
        RowCopy(currentPtr1, newPtr->next, position.strPos + 1, currentPtr1->strRear, newPtr->next->strRear + 1);

    //(2)粘贴sourcePtr中的第一行到光标之后
    currentPtr2 = sourcePtr->next;
    if (currentPtr2->next) {//sourcePtr至少有两行
        if (currentPtr2->strRear != -1)
            RowCopy(currentPtr2, currentPtr1, 0, currentPtr2->strRear, position.strPos + 1);

        //(3)粘贴sourcePtr除第一行、最后一行外的剩余行，和newPtr指向的新行
        //先将sourcePtr除第一行、最后一行外的剩余行拷贝到newPtr中
        currentPtr1 = newPtr;
        for (currentPtr2 = currentPtr2->next;currentPtr2->next;currentPtr1 = currentPtr1->next, currentPtr2 = currentPtr2->next) {
            LinkInsert(currentPtr1);
            currentPtr1 = currentPtr1->next;
            RowCopy(currentPtr2, currentPtr1, 0, currentPtr2->strRear, 0);
        }
        //再将newPtr插入到原行链表中
        currentPtr1 = position.linkPos;
        currentPtr2 = newPtr->next;
        PtrToNode tempPtr = currentPtr1->next;
        currentPtr1->next = currentPtr2;
        for (;currentPtr2->next;currentPtr2 = currentPtr2->next);//找到newPtr的最后一行
        currentPtr2->next = tempPtr;
        delete newPtr;

        //更改当前光标所在位置
        position.linkPos = currentPtr2;//即newPtr的最后一行
        for (currentPtr2 = sourcePtr;currentPtr2->next;currentPtr2 = currentPtr2->next);//找到sourcePtr的最后一行
        position.strPos = currentPtr2->strRear;
    }
    else{//sourcePtr只有一行
        if (newPtr->next->strRear != -1)
            RowCopy(newPtr->next, position.linkPos, 0, newPtr->next->strRear, position.strPos + 1);
        LinkDelete(newPtr, newPtr->next, NULL);
        delete newPtr;

        //更改当前光标所在位置
        position.strPos += sourcePtr->next->strRear + 1;
    }
}

/* 全选 */
void Trunk::SelectAll(Link headPtr, struct Position &position1, struct Position &position2)
{
    Single single;

    position2 = position1 = { headPtr->next, -1 };
    while (position2.linkPos->next)
        single.Direction(1, position2, headPtr);
    while (!IsLinkEnd(position2))
        single.Direction(3, position2, headPtr);
}
