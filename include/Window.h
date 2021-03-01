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
		H_SCROLL = WS_HSCROLL,
		V_SCROLL = WS_VSCROLL
	};
	
	// Functions >
	std::wstring string_to_wstring(std::string source);
	std::string wchar_to_string(const wchar_t *text, const int &length);
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
		Window(std::string title, const int &width, const int &height);
		Window(Window &parent, const int &x, const int &y, const int &width, const int &height) noexcept;
		Window(const HWND &parent_handle, const HINSTANCE &instance, const int &x, const int &y, const int &width, const int &height);
		void show_window() const noexcept;
		void hide_window() const noexcept;
		HWND get_handle() const noexcept;
		HINSTANCE get_instance() const noexcept;
		int get_width() const noexcept;
		int get_height() const noexcept;
		bool set_icon(std::string file_path) const;
		void set_cursor(Cursor cursor_type) const noexcept;
		void set_style(Style wnd_style) noexcept;
		bool set_menu(unsigned int resource_id) const noexcept;
	};

	
	
	class IO {
		friend class Window;
	private:
		static HWND window_with_focus_;
		static POINT mouse_pos_;
		static bool left_button_down_;
		static bool middle_button_down_;
		static bool right_button_down_;
	
	public:
		static int get_mouse_x();
		static int get_mouse_y();
		static bool mouse_l_button_down();
		static bool mouse_r_button_down();
		static bool mouse_m_button_down();
	};
	
}

