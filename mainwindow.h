#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>
#include <QLineEdit>
#include "cursor.h"
#include "text.h"
#include "entitle.h"
#include <vector>

typedef struct CanRec{
    Link dataPtr; //撤销区/恢复区
    struct Position preposition1;
    struct Position preposition2; //前一步光标位置
    int preLR; //前一步光标状态
}CanRec;

class QGraphicsScene;
class QGraphicsView;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
  //  void adjustViewSize();          //页面大小初始化

    void on_actionSearch_triggered();

private :
    void initScene();
    void drawCursor();
    void paintText();
    void keyPressEvent(QKeyEvent *ev);
    bool eventFilter(QObject *target, QEvent *event);

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Cursor *cursor;
    Text *text;
    QLineEdit *findText;            //用于输入要查找的内容
    QLineEdit *sourceRepl;          //被替换文本
    QLineEdit *destRepl;            //替换文本

    Link headPtr, bufferPtr, recoverPtr; //行链表头指针，缓存区头指针，恢复区指针

    vector<CanRec> cancel;
    vector<CanRec> recover;

    struct Position position1, position2; //当前光标
    int LR;//块选择时显示的光标是position1（LR为1）还是position2（LR为2）,不是块选择状态（LR=0）

private slots:
//    void findText();                    //查找文本界面
    void findTextNext();                //向后查找
    void replaceText();                 //文本替换
    void on_actionReplace_triggered();
};


#endif // MAINWINDOW_H
