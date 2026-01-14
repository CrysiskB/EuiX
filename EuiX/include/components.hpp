#ifndef msr_components_Hpp
#define msr_components_Hpp

#include "easyx.h"
#include <string>

// TODO 对宏定义添加其对应的命名空间前缀
#ifndef FRONT
#define FRONT L"Courier New"
#endif

// Button 的参数
#define TEXTBOX 0 // 文本框形式
#define BUTTON 1  // 按钮形式

namespace EuiX
{
	class Components
	{
	public:
		int type;
		std::wstring bchar;
		Components();
		Components(
			int x,
			int y,
			size_t limits,
			int id,
			bool dclick = false,
			int triger = 0,
			bool equ = true,
			COLORREF dormantC = BLACK);
		Components &Init(
			int x,
			int y,
			size_t limits,
			int id,
			bool dclick = false,
			int triger = 0,
			bool equ = true,
			COLORREF dormantC = BLACK);
		Components &Set(
			int w,
			int h,
			std::wstring Char,
			int m,
			int position,
			COLORREF BoxTextC,
			COLORREF outColor,
			COLORREF outLineC,
			int outWeight,
			COLORREF inColor,
			COLORREF inLineC,
			int inWeight,
			std::wstring Text,
			std::wstring Title,
			UINT Type);
		Components &Set(
			COLORREF ButtonInTC,
			COLORREF ButtonClickTC,
			COLORREF ButtonClickBC,
			COLORREF ButtonClickLC,
			int ButtonCLWeight);
		Components &Set(
			COLORREF TextColor,
			COLORREF TextBoxC,
			COLORREF LineColor,
			int LineWeight,
			int TextType,
			char Sign);
		Components &Set(
			int nHeight,
			int nWidth,
			LPCTSTR lpszFace,
			int nEscapement,
			int nOrientation,
			int nWeight,
			bool bItalic,
			bool bUnderline,
			bool bStrikeOut,
			BYTE fbQuality,
			BYTE fbPitchAndFamily);
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
}

#endif