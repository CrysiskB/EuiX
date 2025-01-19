#ifndef msr_analyse_Hpp
#define msr_analyse_Hpp

#include <string>
#include <vector>

// #ifndef LANGUAGE
// #define LANGUAGE L"en_US"
// #endif
// #ifndef DATA_CONF
// #define DATA_CONF L"data" // data文件夹名
// #endif
// #ifndef LANGUAGE_CONF
// #define LANGUAGE_CONF L"language.ini" // 语言配置文件名
// #endif
// #ifndef NC
// #define NC 51 // 读取配置名最大字符数
// #endif

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
}

#endif