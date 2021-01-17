#pragma once

#include <string>
#include <Windows.h>
#include <Window.h>

namespace WinGui {

	class Button {
	private:
		HWND btn_handle_;
		HWND parent_;
		DWORD id_;
		int x_;
		int y_;
		int width_;
		int height_;
		std::string btn_label_;

	public:
		Button(Window parent, std::string btn_label, int x, int y, int width, int height);

	};
}
