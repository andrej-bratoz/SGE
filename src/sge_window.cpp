#include <Windows.h>
#include "inc/sge_macros.h"
#include "inc/sge_window.h"

std::vector<SGE::Windows::WindowBase*> SGE::Windows::Application::_windows;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return SGE::Windows::Application::DispatchWindowMessage(hwnd, uMsg, wParam, lParam);
}

SGE::Windows::WindowBase::WindowBase() : _size(800,600), _title(L"")
{}

SGE::Windows::WindowBase::WindowBase(const String& title) : _size(800,600)
{
	_title = title;
}

SGE::Windows::WindowBase::WindowBase(const String& title, Size size)
{
	_title = title;
	_size = size;
	this->_win32Window = nullptr;
}

void SGE::Windows::WindowBase::Register()
{
	Application::RegisterWindow(this);
}



// ToDo: Code Duplication!
LRESULT SGE::Windows::WindowBase::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		HandleKeyDownOrChar(wParam, lParam);
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void SGE::Windows::WindowBase::EnsureHandle()
{
	if (this->_win32Window != nullptr) return;

#pragma warning(disable: 4311 4302)
	const auto nameStr = String(L"SparkPlugWindow_") + reinterpret_cast<int>(this);
#pragma warning(default: 4311 4302)
	wchar_t name[1024];
	wcscpy_s(name, nameStr.AsStdWString().c_str());

	//Set up window class
	WNDCLASS wnd;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hIcon = LoadIcon(0, IDI_WINLOGO);
	wnd.lpszMenuName = 0;
	wnd.style = 0;
	wnd.hbrBackground = 0;
	wnd.lpfnWndProc = WindowProc;
	wnd.hInstance = GetModuleHandle(nullptr);
	wnd.lpszClassName = name;

	auto result = RegisterClass(&wnd);
	
	if (!result) { throw 0; }
	
	this->_win32Window = CreateWindowEx(
		0,
		name,
		_title.AsStdWString().c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(nullptr),
		NULL
	);

	if (this->_win32Window == nullptr) {
		int i = GetLastError();
		throw i; // todo exceptions
	}
	Register();
}

void SGE::Windows::WindowBase::Show()
{
	EnsureHandle();
	ShowWindow(this->_win32Window, SW_SHOW);
}

void SGE::Windows::WindowBase::Hide() const
{
	ShowWindow(this->_win32Window, SW_HIDE);
}

void SGE::Windows::WindowBase::OnKeyDown(SGEKeyDownHandler handler)
{
	keydown_handlers.push_back(handler);
}

void SGE::Windows::WindowBase::OnChar(SGEKeyDownHandler handler)
{
	char_handlers.push_back(handler);
}

HWND SGE::Windows::WindowBase::GetHandle() const
{
	return this->_win32Window;
}

SGE::Windows::WindowBase::~WindowBase()
{
	Application::RegisterWindow(this);
}

//
void SGE::Windows::WindowBase::HandleKeyDownOrChar(WPARAM wParam, LPARAM lParam)
{
	const auto isLShiftPressed = GetKeyState(VK_LSHIFT) & 0x8000;
	const auto isRShiftPressed = GetKeyState(VK_RSHIFT) & 0x8000;
	//
	const auto isLControlPressed = GetKeyState(VK_LCONTROL) & 0x8000;
	const auto isRControlPressed = GetKeyState(VK_RCONTROL) & 0x8000;
	//
	const auto isAltPressed = GetKeyState(VK_MENU) & 0x8000;
	const auto keysData = static_cast<Input::Keyboard::Keys>(wParam);

	wchar_t _chars[32];
	wchar_t _actualChar = '\0';
	auto getChars = ::SGE_GetKeyboardState(lParam, wParam, _chars);
	if (getChars > 0)
	{
		_actualChar = _chars[0];
	}

	for (auto i : this->keydown_handlers)
	{
		//Todo: inside or outside the loop? 
		Input::Keyboard::KeyDownEventArgs keyDownEventArgs;
		keyDownEventArgs.KeyCode = keysData;
		keyDownEventArgs.LShift = isLShiftPressed;
		keyDownEventArgs.RShift = isRShiftPressed;
		keyDownEventArgs.LControl = isLControlPressed;
		keyDownEventArgs.RControl = isRControlPressed;
		keyDownEventArgs.Alt = isAltPressed;
		keyDownEventArgs.PressedChar = _actualChar;

		i(this, keyDownEventArgs);
	}
}
