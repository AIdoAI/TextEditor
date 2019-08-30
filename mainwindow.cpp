#include <QGraphicsView>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <iostream>
#include "functions.h"
#include "single_operation.h"
#include "string_operation.h"
#include "trunk_operation.h"
#include "text.h"
#include <vector>

using namespace std;

MainWindow::~MainWindow()
{
    LinkDelete(headPtr, headPtr->next, NULL);
    delete headPtr;
    LinkDelete(bufferPtr, bufferPtr->next, NULL);
    delete bufferPtr;

    vector<CanRec>::iterator iter = cancel.begin();
    for(;iter!=cancel.end();iter++)
        LinkDelete(iter->dataPtr, iter->dataPtr, NULL);
    for(iter = recover.begin();iter!=recover.end();iter++)
        LinkDelete(iter->dataPtr, iter->dataPtr, NULL);

    delete ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
    setCentralWidget(view);
    resize(430,510);

    initScene();
    drawCursor();
    paintText();
    connect(text,SIGNAL(firstChanged(int,int)),cursor,SLOT(getFirstRC(int,int)));
    connect(cursor,SIGNAL(cursorChanged(int,int)),text,SLOT(getCursorRange(int,int)));

    headPtr = new Node; //行链表头指针
    headPtr->next = NULL;
    LinkInsert(headPtr);
    bufferPtr = new Node; //缓存区头指针
    bufferPtr->next = NULL;

    position2 = position1 = { headPtr->next,-1 }; //撤销之前的光标、前一步光标、当前光标所在位置初始化
    LR = 0;

    scene->installEventFilter(this);
}

void MainWindow :: initScene()
{
    scene->setSceneRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == scene){
        if (event->type() == QEvent::KeyPress){
            QKeyEvent *ev = static_cast<QKeyEvent *>(event);
            MainWindow::keyPressEvent(ev);
            return true;
        }
    }
    return QMainWindow::eventFilter(target, event);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    Trunk trunk;
    Single single;

    //撤销准备：保存前一次操作时的数据结构和光标位置和状态
    struct Position tempposition1, tempposition2;
    Link tempcancelPtr = new Node;
    tempcancelPtr->next = NULL;
    LinkInsert(tempcancelPtr);

    trunk.SelectAll(headPtr, tempposition1, tempposition2);
    trunk.TrunkCopy(tempcancelPtr, tempposition1, tempposition2);
    PosToPos(headPtr, position1, tempcancelPtr, tempposition1);
    PosToPos(headPtr, position2, tempcancelPtr, tempposition2);
    int tempLR = LR;

    
    if (ev->modifiers() == Qt::ControlModifier) {
        //WFile wf;

        switch(ev->key()){
        case Qt::Key_N: //ctrl+N 新建文件
            //wf.FileNew(headPtr);
            break;
        case Qt::Key_O: //ctrl+O 打开文件
            //wf.FileOpen(headPtr);
            break;
        case Qt::Key_S: //ctrl+S 保存文件
            //wf.FileSave(headPtr);
            break;
        case Qt::Key_E: //ctrl+E 文件另存为
            //wf.FileSave_As(headPtr);
            break;

        case Qt::Key_A: //ctrl+A 全选
            trunk.SelectAll(headPtr, position1, position2);
            LR = 2;
            PositionRowColumn(headPtr, position2, cursor);
            PositionShadow(headPtr, position1, position2, text);
            break;
        case Qt::Key_X: //ctrl+x 剪切
            trunk.TrunkCut(headPtr, bufferPtr, position1, position2);
            LR = 0;
            LinkToQstring(headPtr, text);
            PositionRowColumn(headPtr, position2, cursor);
            break;
        case Qt::Key_C: //ctrl+c 拷贝
            trunk.TrunkCopy(bufferPtr, position1, position2);
            break;
        case Qt::Key_V://ctrl+v 粘贴
            trunk.TrunkPaste(bufferPtr, position2);
            position1 = position2;
            LR = 0;
            LinkToQstring(headPtr, text);
            PositionRowColumn(headPtr, position2, cursor);
            break;

        case Qt::Key_Z: //ctrl+z 撤销
        case Qt::Key_Y: //ctrl+y 恢复
            if(ev->key() == Qt::Key_Z && cancel.size()>0){
                //恢复准备：保存撤销前一次操作时的数据结构和光标位置和状态
                CanRec rc = {headPtr, position1, position2, LR};
                recover.push_back(rc);
                //撤销
                vector<CanRec>::iterator iter = cancel.end() - 1;
                headPtr = iter->dataPtr;
                position1 = iter->preposition1;
                position2 = iter->preposition2;
                LR = iter->preLR;
                cancel.erase(iter);
            }
            else if(ev->key() == Qt::Key_Y && recover.size()>0){
                vector<CanRec>::iterator iter = recover.end() - 1;
                headPtr = iter->dataPtr;
                position1 = iter->preposition1;
                position2 = iter->preposition2;
                LR = iter->preLR;
                recover.erase(iter);
            }

            LinkToQstring(headPtr, text);
            if(LR == 1)
                PositionRowColumn(headPtr, position1, cursor);
            else
                PositionRowColumn(headPtr, position2, cursor);
            if(LR)
                PositionShadow(headPtr, position1, position2, text);

            if(ev->key() == Qt::Key_Z)
                return;
            break;

//        case Qt::Key_F://ctrl+f 查找字符串
//            StrOperation stroper;
//            stroper.StringInteraction(position2, headPtr, "查找", text, cursor);
//            position1 = position2;
//            LR = 0;
//            break;
//        case Qt::Key_R://ctrl+r 替换字符串
//            StrOperation stroper;
//            stroper.StringInteraction(position2, headPtr, "替换", text, cursor);
//            position1 = position2;
//            LR = 0;
//            break;

        default:
            return;
        }//end of switch
        if(ev->key()!=Qt::Key_C && ev->key()!=Qt::Key_A)//拷贝 和 全选 不删除阴影
            text->deleteShadow();
    }//end of if

    else if (ev->modifiers() == Qt::ShiftModifier) {//块选择
        switch (ev->key()) {
        case Qt::Key_Up:
            if (!LR || LR == 1 || (LR == 2 && position1.linkPos == position2.linkPos && position2.linkPos != headPtr->next)){
                single.Direction(UP, position1, headPtr);
                PositionRowColumn(headPtr, position1, cursor);
                LR = 1;
            }
            else{
                single.Direction(UP, position2, headPtr);
                PositionRowColumn(headPtr, position2, cursor);
            }
            break;
        case Qt::Key_Down:
            if(!LR || LR == 2 || (LR == 1 && position1.linkPos == position2.linkPos && position2.linkPos->next)){
                single.Direction(DOWN, position2, headPtr);
                PositionRowColumn(headPtr, position2, cursor);
                LR = 2;
            }
            else{
                single.Direction(DOWN, position1, headPtr);
                PositionRowColumn(headPtr, position1, cursor);
            }
            break;
        case Qt::Key_Left:
            if (!LR || LR == 1 || (LR == 2 && position1.linkPos == position2.linkPos && position1.strPos == position2.strPos)){
                single.Direction(LEFT, position1, headPtr);
                PositionRowColumn(headPtr, position1, cursor);
                LR = 1;
            }
            else{
                single.Direction(LEFT, position2, headPtr);
                PositionRowColumn(headPtr, position2, cursor);
            }
            break;
        case Qt::Key_Right:
            if(!LR || LR == 2 || (LR == 1 && position1.linkPos == position2.linkPos && position1.strPos == position2.strPos)){
                single.Direction(RIGHT, position2, headPtr);
                PositionRowColumn(headPtr, position2, cursor);
                LR = 2;
            }
            else{
                single.Direction(RIGHT, position1, headPtr);
                PositionRowColumn(headPtr, position1, cursor);
            }
            break;

        default:
            return;
        }//end of switch
        PositionShadow(headPtr, position1, position2, text);
    }//end of else if

    else {
        text->deleteShadow();

        switch (ev->key()) {
        case Qt::Key_Home:
        case Qt::Key_End:
            if(ev->key() == Qt::Key_Home)
                position2 = {headPtr->next, -1};
            else
                trunk.SelectAll(headPtr, position1, position2);
            position1 = position2;
            LR = 0;
            PositionRowColumn(headPtr, position2, cursor);
            break;

        //方向键
        case Qt::Key_Up:
        case Qt::Key_Down: {
            int direction = (ev->key() == Qt::Key_Up) ? UP : DOWN;
            if (LR == 0 || LR == 1) {
                single.Direction(direction, position1, headPtr);
                position2 = position1;
            }
            else if (LR == 2) {
                single.Direction(direction, position2, headPtr);
                position1 = position2;
            }
            LR = 0;
            PositionRowColumn(headPtr, position2, cursor);
            break;
        }
        case Qt::Key_Left:
        case Qt::Key_Right: {
            int direction = (ev->key() == Qt::Key_Left) ? LEFT : RIGHT;
            if (LR){
                if(direction == LEFT)
                    position2 = position1;
                else
                    position1 = position2;
            }
            else {
                single.Direction(direction, position2, headPtr);
                position1 = position2;
            }
            LR = 0;
            PositionRowColumn(headPtr, position2, cursor);
            break;
        }

        case Qt::Key_Escape: //Esc退出
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
            PositionRowColumn(headPtr, position2, cursor);
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
            LinkToQstring(headPtr, text);
            PositionRowColumn(headPtr, position2, cursor);
            break;

        case Qt::Key_Enter:
        case Qt::Key_Return: //key_enter对应的是小键盘的enter，key_return对应的是主键盘的enter
            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.AddRow(position2);//换行
            position1 = position2;
            LR = 0;
            LinkToQstring(headPtr, text);
            PositionRowColumn(headPtr, position2, cursor);
            break;

        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
        case Qt::Key_CapsLock:
        case Qt::Key_Alt:
        case Qt::Key_Pause:
        case Qt::Key_Insert:

        case Qt::Key_F1:
        case Qt::Key_F2:
        case Qt::Key_F3:
        case Qt::Key_F4:
        case Qt::Key_F5:
        case Qt::Key_F6:
        case Qt::Key_F7:
        case Qt::Key_F8:
        case Qt::Key_F9:
        case Qt::Key_F10:
        case Qt::Key_F11:
        case Qt::Key_F12:
            return;

        default: {
            QString qpress = ev->text();
            string spress = qpress.toStdString();
            const char *cpress = spress.c_str();
            char p = cpress[0];

            if (LR)
                trunk.TrunkDelete(headPtr, position1, position2);
            single.Insert(p, position2);//插入字符
            position1 = position2;
            LR = 0;
            LinkToQstring(headPtr, text);
            PositionRowColumn(headPtr, position2, cursor);
            break;
        }
        }//end of switch
    }//end of else


    //更新撤销区 前一步光标所在位置 和 状态
    CanRec cc = {tempcancelPtr, tempposition1, tempposition2, tempLR};
    cancel.push_back(cc);
}

void MainWindow::drawCursor()
{
    cursor = new Cursor();
    cursor->setFocus();
    cursor->setFocusPolicy(Qt::StrongFocus);
    //设置背景透明
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0,0,0,0));
    cursor->setPalette(palette);
    scene->addWidget(cursor);
}

void MainWindow::paintText()
{
    text = new Text( );
    QPalette tpalette;
    tpalette.setColor(QPalette::Background, QColor(0,0,0,0));
    text->setPalette(tpalette);
    scene->addWidget(text);
    scene->addWidget(text->V_bar);
}

void MainWindow::on_actionSearch_triggered()
{
    QDialog *findDlg = new QDialog(this);
    findDlg->setWindowTitle("查找");
    //将行编辑器的父窗口设置为刚才新建的对话框中
    findText = new QLineEdit(findDlg);
    findText->setPlaceholderText("请输入想要查找的内容：");
    QPushButton *findBtn = new QPushButton("向后查找",findDlg);
    QPushButton *findNextBtn = new QPushButton("下一个",findDlg);

    //垂直布局
    QVBoxLayout *layout = new QVBoxLayout(findDlg);
    layout->addWidget(findText);
    layout->addWidget(findBtn);
    layout->addWidget(findNextBtn);

    findDlg->show();
    connect(findBtn,SIGNAL(clicked()),this,SLOT(findTextNext()));
    connect(findNextBtn,SIGNAL(clicked()),this,SLOT(findTextNext()));

    QMessageBox box;
    box.setWindowTitle(tr("文档编辑器"));
    box.setText(tr("是否要查找？"));
    //水平布局
//    QHBoxLayout *layout = new QHBoxLayout(isFind);
//    layout->addWidget(yes);
//    layout->addWidget(no);
//    isFind->show();
//    connect(yes,SIGNAL(clicked()),this,SLOT(findText()));
}

//void MainWindow::findText()
//{
//}

//void MainWindow::findTextNext()//第一次
//{
//    //获取行编辑器输入内容
//    QString ftext = findText->text();
//    //向后查找
//    //查找失败
//    StrOperation strOper;
//    if(!strOper.FindString(headPtr, ftext, position2)){
//        QMessageBox::warning(this,"查找",tr("找不到 %1").arg(ftext));
//    }
//}

//void MainWindow::findTextNext()//第二次
//{
//    //获取行编辑器输入内容
//    QString ftext = findText->text();

//    //向后查找
//    StrOperation strOper;
//    if(!strOper.FindString(headPtr, ftext, position2)){//查找失败
//        QMessageBox::warning(this,"查找",tr("找不到 %1").arg(ftext));
//    }
//}

//void MainWindow::on_actionReplace_triggered()
//{
//    QDialog *searchDlg = new QDialog(this);
//    searchDlg->setWindowTitle("替换");
//    sourceRepl = new QLineEdit(searchDlg);      //将行编辑器的父窗口设置为刚才新建的对话框中
//    destRepl = new QLineEdit(searchDlg);
//    sourceRepl->setPlaceholderText("请输入想被替换的内容：");        //设置提示文本
//    destRepl->setPlaceholderText("请输入想替换的内容：");
//    QPushButton *searBtn = new QPushButton("替换",searchDlg);

//    //垂直布局
//    QVBoxLayout *layout = new QVBoxLayout(searchDlg);
//    layout->addWidget(sourceRepl);
//    layout->addWidget(destRepl);
//    layout->addWidget(searBtn);

//    searchDlg->show();
//    connect(searBtn,SIGNAL(clicked()),this,SLOT(replaceText()));
//}

//void MainWindow::replaceText()
//{
//    //获取行编辑器输入内容
//    QString sourceText = sourceRepl->text();        //被替换文本
//    QString desText = destRepl->text();             //替换文本
//    str = sourceText.toStdString();
//    replacestr = desText.toStdString();

//    StrOperation strOper;
//    Single single;
//    bool subflag = 0, flag = 0;//找到为1没找到为0
//    struct Position tempPosition = position;

//    do {
//        if (strOper.FindString(headPtr, str, tempPosition)) {
//            PositionRowColumn(headPtr, tempPosition, cursor);
//            if(!subflag){//如果是第一次进这个循环
//                position = tempPosition;
//                //询问是否全部替换
//                if()
//                    return;
//            }
//            //先删除原字符串,再插入新字符串
//            for (int count = 1;count <= str.length();count++) {
//                if (position.strPos == position.linkPos->strRear)
//                      single.Direction(3, position, headPtr);
//                single.Delete(headPtr, position);
//                single.Insert(replaceStr[count - 1], position);
//            }
//            subflag = 1;
//            flag = 1;
//            for (int count = 0;count <= str.length();count++)
//                single.Direction(3, tempPosition, headPtr);//光标右移，继续寻找下一个
//        }
//        else
//            subflag = 0;
//    } while (subflag);

//    if (!flag)//替换失败
//        QMessageBox::warning(this,"替换",tr("找不到 %1").arg(sourceText));
//    else{

//        LinkToQstring(headPtr, text);
//    }

////    if(1){
////        QMessageBox::warning(this,"替换",tr("找不到 %1").arg(sourceText));
////    }
//}
