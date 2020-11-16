#include <iostream>
#include <Windows.h>


#include "inc/sge_engine.h"
#include "inc/sge_window.h"
#include "inc/sge_string.h"

void OnSimpleKeyDown(SGE::Windows::WindowBase* win, 
	const SGE::Input::Keyboard::KeyDownEventArgs& args)
{
	auto data = SGE::String(L"======\nCtrl: ") + args.Control();
	data += SGE::String("\nShift: ") + args.Shift();
	data += SGE::String("\nAlt:") + args.Alt;
	if(args.PressedChar != 0)
	{
		data += SGE::String("\nChar: ") + std::wstring(1,args.PressedChar);
	}

	data += SGE::String("\n=====");

	::OutputDebugStringW(data.AsStdWString().c_str());
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nShowCmd)
{

	
	SGE::String title = "Hello World";
	auto newTitle = title + " Andrej" + "\n You are pretty cool \n";
	newTitle = newTitle * 2;

	newTitle = newTitle + 12000;

	newTitle += "\n And this is how it ends";
	
	SGE::Debug::WriteLine(newTitle.AsStdWString().c_str());
	SGE::Debug::WriteLine(newTitle.AsStdString().c_str());

	SGE::Windows::WindowBase w("Sparkplug engine window");
	w.Show();
	w.OnKeyDown(reinterpret_cast<SGE::Windows::SGEKeyDownHandler>(OnSimpleKeyDown));

	try
	{
		SGE_CHECKED_MSG(E_NOINTERFACE, L"Smells like error")
	}
	catch(SGE::Exceptions::SGEComException& c)
	{
		SGE::Debug::WriteLine(c.Message().AsStdWString().c_str());
		SGE::Debug::WriteLine(SGE::String::FromHResult(c.Error()));

		SGE::Engine::SGEApp engine(w);
		engine.Initialize();
	}
	SGE::Windows::Application::Run();
	return EXIT_SUCCESS;
}
