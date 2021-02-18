#pragma once
#include <string>
#include <Windows.h>

namespace WinGUI {

	class Window;
	
	class Control {
	protected:
		HWND handle_;
		HWND parent_handle_;
		DWORD id_;
		int x_;
		int y_;
		std::string name_;
		Control(const Window &parent, const int &x, const int &y, std::string name);

	public:
		int get_x_pos() const;
		int get_y_pos() const;
		std::string get_name() const;
		HWND get_handle() const;
		DWORD get_id() const;
	};
}
