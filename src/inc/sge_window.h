// ReSharper disable CppClangTidyClangDiagnosticMicrosoftInclude
#pragma once
#include <vector>
#include <Windows.h>


#include "inc/sge_macros.h"

#include "inc/sge_input.h"
#include "inc/sge_string.h"
#include "inc/sge_macros.h"

namespace SGE::Windows
{
	struct Size
	{
		int Width;
		int Height;

		Size() : Width(0), Height(0) { }
		Size(int width, int height)
		{
			Width = width;
			Height = height;
		}
	};

	struct Point
	{
		int X;
		int Y;
	};

	typedef void (*SGEKeyDownHandler)(WindowBase* const, const SGE::Input::Keyboard::KeyDownEventArgs&);

	
	class WindowBase
	{
	public:
		WindowBase();
		WindowBase(const String& title);
		WindowBase(const String& title, Size size);
		
		void EnsureHandle();
		
		void Show();
		void Hide() const;

		void OnKeyDown(SGEKeyDownHandler handler);
		void OnChar(SGEKeyDownHandler handler);

		[[nodiscard]] HWND GetHandle() const;

		virtual ~WindowBase();

	protected:
		void Register();
		virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM, LPARAM);

	private:
		void HandleKeyDownOrChar(WPARAM wParam, LPARAM lParam);

		
		Size _size;
		String _title;
		HWND _win32Window = nullptr;
		std::vector<SGEKeyDownHandler> keydown_handlers;
		std::vector<SGEKeyDownHandler> char_handlers;
		friend class Application;
		
	};

	class Application
	{
	public:
		static void Run(WindowBase& window)
		{
			window.EnsureHandle();
			MSG msg = {};
			while (GetMessage(&msg, window.GetHandle(), 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		static void Run()
		{
			MSG msg = {};
			while (GetMessage(&msg, nullptr, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		static void RegisterWindow(WindowBase* hwnd)
		{
			_windows.push_back(hwnd);
		}

		static void RemoveWindow(WindowBase* hwnd) noexcept
		{
			RemoveFromVector(Application::_windows, hwnd);
		}

		static LRESULT DispatchWindowMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			for(auto* i : _windows)
			{
				if (i->GetHandle() == hwnd)
				{
					return i->WndProc(hwnd, uMsg, wParam, lParam);
				}
			}
			return  DefWindowProcW(hwnd, uMsg, wParam, lParam);
		}	
		
	private:
		static std::vector<WindowBase*> _windows;
	};

}
