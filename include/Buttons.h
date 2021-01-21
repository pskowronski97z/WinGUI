#pragma once
#include <string>
#include <Windows.h>
#include <Window.h>

namespace WinGui {

	class Button {
		friend class Window;
	protected:
		HWND handle_;
		HWND parent_handle_;
		DWORD id_;
		int x_;
		int y_;
		std::string name_;
		bool state_;
		bool is_focus_set_;
		Button(const Window &parent, const int &x, const int &y, std::string name);
		virtual ~Button() = default;
		Button(const Button &button) = default;
		virtual void on_click() = 0;
		virtual void on_focus_set();
		virtual void on_focus_lost();
	
	public:
		int get_x_pos() const;
		int get_y_pos() const;
		std::string get_name() const;
		HWND get_handle() const;
		DWORD get_id() const;
		Button(Button &&button) = delete;
		Button& operator=(const Button&) = delete;
		Button& operator=(Button&&) = delete;
	};
	
	class ClickButton : public Button {

	protected:
		int width_;
		int height_;
		void on_click() override;
	
	public:
		ClickButton(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height);	
		bool was_clicked();
		bool is_focused() const;
	};

	class CheckBox : public Button {
	protected:
		void on_click() override;
	public:
		CheckBox(const Window &parent, std::string name, const int &x, const int &y);
		
	};
}
