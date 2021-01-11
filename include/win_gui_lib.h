#pragma once
#include <string>
#include <Windows.h>

namespace WinGui {

	// Constants >
	constexpr auto _default_form_class = L"default_form_class";
	// < Constants

	// Functions >
	std::wstring string_to_wstring(std::string source);
	// < Functions

	// TODO: Zabezpieczenia + wyj¹tki
	class Form {
	private:
		std::string _title;
		HWND _wnd_handle;
		WNDCLASS _wnd_class;
		HINSTANCE _instance;
		DWORD _wnd_style;
		int _width;
		int _height;

		static LRESULT CALLBACK form_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param);
	
	public:
		Form(std::string title, int width, int height);
		void show_form();
		void hide_form();
		HWND get_handle();
		HINSTANCE get_instance();
		int get_width();
		int get_height();
		
	};



	
}

