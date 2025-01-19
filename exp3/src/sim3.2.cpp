#include "sim3.2.hpp"
#include <functional>

bool g_quit = false;

WNDPROC g_OriginWndProc = NULL;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CLOSE)
        g_quit = true;
    LRESULT result = CallWindowProc((WNDPROC)g_OriginWndProc, hwnd, uMsg, wParam, lParam);
    return result;
}

Sim::Sim(const int width, const int height)
    : _width(width), _height(height), _res({CRA, CRB, CRC}), _dx(_width * 6 / 64), _dy(height * 40 / 64 / PROCESS)
{
    SetConfig(L"zh_CN");
    _hwnd = initgraph(width, height, EX_DBLCLKS);
    g_OriginWndProc = (WNDPROC)GetWindowLongPtr(_hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
    _name = LoadLanguage(LANGUAGE, L"Banker Algorithrm Simulate");
    SetWindowText(_hwnd, _name.c_str());
    setbkcolor(WHITE);
    SetWorkingImage(NULL);
    setbkmode(TRANSPARENT);

    // 控件初始化
    _compute.Init(width * 2 / 64, height * 4 / 64, 0, 1, false, 1, true, RGB(85, 85, 85))
        .Set(width * 5 / 64, height * 5 / 64, L"Compute", ROUND, TMID, WHITE, RGB(63, 63, 70), RGB(85, 85, 85), 2, RGB(63, 63, 70), RGB(0, 151, 251), 2, L"Compute security", L"Compute", MB_OK)
        .Set(WHITE, WHITE, RGB(0, 122, 204), RGB(0, 122, 204), 2)
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _clear.Init(width * 49 / 64, height * 3 / 64, 0, 2, false, 1, true, RGB(85, 85, 85))
        .Set(width * 4 / 64, height * 4 / 64, L"Clear", SQUAR, TMID, WHITE, RGB(63, 63, 70), RGB(85, 85, 85), 2, RGB(63, 63, 70), RGB(0, 151, 251), 2, L"Clear", L"Clear", MB_OK)
        .Set(WHITE, WHITE, RGB(0, 122, 204), RGB(0, 122, 204), 2)
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _texts.resize(PROCESS);
    for (std::size_t i = 0; i < PROCESS; i++)
        for (std::size_t j = 0; j < 3; j++)
            _texts[i]
                .emplace_back()
                .Init(_width * 50 / 64 + j * _width * 4 / 64, _height * 14 / 64 + i * _dy, 2, i * 3 + j + 1)
                .Set(_width * 3 / 64, _height * 3 / 64, L"", SQUAR, TMID, WHITE, RGB(51, 51, 55), RGB(150, 150, 150), 2, RGB(63, 63, 70), RGB(0, 122, 204), 2, L"2 limits", L"TextBox", MB_OK)
                .Set(WHITE, WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2)
                .Set(WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2, NUMB, L'\n')
                .Set(_height * 3 / 64 - 4, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    // 初始化整体资源控件
    for (std::size_t i = 0; i < 3; i++)
    {
        _res.allTs[i]
            .Init(_width * 54 / 64 + i * _width * 3 / 64 - 4, _height * 3 / 64 + 4, 2, i + 1)
            .Set(_width * 3 / 64 - 6, _height * 3 / 64, L"", SQUAR, TMID, WHITE, RGB(51, 51, 55), RGB(150, 150, 150), 2, RGB(63, 63, 70), RGB(0, 122, 204), 2, L"2 limits", L"TextBox", MB_OK)
            .Set(WHITE, WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2)
            .Set(WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2, NUMB, L'\n')
            .Set(_height * 3 / 64 - 4, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);
    }
    _res.syncRes();

    // 初始化进程资源
    _prcs.reserve(PROCESS);
    _prcs.emplace_back(Res{5, 5, 9}, Res{2, 1, 2});
    _prcs.emplace_back(Res{5, 3, 6}, Res{4, 0, 2});
    _prcs.emplace_back(Res{4, 0, 11}, Res{4, 0, 5});
    _prcs.emplace_back(Res{4, 2, 5}, Res{2, 0, 4});
    _prcs.emplace_back(Res{4, 2, 4}, Res{3, 1, 4});
    // 初始化进程资源控件
    for (std::size_t i = 0; i < PROCESS; i++)
    {
        for (std::size_t j = 0; j < 3; j++)
        {
            _prcs[i]
                .maxTs[j]
                .Init(_width * 10 / 64 + j * _dx + 8, _height * 13 / 64 + i * _dy + 4, 2, i * 3 + j + 1)
                .Set(_width * 3 / 64, _height * 4 / 64, L"", ROUND, TMID, WHITE, RGB(67, 67, 70), RGB(150, 150, 150), 2, RGB(76, 76, 80), RGB(230, 230, 230), 2, L"2 limits", L"TextBox", MB_OK)
                .Set(WHITE, WHITE, RGB(76, 76, 80), RGB(230, 230, 230), 2)
                .Set(WHITE, RGB(88, 88, 93), RGB(230, 230, 230), 3, NUMB, L'\n')
                .Set(_height * 3 / 64, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);
            _prcs[i]
                .allcTs[j]
                .Init(_width * 30 / 64 + j * _dx + 8, _height * 13 / 64 + i * _dy + 4, 2, i * 3 + j + 1)
                .Set(_width * 3 / 64, _height * 4 / 64, L"", ROUND, TMID, WHITE, RGB(67, 67, 70), RGB(150, 150, 150), 2, RGB(76, 76, 80), RGB(230, 230, 230), 2, L"2 limits", L"TextBox", MB_OK)
                .Set(WHITE, WHITE, RGB(76, 76, 80), RGB(230, 230, 230), 2)
                .Set(WHITE, RGB(88, 88, 93), RGB(230, 230, 230), 3, NUMB, L'\n')
                .Set(_height * 3 / 64, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);
        }
        _prcs[i].syncRes();
    }

    // 初始化剩余资源
    _lastRes = _res.all;
    for (const auto &prc : _prcs)
        _lastRes -= prc.allc;
}

void Sim::Drawbk()
{
    int oldlinecolor = getlinecolor();
    int oldbkcolor = getbkcolor();
    int oldfillcolor = getfillcolor();
    LINESTYLE lo;
    getlinestyle(&lo);

    setbkcolor(RGB(45, 45, 48));
    cleardevice();
    setfillcolor(RGB(255, 221, 136));
    solidrectangle(0, 0, _width, 5);
    solidrectangle(0, 0, 3, _height);
    setfillcolor(RGB(71, 78, 94));
    solidrectangle(0, _height - 5, _width, _height);
    solidrectangle(_width - 5, 0, _width, _height);

    setfillcolor(RGB(63, 63, 70));
    setlinecolor(RGB(150, 150, 150));
    setlinestyle(PS_SOLID, 3);
    fillroundrect(_width * 8 / 64, _height * 2 / 64, _width * 48 / 64, _height * 11 / 64, _height / 20, _height / 20);
    fillroundrect(_width * 2 / 64, _height * 12 / 64, _width * 7 / 64, _height * 51 / 64, _height / 20, _height / 20);
    fillroundrect(_width * 8 / 64, _height * 12 / 64, _width * 48 / 64, _height * 51 / 64, _height / 20, _height / 20);
    fillroundrect(_width * 2 / 64, _height * 52 / 64, _width * 62 / 64, _height * 62 / 64, _height / 20, _height / 20);
    fillroundrect(_width * 49 / 64, _height * 12 / 64, _width * 62 / 64, _height * 51 / 64, _height / 20, _height / 20);

    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);
    setlinestyle(&lo);
}

void Sim::Drawbutton()
{
    int id = 0;
    bool empty = true;
    bool full = true;
    bool hasvalue = false;
    for (const auto &texts : _texts)
    {
        for (const auto &text : texts)
            if (!text.bchar.empty())
            {
                hasvalue = true;
                empty = false;
            }
            else
                full = false;
        if (!empty && !full)
            break;
        else
        {
            empty = true;
            full = true;
        }
    }
    bool validMA = _res.validAll();
    for (auto &prc : _prcs)
    {
        if (!prc.validMax() || !prc.validAllc())
        {
            validMA = false;
            break;
        }
    }
    if (_compute.Use(id, NULL, validMA && (empty || full)))
    {
        _line.clear();
        std::vector<std::vector<std::size_t>> sequences;
        std::vector<Prc> tprcs(_prcs);
        Res tres(_lastRes);
        bool valid = true;
        for (std::size_t i = 0; i < PROCESS; i++)
        {
            if (_texts[i].back().bchar.empty())
                continue;
            Res tmp(std::stoi(_texts[i][0].bchar), std::stoi(_texts[i][1].bchar), std::stoi(_texts[i][2].bchar));
            if (tmp <= tres && tmp + tprcs[i].allc <= tprcs[i].max)
            {
                tprcs[i].allc += tmp;
                tres -= tmp;
            }
            else
            {
                valid = false;
                break;
            }
        }
        if (valid && Compute(sequences, tprcs, tres))
        {
            for (const auto &sequence : sequences)
            {
                for (const auto &pid : sequence)
                    _line += std::to_wstring(pid);
                _line.push_back(L' ');
            }
        }
        else
            _line = valid ? L"Unable to find secure process sequence!" : L"Invalid pre-demand!";
    }
    if (_clear.Use(id, NULL, hasvalue || !_line.empty()))
    {
        _line.clear();
        for (auto &texts : _texts)
            for (auto &text : texts)
                text.bchar.clear();
    }
}

void Sim::Drawcomp()
{
    _compute.Setmytextstyle();
    for (std::size_t i = 0; i < _prcs.size(); i++)
        Draw(_width * 3 / 64 - 1, _height * 13 / 64 + i * _dy + 4, _width * 3 / 64, _height * 4 / 64, ROUND, RGB(67, 67, 70), RGB(150, 150, 150), 2, L"P" + std::to_wstring(i + 1), TMID);
    Draw(_width * 9 / 64, _height * 3 / 64, _width * 18 / 64, _height * 3 / 64, ROUND, RGB(67, 67, 70), RGB(150, 150, 150), 2, L"Maximum Resources Demand", TMID);
    Draw(_width * 9 / 64, _height * 7 / 64, _width * 18 / 64, _height * 3 / 64, ROUND, RGB(67, 67, 70), RGB(150, 150, 150), 2, L"A               B               C", TMID);
    Draw(_width * 29 / 64, _height * 3 / 64, _width * 18 / 64, _height * 3 / 64, ROUND, RGB(67, 67, 70), RGB(150, 150, 150), 2, L"Allocated Resources Number", TMID);
    Draw(_width * 29 / 64, _height * 7 / 64, _width * 18 / 64, _height * 3 / 64, ROUND, RGB(67, 67, 70), RGB(150, 150, 150), 2, L"A               B               C", TMID);
    outtextxy(_width * 50 / 64, _height * 8 / 64, (L"Spare   A:" + std::to_wstring(_lastRes.A) + L" B:" + std::to_wstring(_lastRes.B) + L" C:" + std::to_wstring(_lastRes.C)).c_str());
    static const std::size_t bound = 108;
    std::vector<std::wstring> out;
    out.push_back(_line);
    while (out.back().size() > bound)
    {
        out.push_back(out.back().substr(bound));
        out[out.size() - 2] = out[out.size() - 2].substr(0, bound);
    }
    LOGFONT f;
    gettextstyle(&f);
    settextstyle(f.lfHeight - 3, 0, f.lfFaceName);
    for (std::size_t j = 0; j < out.size(); j++)
        outtextxy(_width * 2 / 64 + 4, _height * 52 / 64 + 4 + j * _height * 3 / 64, out[j].c_str());
    settextstyle(&f);
}

void Sim::Drawset()
{
    int id;
    for (auto &prc : _prcs)
    {
        id = 0;
        for (auto &maxT : prc.maxTs)
            maxT.Use(id, &maxT.bchar);
        id = 0;
        for (auto &allcT : prc.allcTs)
            allcT.Use(id, &allcT.bchar);
    }
    id = 0;
    for (auto &allT : _res.allTs)
        allT.Use(id, &allT.bchar);
    id = 0;
    for (auto &texts : _texts)
        for (auto &text : texts)
            text.Use(id, &text.bchar);
    for (auto &prc : _prcs)
    {
        for (auto &maxT : prc.maxTs)
            if (maxT.Det() && prc.validMax())
                prc.syncMax();
        for (auto &allcT : prc.allcTs)
            if (allcT.Det() && prc.validAllc())
                prc.syncAllc();
    }
    for (auto &allT : _res.allTs)
        if (allT.Det() && _res.validAll())
        {
            _res.syncAll();
            _lastRes = _res.all;
            for (const auto &prc : _prcs)
                _lastRes -= prc.allc;
        }
    for (auto &texts : _texts)
        for (auto &text : texts)
            text.Det();
}

Sim::~Sim()
{
}

bool Sim::Compute(std::vector<std::vector<std::size_t>> &sequences, std::vector<Prc> prcs, Res res)
{
    if (sequences.empty())
        sequences.emplace_back(std::vector<std::size_t>());
    else if (sequences.back().size() == prcs.size()) // 找到一个安全序列
        return true;
    Res tmp;
    // 回溯法遍历
    for (std::size_t i = 0; i < prcs.size(); i++)
    {
        tmp = prcs[i].max - prcs[i].allc; // 保护现场
        if (tmp == 0 || !(tmp <= res))    // 已执行完成或不足以分配
            continue;
        res += prcs[i].allc;                       // 释放资源
        prcs[i].allc = prcs[i].max;                // 完成执行
        sequences.back().emplace_back(i + 1);      // 添加完成的进程号
        if (Compute(sequences, prcs, res))         // 递归
            sequences.push_back(sequences.back()); // 保存安全序列
        // 恢复现场
        prcs[i].allc -= tmp;
        res -= prcs[i].allc;
        sequences.back().pop_back();
    }
    if (sequences.back().empty()) // 退出最后一层递归
    {
        sequences.pop_back();
        return !sequences.empty();
    }
    return false;
}
