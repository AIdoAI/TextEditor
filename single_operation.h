#ifndef   SINGLE_OPERATION       //如果没有定义这个宏
#define   SINGLE_OPERATION       //定义这个宏

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Single{
    friend class MainWindow;
    friend class StrOperation;

    public:
        /* 当用户按下方向键的时候，改变光标当前位置 */
        void Direction(int direction, struct Position &position, Link headPtr);

    private:
        /* 换行 */
        void AddRow(struct Position &position);
        /* 插入单个字符 */
        void Insert(const char press, struct Position &position);
        /* 删除光标前一个字 或者 删除行 */
        void BackSpace(Link headPtr, struct Position &position);
        /* 删除光标后的一个字 */
        void Delete(Link headPtr, const struct Position position);
};

#endif
