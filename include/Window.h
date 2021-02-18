#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

namespace WinGUI {

	enum class Cursor {
		ARROW = 32512,
		ARROW_WAIT = 32650,
		CROSS = 32515,
		HAND = 32649,
		HELP = 32651,
		NO = 32648,
		WAIT = 32514,
		SIZE_WIDTH = 32644,
		SIZE_HEIGHT = 32645,
		SIZE_TO_LEFT = 32642,
		SIZE_TO_RIGHT = 32643,
		SIZE_ALL = 32646
	};

	enum class Style {
		MAXIMIZE = WS_MAXIMIZEBOX,
		MINIMIZE = WS_MINIMIZEBOX,
		SIZEABLE = WS_SIZEBOX,
		HSCROLL = WS_HSCROLL,
		VSCROLL = WS_VSCROLL
	};
	
	// Functions >
	/*std::wstring string_to_wstring(std::string source);
	std::string wchar_to_string(const wchar_t *text, const int &length);*/
	// < Functions

	// TODO: Zabezpieczenia + wyj¹tki
	class Window {
	private:
		std::string title_;
		HWND wnd_handle_;
		HWND parent_wnd_handle_;
		WNDCLASS wnd_class_;
		HINSTANCE instance_;
		DWORD wnd_style_;
		int width_;
		int height_;

		static LRESULT CALLBACK wnd_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param);
	
	public:
		Window(std::string title, int width, int height);
		Window(Window &parent, int x, int y, int width, int height);
		Window(HWND parent_handle, HINSTANCE instance, int x, int y, int width, int height);
		void show_window() const;
		void hide_window() const;
		HWND get_handle() const;
		HINSTANCE get_instance() const;
		int get_width() const;
		int get_height() const;
		bool set_icon(std::string file_path) const;
		void set_cursor(Cursor cursor_type) const;
		void set_style(Style wnd_style);
		
	};

}

