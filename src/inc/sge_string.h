#pragma once
#include <sstream>
#include <locale>
#include <string>

namespace SGE
{
	class String
	{
	public:
		String() { _cString = std::wstring(L""); }
		String(std::wstring string) { _cString = std::wstring(string); }
		String(std::string string) { _cString = std::wstring(string.begin(), string.end()); }
		String(const wchar_t* chars) { _cString = std::wstring(chars); }
		String(const char* chars)
		{
			auto string = std::string(chars);
			_cString = std::wstring(string.begin(), string.end());
		}
		String(const String& string)
		{
			this->_cString = std::wstring(string._cString);
		};

		String& operator=(const String& s) = default;

		String& operator+=(const String& s)
		{
			*this = *this + s;
			return *this;
		}

		String operator+(const String& s) const
		{
			String result;
			result._cString = std::wstring(_cString);
			result._cString += std::wstring(s._cString);
			return result;
		}

		String operator+(int i) const
		{
			String result;
			result._cString = std::wstring(_cString);
			result._cString += std::to_wstring(i);
			return result;
		}

		String operator+(bool data) const
		{
			String result;
			result._cString = std::wstring(_cString);
			result._cString += data ? L"True" : L"False";
			return result;
		}

		String operator*(int repeat) const
		{
			String result;
			std::wstringstream os;
			for(int i = 0; i < repeat; i++)
			{
				os << _cString;
			}

			result._cString = os.str();
			return result;
		}

		[[nodiscard]]
		int Length() const
		{
			return _cString.length();
		}

		[[nodiscard]]
		std::wstring AsStdWString() const
		{
			return std::wstring(_cString);
		}

//POSSIBLE LOSS OF DATA - DISABLE
#pragma warning(disable: 4244)
		std::string AsStdString()
		{
			return std::string(std::string(_cString.begin(), _cString.end()));
		};
#pragma warning(default: 4244)
		
	private:
		std::wstring _cString;
	};
}
