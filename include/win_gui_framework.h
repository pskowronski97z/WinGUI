#pragma once
#include <vector>
#include <Windows.h>

namespace Win_GUI {

	class Window {
	private:
		std::vector<HWND> button_handles;
		std::vector<HWND> check_box_handles;
		std::vector<HWND> text_field_handles;
		HWND wnd_handle = nullptr;
		WNDCLASS wnd_class = {};

	public:
		Window(std::string title_label, int width, int height, bool is_size_fixed);

		// TODO: Documentation
		bool add_button(bool *control, std::string button_label, int x, int y, int width, int height);

		// TODO: Documentation
		bool add_group_box(std::string group_box_label, int x, int y, int width, int height) const;

		// TODO: Documentation
		bool add_check_box(bool *control, std::string check_box_label, int x, int y);

		bool add_pane(int x, int y, int width, int height) const;

		bool add_rich_text_input(std::string *buffer, std::string name, int x, int y, int width, int height,  bool v_scroll, bool h_scroll, bool is_password);

		bool add_int_input(int *control, std::string name, int x, int y, int width, int min, int max);

		bool add_float_input(float *control, std::string name, int x, int y, int width);

		bool add_label(std::string label_text, int x, int y);
		
		// TODO: Secure from multiple calls
		HWND show_window() const;

		// TODO: Secure from multiple calls
		void show_window_async() const;
	};
	
}
