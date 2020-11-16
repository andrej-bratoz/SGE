#pragma once
#include <windows.h>
#include <vector>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "sge_string.h"

namespace SGE::Debug
{
	inline void WriteLine(const String s)
	{
		OutputDebugStringW(s.AsStdWString().c_str());
		OutputDebugStringW(String::EndLine().AsStdWString().c_str());
	}

	inline void Write(const String s)
	{
		OutputDebugStringW(s.AsStdWString().c_str());
	}
}

namespace SGE::Exceptions
{
	class SGEException : public std::exception{};
	
	class SGEComException : public SGEException
	{
	public:
		SGEComException(HRESULT hr) : result(hr),_line(-1), _description(L"") {}
		SGEComException(HRESULT hr,int line, const String file) : _callerInfo(file), _line(line),result(hr), _description(L"") {}
		SGEComException(HRESULT hr, String description, int line, const String file) :_callerInfo(file), _line(line) ,result(hr), _description(description) {}
		
		[[nodiscard]] int GetLine() const { return _line; } 
		
		[[nodiscard]] const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

		[[nodiscard]] String Message() const
		{
			return _description;
		}

		[[nodiscard]] String File() const
		{
			return _callerInfo;
		}

		HRESULT Error() const { return result; }

	private:
		int _line{};
		HRESULT result{};
		const String _description;
		const String _callerInfo;
	};
}

namespace SGE
{

	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	
	/*inline void __checked(HRESULT hr)
	{
		if(FAILED(hr))
		{
#if _DEBUG || SGE_TRACE_ON
			String str(L"");
			str = String("SGE Error") + String(L"[") + int_to_hex(hr) + String("]")+ "\n";
			OutputDebugStringW(str.AsStdWString().c_str());
#endif
			throw SGE::Exceptions::SGEComException(hr);
		}
	}

	inline void __checked(HRESULT hr, int line, String file)
	{
		if (FAILED(hr))
		{
#if _DEBUG || SGE_TRACE_ON
			String str(L"");
			str = String("SGE Error") + String(L"[") + int_to_hex(hr) + String("]") + " -> File: " + file + " Line: " + line + "\n";
			OutputDebugStringW(str.AsStdWString().c_str());
#endif
			
			throw SGE::Exceptions::SGEComException(hr, line,file);
		}
	}*/

	inline void __checked(HRESULT hr, int line, const String file, const String message)
	{
		if (FAILED(hr))
		{
#if _DEBUG || SGE_TRACE_ON
			String str(L"");
			str = String(L"SGE Error: (Line ") + line + String(L", File :") + String(file) + String(" ) Code: [") + String::FromHResult(hr) + String(L"] | Message: ");
			str += message;
			SGE::Debug::WriteLine(str.AsStdWString().c_str());
#endif
			
			throw SGE::Exceptions::SGEComException(hr, message ,line, file);
		}
	}
}

template <class T>
constexpr void RemoveFromVector(std::vector<T> v, T value)
{
	v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

inline int SGE_GetKeyboardState(LPARAM inLParam, WPARAM inWParam, LPWSTR data)
{
	BYTE keyState[256];
	GetKeyboardState(keyState);
	// clear all of the modifier keys so ToUnicode will ignore them
	keyState[VK_CONTROL] = keyState[VK_SHIFT] = keyState[VK_MENU] = 0;
	keyState[VK_LCONTROL] = keyState[VK_LSHIFT] = keyState[VK_LMENU] = 0;
	keyState[VK_RCONTROL] = keyState[VK_RSHIFT] = keyState[VK_RMENU] = 0;
	// convert the WM_KEYDOWN/WM_KEYUP/WM_SYSKEYDOWN/WM_SYSKEYUP to characters
	UINT scanCode = (inLParam >> 16) & 0xFF;
	int inOutBufLenCharacters = 32;	
	int i = ToUnicode(static_cast<UINT>(inWParam), scanCode, keyState, data, inOutBufLenCharacters, 0);
	return i;
}

#define SGE_CHECKED_MSG(hr,msg) SGE::__checked(hr, __LINE__, __FILE__, msg);
#define SGE_CHECKED(hr) SGE_CHECKED_MSG(hr, L"")