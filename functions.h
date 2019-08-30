#ifndef   FUNCTIONS       //���û�ж��������
#define   FUNCTIONS       //���������

#include <string>
#include "mainwindow.h"

using namespace std;

/* ����������½�� (���뵽startPtr��next��) */
extern void LinkInsert(PtrToNode startPtr);
/* �ͷ������� (��startPtr��ʼ(����startPtr)��endPtr����(������endPtr)) */
extern void LinkDelete(Link headPtr, PtrToNode startPtr, PtrToNode endPtr);

/* �ѱ䳤��ÿ������HEAPLENGTH���ַ����� */
extern void LengthenHeap(PtrToNode currentPtr, int n);
/* �п��� ����currentPtr1��i1start(����)��i1end(����)����currentPtr2��i2start(����)��ʼ */
extern void RowCopy(PtrToNode currentPtr1, PtrToNode currentPtr2, int i1start, int i1end, int i2start);

/* position�����������һ���ַ�����1�����򷵻�0 */
extern bool IsLinkEnd(struct Position position);
/* ��position1��headPtr�е�λ�� ��� position2��cancelPtr�е�λ�� */
extern void PosToPos(Link headPtr1, const struct Position position1, Link headPtr2, struct Position &position2);

/* �����ݽṹת����Qstring��ʽ����� */
extern void LinkToQstring(Link headPtr, Text *text);
/* �����ݽṹ��position��λ��ת����int�͵��к��У�Ȼ����ú��������� */
extern void PositionRowColumn(Link headPtr, const struct Position position, Cursor *cursor);
/* �����ݽṹ��position1��position2��λ��ת����int�͵��к���, Ȼ����ú��������Ӱ */
extern void PositionShadow(Link headPtr, const struct Position position1, const struct Position position2, Text *text);

#endif
