#include "text.h"
#include "cursor.h"
#include <iostream>
//#include <QFontMetrics>

Text::Text(QWidget *parent) : QWidget(parent)
{
    setGeometry(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    //竖直滚动条
    V_bar = new QScrollBar(Qt::Vertical,this);
    V_bar->setMaximum(0);
    V_bar->setMinimum(0);
    V_bar->setSingleStep(1);                //设置滑块上下移动步长
    V_bar->setPageStep(SCREEN_MAX_ROW);     //设置滑块翻页步长
    V_bar->setSliderPosition(0);            //设置滑块位置
    V_bar->setGeometry(SCREEN_WIDTH-20,0,20,SCREEN_HEIGHT); //滚动条控件的大小和位置
    connect(V_bar,SIGNAL(valueChanged(int)),this,SLOT(setSliderValue(int)));

    //水平滚动条
    H_bar = new QScrollBar(Qt::Horizontal,this);
    H_bar->setMinimum(0);
    H_bar->setMaximum(0);
    H_bar->setSingleStep(1);
    H_bar->setPageStep(SCREEN_MAX_COL);
    H_bar->setSliderPosition(0);
    H_bar->setGeometry(0,SCREEN_HEIGHT-20,SCREEN_WIDTH,20);
    connect(H_bar,SIGNAL(valueChanged(int)),this,SLOT(setSliderValue(int)));


    //定时刷新文本内容
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

void Text::paintEvent(QPaintEvent *event)
{
    QFont font;         //字体信息
    QPainter painter(this);
    font.setPointSize(20);
    painter.setFont(font);
//    QFontMetrics fontMetrics(font);
//    cout<<"width"<<fontMetrics

    for(int i = first_row, j = 0 ; i < str.size(); i++, j++){
        painter.drawText(0,25+j*WORD_HEIGHT,str[i].mid(first_col));
    }

    //阴影绘制
    painter.setPen(Qt::NoPen);      //边界无颜色
    //设置画刷颜色(填充色)
    if (!isShadow)    //透明
        painter.setBrush(QColor(0,0,0,0));            //全透明
    else
        painter.setBrush(QColor(0,0,255,50));        //半透明，蓝色

//    cout<<"row1 = "<<row1<<" col1 ="<<col1<<" row2 = "<<row2<<" col2 = "<<col2<<endl;
    //绘制绘图边界
    if(row1 != row2){
        //不止一行
        //1st
        painter.drawRect(QRect(col1*WORD_WIDTH,row1*WORD_HEIGHT,
                               SCREEN_WIDTH,WORD_HEIGHT));
        //the mid
        painter.drawRect(QRect(0,(row1+1)*WORD_HEIGHT,
                         SCREEN_WIDTH,(row2-row1-1)*WORD_HEIGHT));
        //last
        painter.drawRect(QRect(0,row2*WORD_HEIGHT,
                               col2*WORD_WIDTH,WORD_HEIGHT));
    }
    else{
        //只有一行
        painter.drawRect(QRect(col1*WORD_WIDTH,row1*WORD_HEIGHT,
                               (col2-col1)*WORD_WIDTH,WORD_HEIGHT));
    }

}

//void Text::setText(QString s)
//{

//}

void Text::setSliderValue(int value)
{
//    QString str = QString("%1").arg(value);
//    cout<<"str = "<<str.toStdString()<<endl;

    first_row = V_bar->value();
    first_col = H_bar->value();

    emit firstChanged(first_row, first_col);
}

void Text::getString(vector<QString> s)
{
    str = s;

    //如果行数超过屏幕显示范围，设置滚动条
    if(str.size() > SCREEN_MAX_ROW){
        first_row = str.size()-SCREEN_MAX_ROW;
        V_bar->setMaximum(str.size()-SCREEN_MAX_ROW);
        V_bar->setSliderPosition(first_row);
    }
    else {
        V_bar->setMaximum(0);
    }

    //找到最大列数
    int maxCol = 0;
    for(int i = 0; i < str.size(); i++){
        if(str[i].length()>maxCol)
            maxCol = str[i].length();
    }
    //列数处理如上
    if(maxCol > SCREEN_MAX_COL){
        first_col = maxCol - SCREEN_MAX_COL;
        H_bar->setMaximum(first_col);
        H_bar->setSliderPosition(first_col);
    }
    else{
        H_bar->setMaximum(0);
    }
}

void Text::drawShadow(int Row1, int Col1, int Row2, int Col2)
{
    cout<<"Text::drawShadow"<<endl;
    isShadow = true;
    row1 = Row1;
    col1 = Col1;
    row2 = Row2;
    col2 = Col2;
}

void Text::deleteShadow()
{
    cout<<"Text::deleteShadow"<<endl;
    isShadow = false;
}

void Text::getCursorRange(int row, int cow)
{
    V_bar->setSliderPosition(row);
    H_bar->setSliderPosition(cow);
}
