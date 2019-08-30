#ifndef   SINGLE_OPERATION       //���û�ж��������
#define   SINGLE_OPERATION       //���������

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Single{
    friend class MainWindow;
    friend class StrOperation;

    public:
        /* ���û����·������ʱ�򣬸ı��굱ǰλ�� */
        void Direction(int direction, struct Position &position, Link headPtr);

    private:
        /* ���� */
        void AddRow(struct Position &position);
        /* ���뵥���ַ� */
        void Insert(const char press, struct Position &position);
        /* ɾ�����ǰһ���� ���� ɾ���� */
        void BackSpace(Link headPtr, struct Position &position);
        /* ɾ�������һ���� */
        void Delete(Link headPtr, const struct Position position);
};

#endif
