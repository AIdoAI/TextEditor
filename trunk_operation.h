#ifndef   TRUNK_OPERATION       //���û�ж��������
#define   TURNK_OPERATION       //���������

#include "entitle.h"

class Trunk{
    friend class MainWindow;

    public:

    private:
        /* ����� ���е�bufferPtr�� */
        void TrunkCut(Link headPtr, Link destinationPtr, struct Position &position1, struct Position &position2);
        /* �鿽�� ������bufferPtr�� */
        void TrunkCopy(Link destinationPtr, struct Position position1, struct Position position2);
        /* ��ɾ�� */
        void TrunkDelete(Link headPtr, struct Position &position1, struct Position &position2);
        /* ��ճ�� */
        void TrunkPaste(Link sourcePtr, struct Position &position);
        /* ȫѡ */
        void SelectAll(Link headPtr, struct Position &position1, struct Position &position2);
};

#endif
