/*
 * ��������
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

/* ���û����·������ʱ�򣬸ı��굱ǰλ�� */
void Single::Direction(int direction, struct Position &position, Link headPtr)
{
    switch (direction) {
    case UP: //��
    case DOWN: {//��
        PtrToNode currentPtr = NULL;
        if (direction == 0) {//��������
            if (position.linkPos == headPtr->next)//��������ǵ�һ��
                return;
            for (currentPtr = headPtr;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//�ҵ���һ��
        }
        else {//��������
            if (!position.linkPos->next)//������������һ��
                return;
            currentPtr = position.linkPos->next;//�ҵ���һ��
        }

        position.linkPos = currentPtr;
        if (currentPtr->strRear < position.strPos)//�����Ȳ�����������β
            position.strPos = currentPtr->strRear;
        break;
    }

    case LEFT://��
        if (position.strPos == -1) {
            if (position.linkPos != headPtr->next) {//������в��ǵ�һ��
                PtrToNode currentPtr = headPtr;
                for (;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//�ҵ���һ��
                position = { currentPtr, currentPtr->strRear };
            }
        }
        else
            position.strPos--;
        break;

    case RIGHT://��
        if (position.strPos == position.linkPos->strRear) { //����������ĩ
            if (position.linkPos->next)//����������һ����
                position = { position.linkPos->next, -1 };
        }
        else
            position.strPos++;
        break;
    }
}

/* ����
 * AAA(���)BBB
 * ���
 * AAA
 * (���)BBB
*/
void Single::AddRow(struct Position &position)
{
    if (position.linkPos->strRear == position.strPos) {//�������ĩ
        LinkInsert(position.linkPos);
        position = { position.linkPos->next, -1 };
    }
    else{//��겻����ĩ
        LinkInsert(position.linkPos);
        RowCopy(position.linkPos, position.linkPos->next, position.strPos+1, position.linkPos->strRear, 0);
        position.linkPos->strRear = position.strPos; // �ı�ԭ������ַ�����λ��
        position = { position.linkPos->next, -1 };//�ı䵱ǰ�������λ��
    }
}

/* ���뵥���ַ� */
void Single::Insert(const char press, struct Position &position)
{
    PtrToNode currentPtr = position.linkPos;//�ҵ���ǰ��
    for (int i = currentPtr->strRear;i > position.strPos;i--) {//��ĩβ��ʼ��һ�������Ų
        if (i == currentPtr->heapSize*HEAPLENGTH - 1) //�����Ȳ���������һ�������Ķ�
            LengthenHeap(currentPtr, i + 1);
        *(currentPtr->heap + i + 1) = *(currentPtr->heap + i);
    }
    position.strPos++;//���ĵ�ǰ�������λ��
    if (position.strPos > currentPtr->heapSize*HEAPLENGTH - 1) //�����Ȳ���������һ�������Ķ�
        LengthenHeap(currentPtr, position.strPos + 1);
    *(currentPtr->heap + position.strPos) = press;//�����ַ�
    position.linkPos->strRear++;//���µ�ǰ������ַ�����λ��
}

/* ɾ�����ǰһ���� ���� ɾ����
   ɾ����ǰ��ABCD
             EFG
   ɾ���к�ABCD(�������)EFG
*/
void Single::BackSpace(Link headPtr, struct Position &position)
{
    if (position.strPos == -1) {//ɾ����
        if (position.linkPos != headPtr->next) {//������ǵ�һ��
            PtrToNode currentPtr = headPtr;
            for (;currentPtr->next != position.linkPos;currentPtr = currentPtr->next);//�ҵ�ɾ���е���һ��

            struct Position tempPosition = { currentPtr, currentPtr->strRear };
            if (position.linkPos->strRear != -1)
                RowCopy(position.linkPos, currentPtr, 0, position.linkPos->strRear, currentPtr->strRear + 1);//��ǰ��ճ������һ��ĩβ
            LinkDelete(headPtr, position.linkPos, position.linkPos->next);//ɾ����һ��
            position = tempPosition;//���µ�ǰ�����ָλ��(��Ϊ��һ�е� ԭ ĩβ)
        }
    }
    else{//ɾ�����ǰһ����
        //ת����ɾ������һ����
        Single single;
        single.Direction(2, position, headPtr);
        single.Delete(headPtr, position);
    }
}

/* ɾ�������һ���� */
void Single::Delete(Link headPtr, const struct Position position)
{
    PtrToNode currentPtr = position.linkPos;

    //�ӹ������λ�ÿ�ʼŲ
    if (position.strPos == currentPtr->strRear) {//��ĩ
        if (currentPtr->next) {//�����һ������,����һ�е���ȫ���ƶ�����һ��ĩβ
            RowCopy(currentPtr->next, currentPtr, 0, currentPtr->next->strRear, currentPtr->strRear + 1);
            LinkDelete(headPtr, currentPtr->next, currentPtr->next->next);//ɾ����һ��
        }
    }
    else {//������ĩ
        for (int i = position.strPos + 1;i <= currentPtr->strRear;i++)//�ӹ���ʼ��һ������ǰŲ
            *(currentPtr->heap + i) = *(currentPtr->heap + i + 1);
        position.linkPos->strRear--;//���µ�ǰ������ַ�����λ��
    }
}
