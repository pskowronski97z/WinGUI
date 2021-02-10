#include <Window.h>
#include <Context.h>
#include <iostream>
#include <Buttons.h>
#include <Inputs.h>

#define DEFAULT_WND_CLASS L"default_wnd_class"

std::wstring WinGui::string_to_wstring(std::string source) {
	std::wstring result;

	for (auto &ch : source)
		result += (wchar_t)ch;
	return result;
}

std::string WinGui::wchar_to_string(const wchar_t *text, const int &length) {

	std::string result;
	char char_buffer;

	for (int i = 0; i < length; i++) {
		char_buffer = (char)text[i];
		result += char_buffer;
	}

	return result;
}

LRESULT CALLBACK WinGui::Window::wnd_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) {

	Button *btn_pointer = nullptr;
	Input<std::string> *rt_pointer = nullptr;
	Input<float> *fp_in_pointer = nullptr;
	unsigned short ctrl_class_id;
	switch (msg) {
	case WM_COMMAND:

		ctrl_class_id = (HIBYTE(w_param) << 8);
		if (ctrl_class_id == BUTTON)
			btn_pointer = Context::get_btn_pointer(LOBYTE(w_param));
		else if (ctrl_class_id == CHECK_BOX)
			btn_pointer = Context::get_cb_pointer(LOBYTE(w_param));

		if (btn_pointer != nullptr) {
			switch (HIWORD(w_param)) {
			case BN_CLICKED:
				btn_pointer->on_click();
				break;

			case BN_DBLCLK:
				btn_pointer->on_click();
				break;

			case BN_KILLFOCUS:
				btn_pointer->on_focus_lost();
				break;

			case BN_SETFOCUS:
				btn_pointer->on_focus_set();
				break;

			default: break;
			}
			break;
		}

		switch (ctrl_class_id) {
		case INT_INPUT:

			break;

		case FLOAT_INPUT:
			if (HIWORD(w_param) == WM_MOUSEMOVE) {
				fp_in_pointer = Context::get_fp_in_pointer(LOBYTE(w_param));
				fp_in_pointer->on_value_entered(l_param);
			}
			break;

		case RICH_TEXT_INPUT:
			if (HIWORD(w_param) == WM_MOUSEMOVE) {
				rt_pointer = Context::get_rt_pointer(LOBYTE(w_param));
				rt_pointer->on_text_entered(l_param);
			}
			break;
		}
	case WM_NOTIFY:

		break;

	case WM_CLOSE:
		PostQuitMessage(404);
		break;

	default: break;
	}

	return DefWindowProc(wnd_handle, msg, w_param, l_param);
}

WinGui::Window::Window(std::string title, int width, int height) : title_(title), width_(width), height_(height) {

	instance_ = GetModuleHandle(nullptr);

	wnd_class_.lpszClassName = DEFAULT_WND_CLASS;
	wnd_class_.lpfnWndProc = wnd_proc;
	wnd_class_.hInstance = instance_;
	wnd_class_.hCursor = nullptr;
	wnd_class_.hIcon = nullptr;
	wnd_class_.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wnd_class_.lpszMenuName = nullptr;
	wnd_class_.style = 0;
	wnd_class_.cbClsExtra = 0;
	wnd_class_.cbWndExtra = 0;

	RegisterClass(&wnd_class_);

	wnd_style_ = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	std::wstring w_title = string_to_wstring(title_);

	wnd_handle_ = CreateWindowExW(
		WS_EX_WINDOWEDGE,
		DEFAULT_WND_CLASS,
		w_title.c_str(),
		wnd_style_,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		abs(width_),
		abs(height_),
		nullptr,
		nullptr,
		instance_,
		0);

	Context::register_gui_object(this);
}

HWND WinGui::Window::get_handle() const { return wnd_handle_; }

HINSTANCE WinGui::Window::get_instance() const { return instance_; }

int WinGui::Window::get_width() const { return width_; }

int WinGui::Window::get_height() const { return height_; }

bool WinGui::Window::set_icon(std::string file_path) const {
	std::wstring file_path_w = string_to_wstring(file_path);
	HANDLE icon = LoadImage(
		nullptr,
		file_path_w.c_str(),
		IMAGE_ICON,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_DEFAULTCOLOR | LR_LOADFROMFILE);

	if (icon == nullptr)
		return false;

	SendMessage(wnd_handle_,WM_SETICON,ICON_BIG, (LPARAM)icon);
	return true;
}

void WinGui::Window::show_window() const { ShowWindow(wnd_handle_, SW_SHOW); }

void WinGui::Window::hide_window() const { ShowWindow(wnd_handle_, SW_HIDE); }

void WinGui::Window::set_cursor(Cursor cursor_type) const {
	HCURSOR cursor = LoadCursor(nullptr, MAKEINTRESOURCE(cursor_type));
	SetClassLongPtr(wnd_handle_,GCLP_HCURSOR, (LONG_PTR)cursor);
}

void WinGui::Window::set_style(Style wnd_style) {
	wnd_style_ |= (DWORD)wnd_style;
	SetWindowLongPtr(wnd_handle_,GWL_STYLE, (LONG_PTR)wnd_style_);
}
