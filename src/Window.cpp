#include <Window.h>
#include <Context.h>
#include <iostream>

std::wstring WinGui::string_to_wstring(std::string source) {
	std::wstring result;

	for (auto &ch : source)
		result += (wchar_t)ch;
	return result;
}


LRESULT CALLBACK WinGui::Window::wnd_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) {
	return DefWindowProc(wnd_handle, msg, w_param, l_param);
}

WinGui::Window::Window(std::string title, int width, int height) : title_(title), width_(width), height_(height) {

	instance_ = GetModuleHandle(nullptr);

	wnd_class_.lpszClassName = default_wnd_class;
	wnd_class_.lpfnWndProc = wnd_proc;
	wnd_class_.hInstance = instance_;
	wnd_class_.hCursor = nullptr;
	wnd_class_.hIcon = nullptr;
	wnd_class_.hbrBackground = (HBRUSH)(COLOR_ACTIVEBORDER + 1);
	wnd_class_.lpszMenuName = nullptr;
	wnd_class_.style = 0;
	wnd_class_.cbClsExtra = 0;
	wnd_class_.cbWndExtra = 0;

	RegisterClass(&wnd_class_);

	wnd_style_ =  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	std::wstring w_title = string_to_wstring(title_);

	wnd_handle_ = CreateWindowExW(
		WS_EX_WINDOWEDGE,
		default_wnd_class,
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

HWND WinGui::Window::get_handle() const {
	return wnd_handle_;
}

HINSTANCE WinGui::Window::get_instance() const {
	return instance_;
}

int WinGui::Window::get_width() const {
	return width_;
}

int WinGui::Window::get_height() const {
	return height_;
}

bool WinGui::Window::set_icon(std::string file_path) const {
	std::wstring file_path_w = string_to_wstring(file_path);
	HANDLE icon = LoadImage(
		nullptr,
		file_path_w.c_str(),
		IMAGE_ICON,
		LR_DEFAULTSIZE, 
		LR_DEFAULTSIZE,
		LR_DEFAULTCOLOR | LR_LOADFROMFILE);

	if(icon == nullptr)
		return false;

	SendMessage(wnd_handle_,WM_SETICON,ICON_BIG, (LPARAM)icon);
	return true;
}

void WinGui::Window::show_window() const {
	ShowWindow(wnd_handle_, SW_SHOW);
}

void WinGui::Window::hide_window() const {
	ShowWindow(wnd_handle_, SW_HIDE);
}

void WinGui::Window::set_cursor (Cursor cursor_type) const {
	HCURSOR cursor = LoadCursor(nullptr, MAKEINTRESOURCE(cursor_type));
	SetClassLongPtr(wnd_handle_,GCLP_HCURSOR, (LONG_PTR)cursor);
}

void WinGui::Window::set_style(Style wnd_style) {
	wnd_style_ |= (DWORD)wnd_style;
	SetWindowLongPtr(wnd_handle_,GWL_STYLE, (LONG_PTR)wnd_style_);
}
