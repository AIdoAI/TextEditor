#ifndef   STRING_OPERATION       //���û�ж��������
#define   STRING_OPERATION       //���������

#include <iostream>
#include <string.h>
#include "mainwindow.h"

using namespace std;

//ע�������ַ�����1��ʼ�洢

class StrOperation{
    friend class MainWindow;

    public:

    private:
        /* �����ַ���(ʹ��KMP�㷨) */
        bool FindString(Link headPtr, string str, struct Position &position);
        /* �滻�ַ��� */
        void ReplaceString(Link headPtr, string str, string replaceStr, struct Position &position);
        /* ����or�滻�ַ������û����� (fr: "����"or"�滻")*/
        void StringInteraction(struct Position &position, Link headPtr, string fr, Text *text, Cursor *cursor);
};

#endif
