/*
 * 单光标操作
 *
 * qreamly
 * 2018.5.21
 */

#include <iostream>
#include <string>
#include <conio.h>
#include "entitle.h"
#include "single_operation.h"
#include "trunk_operation.h"
#include "functions.h"

using namespace std;

/* 当用户按下方向键的时候，改变光标当前位置 */
void Single::Direction(int direction, struct Position &position, Link headPtr)
{
    switch (direction) {
    case UP: //上
    case DOWN: {//下
        PtrToNode currentPtr = NULL;
        if (direction == 0) {//若是向上
            if (position.linkPos == headPtr->next)//如果该行是第一行
                return;
            for (currentPtr = headPtr;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//找到上一行
        }
        else {//若是向下
            if (!position.linkPos->next)//如果该行是最后一行
                return;
            currentPtr = position.linkPos->next;//找到下一行
        }

        position.linkPos = currentPtr;
        if (currentPtr->strRear < position.strPos)//若长度不够，跳到行尾
            position.strPos = currentPtr->strRear;
        break;
    }

    case LEFT://左
        if (position.strPos == -1) {
            if (position.linkPos != headPtr->next) {//如果该行不是第一行
                PtrToNode currentPtr = headPtr;
                for (;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//找到上一行
                position = { currentPtr, currentPtr->strRear };
            }
        }
        else
            position.strPos--;
        break;

    case RIGHT://右
        if (position.strPos == position.linkPos->strRear) { //如果光标在行末
            if (position.linkPos->next)//如果不是最后一个字
                position = { position.linkPos->next, -1 };
        }
        else
            position.strPos++;
        break;
    }
}

/* 换行
 * AAA(光标)BBB
 * 变成
 * AAA
 * (光标)BBB
*/
void Single::AddRow(struct Position &position)
{
    if (position.linkPos->strRear == position.strPos) {//光标在行末
        LinkInsert(position.linkPos);
        position = { position.linkPos->next, -1 };
    }
    else{//光标不在行末
        LinkInsert(position.linkPos);
        RowCopy(position.linkPos, position.linkPos->next, position.strPos+1, position.linkPos->strRear, 0);
        position.linkPos->strRear = position.strPos; // 改变原行最后字符所在位置
        position = { position.linkPos->next, -1 };//改变当前光标所在位置
    }
}

/* 插入单个字符 */
void Single::Insert(const char press, struct Position &position)
{
    PtrToNode currentPtr = position.linkPos;//找到当前行
    for (int i = currentPtr->strRear;i > position.strPos;i--) {//从末尾开始，一个个向后挪
        if (i == currentPtr->heapSize*HEAPLENGTH - 1) //若长度不够，申请一个更长的堆
            LengthenHeap(currentPtr, i + 1);
        *(currentPtr->heap + i + 1) = *(currentPtr->heap + i);
    }
    position.strPos++;//更改当前光标所在位置
    if (position.strPos > currentPtr->heapSize*HEAPLENGTH - 1) //若长度不够，申请一个更长的堆
        LengthenHeap(currentPtr, position.strPos + 1);
    *(currentPtr->heap + position.strPos) = press;//插入字符
    position.linkPos->strRear++;//更新当前行最后字符所在位置
}

/* 删除光标前一个字 或者 删除行
   删除行前：ABCD
             EFG
   删除行后：ABCD(光标在这)EFG
*/
void Single::BackSpace(Link headPtr, struct Position &position)
{
    if (position.strPos == -1) {//删除行
        if (position.linkPos != headPtr->next) {//如果不是第一行
            PtrToNode currentPtr = headPtr;
            for (;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//找到删除行的上一行

            struct Position tempPosition = { currentPtr, currentPtr->strRear };
            if (position.linkPos->strRear != -1)
                RowCopy(position.linkPos, currentPtr, 0, position.linkPos->strRear, currentPtr->strRear + 1);//当前行粘贴到上一行末尾
            LinkDelete(headPtr, position.linkPos, position.linkPos->next);//删除这一行
            position = tempPosition;//更新当前光标所指位置(变为上一行的 原 末尾)
        }
    }
    else{//删除光标前一个字
        //转换成删除光标后一个字
        Single single;
        single.Direction(2, position, headPtr);
        single.Delete(headPtr, position);
    }
}

/* 删除光标后的一个字 */
void Single::Delete(Link headPtr, const struct Position position)
{
    PtrToNode currentPtr = position.linkPos;

    //从光标所在位置开始挪
    if (position.strPos == currentPtr->strRear) {//行末
        if (currentPtr->next) {//如果下一行有字,则下一行的字全部移动到这一行末尾
            RowCopy(currentPtr->next, currentPtr, 0, currentPtr->next->strRear, currentPtr->strRear + 1);
            LinkDelete(headPtr, currentPtr->next, currentPtr->next->next);//删除下一行
        }
    }
    else {//不是行末
        for (int i = position.strPos + 1;i <= currentPtr->strRear;i++)//从光标后开始，一个个向前挪
            *(currentPtr->heap + i) = *(currentPtr->heap + i + 1);
        position.linkPos->strRear--;//更新当前行最后字符所在位置
    }
}
