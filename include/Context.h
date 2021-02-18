#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <string>
#include <vector>
#include <Windows.h>

#define FONT_HEIGHT 16
#define TEXT_INPUT_HEIGHT 19
#define RADIO_BUTTON 0x0000
#define BUTTON 0x0100
#define CHECK_BOX 0x0200
#define RICH_TEXT_INPUT 0x0300
#define INT_INPUT 0x0400
#define FLOAT_INPUT 0x0500
#define TAB_CONTROL 0x0600
#define TREE_VIEW 0x0700
#define TABS_CONTAINER 0x0800
#define COMBO_BOX 0x0900
#define LIST_BOX 0x0A00
#define DEFAULT 0xFF00


namespace WinGUI {

	class Window;
	class ClickButton;
	class CheckBox;
	template<typename T>
	class Input;
	class TabsContainer;
	
	class Context final {

	private:
		static std::vector<Window*> wnd_pointers_;
		static std::vector<ClickButton*> btn_pointers_;
		static std::vector<CheckBox*> cb_pointers_;
		static std::vector<Input<std::string>*> rt_pointers_;
		static std::vector<Input<float>*> fp_in_pointers_;
		static std::vector<Input<int>*> int_in_pointers_;
		static std::vector<TabsContainer*> tab_container_pointers;
		static byte rb_counter_;
	
	public:
		static bool register_gui_object(Window *wnd_pointer);
		static bool register_gui_object(ClickButton *btn_pointer);
		static bool register_gui_object(CheckBox *cb_pointer);
		static bool register_gui_object(Input<std::string> *rt_pointer);
		static bool register_gui_object(Input<float> *fp_in_pointer);
		static bool register_gui_object(Input<int> *int_in_pointer);
		static bool register_gui_object(TabsContainer *tab_container_pointer);
		
		static int get_btn_buffer_size();
		static int get_cb_buffer_size();
		static int get_rt_buffer_size();
		static int get_fp_in_buffer_size();
		static int get_int_in_buffer_size();
		static int get_tab_cont_buffer_size();
		
		static unsigned short get_new_rb_id();

		static ClickButton *get_btn_pointer(int index);
		static ClickButton *get_btn_pointer(HWND handle);
		static CheckBox *get_cb_pointer(int index);
		static CheckBox *get_cb_pointer(HWND handle);
		static Input<std::string> *get_rt_pointer(int index);
		static Input<float> *get_fp_in_pointer(int index);
		static Input<int> *get_int_in_pointer(int index);
		static TabsContainer *get_tab_cont_pointer(HWND handle);
		
	};
}
