#include "win_gui_lib.h"

#include <vcruntime_typeinfo.h>

LRESULT CALLBACK WinGui::Form::form_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) {
	return DefWindowProc(wnd_handle, msg, w_param, l_param);
}

std::wstring WinGui::string_to_wstring(std::string source) {
	std::wstring result;

	for (auto &ch : source)
		result += (wchar_t)ch;
	return result;
}


WinGui::Form::Form(std::string title, int width, int height) : _title(title), _width(width), _height(height) {

	_instance = GetModuleHandle(nullptr);

	_wnd_class.lpszClassName = _default_form_class;
	_wnd_class.lpfnWndProc = form_proc;
	_wnd_class.hInstance = _instance;
	_wnd_class.hCursor = nullptr;
	_wnd_class.hIcon = nullptr;
	_wnd_class.hbrBackground = (HBRUSH)(COLOR_ACTIVEBORDER + 1);
	_wnd_class.lpszMenuName = nullptr;
	_wnd_class.style = 0;
	_wnd_class.cbClsExtra = 0;
	_wnd_class.cbWndExtra = 0;

	RegisterClass(&_wnd_class);

	_wnd_style =  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;

	std::wstring w_title = string_to_wstring(_title);

	_wnd_handle = CreateWindowExW(
		WS_EX_WINDOWEDGE,
		_default_form_class,
		w_title.c_str(),
		_wnd_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		abs(_width),
		abs(_height),
		nullptr,
		nullptr,
		_instance,
		0);
	
}

HWND WinGui::Form::get_handle() {
	return _wnd_handle;
}

HINSTANCE WinGui::Form::get_instance() {
	return _instance;
}

int WinGui::Form::get_width() {
	return _width;
}

int WinGui::Form::get_height() {
	return _height;
}

void WinGui::Form::show_form() {
	ShowWindow(_wnd_handle, SW_SHOW);
}

void WinGui::Form::hide_form() {
	ShowWindow(_wnd_handle, SW_HIDE);
}

