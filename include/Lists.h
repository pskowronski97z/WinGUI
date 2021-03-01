#pragma once
#include <Control.h>
#include <vector>
#include <CommCtrl.h>

namespace WinGUI {

	// Defined in Window.cpp
	std::wstring string_to_wstring(std::string source);
	
	class List : public Control {
	public:
		virtual bool add_item(std::string item) const = 0;
		virtual bool remove_item(int index) const = 0;
		virtual int get_selected_index() const = 0;
		virtual void clear() const = 0;
		virtual ~List() = default;
		List(const List &list) = default;
		List(List &&list) = delete;
		List &operator=(const List &) = delete;
		List &operator=(List &&) = delete;
		List(const Window &parent, std::string name, const int &x, const int &y);
	};

	class ComboBox : public List {
		int width_;
		int list_height_;
	public:
		ComboBox(const Window &parent, std::string name, const int &x, const int &y, const int &width,  const int &list_height) noexcept;
		bool add_item(std::string item) const override;
		bool remove_item(int index) const noexcept override ;
		int get_selected_index() const noexcept override;
		void clear() const noexcept override;	
	};

	class ListBox : public List {
		int width_;
		int height_;
	public:
		ListBox(const Window &parent, std::string name, const int &x, const int &y, const int &width,  const int &height, bool multiple_selection) noexcept;
		bool add_item(std::string item) const override;
		bool remove_item(int index) const noexcept override;
		// For single selection
		int get_selected_index() const noexcept override;
		void clear() const noexcept override;
		// For multiple selection
		std::vector<int> get_selected_indexes() const;
	};

	class TreeView : public Control {
	private:
		int width_;
		int height_;
		std::vector<TVITEM> items_;
		int selected_item_;
	public:
		TreeView(const Window &parent,std::string name, const int &x, const int &y, const int &width,  const int &height) noexcept;
		bool add_item(std::string item_name, int parent_index);
		bool remove_item(int index);
		bool clear();
		int set_selected_item(HTREEITEM item_handle);
		int get_selected_index() const noexcept;
	};
}
