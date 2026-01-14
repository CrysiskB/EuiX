#include "widget.hpp"
#include "analyse.hpp"
#include <mutex>
#include <thread>
namespace EuiX
{

    static ExMessage widget_msg;
    static int widget_msgtemp = 0;
    static bool widget_sta = false;
    static bool widget_next = false;
    static bool widget_outnext = false;
    static bool enable = false;
    static bool finish = false;

    static std::mutex g_mutex;
    static std::mutex g_mmutex;
    static std::mutex g_enable;

    Filter::Filter(const uint16_t frequency)
        : frequency(frequency)
    {
        startTime = std::chrono::high_resolution_clock::now();
        updateEnabled = true;
    }

    bool Filter::filter()
    {
        const auto &currentTime = std::chrono::high_resolution_clock::now();
        const auto &time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        if (time >= frequency)
        {
            startTime = currentTime;
            if (updateEnabled)
                return true;
        }
        return false;
    }

    void Draw(int x, int y, int w, int h, int m, COLORREF FillColor, COLORREF LineColor, int Weight, std::wstring Char, int position, bool autoSize)
    {
        if (!w || !h)
            return;
        std::lock_guard<std::mutex> guard(g_mutex);
        int oldlinecolor = getlinecolor();
        int oldbkcolor = getbkcolor();
        int oldfillcolor = getfillcolor();
        LINESTYLE lo;
        getlinestyle(&lo);
        setfillcolor(FillColor);
        setlinecolor(LineColor);
        setlinestyle(PS_SOLID, Weight);
        switch (m)
        {
        case ROUND:
            fillroundrect(x - h / 16, y - h / 16, x + w + h / 8, y + h + h / 8, h / 2, h / 2);
            break;
        case SQUAR:
            fillrectangle(x, y, x + w, y + h);
            break;
        default:
            printf("WARNING:Wrong m");
            return;
            break;
        }
        setlinecolor(oldlinecolor);
        setbkcolor(oldbkcolor);
        setfillcolor(oldfillcolor);
        setlinestyle(&lo);
        std::wstring temp = LoadLanguage(LANGUAGE, Char).c_str();
        if (autoSize && textwidth(temp.c_str()) > w - 8)
        {
            LOGFONT f, o;
            gettextstyle(&f);
            o = f;
            f.lfHeight = LONG(f.lfHeight * (float((w - 8)) / float(textwidth(temp.c_str()))));
            settextstyle(f.lfHeight, 0, f.lfFaceName);
            outtextxy(x + (w - textwidth(temp.c_str())) / 2, y + (h - textheight(temp.c_str())) / 2, temp.c_str());
            settextstyle(&o);
            return;
        }
        switch (position)
        {
        case TMID:
            outtextxy(x + (w - textwidth(temp.c_str())) / 2, y + (h - textheight(temp.c_str())) / 2, temp.c_str());
            break;
        case TLEFT:
            outtextxy(x + 4, y + (h - textheight(temp.c_str())) / 2, temp.c_str());
            break;
        case TRIGHT:
            outtextxy(x + (w - textwidth(temp.c_str())) - 4, y + (h - textheight(temp.c_str())) / 2, temp.c_str());
            break;
        case TTEXT:
            outtextxy(x + 4, y + (h - textheight(temp.c_str())) / 2, temp.c_str());
            break;
        default:
            break;
        }
    }

    int MouseM(int x, int y, int w, int h, int defnull)
    {
        std::lock_guard<std::mutex> guard(g_mmutex);
        int n = 0;
        peekmessage(&widget_msg, EX_MOUSE);
        if (widget_next)
        {
            if ((widget_msg.lbutton || widget_msg.rbutton) && (widget_msg.x >= x && widget_msg.x <= x + w && widget_msg.y >= y && widget_msg.y <= y + h))
            {
                switch (widget_msgtemp)
                {
                case OUTCLICK:
                case ROUTCLICK:
                    n = STAY;
                    break;
                default:
                    n = widget_msgtemp;
                    break;
                }
                return n;
            }
            else if ((widget_msg.lbutton || widget_msg.rbutton) && (widget_msg.x < x || widget_msg.x > x + w || widget_msg.y < y || widget_msg.y > y + h))
            {
                switch (widget_msgtemp)
                {
                case CLICK:
                    n = OUTCLICK;
                    break;
                case RCLICK:
                    n = ROUTCLICK;
                    break;
                default:
                    n = OUTSTAY;
                    break;
                }
                return n;
            }
            else if ((!widget_msg.lbutton && !widget_msg.rbutton) && (widget_msg.x >= x && widget_msg.x <= x + w && widget_msg.y >= y && widget_msg.y <= y + h))
            {
                switch (widget_msgtemp)
                {
                case CLICK:
                    n = UP;
                    break;
                case RCLICK:
                    n = RUP;
                    break;
                default:
                    n = STAY;
                    break;
                }
                widget_msg.message = WM_MOUSEMOVE;
                widget_next = false;
                flushmessage(EX_MOUSE);
                return n;
            }
            else
            {
                return OUTSTAY;
            }
        }
        if (!widget_next && widget_msg.x >= x && widget_msg.x <= x + w && widget_msg.y >= y && widget_msg.y <= y + h)
            switch (widget_msg.message)
            {
            case WM_LBUTTONDBLCLK:
                n = DCLICK;
                widget_msgtemp = DCLICK;
                widget_next = true;
                break;
            case WM_LBUTTONUP:
                n = UP;
                widget_msgtemp = UP;
                widget_next = true;
                break;
            case WM_LBUTTONDOWN:
                n = CLICK;
                widget_msgtemp = CLICK;
                widget_next = true;
                break;
            case WM_RBUTTONUP:
                n = RUP;
                widget_msgtemp = RUP;
                widget_next = true;
                break;
            case WM_RBUTTONDOWN:
                n = RCLICK;
                widget_msgtemp = RCLICK;
                widget_next = true;
                break;
            default:
                n = STAY;
                break;
            }
        else if (!widget_next && (widget_msg.x < x || widget_msg.x > x + w || widget_msg.y < y || widget_msg.y > y + h))
            switch (widget_msg.message)
            {
            case WM_LBUTTONDOWN:
                n = OUTCLICK;
                break;
            case WM_RBUTTONDOWN:
                n = ROUTCLICK;
                break;
            default:
                n = OUTSTAY;
                break;
            }
        n = n == 0 ? defnull : n;
        flushmessage(EX_MOUSE);
        return n;
    }

    // void KeyM(int x, int y, int w, int h, std::wstring &Char, int m, COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int Weight, size_t limits, int Type, char Sign)
    // {
    //     int n = -1, c = 0;
    //     bool state = true;
    //     setbkmode(TRANSPARENT);
    //     settextcolor(TextColor);
    //     int wtemp;
    //     wtemp = w == 0 ? (int)(limits)*textwidth('a') + 2 : w;
    //     while (state)
    //     {
    //         for (; state && peekmessage(&widget_msg, EX_MOUSE | EX_CHAR, false); peekmessage(NULL, EX_MOUSE | EX_CHAR))
    //         {
    //             if (widget_msg.message == WM_CHAR)
    //                 switch (widget_msg.ch)
    //                 {
    //                 case '\b':
    //                     if (Char.size() > 0)
    //                         Char.pop_back();
    //                     break;
    //                 case '\r':
    //                 case VK_ESCAPE:
    //                     state = false;
    //                     break;
    //                 default:
    //                     if (widget_msg.ch == Sign)
    //                         state = false;
    //                     else if (Type == NUMB && Char.size() < limits && widget_msg.ch >= '0' && widget_msg.ch <= '9')
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == TEMP && Char.size() < limits && (widget_msg.ch >= '0' && widget_msg.ch <= '9' || widget_msg.ch == '.'))
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == TIME && Char.size() < limits && widget_msg.ch >= '0' && widget_msg.ch <= '9' || widget_msg.ch == ':')
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == DATE && Char.size() < limits && widget_msg.ch >= '0' && widget_msg.ch <= '9' || widget_msg.ch == '-')
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == CHAR && Char.size() < limits && ((widget_msg.ch >= '0' && widget_msg.ch <= '9') || (widget_msg.ch >= 'a' && widget_msg.ch <= 'z') || (widget_msg.ch >= 'A' && widget_msg.ch <= 'Z') || widget_msg.ch == '.' || widget_msg.ch == '_' || widget_msg.ch == '@'))
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == ALLC && Char.size() < limits && widget_msg.ch != ' ')
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     else if (Type == ALLS && Char.size() < limits)
    //                     {
    //                         Char.push_back(widget_msg.ch);
    //                     }
    //                     break;
    //                 }
    //             else if ((c = MouseM(x, y, w, h, OUTSTAY)) == OUTCLICK || c == ROUTCLICK)
    //                 state = false;
    //         }
    //         Draw(x, y, wtemp, h, m, TextBoxC, LineColor, Weight, Char, TTEXT);
    //         FlushBatchDraw();
    //     }
    // }

    static void PinM(int x, int y, int w, int h, std::wstring &Char, size_t limits, int Type, char Sign)
    {
        ExMessage msg;
        int c = 0;
        while (enable)
        {
            for (; enable && peekmessage(&msg, EX_MOUSE | EX_CHAR, false); peekmessage(NULL, EX_MOUSE | EX_CHAR))
            {
                if (msg.message == WM_CHAR)
                {
                    std::lock_guard<std::mutex> guard(g_enable);
                    switch (msg.ch)
                    {
                    case '\b':
                        if (Char.size() > 0)
                            Char.pop_back();
                        break;
                    case '\r':
                    case VK_ESCAPE:
                        enable = false;
                        finish = true;
                        break;
                    default:
                        if (msg.ch == Sign)
                        {
                            enable = false;
                            finish = true;
                        }
                        else if (Type == NUMB && Char.size() < limits && msg.ch >= '0' && msg.ch <= '9')
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == TEMP && Char.size() < limits && ((msg.ch >= '0' && msg.ch <= '9') || msg.ch == '.'))
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == TIME && Char.size() < limits && ((msg.ch >= '0' && msg.ch <= '9') || msg.ch == ':'))
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == DATE && Char.size() < limits && ((msg.ch >= '0' && msg.ch <= '9') || msg.ch == '-'))
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == CHAR && Char.size() < limits && ((msg.ch >= '0' && msg.ch <= '9') || (msg.ch >= 'a' && msg.ch <= 'z') || (msg.ch >= 'A' && msg.ch <= 'Z') || msg.ch == '.' || msg.ch == '_' || msg.ch == '@'))
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == ALLC && Char.size() < limits && msg.ch != ' ')
                        {
                            Char.push_back(msg.ch);
                        }
                        else if (Type == ALLS && Char.size() < limits)
                        {
                            Char.push_back(msg.ch);
                        }
                        break;
                    }
                }
                else if ((c = MouseM(x, y, w, h, OUTSTAY)) == OUTCLICK || c == ROUTCLICK)
                {
                    enable = false;
                    finish = true;
                }
            }
        }
    }

    bool KeyM(int x, int y, int w, int h, std::wstring &Char, int m, COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int Weight, size_t limits, int Type, char Sign)
    {
        setbkmode(TRANSPARENT);
        settextcolor(TextColor);
        int wtemp;
        wtemp = w == 0 ? (int)(limits)*textwidth('a') + 2 : w;
        std::lock_guard<std::mutex> guard(g_enable);
        if (!enable && !finish)
        {
            enable = true;
            std::thread p(PinM, x, y, w, h, std::ref(Char), limits, Type, Sign);
            p.detach();
        }
        else if (finish)
            finish = false;
        Draw(x, y, wtemp, h, m, TextBoxC, LineColor, Weight, Char, TTEXT);
        return enable;
    }

    bool KEYUP(int vk)
    {
        if (KEYDOWN(vk) == true)
            widget_sta = true;
        if ((KEYDOWN(vk) == false) && widget_sta)
        {
            widget_sta = false;
            return true;
        }
        return false;
    }

    bool LoopC(int Max, int Count, int Pages, int *X)
    {
        if (Max * Pages < Count || Count == 0)
        {
            *X = 0;
            return false;
        }
        *X = Count - Max * (Pages - 1);
        return true;
    }

    void Calender(std::wstring &Year, std::wstring &Month, std::wstring &Day)
    {
        int x[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, y, m, d;
        if (Year.size() == 0 || Month.size() == 0 || Day.size() == 0)
        {
            return;
        }
        y = std::stoi(Year);
        m = std::stoi(Month);
        d = std::stoi(Day);
        if (y < 1)
        {
            Year.clear();
            return;
        }
        else if (m < 1 || m > 12)
        {
            Month.clear();
            return;
        }
        else if (d < 1)
        {
            Day.clear();
            return;
        }
        if (y != 0 && m != 0 && d != 0)
        {
            x[1] = ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) + x[1];
            if (d > x[m - 1])
                Day.clear();
        }
    }

    long Clock(
        std::wstring FYear, std::wstring FMonth, std::wstring FDay,
        std::wstring &LYear, std::wstring &LMonth, std::wstring &LDay)
    {
        long all = 0;
        int x[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, fy, fm, fd, ly, lm, ld;
        fy = std::stoi(FYear);
        fm = std::stoi(FMonth);
        fd = std::stoi(FDay);
        ly = std::stoi(LYear);
        lm = std::stoi(LMonth);
        ld = std::stoi(LDay);
        if (fy != 0 && ly != 0 && fm != 0 && lm != 0 && fd != 0 && ld != 0)
        {
            if (fy > ly)
            {
                LYear.clear();
                return 0;
            }
            if (fy == ly && fm > lm)
            {
                LMonth.clear();
                return 0;
            }
            if (fy == ly && fm == lm && fd > ld)
            {
                LDay.clear();
                return 0;
            }
            x[1] = ((fy % 4 == 0 && fy % 100 != 0) || fy % 400 == 0) + x[1];
            while (fy != ly || fm != lm || fd != ld)
            {
                all++;
                fd++;
                if (fd == x[fm - 1] + 1)
                {
                    fd = 1;
                    fm++;
                }
                if (fm == 13)
                {
                    fm = 1;
                    fy++;
                    x[1] = ((fy % 4 == 0 && fy % 100 != 0) || fy % 400 == 0) + 28;
                }
            }
            all++;
        }
        return all;
    }

    std::wstring randStr(const UINT minLen, const UINT maxLen)
    {
        std::srand(std::time(NULL));
        int len = minLen + rand() % (maxLen - minLen + 1);
        std::wstring str;
        for (int i = 0; i < len; ++i)
            switch (rand() % 3)
            {
            case 1:
                str.push_back('A' + rand() % 26);
                break;
            case 2:
                str.push_back('a' + rand() % 26);
                break;
            default:
                str.push_back('0' + rand() % 10);
                break;
            }
        return str;
    }
}
