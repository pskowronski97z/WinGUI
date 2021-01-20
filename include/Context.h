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
	class Button;
	
	class Context final {

	private:
		static std::vector<Window*> wnd_pointers_;
		static std::vector<Button*> btn_pointers_;

	public:
		static bool register_gui_object(Window *wnd_pointer);
		static bool register_gui_object(Button *btn_pointer);
		static int get_btn_buffer_size();
		static Button* get_btn_pointer(int index);
		static Button* get_btn_pointer(HWND handle);
	};
}
