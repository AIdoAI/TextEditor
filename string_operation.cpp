/*
 * ���ַ����Ĳ���
 *
 * qreamly
 * 2018.5.21
 */

#include <iostream>
#include <string>
#include <conio.h>
#include "entitle.h"
#include "string_operation.h"
#include "single_operation.h"
#include "functions.h"

#include <QDebug>

using namespace std;

//*************************************
//д��ǰ�棺�ַ�����1��ʼ�洢�����Ǵ�0��
//*************************************

/* ��ģʽ��str��next��������ֵ����������nextval */
void Nextval(string str, int *nextval)
{
    int i = 1;
    nextval[1] = 0;
    int j = 0;
    while (i < str.length()) {
        if (j == 0 || str[i] == str[j]) {
            ++i;
            ++j;
            str[i] != str[j] ? nextval[i] = j : nextval[i] = nextval[j];
        }
        else
            j = nextval[j];
    }
}
/*
 * �����ַ�����(ʹ��KMP�㷨)
 * ��ʾ�û�����Ҫ���ҵ��ַ������ӵ�ǰ��괦���λ��
 * �ҵ�ʱ����������ַ�֮ǰ��û�ҵ��򲻸ı���λ�á�
 */
bool StrOperation::FindString(Link headPtr, string str, struct Position &position)
{
    if (IsLinkEnd(position))
        return 0;

    str = '#' + str;
    int *nextval = new int[str.length() - 1];
    Nextval(str, nextval);

    //�ӵ�ǰ��괦���λ���ҵ����һ�����ϵ��ַ���
    PtrToNode currentPtr = position.linkPos;
    int i = position.strPos + 1;
    int j = 1;
    while (j <= str.length() - 1) {
        if (i > currentPtr->strRear) {
            if (!currentPtr->next)
                break;
            i = 0;
            currentPtr = currentPtr->next;
        }
        if (j == 0 || *(currentPtr->heap + i) == str[j]) {
            ++i;
            ++j;
        }
        else
            j = nextval[j];
    }

    if (j > str.length() - 1) {//ƥ��ɹ�
        //�ҵ�ʱ����������ַ�֮ǰ
        PtrToNode currentPtr2 = currentPtr;
        for (int count = 1;count <= str.length();count++){
            if (i == -1) {
                for (currentPtr2 = headPtr;currentPtr2->next != currentPtr;currentPtr2 = currentPtr2->next);//�ҵ���һ��
                currentPtr = currentPtr2;
                i = currentPtr2->strRear - 1;
            }
            else
                i--;
        }
        position = { currentPtr2, i };
        return 1;
    }
    return 0;

    delete []nextval;
}

/*
 * �滻�ַ�����
 * ��ʾ�û�����ԭ�ַ��������ַ������ӵ�ǰ��괦���λ��
 * �ҵ�ʱ����������ַ�֮ǰ�����û����Ƿ��滻����ȷ�ϡ�
 * �滻�����������ַ���֮��
 */
void StrOperation::ReplaceString(Link headPtr, string str, string replaceStr, struct Position &position)
{
    Single single;

    str = "�滻��" + replaceStr + ",ȷ����(ȷ���밴�س����������밴������)" + '\n';
    QString qstr = QString::fromStdString(str);
    qDebug() << qstr;
    char c = 0;
    if ((c=_getch()) == 13) {
        //��ɾ��ԭ�ַ���,�ٲ������ַ���
        for (int count = 1;count <= str.length();count++) {
            if (position.strPos == position.linkPos->strRear) {
                single.Direction(3, position, headPtr);
            }
            single.Delete(headPtr, position);
            single.Insert(replaceStr[count - 1], position);
        }
        qDebug() << "�滻�ɹ���";
    }
}

/* ����or�滻�ַ������û����� */
void StrOperation::StringInteraction(struct Position &position, Link headPtr, string fr, Text *text, Cursor *cursor)
{//fr: "����"or"�滻"
    string str = "", replaceStr = "";

    qDebug() << "������Ҫ���ҵ��ַ���: ";
    QTextStream qin(stdin);
    QString qstr;
    qin >> qstr;
    str = qstr.toStdString();
    getchar();//���տո��
    if (fr == "�滻") {
        qDebug() << "������Ҫ�滻�����ַ���(�մ���ֱ�Ӱ��س���): ";
        char c = getchar();
        while (c != '\n') {
            replaceStr += c;
            c = getchar();
        }
    }

    StrOperation strOper;
    Single single;
    char tempPress = 0;
    bool subflag = 0, flag = 0;//�ҵ�Ϊ1û�ҵ�Ϊ0
    struct Position tempPosition = position;

    do {
        if (strOper.FindString(headPtr, str, tempPosition)) {
            PositionRowColumn(headPtr, position, cursor);
            if (fr == "�滻"){
                strOper.ReplaceString(headPtr, str, replaceStr, tempPosition);
                LinkToQstring(headPtr, text);
                PositionRowColumn(headPtr, position, cursor);
             }
            position = tempPosition;
            subflag = 1;
            flag = 1;
            qDebug() << "����������һ���밴�س����˳��밴ESC" << endl;
            if ((tempPress = _getch()) == 27)
                break;
            for (int count = 0;count <= str.length();count++)
                single.Direction(3, tempPosition, headPtr);//������ƣ�����Ѱ����һ��
        }
        else
            subflag = 0;
    } while (subflag);

    if (!flag)
        qDebug() << QString::fromStdString("��Ǹ��û�ҵ�" + str) << endl << endl;
    else {
        if (tempPress != 27){
            qDebug() << "�޸�����������";
            PositionRowColumn(headPtr, position, cursor);//�����ʾ�����ݽṹĩβ
        }
        qDebug() << QString::fromStdString(fr) << "����" << endl << endl;
    }
}
