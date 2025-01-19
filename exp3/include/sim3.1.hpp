#ifndef msr_sim3_1_Hpp
#define msr_sim3_1_Hpp

#include "conf.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

extern bool g_quit;

class Sim
{
public:
    Sim(const int width = EX_WIDTH, const int height = EX_HEIGHT);
    void Drawbk();
    void Drawbutton();
    void Drawcomp();
    void Drawset();
    ~Sim();

private:
    bool Loggin(); // 用户初始参数设置

    void Producer(const std::size_t i); // 生产者
    void Consumer(const std::size_t i); // 消费者

private:
    HWND _hwnd;         // 主窗口
    std::wstring _name; // 窗口名
    const int _width;   // 窗口宽度
    const int _height;  // 窗口高度

    Components _send;
    Components _text; // 输入文本框
    Components _size; // 缓冲区大小输入文本框
    Components _clear;
    Components _rand;

    std::size_t HZ;       // 生产、消费速度频率
    std::size_t PRODUCER; // 生产者数量
    std::size_t CONSUMER; // 消费者数量
    std::size_t MAXNC;    // 缓冲大小

    std::mutex _mP;
    int _crP; // 当前正在运行的生产者数

    int _dprc; // 生产者控件增量坐标dy
    int _dcon; // 消费者控件增量坐标dy

    std::vector<bool> _readyPs; // 生产者空闲状态

    std::vector<std::unique_ptr<std::wstring>> _prcstrs; // 生产者生产缓冲区指针
    std::vector<std::unique_ptr<std::wstring>> _constrs; // 消费者消费缓冲区指针

    std::vector<std::wstring> _resC; // 消费者消费结果

    std::mutex _mebuffer;
    std::mutex _mfbuffer;
    std::vector<std::unique_ptr<std::wstring>> _ebuffers; // 空缓冲池（缓冲区指针）
    std::vector<std::unique_ptr<std::wstring>> _fbuffers; // 满缓冲池（缓冲区指针）

    std::vector<std::thread> _prcs; // 生产者线程
    std::vector<std::thread> _cons; // 消费者线程

    std::vector<std::unique_ptr<ProgressBar>> _prcpbs; // 生产者进度条
    std::vector<std::unique_ptr<ProgressBar>> _conpbs; // 消费者进度条
};

#endif