#pragma once

#include <string>
#include <Windows.h>
#include <Window.h>

namespace WinGui {

	class Button {
	protected:
		HWND btn_handle_;
		HWND parent_;
		DWORD id_;
		int x_;
		int y_;
		int width_;
		int height_;
		std::string btn_label_;
		bool is_focus_set_;
		bool was_clicked_;
		bool was_dbclicked_;

	public:
		Button(Window parent, std::string btn_label, int x, int y, int width, int height);
		HWND get_btn_handle() const;
		virtual void on_click();
		virtual void on_double_click();
		virtual void on_focus_set();
		virtual void on_focus_lost();
		bool was_clicked();
		bool was_dbclicked();
		bool is_focused() const;
		std::string get_name() const;
	};
}
