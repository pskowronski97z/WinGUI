#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <vector>
#include <Windows.h>

#define FONT_HEIGHT 16
#define RB_GROUPS_LIMIT 0xc000
#define BUTTON 0xC100
#define CHECK_BOX 0xC200
#define RICH_TEXT_INPUT 0xC300
#define INT_INPUT 0xC400
#define FLOAT_INPUT 0xC500
#define TAB_CONTROL 0xC600
#define TREE_VIEW 0xC700
#define DEFAULT 0xFF00


namespace WinGui {

	class Window;
	class ClickButton;
	class CheckBox;
	
	class Context final {

	private:
		static std::vector<Window*> wnd_pointers_;
		static std::vector<ClickButton*> btn_pointers_;
		static std::vector<CheckBox*> cb_pointers_;

	public:
		static bool register_gui_object(Window *wnd_pointer);
		static bool register_gui_object(ClickButton *btn_pointer);
		static bool register_gui_object(CheckBox *cb_pointer);
		static int get_btn_buffer_size();
		static int get_cb_buffer_size();
		static ClickButton* get_btn_pointer(int index);
		static ClickButton* get_btn_pointer(HWND handle);
		static CheckBox* get_cb_pointer(int index);
		static CheckBox* get_cb_pointer(HWND handle);
	};
}
