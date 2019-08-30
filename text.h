#ifndef TEXT_H
#define TEXT_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QKeyEvent>
#include <QFont>        //字体
#include <QScrollBar>
#include <vector>

using namespace std;

class Text : public QWidget
{
    Q_OBJECT
public:
    Text (QWidget * parent = 0);
    void getString(vector<QString> s);                          //设置打印字符
    void drawShadow(int row1,int col1,int row2,int col2);       //绘制阴影
    void deleteShadow();                                        //删除阴影

    QScrollBar *V_bar;            //竖直方向滚动条
    QScrollBar *H_bar;            //水平方向滚动条
public slots:
      void getCursorRange(int row,int cow);                //通过光标位置调整滚动条位置
      void setSliderValue(int value);
signals:
    void firstChanged(int fr,int fc);       //文本显示的第一行和第一列发生改变

protected:
//  void keyPressEvent(QKeyEvent *event);    //键盘事件响应
    void paintEvent(QPaintEvent *event);
private :
    vector<QString> str;
    int first_row = 0;                  //界面显示的第一行与实文本行的对应
    int first_col = 0;                  //界面显示的第一列与实文本行的对应
    int row1,col1,row2,col2;        //shadow area
     bool isShadow;
};
#endif // TEXT_H

