#pragma once
#include <Control.h>
#define FONT_HEIGHT 16
#define STATIC_CTRL 0xFF01

namespace WinGUI {

	//Defined in Window.cpp
	std::wstring string_to_wstring(std::string source);
	
	class Label : public Control {
	private:
		int width_;
	public:
		Label(const Window &parent, std::string name, const int &x, const int &y);
		void set_text(std::string name);
		int get_width() const noexcept;
		
	};

	class ProgressBar : public Control {
	private:
		int width_;
		int height_;
		unsigned short progress_;
	public:
		ProgressBar(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height) noexcept;
		int get_width() const noexcept; 
		int get_height() const noexcept;
		float get_progress() const noexcept;
		bool set_progress(unsigned short progress) noexcept;
	};

	class GroupBox : public Control {
	private:
		int width_;
		int height_;
	public:
		GroupBox(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height);
		int get_width() const noexcept;
		int get_height() const noexcept;
	};
}
