#ifndef msr_widget_Hpp
#define msr_widget_Hpp

#include "easyx.h"
#include <string>
#include <chrono>

namespace EuiX
{
#define ROUND 0 // 圆的
#define SQUAR 1 // 方的

#define TLEFT 1	 // 文本左对齐
#define TMID 2	 // 文本居中
#define TRIGHT 3 // 文本右对齐
#define TTEXT 4	 // 文本输入对齐

// MouseM 的参数
#define UP 1		// 左键先点击后松开在内
#define CLICK 2		// 左键点击在内
#define STAY 3		// 停留在内
#define OUTCLICK 4	// 左键点击在外
#define OUTSTAY 5	// 停留在外
#define RUP 6		// 右键先点击后松开在内
#define RCLICK 7	// 右键点击在内
#define ROUTCLICK 8 // 右键点击在外
#define DCLICK 9	// 左键双击

// KeyM Compare的参数
#define CHAR 1 // 包括字母与特殊字符、部分比较
#define NUMB 2 // 只数字、数字比较
#define TIME 3
#define DATE 4
#define TEMP 5 // 只小数点与数字、非Compare参数
#define ALLC 6 // 全部字符、完全比较
#define ALLS 7 // 全部字符包括空格

// 按键检测参数宏，被用于KEYUP函数
#define KEYDOWN(vk) ((GetAsyncKeyState(vk) & 0x8000) ? true : false)

	struct Filter // 滤波器
	{
		std::chrono::steady_clock::time_point startTime;
		const uint16_t frequency;
		bool updateEnabled;
		Filter(const uint16_t frequency);
		bool filter();
	};

	void Draw(int x, int y, int w, int h, int m, COLORREF FillColor, COLORREF LineColor, int Weight, std::wstring Char, int position, bool autoSize = true);
	int MouseM(int x, int y, int w, int h, int defnull);
	// void KeyM(int x, int y, int w, int h, std::wstring &Char, int m, COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int Weight, size_t limits, int Type, char Sign);
	bool KeyM(int x, int y, int w, int h, std::wstring &Char, int m, COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int Weight, size_t limits, int Type, char Sign);
	bool KEYUP(int vk);
	void Calender(std::wstring &Year, std::wstring &Month, std::wstring &Day);
	long Clock(
		std::wstring FYear, std::wstring FMonth, std::wstring FDay,
		std::wstring &LYear, std::wstring &LMonth, std::wstring &LDay);
	std::wstring randStr(const UINT minLen, const UINT maxLen);
}

#endif