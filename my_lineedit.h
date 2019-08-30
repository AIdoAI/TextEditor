#ifndef MYLINEEDIT_H
#define MTLINEEDIT_H

#include "entitle.h"

#include <QWidget>  
#include <QKeyEvent>  
#include <QLineEdit>  

class MyEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit MyEdit(QWidget *parent = 0);
	~MyEdit(void);

	QLineEdit *edit = new QLineEdit(this);
	Link headPtr, bufferPtr, cancelPtr; //������ͷָ�룬������ͷָ�룬������ͷָ��
	struct Position position1, position2, preposition1, preposition2; //��ǰ��꣬ǰһ�����
	int LR;//��ѡ��ʱ��ʾ�Ĺ����position1��LRΪ1������position2��LRΪ2��,���ǿ�ѡ��״̬��LR=0��

protected:
    void keyPressEvent(QKeyEvent *ev);

	bool eventFilter(QObject *target, QEvent *event);
};

#endif
