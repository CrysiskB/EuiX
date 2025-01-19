#include "sim3.1.hpp"

int main(int argc, char *argv[])
{
    Sim mone(EX_WIDTH, EX_HEIGHT);
    BeginBatchDraw();
    while (!g_quit)
    {
        mone.Drawbk();
        mone.Drawcomp();
        mone.Drawbutton();
        mone.Drawset();
        FlushBatchDraw();
    }
    EndBatchDraw();
    closegraph();
    return 0;
}