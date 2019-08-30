#include "cursor.h"
#include <iostream>
using namespace std;
Cursor::Cursor(QWidget *parent) : QWidget(parent)
{
    setGeometry(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    row = 0;
    col = 0;

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
    //光标闪烁
    QTimer *blink = new QTimer(this);
    connect(blink,SIGNAL(timeout()),this,SLOT(cursorBlink()));
    blink->start(500);
}

void Cursor :: paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (isTrans)    //透明
        painter.setPen(Qt::white);
    else
        painter.setPen(Qt::black);
    painter.drawLine((col-first_col)*WORD_WIDTH, (row-first_row)*WORD_HEIGHT,
                     (col-first_col)*WORD_WIDTH, (row-first_row)*WORD_HEIGHT+CUR_HEIGHT);
//    cout<<"fr = "<<first_row<<" fc = "<<first_col<<endl;
//    cout<<"x = "<<col-first_col<<" y = "<<row-first_row<<endl;
}

void Cursor::setCursor(int nr,int nc)
{
    row = nr;
    col = nc;
    emit cursorChanged(row,col);
}

void Cursor::cursorBlink()
{
    if (isTrans)
        isTrans = false;
    else
        isTrans = true;
}

int Cursor::getx() {return col*WORD_WIDTH;}
int Cursor::gety() {return row*WORD_HEIGHT;}

void Cursor::getFirstRC(int fr,int fc)
{
    first_col = fc;
    first_row = fr;
}
