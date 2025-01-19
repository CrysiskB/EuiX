#ifndef EuiX_HPP
#define EuiX_HPP

#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "easyx.h"

#define ROUND 0 // 圆的
#define SQUAR 1 // 方的

#define TLEFT 1  // 文本左对齐
#define TMID 2   // 文本居中
#define TRIGHT 3 // 文本右对齐
#define TTEXT 4  // 文本输入对齐

// MouseM 的参数
#define UP 1        // 左键先点击后松开在内
#define CLICK 2     // 左键点击在内
#define STAY 3      // 停留在内
#define OUTCLICK 4  // 左键点击在外
#define OUTSTAY 5   // 停留在外
#define RUP 6       // 右键先点击后松开在内
#define RCLICK 7    // 右键点击在内
#define ROUTCLICK 8 // 右键点击在外
#define DCLICK 9    // 左键双击

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

// Button 的参数
#define TEXTBOX 0 // 文本框形式
#define BUTTON 1  // 按钮形式

namespace EuiX
{
    // 语言
    extern std::wstring LANGUAGE;

    // 设置语言
    void SetConfig(const wchar_t *language);

    // 设置语言、data文件夹路径、语言配置文件名、读取配置最大字符数
    void SetConfig(const wchar_t *language, const wchar_t *dataPath, const wchar_t *languagePath, const int confNum);

    // 将string型输入in：A_B_C分解，返回A，out1接收B，out2接收C。或者输入A_B，返回A，out1接收B，out2返回空
    std::wstring Depart(std::wstring in, std::wstring &out1, std::wstring &out2);

    // | key $flag value |
    std::vector<std::pair<std::wstring, std::wstring>> DepartFlag(std::wstring in, wchar_t flag);

    // 将输入in1和in2比较，从第一个下划线开始比较
    // 输入为A_B_C和D_E_F时则比较B_C和E_F，若相同则返回true
    // 输入为一个A_B，另一个C_D_E，则用B分别比较D和E，有相同的则返回true
    // 输入为A_B和C_D，则只比较B和D，相同则返回true
    // 有一个输入为单A时，则将A和另一输入的每个下划线后面的比较，有相同则返回true：B比较B，B_C比较C,B_C_D比较C和D
    // 以下划线分隔，每次比较最多拆解2次下划线
    bool ContainFind(std::wstring in1, std::wstring in2);

    bool LoadLanguage();

    // 读取语言配置文件，若无则自动初始化，查找language下的obj，并返回该obj的别名，用于语言系统
    std::wstring LoadLanguage(std::wstring language, std::wstring obj);

    std::wstring Trim(std::wstring str);
    std::wstring CAtoW(const char *str);
    std::wstring AtoW(const std::string &str);
    std::string WtoA(const std::wstring &str);

    std::vector<std::wstring> LoadConf(const wchar_t *conf, const wchar_t *app, const std::vector<std::wstring> &keys, const std::vector<std::wstring> &defaultValue = std::vector<std::wstring>());

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
    void KeyM(int x, int y, int w, int h, std::wstring &Char, int m, COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int Weight, size_t limits, int Type, char Sign);
    bool KEYUP(int vk);
    void Calender(std::wstring &Year, std::wstring &Month, std::wstring &Day);
    long Clock(
        std::wstring FYear, std::wstring FMonth, std::wstring FDay,
        std::wstring &LYear, std::wstring &LMonth, std::wstring &LDay);
    std::wstring randStr(const UINT minLen, const UINT maxLen);

    class Components
    {
    public:
        int type;
        std::wstring bchar;

        Components();
        Components(int x, int y, size_t limits, int id, bool dclick = false, int triger = 0, bool equ = true, COLORREF dormantC = BLACK);
        Components &Init(int x, int y, size_t limits, int id, bool dclick = false, int triger = 0, bool equ = true, COLORREF dormantC = BLACK);
        Components &Set(int w, int h, std::wstring Char, int m, int position, COLORREF BoxTextC, COLORREF outColor, COLORREF outLineC, int outWeight,
                        COLORREF inColor, COLORREF inLineC, int inWeight, std::wstring Text, std::wstring Title, UINT Type);
        Components &Set(COLORREF ButtonInTC, COLORREF ButtonClickTC, COLORREF ButtonClickBC, COLORREF ButtonClickLC, int ButtonCLWeight);
        Components &Set(COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int LineWeight, int TextType, char Sign);
        Components &Set(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight,
                        bool bItalic, bool bUnderline, bool bStrikeOut, BYTE fbQuality, BYTE fbPitchAndFamily);
        bool Use(int &id);
        bool Use(int &id, std::wstring *input);
        bool Use(int &id, std::wstring *input, int triger);
        bool Det();
        Components &Setmytextstyle();
        ~Components();

    private:
        bool Button(int &id, int triger);

        std::wstring *_input, _Text, _Title;
        int _triger, _id, _x, _y, _w, _h, _m, _position, _outWeight, _inWeight, _ButtonCLWeight, _LineWeight,
            _TextType, _nHeight, _nWidth, _nEscapement, _nOrientation, _nWeight;
        bool _equ, _dclick, _bItalic, _bUnderline, _bStrikeOut, _press;
        COLORREF _dormantC, _BoxTextC, _outColor, _outLineC, _inColor, _inLineC,
            _ButtonInTC, _ButtonClickTC, _ButtonClickBC, _ButtonClickLC, _TextColor, _TextBoxC, _LineColor;
        size_t _limits;
        UINT _Type;
        char _Sign;
        LPCTSTR _lpszFace;
        BYTE _fbQuality, _fbPitchAndFamily;
    };
    
    class ProgressBar
    {
    public:
        ProgressBar() {};
        ProgressBar(int x, int y, int w, int h, int l, int dl, int m, COLORREF outColor, COLORREF outLineC, int outWeight, COLORREF inColor, COLORREF inLineC, int inWeight);
        void Init(int x, int y, int w, int h, int l, int dl, int m, COLORREF outColor, COLORREF outLineC, int outWeight, COLORREF inColor, COLORREF inLineC, int inWeight);
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
