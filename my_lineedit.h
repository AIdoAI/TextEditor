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
	Link headPtr, bufferPtr, cancelPtr; //行链表头指针，缓存区头指针，撤销区头指针
	struct Position position1, position2, preposition1, preposition2; //当前光标，前一步光标
	int LR;//块选择时显示的光标是position1（LR为1）还是position2（LR为2）,不是块选择状态（LR=0）

protected:
    void keyPressEvent(QKeyEvent *ev);

	bool eventFilter(QObject *target, QEvent *event);
};

#endif
