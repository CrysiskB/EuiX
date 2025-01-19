#include "sim3.1.hpp"
#include "semaphore.hpp"
#include <functional>
#include <wchar.h>

bool g_quit = false;

WNDPROC g_OriginWndProc = NULL;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CLOSE)
        g_quit = true;
    LRESULT result = CallWindowProc((WNDPROC)g_OriginWndProc, hwnd, uMsg, wParam, lParam);
    return result;
}

Semaphore g_emptys(BUFFER_SIZE);
Semaphore g_fulls(0);

Sim::Sim(const int width, const int height) : _width(width), _height(height)
{
    SetConfig(L"zh_CN");
    _hwnd = initgraph(width, height, EX_DBLCLKS);
    g_OriginWndProc = (WNDPROC)GetWindowLongPtr(_hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
    _name = LoadLanguage(LANGUAGE, L"Producer-Consumer Simulate");
    SetWindowText(_hwnd, _name.c_str());
    setbkcolor(WHITE);
    SetWorkingImage(NULL);
    setbkmode(TRANSPARENT);
    if (!Loggin())
    {
        g_quit = true;
        return;
    }

    _crP = PRODUCER;
    _readyPs.resize(PRODUCER, true);
    _prcstrs.resize(PRODUCER);
    _constrs.resize(CONSUMER);
    _resC.resize(CONSUMER);

    // 初始化缓冲池
    _ebuffers.reserve(BUFFER_SIZE);
    for (std::size_t i = 0; i < BUFFER_SIZE; i++)
        _ebuffers.emplace_back(std::make_unique<std::wstring>());
    _fbuffers.reserve(BUFFER_SIZE);

    // 控件初始化
    _text.Init(width * 2 / 16, height * 58 / 64, MAXNC, 1)
        .Set(width * 41 / 64, height / 20, L"", SQUAR, TLEFT, RGB(153, 153, 153), RGB(51, 51, 55), RGB(67, 67, 70), 2, RGB(63, 63, 70), RGB(0, 122, 204), 2, L"Message(without 'space') max " + std::to_wstring(MAXNC), L"Message", MB_OK | MB_SETFOREGROUND)
        .Set(WHITE, WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2)
        .Set(WHITE, RGB(67, 67, 70), RGB(0, 122, 204), 2, ALLC, '\n')
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _size.Init(_width * 31 / 64, _height * 54 / 64 + 4, 2, 2)
        .Set(_width * 2 / 64, _height * 3 / 64, std::to_wstring(BUFFER_SIZE), SQUAR, TMID, WHITE, RGB(45, 45, 48), RGB(45, 45, 48), 2, RGB(76, 76, 80), RGB(230, 230, 230), 2, L"2 limits", L"TextBox", MB_OK | MB_SETFOREGROUND)
        .Set(WHITE, WHITE, RGB(76, 76, 80), RGB(230, 230, 230), 3)
        .Set(WHITE, RGB(88, 88, 93), RGB(230, 230, 230), 3, NUMB, L'\n')
        .Set(_height * 3 / 64, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _clear.Init(width * 1 / 64, height * 58 / 64, 0, 1, false, 1, true, RGB(85, 85, 85))
        .Set(width * 6 / 64, height / 20, L"Clear", SQUAR, TMID, WHITE, RGB(63, 63, 70), RGB(85, 85, 85), 2, RGB(63, 63, 70), RGB(0, 151, 251), 2, L"Clear message", L"Clear", MB_OK | MB_SETFOREGROUND)
        .Set(WHITE, WHITE, RGB(0, 122, 204), RGB(0, 122, 204), 2)
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _send.Init(width * 50 / 64, height * 58 / 64, 0, 2, false, 1, true, RGB(85, 85, 85))
        .Set(width * 6 / 64, height / 20, L"Send", SQUAR, TMID, WHITE, RGB(63, 63, 70), RGB(85, 85, 85), 2, RGB(63, 63, 70), RGB(0, 151, 251), 2, L"Send message", L"Send", MB_OK | MB_SETFOREGROUND)
        .Set(WHITE, WHITE, RGB(0, 122, 204), RGB(0, 122, 204), 2)
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    _rand.Init(width * 57 / 64, height * 58 / 64, 0, 3, false, 1, true, RGB(85, 85, 85))
        .Set(width * 6 / 64, height / 20, L"Rand", SQUAR, TMID, WHITE, RGB(63, 63, 70), RGB(85, 85, 85), 2, RGB(63, 63, 70), RGB(0, 151, 251), 2, L"Rand message", L"Rand", MB_OK | MB_SETFOREGROUND)
        .Set(WHITE, WHITE, RGB(0, 122, 204), RGB(0, 122, 204), 2)
        .Set(height / 20, DEFAULT, FRONT, DEFAULT, DEFAULT, DEFAULT, false, false, false, ANTIALIASED_QUALITY, DEFAULT_PITCH);

    // 创建线程、进度条
    _prcpbs.reserve(PRODUCER);
    _dprc = height * 54 / 64 / PRODUCER;
    for (std::size_t i = 0; i < PRODUCER; i++)
        _prcpbs.emplace_back(std::make_unique<ProgressBar>())->Init(width * 8 / 64, height * 3 / 64 + i * _dprc, width * 5 / 16, height / 20, MAXNC, 1, ROUND, RGB(63, 63, 70), RGB(160, 160, 160), 2, RGB(0, 122, 204), RGB(150, 150, 150), 3);
    _conpbs.reserve(CONSUMER);
    _dcon = height * 54 / 64 / CONSUMER;
    for (std::size_t i = 0; i < CONSUMER; i++)
        _conpbs.emplace_back(std::make_unique<ProgressBar>())->Init(width * 40 / 64, height * 3 / 64 + i * _dcon, width * 5 / 16, height / 20, MAXNC, 1, ROUND, RGB(63, 63, 70), RGB(160, 160, 160), 2, RGB(0, 122, 204), RGB(150, 150, 150), 3);
    // 开辟线程：
    _prcs.reserve(PRODUCER);
    for (std::size_t i = 0; i < PRODUCER; i++)
        _prcs.emplace_back(std::bind(&Sim::Producer, this, i)).detach();
    _cons.reserve(CONSUMER);
    for (std::size_t i = 0; i < CONSUMER; i++)
        _cons.emplace_back(std::bind(&Sim::Consumer, this, i)).detach();
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
    setlinestyle(PS_SOLID, 2);
    for (std::size_t i = 0; i < CONSUMER; i++)
        fillroundrect(_width * 33 / 64, _height * 8 / 64 + i * _dcon, _width * 60 / 64, _height * 1 / 64 + (i + 1) * _dcon, _height / 20, _height / 20);
    _send.Setmytextstyle();
    settextcolor(WHITE);
    outtextxy(_width * 26 / 64 + 12, _height * 54 / 64, L"Empty:");
    outtextxy(_width * 60 / 64 + 4, _height * 54 / 64, (L"Full:" + std::to_wstring(g_fulls.Count())).c_str());

    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);
    setlinestyle(&lo);
}

void Sim::Drawbutton()
{
    int id = 0;
    if (_clear.Use(id, NULL, !_text.bchar.empty()))
        _text.bchar.clear();
    if (_send.Use(id, NULL, g_emptys.Count() > 0 && !_text.bchar.empty() && _crP > 0))
        for (std::size_t i = 0; i < PRODUCER; i++)
            if (_readyPs[i])
            {
                _readyPs[i] = false;
                break;
            }
    if (_rand.Use(id, NULL, g_emptys.Count() > 0 && _crP > 0))
    {
        _text.bchar = randStr(MAXNC / 2, MAXNC);
        for (std::size_t i = 0; i < PRODUCER; i++)
        {
            if (_readyPs[i])
            {
                _readyPs[i] = false;
                break;
            }
        }
    }
}

void Sim::Drawcomp()
{
    settextcolor(WHITE);
    for (std::size_t i = 0; i < PRODUCER; i++)
    {
        Draw(_width * 1 / 64, _height * 3 / 64 + i * _dprc + 1, _width * 6 / 64, _height / 20, ROUND, RGB(45, 45, 48), RGB(150, 150, 150), 2, L"PRC" + std::to_wstring(i + 1), TMID);
        Draw(_width * 2 / 64, _height * 8 / 64 + i * _dprc + 1, _width * 6 / 16, _height / 20, ROUND, RGB(63, 63, 70), RGB(150, 150, 150), 2, _prcstrs[i].get() ? _prcstrs[i].get()->data() : L"", TLEFT);
    }
    for (std::size_t i = 0; i < CONSUMER; i++)
    {
        Draw(_width * 33 / 64, _height * 3 / 64 + i * _dcon + 1, _width * 6 / 64, _height / 20, ROUND, RGB(45, 45, 48), RGB(150, 150, 150), 2, L"CON" + std::to_wstring(i + 1), TMID);
        std::vector<std::wstring> out;
        out.push_back(_resC[i]);
        while (out.back().size() > 35)
        {
            out.push_back(out.back().substr(35));
            out[out.size() - 2] = out[out.size() - 2].substr(0, 35);
        }
        for (std::size_t j = 0; j < out.size(); j++)
            outtextxy(_width * 33 / 64 + 4, _height * 8 / 64 + i * _dcon + 4 + j * _height * 3 / 64, out[j].c_str());
    }
    for (std::size_t i = 0; i < PRODUCER; i++)
        _prcpbs[i]->Put();
    for (std::size_t i = 0; i < CONSUMER; i++)
        _conpbs[i]->Put();
}

void Sim::Drawset()
{
    int id = 0;
    static bool in = false;
    _text.Use(id, &_text.bchar);
    if (!in && !_size.Use(id, &_size.bchar))
        _size.bchar = std::to_wstring(g_emptys.Count());
    else if (!in)
        in = true;
    // std::thread p(std::bind(&Components::Det, &_text));
    // p.detach();
    _text.Det();
    if (_size.Det())
    {
        const auto &tmp = std::stoi(_size.bchar);
        if (tmp > _ebuffers.size())
        {
            _ebuffers.reserve(tmp);
            for (std::size_t i = tmp - _ebuffers.size(); i > 0; i--)
                _ebuffers.emplace_back(std::make_unique<std::wstring>());
        }
        else
            _ebuffers.resize(tmp);
        g_emptys.Set(tmp);
        in = false;
    }
}

Sim::~Sim()
{
}

bool Sim::Loggin()
{
    std::wstring ccon, cprc, hz, nc;
    ccon.resize(2);
    cprc.resize(2);
    hz.resize(5);
    nc.resize(3);
    while (InputBox(cprc.data(), 2, (L"Producer num Max: " + std::to_wstring(MAX_PRODUCER)).c_str(), 0, L"5", 0, 0, false) &&
           InputBox(ccon.data(), 2, (L"Consumer num Max" + std::to_wstring(MAX_CONSUMER)).c_str(), 0, L"2", 0, 0, false))
    {
        CONSUMER = (unsigned)_wtoi(ccon.c_str());
        PRODUCER = (unsigned)_wtoi(cprc.c_str());
        if (CONSUMER > 0 && CONSUMER <= MAX_CONSUMER && PRODUCER > 0 && PRODUCER <= MAX_PRODUCER)
        {
            if (InputBox(hz.data(), 5, L"Yield speed (ms)", 0, L"250", 0, 0, false) &&
                InputBox(nc.data(), 3, L"Buffer size", 0, L"50", 0, 0, false))
            {
                HZ = (unsigned)_wtoi(hz.c_str());
                MAXNC = (unsigned)_wtoi(nc.c_str());
                return true;
            }
        }
        else if (!(IDRETRY == MessageBox(_hwnd, L"Invalid Producer or Conusmer num!", L"Wrong", MB_RETRYCANCEL | MB_ICONWARNING | MB_SETFOREGROUND)))
            return false;
    }
    return false;
}

void Sim::Producer(const std::size_t i)
{
    Filter f(HZ); // 滤波器
    bool start = true;
    std::wstring tmp;
    while (!g_quit)
        if (!_readyPs[i])
        {
            if (start)
            {
                // 设置状态
                start = false;
                _mP.lock();
                _crP--;
                _mP.unlock();

                g_emptys.Wait();
                // 从空缓冲池中摘下一个缓冲区交给生产者
                _mebuffer.lock();
                _prcstrs[i] = std::move(_ebuffers.back());
                _ebuffers.pop_back();
                _mebuffer.unlock();

                // 准备生产内容
                tmp = _text.bchar;
                _text.bchar.clear();
                // 重置进度条
                _prcpbs[i]->Clear();
                _prcpbs[i]->Set(tmp.size(), 1);
            }
            if (f.filter()) // 滤波
            {
                _prcstrs[i]->push_back(tmp[_prcstrs[i]->size()]); // 模拟生产过程
                if (!_prcpbs[i]->Use())                           // 进度条结束
                {
                    // 重置状态
                    _readyPs[i] = true;
                    start = true;
                    _mP.lock();
                    _crP++;
                    _mP.unlock();

                    // 将缓冲区从生产者中摘下移入满缓冲池
                    _mfbuffer.lock();
                    _fbuffers.emplace_back(std::move(_prcstrs[i]));
                    _mfbuffer.unlock();
                    g_fulls.Signal();
                }
            }
        }
}

void Sim::Consumer(const std::size_t i)
{
    Filter f(HZ);
    while (!g_quit)
    {
        g_fulls.Wait();
        // 从满缓冲池中摘下一个缓冲区交给消费者
        _mfbuffer.lock();
        _constrs[i] = std::move(_fbuffers.back());
        _fbuffers.pop_back();
        _mfbuffer.unlock();

        // 重置进度条
        _conpbs[i]->Clear();
        _conpbs[i]->Set(_constrs[i]->size(), 1);
        int n = 0;
        while (!g_quit)
        {
            if (f.filter())
            {
                _resC[i].push_back(_constrs[i]->at(n++)); // 模拟消费过程
                if (!_conpbs[i]->Use())                   // 进度条结束
                {
                    // 将缓冲区从消费者中摘下移入空缓冲池
                    _constrs[i]->clear();
                    _mebuffer.lock();
                    _ebuffers.emplace_back(std::move(_constrs[i]));
                    _mebuffer.unlock();
                    g_emptys.Signal();
                    break;
                }
            }
        }
    }
}
