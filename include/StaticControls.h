#pragma once
#include <Control.h>
#define FONT_HEIGHT 16

namespace WinGui {

	//Defined in Window.cpp
	std::wstring string_to_wstring(std::string source);
	
	class Label : public Control {
	private:
		int width_;
	public:
		Label(const Window &parent, const int &x, const int &y, std::string name);
		void set_name(std::string name);
		int get_width() const;
		
	};

	class ProgressBar : public Control {
	private:
		int width_;
		int height_;
		unsigned short progress_;
	public:
		ProgressBar(const Window &parent, const int &x, const int &y, const int &width, const int &height, std::string name);
		int get_width() const;
		int get_height() const;
		float get_progress() const;
		bool set_progress(unsigned short progress);
	};
	
}
