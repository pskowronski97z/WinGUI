#pragma once
#include <Control.h>
#include <StaticControls.h>

namespace WinGui {

	// Allowed types: string, float, int
	template <typename T> class Input;
	// Defined in Window.cpp
	std::string wchar_to_string(const wchar_t *text, const int &length);
	
	template<>
	class Input<std::string> : public Control {
		friend class Window;
	private:
		int width_;
		int height_;
		std::string text_;
		void on_text_entered(LPARAM l_param);
	
	public:
		Input(const Window &parent, const std::string &name, const int &x, const int &y, const int &width,
		      const int &height, const bool &v_scroll, const bool &h_scroll, const bool &is_password);
		std::string get_text() const;
	};

	template<>
	class Input<float> : public Control {
		friend class Window;
	private:
		int width_;
		float value_;
		static WNDPROC std_edit_proc_;
		void on_value_entered(LPARAM l_param);
		static const wchar_t *format_float_input(const wchar_t *source);
		static LRESULT CALLBACK float_input_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
	public:
		Input(const Window &parent, const std::string &name, const int &x, const int &y, const int &width);
		float get_value() const;
	};

	template<>
	class Input<int> : public Control {
		friend class Window;
	private:
		int width_;
		int value_;
		static WNDPROC std_edit_proc_;
		void on_value_entered(LPARAM l_param);
		static LRESULT CALLBACK int_input_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);
		static const wchar_t *format_int_input(const wchar_t *source);
	public:
		Input(const Window &parent, const std::string &name, const int &x, const int &y, const int &width, const int &min, const int &max);
		float get_value() const;
		
	};
}
