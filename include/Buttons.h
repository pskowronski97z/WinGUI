#pragma once
#include <string>
#include <Control.h>
#include <vector>


namespace WinGUI {

	class Window;
	std::wstring string_to_wstring(std::string source);
	
	class Button : public Control {
		friend class Window;
	protected:
		bool state_;
		bool is_focus_set_;	
		Button(const Window &parent, const int &x, const int &y, std::string name);
		virtual ~Button() = default;
		Button(const Button &button) = default;
		virtual void on_click() = 0;
		virtual void on_focus_set();
		virtual void on_focus_lost();
	public:
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
		bool is_checked() const;	
	};

	class RadioButton : public Control {
	public:
		RadioButton(const Window &parent, std::string name, const int &x, const int &y, bool new_group);
		bool is_checked() const;
	};

	class TabsContainer : public Control {
		friend class Window;
	private:
		int width_;
		int height_;
		int container_x_;
		int container_y_;
		int container_width_;
		int container_height_;
		std::vector<Window*> tabs_;
		void show_distinct(int index) const;
	
	public:
		TabsContainer(const Window &parent, const int &x, const int &y, const int &width, const int &height);
		Window *add_tab(std::string name);
		int get_container_x() const;
		int get_container_y() const;
		int get_container_width() const;
		int get_container_height() const;
		
	};
}
