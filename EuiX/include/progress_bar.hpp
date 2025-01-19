#ifndef msr_progress_bar_Hpp
#define msr_progress_bar_Hpp

#include "easyx.h"
#include <mutex>

namespace EuiX
{
    class ProgressBar
    {
    public:
        ProgressBar() {};
        ProgressBar(
            int x,
            int y,
            int w,
            int h,
            int l,
            int dl,
            int m,
            COLORREF outColor,
            COLORREF outLineC,
            int outWeight,
            COLORREF inColor,
            COLORREF inLineC,
            int inWeight);
        void Init(
            int x,
            int y,
            int w,
            int h,
            int l,
            int dl,
            int m,
            COLORREF outColor,
            COLORREF outLineC,
            int outWeight,
            COLORREF inColor,
            COLORREF inLineC,
            int inWeight);
        void Set(int l, int dl);
        bool Use();
        float Put();
        void Clear();
        ~ProgressBar();

    private:
        int _x, _y, _w, _h, _l, _dl, _m, _outWeight, _inWeight, _cl;
        COLORREF _outColor, _outLineC, _inColor, _inLineC;
        std::mutex _mutex;
    };
}

#endif