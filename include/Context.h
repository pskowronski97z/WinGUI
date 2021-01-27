#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <string>
#include <vector>
#include <Windows.h>

#define FONT_HEIGHT 16
#define RADIO_BUTTON 0x0000
#define BUTTON 0x0100
#define CHECK_BOX 0x0200
#define RICH_TEXT_INPUT 0x0300
#define INT_INPUT 0x0400
#define FLOAT_INPUT 0x0500
#define TAB_CONTROL 0x0600
#define TREE_VIEW 0x0700
#define DEFAULT 0xFF00


namespace WinGui {

	class Window;
	class ClickButton;
	class CheckBox;
	
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

	class Context final {

	private:
		static std::vector<Window*> wnd_pointers_;
		static std::vector<ClickButton*> btn_pointers_;
		static std::vector<CheckBox*> cb_pointers_;
		static byte rb_counter_;
	
	public:
		static bool register_gui_object(Window *wnd_pointer);
		static bool register_gui_object(ClickButton *btn_pointer);
		static bool register_gui_object(CheckBox *cb_pointer);
		static int get_btn_buffer_size();
		static int get_cb_buffer_size();
		static unsigned short get_new_rb_id();
		static ClickButton* get_btn_pointer(int index);
		static ClickButton* get_btn_pointer(HWND handle);
		static CheckBox* get_cb_pointer(int index);
		static CheckBox* get_cb_pointer(HWND handle);
	};
}
