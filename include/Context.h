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
#define BUTTON 0x1000
#define CHECK_BOX 0x2000
#define RICH_TEXT_INPUT 0x3000
#define INT_INPUT 0x4000
#define FLOAT_INPUT 0x5000
#define TAB_CONTROL 0x6000
#define TREE_VIEW 0x7000
#define TABS_CONTAINER 0x8000
#define COMBO_BOX 0x9000
#define LIST_BOX 0xA000
#define TREE_VIEW 0xB000
#define DEFAULT 0xFF00


namespace WinGUI {

	class Window;
	class ClickButton;
	class CheckBox;
	template<typename T>
	class Input;
	class TabsContainer;
	class TreeView;
	
	class Context final {

	private:
		static std::vector<ClickButton*> btn_pointers_;
		static std::vector<CheckBox*> cb_pointers_;
		static std::vector<Input<std::string>*> rt_pointers_;
		static std::vector<Input<float>*> fp_in_pointers_;
		static std::vector<Input<int>*> int_in_pointers_;
		static std::vector<TabsContainer*> tab_container_pointers;
		static std::vector<TreeView*> tv_pointers;
		static unsigned short rb_counter_;
	
	public:
		static bool register_gui_object(ClickButton *btn_pointer);
		static bool register_gui_object(CheckBox *cb_pointer);
		static bool register_gui_object(Input<std::string> *rt_pointer);
		static bool register_gui_object(Input<float> *fp_in_pointer);
		static bool register_gui_object(Input<int> *int_in_pointer);
		static bool register_gui_object(TabsContainer *tab_container_pointer);
		static bool register_gui_object(TreeView *tv_pointer);
		
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
		static TreeView *get_tv_pointer(HWND handle);
		
	};
}
