#include "trunk_operation.h"
#include "single_operation.h"
#include "string_operation.h"
#include "functions.h"

#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>
#include "my_lineedit.h"
#include "Wfile.h"

/* ����������ݽṹ */
void Output(Link headPtr)
{
    PtrToNode currentPtr = headPtr->next;
    for (;currentPtr;currentPtr = currentPtr->next) {
        for (int i = 0;i <= currentPtr->strRear;i++)
            qDebug() << *(currentPtr->heap + i);
        qDebug() << endl;
    }
}

MyEdit::MyEdit(QWidget *parent) : QLineEdit(parent)
{
    edit->setPlaceholderText("Input:");
    edit->installEventFilter(this); //�¼�����

    headPtr = new Node; //������ͷָ��
    headPtr->next = NULL;
    LinkInsert(headPtr);
    bufferPtr = new Node; //������ͷָ��
    bufferPtr->next = NULL;
    cancelPtr = new Node; //������ͷָ��
    cancelPtr->next = NULL;

    preposition1 = position1 = { headPtr->next,-1 }; //ǰһ����ꡢ��ǰ�������λ�ó�ʼ��
    preposition2 = position2 = position1;
    LR = 0;
}

MyEdit::~MyEdit(void)
{
    LinkDelete(headPtr, headPtr->next, NULL);
    delete headPtr;
    LinkDelete(bufferPtr, bufferPtr->next, NULL);
    delete bufferPtr;
    LinkDelete(cancelPtr, cancelPtr->next, NULL);
    delete cancelPtr;
}

bool MyEdit::eventFilter(QObject *target, QEvent *event)
{
    if (target == edit){
        if (event->type() == QEvent::KeyPress){
            QKeyEvent *ev = static_cast<QKeyEvent *>(event);
            MyEdit::keyPressEvent(ev);
            return true;
        }
    }
    return QLineEdit::eventFilter(target, event);
}

void MyEdit::keyPressEvent(QKeyEvent *ev)
{
    Trunk trunk;
    Single single;
    StrOperation stroper;

    //����׼��������ǰһ�β���ʱ�����ݽṹ�͹��λ��
    struct Position tempposition1, tempposition2;
    Link tempcancelPtr = new Node;
    tempcancelPtr->next = NULL;
    trunk.SelectAll(headPtr, tempposition1, tempposition2);
    trunk.TrunkCopy(headPtr, tempcancelPtr, tempposition1, tempposition2);
    tempposition1 = position1;
    tempposition2 = position2;


    if (ev->modifiers() == Qt::ControlModifier) {
        WFile wf;

        switch(ev->key()){
        case Qt::Key_N: //ctrl+N �½��ļ�
            wf.FileNew(headPtr);
            break;
        case Qt::Key_O: //ctrl+O ���ļ�
            wf.FileOpen(headPtr);
            break;
        case Qt::Key_S: //ctrl+S �����ļ�
            wf.FileSave(headPtr);
            break;
        case Qt::Key_E: //ctrl+E �ļ����Ϊ
            wf.FileSave_As(headPtr);
            break;

        case Qt::Key_A: //ctrl+A ȫѡ
            trunk.SelectAll(headPtr, position1, position2);
            LR = 2;
            break;
        case Qt::Key_X: //ctrl+x ����
            trunk.TrunkCut(headPtr, bufferPtr, position1, position2);
            Output(headPtr);
            LR = 0;
            break;
        case Qt::Key_C: //ctrl+c ����
            trunk.TrunkCopy(headPtr, bufferPtr, position1, position2);
            Output(headPtr);
            break;
        case Qt::Key_V://ctrl+v ճ��
            trunk.TrunkPaste(headPtr, bufferPtr, position2);
            Output(headPtr);
            position1 = position2;
            LR = 0;
            break;

        case Qt::Key_Z: {//ctrl+z ����(��֧��һ��)
            LinkDelete(headPtr, headPtr->next, NULL);
            headPtr->next = NULL;
            LinkInsert(headPtr);
            struct Position tempposition = { headPtr->next, -1 };
            trunk.TrunkPaste(headPtr, cancelPtr, tempposition);
            position1 = preposition1;
            position2 = preposition2;
            Output(headPtr);
            return;
        }
        case Qt::Key_Y://ctrl+y �ָ�
            break;

        case Qt::Key_F://ctrl+f �����ַ���
            stroper.StringInteraction(position2, headPtr, "����");
            position1 = position2;
            LR = 0;
            break;
        case Qt::Key_R://ctrl+r �滻�ַ���
            stroper.StringInteraction(position2, headPtr, "�滻");
            position1 = position2;
            LR = 0;
            break;

        default:
            return;
        }
    }

    else if (ev->modifiers() == Qt::ShiftModifier) {//��ѡ��
        switch (ev->key()) {
        case Qt::Key_Up:
            single.Direction(UP, position1, headPtr);
            LR = 1;
            break;
        case Qt::Key_Down:
            single.Direction(DOWN, position2, headPtr);
            LR = 2;
            break;
        case Qt::Key_Left:
            single.Direction(LEFT, position1, headPtr);
            LR = 1;
            break;
        case Qt::Key_Right:
            single.Direction(RIGHT, position2, headPtr);
            LR = 2;
            break;

        default:
            return;
        }
    }

    else {
        switch (ev->key()) {
        //�����
        case Qt::Key_Up:
        case Qt::Key_Down: {
            int direction = (ev->key() == Qt::Key_Up) ? UP : DOWN;
            if (LR == 0 || LR == 1) {
                single.Direction(direction, position2, headPtr);
                position1 = position2;
            }
            else if (LR == 2) {
                single.Direction(direction, position1, headPtr);
                position2 = position1;
            }
            LR = 0;
            break;
        }
        case Qt::Key_Left:
        case Qt::Key_Right: {
            int direction = (ev->key() == Qt::Key_Left) ? LEFT : RIGHT;
            if (LR == 1)
                position1 = position2;
            else if(LR == 2)
                position2 = position1;
            else {
                single.Direction(direction, position2, headPtr);
                position1 = position2;
            }
            LR = 0;
            break;
        }

        case Qt::Key_Escape:
            if (LR == 1)
                position2 = position1;
            else if (LR == 2)
                position1 = position2;
            else{
                qDebug() << "�˳�Miniword" << endl;
                //�ر��ļ�
                this->close();
            }
            LR = 0;
            break;

        case Qt::Key_Backspace: //Backspace�˸�
        case Qt::Key_Delete: //Deleteɾ��
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            else {
                ev->key() == Qt::Key_Backspace ? single.BackSpace(headPtr, position2) : single.Delete(headPtr, position2);
                position1 = position2;
            }
            LR = 0;
            Output(headPtr);
            break;

        case Qt::Key_Enter:
        case Qt::Key_Return: //key_enter��Ӧ����С���̵�enter�������̵�enter��Ӧ����key_return
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.AddRow(headPtr, position2);//����
            position1 = position2;
            Output(headPtr);
            LR = 0;
            break;
        default: {
            int press1 = ev->key();
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.Insert(headPtr, press1, position2);//�����ַ�
            position1 = position2;
            Output(headPtr);
            LR = 0;
            break;
        }
        }//end of switch
    }//end of else


    //���³�����
    LinkDelete(cancelPtr, cancelPtr->next, NULL);
    delete cancelPtr;
    cancelPtr = tempcancelPtr;
    //����ǰһ���������λ��
    preposition1 = tempposition1;
    preposition2 = tempposition2;
}
