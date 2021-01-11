#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <CommCtrl.h>

namespace Win_GUI {
	class Tab;
	class Tree_View;
	
	class List {
	protected:
		HWND handle = nullptr;
	public:
		virtual bool add_item(std::string item) const = 0;
		virtual bool remove_item(int index) const = 0;
		virtual int get_selected_index() const = 0;
		virtual void clear() const = 0;
		virtual ~List() = default;
		List() = default;
		List(const List &list) = default;
		List(List &&list) = delete;
		List& operator=(const List&) = delete;
		List& operator=(List&&) = delete;
	};
	
	class List_Box final : public List {
	public:
		List_Box(int x, int y, int width, int height, HWND parent, bool multiple_selection);
		bool add_item(std::string item) const override;
		bool remove_item(int index) const override;
		int get_selected_index() const override;
		void clear() const override;
		std::vector<int> get_selected_indexes() const;
	};

	class Combo_Box final : public List {
	public:
		Combo_Box(int x, int y, int width, HWND parent);
		bool add_item(std::string item) const override;
		bool remove_item(int index) const override;
		int get_selected_index() const override;
		void clear() const override;	
	};

	class Window {
	private:
		std::vector<HWND> button_handles;
		std::vector<HWND> check_box_handles;
		std::vector<HWND> text_field_handles;
		HWND wnd_handle = nullptr;
		WNDCLASS wnd_class = {};

	public:
		/// <summary>
		/// Overlapped system window constructor
		/// </summary>
		/// <param name="title_label"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="is_size_fixed"></param>
		Window(std::string title_label, int width, int height, bool is_size_fixed);

		/// <summary>
		/// Content container constructor
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="parent"></param>
		Window(int x, int y, int width, int height, HWND parent);
		
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

		List_Box add_list_box(std::string name, int x, int y, int width, int height, bool multiple_selection);

		Combo_Box add_combo_box(std::string name, int x, int y, int width);

		bool add_radio_button(int *control, std::string rb_label, int x, int y, bool is_first);

		Tab& add_tab(int x, int y, int width, int height);

		Window add_content_container(int x, int y, int width, int height);

		bool add_progress_bar(int x, int y, int width, int height);
		
		// TODO: Secure from multiple calls
		void show_window() const;

		void hide_window();
		
		// TODO: Secure from multiple calls
		void show_window_async() const;

		Tree_View& add_tree_view(int x, int y, int width, int height);
	};

	class Tab {
	private:
		HWND parent;
		HWND handle;
		int x;
		int y;
		int width;
		int height;
		std::vector<Window> contents;
	
	public:
		Tab(int x, int y, int width, int height, HWND parent);
		bool add_content(std::string tab_name, Window content);
		bool show_distinct(int index);
		HWND get_handle() const;
	};

	class Tree_View {
	private:
		HWND parent_wnd_handle;
		HWND tv_handle;
		std::string name;
		int x;
		int y;
		int width;
		int height;
		std::vector<TVITEM> items;
		int selected_item;
		
	public:
		Tree_View(std::string name, HWND parent, int x, int y, int width, int height);
		bool add_item(std::string item_name, int parent_index);
		bool remove_item(int index);
		bool clear();
		int set_selected_item(HTREEITEM item_handle);
		int get_selected_item();
		HWND get_handle();
	};
	
}
