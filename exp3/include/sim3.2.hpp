#ifndef msr_sim3_2_Hpp
#define msr_sim3_2_Hpp

#include "conf.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

extern bool g_quit;

struct Res
{
    UINT A;
    UINT B;
    UINT C;
    Res(UINT A = 0, UINT B = 0, UINT C = 0) : A(A), B(B), C(C) {};
    Res Res::operator+(const Res &res)
    {
        return Res(A + res.A, B + res.B, C + res.C);
    }
    Res Res::operator-(const Res &res)
    {
        return Res(A - res.A, B - res.B, C - res.C);
    }
    Res &Res::operator+=(const Res &res)
    {
        A += res.A;
        B += res.B;
        C += res.C;
        return *this;
    }
    Res &Res::operator-=(const Res &res)
    {
        A -= res.A;
        B -= res.B;
        C -= res.C;
        return *this;
    }
    bool Res::operator==(const UINT k)
    {
        return A == k && B == k && C == k;
    }
    bool Res::operator==(const Res &res)
    {
        return A == res.A && B == res.B && C == res.C;
    }
    bool Res::operator>(const Res &res)
    {
        return A > res.A && B > res.B && C > res.C;
    }
    bool Res::operator<(const Res &res)
    {
        return A < res.A && B < res.B && C < res.C;
    }
    bool Res::operator>=(const Res &res)
    {
        return A >= res.A && B >= res.B && C >= res.C;
    }
    bool Res::operator<=(const Res &res)
    {
        return A <= res.A && B <= res.B && C <= res.C;
    }
};

struct Prc
{
    Res max;
    Res allc;
    std::vector<Components> maxTs;
    std::vector<Components> allcTs;
    Prc(Res max, Res allc)
    {
        this->max = max;
        this->allc = allc;
        maxTs.resize(3);
        allcTs.resize(3);
    }
    void syncRes()
    {
        maxTs[0].bchar = std::to_wstring(max.A);
        maxTs[1].bchar = std::to_wstring(max.B);
        maxTs[2].bchar = std::to_wstring(max.C);
        allcTs[0].bchar = std::to_wstring(allc.A);
        allcTs[1].bchar = std::to_wstring(allc.B);
        allcTs[2].bchar = std::to_wstring(allc.C);
    }
    void syncMax()
    {
        max.A = (unsigned)std::stoi(maxTs[0].bchar);
        max.B = (unsigned)std::stoi(maxTs[1].bchar);
        max.C = (unsigned)std::stoi(maxTs[2].bchar);
    }
    void syncAllc()
    {
        allc.A = (unsigned)std::stoi(allcTs[0].bchar);
        allc.B = (unsigned)std::stoi(allcTs[1].bchar);
        allc.C = (unsigned)std::stoi(allcTs[2].bchar);
    }
    bool validMax()
    {
        for (const auto &maxT : maxTs)
            if (maxT.bchar.empty())
                return false;
        return true;
    }
    bool validAllc()
    {
        for (const auto &allcT : allcTs)
            if (allcT.bchar.empty())
                return false;
        return true;
    }
};

struct AllRes
{
    Res all;
    std::vector<Components> allTs;
    AllRes(Res all)
    {
        this->all = all;
        allTs.resize(3);
    }
    void syncRes()
    {
        allTs[0].bchar = std::to_wstring(all.A);
        allTs[1].bchar = std::to_wstring(all.B);
        allTs[2].bchar = std::to_wstring(all.C);
    }
    void syncAll()
    {
        all.A = (unsigned)std::stoi(allTs[0].bchar);
        all.B = (unsigned)std::stoi(allTs[1].bchar);
        all.C = (unsigned)std::stoi(allTs[2].bchar);
    }
    bool validAll()
    {
        for (const auto &allT : allTs)
            if (allT.bchar.empty())
                return false;
        return true;
    }
};

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
    // 银行家算法计算安全序列
    bool Compute(std::vector<std::vector<std::size_t>> &sequences, std::vector<Prc> prcs, Res res);

private:
    HWND _hwnd;         // 主窗口
    std::wstring _name; // 窗口名
    const int _width;   // 窗口宽度
    const int _height;  // 窗口高度

    AllRes _res;            // 总资源
    Res _lastRes;           // 剩余资源
    std::vector<Prc> _prcs; // 进程

    const int _dx; // 排版增量坐标dx
    const int _dy; // 排版增量坐标dy

    std::wstring _line; // 结果字符串

    Components _compute; // 计算按钮
    Components _clear;   // 清空按钮

    std::vector<std::vector<Components>> _texts; // 输入要求文本框
};

#endif