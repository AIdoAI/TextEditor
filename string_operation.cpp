/*
 * 对字符串的操作
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
//写在前面：字符串从1开始存储而不是从0！
//*************************************

/* 求模式串str的next函数修正值并存入数组nextval */
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
 * 查找字符串：(使用KMP算法)
 * 提示用户输入要查找的字符串，从当前光标处向后定位，
 * 找到时光标置于首字符之前，没找到则不改变光标位置。
 */
bool StrOperation::FindString(Link headPtr, string str, struct Position &position)
{
    if (IsLinkEnd(position))
        return 0;

    str = '#' + str;
    int *nextval = new int[str.length() - 1];
    Nextval(str, nextval);

    //从当前光标处向后定位，找到最近一个符合的字符串
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

    if (j > str.length() - 1) {//匹配成功
        //找到时光标置于首字符之前
        PtrToNode currentPtr2 = currentPtr;
        for (int count = 1;count <= str.length();count++){
            if (i == -1) {
                for (currentPtr2 = headPtr;currentPtr2->next != currentPtr;currentPtr2 = currentPtr2->next);//找到上一行
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
 * 替换字符串：
 * 提示用户输入原字符串和新字符串，从当前光标处向后定位，
 * 找到时光标置于首字符之前，由用户对是否替换进行确认。
 * 替换后光标置于新字符串之后
 */
void StrOperation::ReplaceString(Link headPtr, string str, string replaceStr, struct Position &position)
{
    Single single;

    str = "替换成" + replaceStr + ",确定吗？(确定请按回车键，否则请按其他键)" + '\n';
    QString qstr = QString::fromStdString(str);
    qDebug() << qstr;
    char c = 0;
    if ((c=_getch()) == 13) {
        //先删除原字符串,再插入新字符串
        for (int count = 1;count <= str.length();count++) {
            if (position.strPos == position.linkPos->strRear) {
                single.Direction(3, position, headPtr);
            }
            single.Delete(headPtr, position);
            single.Insert(replaceStr[count - 1], position);
        }
        qDebug() << "替换成功，";
    }
}

/* 查找or替换字符串的用户交互 */
void StrOperation::StringInteraction(struct Position &position, Link headPtr, string fr, Text *text, Cursor *cursor)
{//fr: "查找"or"替换"
    string str = "", replaceStr = "";

    qDebug() << "请输入要查找的字符串: ";
    QTextStream qin(stdin);
    QString qstr;
    qin >> qstr;
    str = qstr.toStdString();
    getchar();//吸收空格键
    if (fr == "替换") {
        qDebug() << "请输入要替换的新字符串(空串请直接按回车键): ";
        char c = getchar();
        while (c != '\n') {
            replaceStr += c;
            c = getchar();
        }
    }

    StrOperation strOper;
    Single single;
    char tempPress = 0;
    bool subflag = 0, flag = 0;//找到为1没找到为0
    struct Position tempPosition = position;

    do {
        if (strOper.FindString(headPtr, str, tempPosition)) {
            PositionRowColumn(headPtr, position, cursor);
            if (fr == "替换"){
                strOper.ReplaceString(headPtr, str, replaceStr, tempPosition);
                LinkToQstring(headPtr, text);
                PositionRowColumn(headPtr, position, cursor);
             }
            position = tempPosition;
            subflag = 1;
            flag = 1;
            qDebug() << "继续查找下一个请按回车，退出请按ESC" << endl;
            if ((tempPress = _getch()) == 27)
                break;
            for (int count = 0;count <= str.length();count++)
                single.Direction(3, tempPosition, headPtr);//光标右移，继续寻找下一个
        }
        else
            subflag = 0;
    } while (subflag);

    if (!flag)
        qDebug() << QString::fromStdString("抱歉，没找到" + str) << endl << endl;
    else {
        if (tempPress != 27){
            qDebug() << "无更多相符结果，";
            PositionRowColumn(headPtr, position, cursor);//光标显示在数据结构末尾
        }
        qDebug() << QString::fromStdString(fr) << "结束" << endl << endl;
    }
}
