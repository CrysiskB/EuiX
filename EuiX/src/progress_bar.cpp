#include "progress_bar.hpp"
#include "widget.hpp"

namespace EuiX
{
    int c = 0;
    ProgressBar::ProgressBar(
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
        int inWeight)
    {
        Init(x, y, w, h, l, dl, m, outColor, outLineC, outWeight, inColor, inLineC, inWeight);   
    }

    void ProgressBar::Init(int x, int y, int w, int h, int l, int dl, int m, COLORREF outColor, COLORREF outLineC, int outWeight, COLORREF inColor, COLORREF inLineC, int inWeight)
    {
        _x = x;
        _y = y;
        _w = w;
        _h = h;
        _l = l;
        _dl = dl;
        _m = m;
        _outColor = outColor;
        _outLineC = outLineC;
        _outWeight = outWeight;
        _inColor = inColor;
        _inLineC = inLineC;
        _inWeight = inWeight;
        _cl = 0;
    }

    void ProgressBar::Set(int l, int dl)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _dl = dl;
        _cl = (float)_cl / _l * l;
        _l = l;
    }

    bool ProgressBar::Use()
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_cl + _dl >= _l)
        {
            _cl = _l;
            return false;
        }
        else
        {
            _cl += _dl;
            return true;
        }
    }

    float ProgressBar::Put()
    {
        settextcolor(WHITE);
        Draw(_x, _y, _w, _h, _m, _outColor, _outLineC, _outWeight, L"", 0);
        std::lock_guard<std::mutex> guard(_mutex);
        float tmp = (float)_cl / _l;
        int tw = tmp * _w;
        std::wstring tmps;
        if (tw <= _h * 2)
            tmps = L"";
        else
        {
            tmps = std::to_wstring(tmp * 100);
            tmps = tmps.substr(0, tmps.find(L'.') + 2) + L'%';
        }
        Draw(_x, _y, tmp * _w, _h, _m, _inColor, _inLineC, _inWeight, tmps, TMID);
        return tmp;
    }

    void ProgressBar::Clear()
    {
        std::lock_guard<std::mutex> guard(_mutex);
        _cl = 0;
    }

    ProgressBar::~ProgressBar()
    {
    }
}
