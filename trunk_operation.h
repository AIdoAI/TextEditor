#ifndef   TRUNK_OPERATION       //如果没有定义这个宏
#define   TURNK_OPERATION       //定义这个宏

#include "entitle.h"

class Trunk{
    friend class MainWindow;

    public:

    private:
        /* 块剪切 剪切到bufferPtr中 */
        void TrunkCut(Link headPtr, Link destinationPtr, struct Position &position1, struct Position &position2);
        /* 块拷贝 拷贝到bufferPtr中 */
        void TrunkCopy(Link destinationPtr, struct Position position1, struct Position position2);
        /* 块删除 */
        void TrunkDelete(Link headPtr, struct Position &position1, struct Position &position2);
        /* 块粘贴 */
        void TrunkPaste(Link sourcePtr, struct Position &position);
        /* 全选 */
        void SelectAll(Link headPtr, struct Position &position1, struct Position &position2);
};

#endif
