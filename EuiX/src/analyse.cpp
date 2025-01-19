#include "analyse.hpp"
#include <Windows.h>
#include <string>

namespace EuiX
{
    std::wstring LANGUAGE = L"en_US";
    static std::wstring DATA_PATH = L"data";
    static std::wstring LANGUAGE_PATH = L"language.ini";
    // static wchar_t *DATA_PATH = L"data";
    // static wchar_t *LANGUAGE_PATH = L"language.ini";
    static int NC = 51;

    void SetConfig(const wchar_t *language)
    {
        // std::wcscpy(LANGUAGE, language);
        LANGUAGE = language;
    }

    void SetConfig(const wchar_t *language, const wchar_t *dataPath, const wchar_t *languagePath, const int confNum)
    {
        LANGUAGE = language;
        DATA_PATH = dataPath;
        LANGUAGE_PATH = languagePath;
        // std::wcscpy(LANGUAGE, language);
        // std::wcscpy(DATA_PATH, dataPath);
        // std::wcscpy(LANGUAGE_PATH, languagePath);
        NC = confNum;
    }

    std::wstring Depart(std::wstring in, std::wstring &out1, std::wstring &out2)
    {
        std::wstring outn = L"";
        std::wstring::size_type c;
        out1.clear();
        out2.clear();
        if ((c = in.find('_')) == std::wstring::npos)
            return in;
        outn = in.substr(0, c);
        out1 = in.substr(c + 1);
        if ((c = out1.find(L'_')) == std::wstring::npos)
            return outn;
        out2 = out1.substr(c + 1);
        out1 = out1.substr(0, c);
        return outn;
    }

    std::vector<std::pair<std::wstring, std::wstring>> DepartFlag(std::wstring in, wchar_t flag)
    {
        std::vector<std::pair<std::wstring, std::wstring>> res;
        std::wstring temp = L"";
        std::wstring::size_type c, l;
        for (c = in.find(L'|'); c != std::wstring::npos; c = in.find(L'|'))
        {
            if ((l = in.find(L'|', c + 1)) == std::wstring::npos)
                break;
            temp = in.substr(c + 1, l - c - 1);
            in = in.substr(l);
            if ((c = temp.find(flag)) == std::wstring::npos)
                continue;
            res.emplace_back(std::make_pair(temp.substr(0, c), temp.substr(c + 1)));
        }
        return std::move(res);
    }

    bool ContainFind(std::wstring in1, std::wstring in2)
    {
        std::wstring s1, s2;
        std::wstring::size_type c1, c2, t1, t2;
        c1 = in1.find(L'_');
        c2 = in2.find(L'_');
        t1 = c1;
        t2 = c2;
        if (c1 == std::wstring::npos)
            s1 = in1;
        else
        {
            s1 = in1.substr(t1 + 1);
            if ((c1 = in1.find(L'_', t1 + 1)) != std::wstring::npos)
                s1 = in1.substr(t1 + 1, c1 - 1);
        }
        if (c2 == std::wstring::npos)
            s2 = in2;
        else
        {
            s2 = in2.substr(t2 + 1);
            if ((c2 = in2.find(L'_', t2 + 1)) != std::wstring::npos)
                s2 = in2.substr(t2 + 1, c2 - 1);
        }
        if (c1 == std::wstring::npos && c2 == std::wstring::npos)
        {
            if (s1.compare(s2) == 0)
                return true;
            else
                return false;
        }
        else
        {
            if (c1 == std::wstring::npos)
                ;
            else
            {
                s1 = in1.substr(c1 + 1);
            }
            if (c2 == std::wstring::npos)
                ;
            else
            {
                s2 = in2.substr(c2 + 1);
            }
            if (s1.compare(s2) == 0)
                return true;
            else
                return false;
        }
    }

    bool LoadLanguage()
    {
        std::wstring s = L".\\" + std::wstring(DATA_PATH) + L"\\", conf;
        conf = s + LANGUAGE_PATH;
        if (_waccess(s.c_str(), 0))
        {
            _wmkdir(s.c_str());
        }
        if (_waccess(conf.c_str(), 0))
        {
            HANDLE hfile = CreateFile(conf.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
            WORD unicode_identifier = 0xfeff;
            DWORD bytes_witten;
            WriteFile(hfile, &unicode_identifier, sizeof(WORD), &bytes_witten, NULL);
            CloseHandle(hfile);
            WritePrivateProfileString(L"Language", L"Object", L"Example", conf.c_str());
            return true;
        }
        else
            return false;
    }

    std::wstring LoadLanguage(std::wstring language, std::wstring obj)
    {
        // wchar_t res[NC] = L"";
        std::wstring res;
        res.resize(NC);
        std::wstring conf = L".\\" + std::wstring(DATA_PATH) + L"\\" + LANGUAGE_PATH;
        if (LoadLanguage())
            return obj;
        GetPrivateProfileString(language.c_str(), obj.c_str(), obj.c_str(), res.data(), NC, conf.c_str());
        return res;
    }

    std::wstring Trim(std::wstring str)
    {
        if (str.empty())
            return str;
        str.erase(str.find_last_not_of(L'\0') + 1);
        str.erase(0, str.find_first_not_of(L" "));
        str.erase(str.find_last_not_of(L" ") + 1);
        str.shrink_to_fit();
        return str;
    }

    std::wstring CAtoW(const char *str)
    {
        if (!str)
            return L"";
        int wcLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        std::wstring newBuf;
        newBuf.resize(wcLen - 1);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, newBuf.data(), wcLen);
        return newBuf;
    }

    std::wstring AtoW(const std::string &str)
    {
        if (str.empty())
            return L"";
        int wcLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        std::wstring newBuf;
        newBuf.resize(wcLen - 1);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, newBuf.data(), wcLen);
        return newBuf;
    }

    std::string WtoA(const std::wstring &str)
    {
        if (str.empty())
            return "";
        int cLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, 0, NULL);
        std::string newBuf;
        newBuf.resize(cLen - 1);
        WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, newBuf.data(), cLen, 0, NULL);
        return newBuf;
    }

    static bool InitConf(const wchar_t *conf)
    {
        if (!_waccess(conf, 0))
            return false;
        WritePrivateProfileString(L"Target", L"Key", L"Value", conf);
        return true;
    }

    std::vector<std::wstring> LoadConf(const wchar_t *conf, const wchar_t *app, const std::vector<std::wstring> &keys, const std::vector<std::wstring> &defaultValue)
    {
        InitConf(conf);
        std::vector<std::wstring> temp;
        temp.reserve(keys.size());
        for (size_t i = 0; i < keys.size(); i++)
        {
            // wchar_t res[NC];
            std::wstring res;
            res.resize(NC);
            GetPrivateProfileString(app, keys[i].c_str(), 0, res.data(), NC, conf);
            std::wstring sres = res;
            if (sres.empty())
            {
                if (i < defaultValue.size())
                    sres = L"NULL_" + defaultValue[i];
                else
                    sres = L"NULL";
                WritePrivateProfileString(app, keys[i].c_str(), sres.c_str(), conf);
            }
            temp.push_back(sres);
        }
        return std::move(temp);
    }
}
