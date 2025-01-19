#include "components.hpp"
#include "widget.hpp"

namespace EuiX
{
    Components::Components()
    {
        _input = NULL;
        _triger = 0;
        _equ = false;
        _id = 0;
        _dclick = false;
        type = 0;
        _x = 0;
        _y = 0;
        _limits = 0;
        _w = 0;
        _h = 0;
        bchar = L"";
        _m = 0;
        _position = TMID;
        _BoxTextC = BLACK;
        _outColor = WHITE;
        _outLineC = LIGHTBLUE;
        _outWeight = 0;
        _inColor = LIGHTGRAY;
        _inLineC = BLUE;
        _inWeight = 0 + 2;
        _Text = L"DEFALUT TEXT";
        _Title = L"DEFALUT TITLE";
        _Type = MB_OK;

        _ButtonInTC = BLACK;
        _ButtonClickTC = 0;
        _ButtonClickBC = 0;
        _ButtonClickLC = 0;
        _ButtonCLWeight = 0;

        _TextColor = BLACK;
        _TextBoxC = WHITE;
        _LineColor = LIGHTGRAY;
        _LineWeight = 2;
        _TextType = ALLC;
        _Sign = '\n';

        _nHeight = 0;
        _nWidth = 0;
        _lpszFace = FRONT;
        _nEscapement = 0;
        _nOrientation = 0;
        _nWeight = 0;
        _bItalic = false;
        _bUnderline = false;
        _bStrikeOut = false;
        _fbQuality = ANTIALIASED_QUALITY;
        _fbPitchAndFamily = DEFAULT_PITCH;
        _press = false;
    }

    Components::Components(int x, int y, size_t limits, int id, bool dclick, int triger, bool equ, COLORREF dormantC)
    {
        _input = NULL;
        if (limits == 0)
            type = BUTTON;
        else if (limits > 0)
            type = TEXTBOX;
        _triger = triger;
        _equ = equ;
        _dormantC = dormantC;
        _dclick = dclick;
        _x = x;
        _y = y;
        _limits = limits;
        _id = id;
        _w = 60;
        _h = 30;
        bchar = L"DEFALUT";
        _m = SQUAR;
        _position = TMID;
        _BoxTextC = BLACK;
        _outColor = WHITE;
        _outLineC = LIGHTBLUE;
        _outWeight = 0;
        _inColor = LIGHTGRAY;
        _inLineC = BLUE;
        _inWeight = 0 + 2;
        _Text = L"DEFALUT TEXT";
        _Title = L"DEFALUT TITLE";
        _Type = MB_OK;

        _ButtonInTC = BLACK;
        _ButtonClickTC = 0;
        _ButtonClickBC = 0;
        _ButtonClickLC = 0;
        _ButtonCLWeight = 0;

        _TextColor = BLACK;
        _TextBoxC = WHITE;
        _LineColor = LIGHTGRAY;
        _LineWeight = 2;
        _TextType = ALLC;
        _Sign = '\n';

        _nHeight = 15;
        _nWidth = 0;
        _lpszFace = FRONT;
        _nEscapement = 0;
        _nOrientation = 0;
        _nWeight = 0;
        _bItalic = false;
        _bUnderline = false;
        _bStrikeOut = false;
        _fbQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;
        _fbPitchAndFamily = DEFAULT_PITCH;
        _press = false;
    }

    Components &Components::Init(int x, int y, size_t limits, int id, bool dclick, int triger, bool equ, COLORREF dormantC)
    {
        _input = NULL;
        if (limits == 0)
            type = BUTTON;
        else if (limits > 0)
            type = TEXTBOX;
        _triger = triger;
        _equ = equ;
        _dormantC = dormantC;
        _dclick = dclick;
        _x = x;
        _y = y;
        _limits = limits;
        _id = id;
        _w = 60;
        _h = 30;
        bchar = L"DEFALUT";
        _m = SQUAR;
        _position = TMID;
        _BoxTextC = BLACK;
        _outColor = WHITE;
        _outLineC = LIGHTBLUE;
        _outWeight = 0;
        _inColor = LIGHTGRAY;
        _inLineC = BLUE;
        _inWeight = 0 + 2;
        _Text = L"DEFALUT TEXT";
        _Title = L"DEFALUT TITLE";
        _Type = MB_OK;

        _ButtonInTC = BLACK;
        _ButtonClickTC = 0;
        _ButtonClickBC = 0;
        _ButtonClickLC = 0;
        _ButtonCLWeight = 0;

        _TextColor = BLACK;
        _TextBoxC = WHITE;
        _LineColor = LIGHTGRAY;
        _LineWeight = 2;
        _TextType = ALLC;
        _Sign = '\n';

        _nHeight = 15;
        _nWidth = 0;
        _lpszFace = FRONT;
        _nEscapement = 0;
        _nOrientation = 0;
        _nWeight = 0;
        _bItalic = false;
        _bUnderline = false;
        _bStrikeOut = false;
        _fbQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;
        _fbPitchAndFamily = DEFAULT_PITCH;
        _press = false;
        return *this;
    }

    Components &Components::Set(
        int w, int h, std::wstring Char, int m, int position,
        COLORREF BoxTextC, COLORREF outColor, COLORREF outLineC, int outWeight,
        COLORREF inColor, COLORREF inLineC, int inWeight,
        std::wstring Text, std::wstring Title, UINT Type)
    {
        _w = w;
        _h = h;
        _nHeight = _h / 2;
        bchar = Char;
        _m = m;
        _position = position;
        _BoxTextC = BoxTextC;
        _outColor = outColor;
        _outLineC = outLineC;
        _outWeight = outWeight;
        _inColor = inColor;
        _inLineC = inLineC;
        _inWeight = inWeight;
        _Text = Text;
        _Title = Title;
        _Type = Type;
        return *this;
    }

    Components &Components::Set(COLORREF ButtonInTC, COLORREF ButtonClickTC, COLORREF ButtonClickBC, COLORREF ButtonClickLC, int ButtonCLWeight)
    {
        _ButtonInTC = ButtonInTC;
        _ButtonClickTC = ButtonClickTC;
        _ButtonClickBC = ButtonClickBC;
        _ButtonClickLC = ButtonClickLC;
        _ButtonCLWeight = ButtonCLWeight;
        return *this;
    }

    Components &Components::Set(COLORREF TextColor, COLORREF TextBoxC, COLORREF LineColor, int LineWeight, int TextType, char Sign)
    {
        _TextColor = TextColor;
        _TextBoxC = TextBoxC;
        _LineColor = LineColor;
        _LineWeight = LineWeight;
        _TextType = TextType;
        _Sign = Sign;
        return *this;
    }

    Components &Components::Set(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut, BYTE fbQuality, BYTE fbPitchAndFamily)
    {
        _nHeight = nHeight == 0 ? _h / 2 : nHeight;
        _nWidth = nWidth;
        _lpszFace = lpszFace;
        _nEscapement = nEscapement;
        _nOrientation = nOrientation;
        _nWeight = nWeight;
        _bItalic = bItalic;
        _bUnderline = bUnderline;
        _bStrikeOut = bStrikeOut;
        _fbQuality = fbQuality;
        _fbPitchAndFamily = fbPitchAndFamily;
        return *this;
    }

    bool Components::Use(int &id)
    {
        switch (type)
        {
        case BUTTON:
            return Button(id, id);
            break;
        case TEXTBOX:
            bchar = L"";
            if (Button(id, id))
            {
                _press = true;
                _input = nullptr;
                return true;
            }
            break;
        default:
            break;
        }
        return false;
    }

    bool Components::Use(int &id, std::wstring *input)
    {
        switch (type)
        {
        case BUTTON:
            return Button(id, id);
            break;
        case TEXTBOX:
            bchar = *input;
            if (Button(id, id))
            {
                _press = true;
                _input = input;
                return true;
            }
            break;
        default:
            break;
        }
        return false;
    }
    
    bool Components::Use(int &id, std::wstring *input, int triger)
    {
        switch (type)
        {
        case BUTTON:
            return Button(id, triger);
            break;
        case TEXTBOX:
            bchar = *input;
            if (Button(id, triger))
            {
                _press = true;
                _input = input;
                return true;
            }
            break;
        default:
            break;
        }
        return false;
    }

    bool Components::Det()
    {
        if (type == TEXTBOX && _press)
        {
            LOGFONT f;
            gettextstyle(&f);
            Setmytextstyle();
            if (!KeyM(_x, _y, _w, _h, *_input, _m, _TextColor, _TextBoxC, _LineColor, _LineWeight, _limits, _TextType, _Sign))
            {
                bchar = *_input;
                _press = false;
                return true;
            }
            settextstyle(&f);
        }
        return false;
    }

    Components &Components::Setmytextstyle()
    {
        settextstyle(_nHeight, _nWidth, _lpszFace, _nEscapement, _nOrientation, _nWeight, _bItalic, _bUnderline, _bStrikeOut, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, _fbQuality, _fbPitchAndFamily);
        return *this;
    }

    Components::~Components()
    {
    }

    bool Components::Button(int &id, int triger)// TODO 修改文本框use的逻辑，即输入状态时一直返回true
    {
        COLORREF click, button = _inColor, linec = _inLineC, text;
        int line = _inWeight;
        float H, S, L;
        bool r = !_dclick;
        RGBtoHSL(_outColor, &H, &S, &L);
        click = _ButtonClickBC == 0 ? HSLtoRGB(H, S / 1, L / 2) : _ButtonClickBC;
        RGBtoHSL(_outLineC, &H, &S, &L);
        text = _BoxTextC;
        if ((_triger == 0) || (_equ && triger == _triger) || (!_equ && triger != _triger))
        {
            switch (MouseM(_x, _y, _w, _h, OUTSTAY))
            {
            case DCLICK:
                if (_dclick)
                {
                    r = true;
                }
            case RCLICK:
            case CLICK:
                text = _ButtonInTC == 0 ? _BoxTextC : _ButtonInTC;
                button = click;
                linec = _ButtonClickLC == 0 ? HSLtoRGB(H, S / 1, L / 2) : _ButtonClickLC;
                line = _ButtonCLWeight == 0 ? _inWeight : _ButtonCLWeight;
                text = _ButtonClickTC == 0 ? _TextColor : _ButtonClickTC;
                break;
            case RUP:
                MessageBox(NULL, _Text.c_str(), _Title.c_str(), _Type);
                break;
            case UP:
                id = _id;
                break;
            case STAY:
                text = _ButtonInTC == 0 ? _BoxTextC : _ButtonInTC;
                button = _inColor;
                linec = _inLineC;
                line = _inWeight;
                break;
            default:
                button = _outColor;
                linec = _outLineC;
                line = _outWeight;
                break;
            }
            if (id == _id)
            {
                // std::cout << bchar << " up: " << id << std::endl;
                button = click;
                linec = _ButtonClickLC == 0 ? HSLtoRGB(H, S / 1, L / 2) : _ButtonClickLC;
                line = _ButtonCLWeight == 0 ? _inWeight : _ButtonCLWeight;
                text = _ButtonClickTC == 0 ? _TextColor : _ButtonClickTC;
            }
            LOGFONT f;
            gettextstyle(&f);
            setbkmode(TRANSPARENT);
            Setmytextstyle();
            _w = _w == 0 ? (int)(_limits + 1) * textwidth('w') + 4 : _w;
            settextcolor(text);
            Draw(_x, _y, _w, _h, _m, button, linec, line, bchar, _position);
            settextstyle(&f);
            return id == _id ? r : false;
        }
        else
        {
            button = _outColor;
            linec = _outLineC;
            line = _outWeight;
            LOGFONT f;
            gettextstyle(&f);
            setbkmode(TRANSPARENT);
            Setmytextstyle();
            _w = _w == 0 ? (int)(_limits)*textwidth('w') + 2 : _w;
            settextcolor(_dormantC);
            Draw(_x, _y, _w, _h, _m, button, linec, line, bchar, _position);
            settextstyle(&f);
            return false;
        }
    }
}
