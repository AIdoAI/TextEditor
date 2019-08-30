#ifndef CURSOR_H
#define CURSOR_H

#include <QWidget>
#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QKeyEvent>

#define CUR_HEIGHT 25           //光标高度
#define WORD_WIDTH 17            //字符宽度
#define WORD_HEIGHT 28          //字符高度
#define SCREEN_WIDTH 425        //屏幕宽度
#define SCREEN_HEIGHT 425       //屏幕高度
#define SCREEN_MAX_ROW 14       //屏幕显示的最大行数
#define SCREEN_MAX_COL 23       //屏幕显示的最大列数

class Cursor : public QWidget
{
    Q_OBJECT
public:
    Cursor (QWidget * parent = 0);
    void setCursor(int nr,int nc);           //改变光标位置
    int getx();
    int gety();
public slots:
    void getFirstRC(int fr,int fc);                //获取显示的第一行和第一列
signals:
    void cursorChanged(int nr,int nc);        //告诉外界光标的位置变啦
protected:
    void paintEvent(QPaintEvent *event);
//    void keyPressEvent(QKeyEvent *event);    //键盘事件响应
protected slots:
    void cursorBlink();                      //光标闪烁
private:
    bool isTrans;                               //是否透明
    int row ;
    int col ;
    int first_row = 0;                  //界面显示的第一行与实文本行的对应
    int first_col = 0;                  //界面显示的第一列与实文本行的对应
    int shadow;
};

#endif // CURSOR_H
