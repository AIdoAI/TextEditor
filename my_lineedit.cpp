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

/* 输出整个数据结构 */
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
    edit->installEventFilter(this); //事件过滤

    headPtr = new Node; //行链表头指针
    headPtr->next = NULL;
    LinkInsert(headPtr);
    bufferPtr = new Node; //缓存区头指针
    bufferPtr->next = NULL;
    cancelPtr = new Node; //撤销区头指针
    cancelPtr->next = NULL;

    preposition1 = position1 = { headPtr->next,-1 }; //前一步光标、当前光标所在位置初始化
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

    //撤销准备：保存前一次操作时的数据结构和光标位置
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
        case Qt::Key_N: //ctrl+N 新建文件
            wf.FileNew(headPtr);
            break;
        case Qt::Key_O: //ctrl+O 打开文件
            wf.FileOpen(headPtr);
            break;
        case Qt::Key_S: //ctrl+S 保存文件
            wf.FileSave(headPtr);
            break;
        case Qt::Key_E: //ctrl+E 文件另存为
            wf.FileSave_As(headPtr);
            break;

        case Qt::Key_A: //ctrl+A 全选
            trunk.SelectAll(headPtr, position1, position2);
            LR = 2;
            break;
        case Qt::Key_X: //ctrl+x 剪切
            trunk.TrunkCut(headPtr, bufferPtr, position1, position2);
            Output(headPtr);
            LR = 0;
            break;
        case Qt::Key_C: //ctrl+c 拷贝
            trunk.TrunkCopy(headPtr, bufferPtr, position1, position2);
            Output(headPtr);
            break;
        case Qt::Key_V://ctrl+v 粘贴
            trunk.TrunkPaste(headPtr, bufferPtr, position2);
            Output(headPtr);
            position1 = position2;
            LR = 0;
            break;

        case Qt::Key_Z: {//ctrl+z 撤销(仅支持一次)
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
        case Qt::Key_Y://ctrl+y 恢复
            break;

        case Qt::Key_F://ctrl+f 查找字符串
            stroper.StringInteraction(position2, headPtr, "查找");
            position1 = position2;
            LR = 0;
            break;
        case Qt::Key_R://ctrl+r 替换字符串
            stroper.StringInteraction(position2, headPtr, "替换");
            position1 = position2;
            LR = 0;
            break;

        default:
            return;
        }
    }

    else if (ev->modifiers() == Qt::ShiftModifier) {//块选择
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
        //方向键
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
                qDebug() << "退出Miniword" << endl;
                //关闭文件
                this->close();
            }
            LR = 0;
            break;

        case Qt::Key_Backspace: //Backspace退格
        case Qt::Key_Delete: //Delete删除
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
        case Qt::Key_Return: //key_enter对应的是小键盘的enter，主键盘的enter对应的是key_return
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.AddRow(headPtr, position2);//换行
            position1 = position2;
            Output(headPtr);
            LR = 0;
            break;
        default: {
            int press1 = ev->key();
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.Insert(headPtr, press1, position2);//插入字符
            position1 = position2;
            Output(headPtr);
            LR = 0;
            break;
        }
        }//end of switch
    }//end of else


    //更新撤销区
    LinkDelete(cancelPtr, cancelPtr->next, NULL);
    delete cancelPtr;
    cancelPtr = tempcancelPtr;
    //更新前一步光标所在位置
    preposition1 = tempposition1;
    preposition2 = tempposition2;
}
