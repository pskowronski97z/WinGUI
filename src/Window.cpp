#include <Window.h>
#include <Context.h>
#include <iostream>
#include <Buttons.h>
#include <cassert>
#include <CommCtrl.h>
#include <Inputs.h>
#include <Lists.h>
#include <Exceptions.h>

#define DEFAULT_WND_CLASS L"default_wnd_class"
#define CONTENT_CONTAINER L"content_container"



POINT WinGUI::IO::Mouse::mouse_position_{0, 0};
POINT WinGUI::IO::Mouse::mouse_delta_{0, 0};
HWND WinGUI::IO::Mouse::window_with_focus_ = nullptr;
bool WinGUI::IO::Mouse::left_button_down_ = false;
bool WinGUI::IO::Mouse::right_button_down_ = false;
bool WinGUI::IO::Mouse::middle_button_down_ = false;
int WinGUI::IO::Mouse::wheel_delta_ = 0;



HWND WinGUI::IO::Keyboard::window_with_focus_ = nullptr;
char WinGUI::IO::Keyboard::typed_char_ = 0;
bool WinGUI::IO::Keyboard::keys_down_[255]{false};
bool WinGUI::IO::Keyboard::keys_up_[255]{false};
bool WinGUI::IO::Keyboard::sys_keys_down_[255]{false};
bool WinGUI::IO::Keyboard::sys_keys_up_[255]{false};

std::wstring WinGUI::string_to_wstring(std::string source) {

	std::wstring result;

	for (auto &ch : source)
		result += (wchar_t)ch;
	return result;
}

std::string WinGUI::wchar_to_string(const wchar_t *text, const int &length) {

	std::string result;
	char char_buffer;

	for (int i = 0; i < length; i++) {
		char_buffer = (char)text[i];
		result += char_buffer;
	}

	return result;
}



LRESULT CALLBACK WinGUI::Window::wnd_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param) {

	Button *btn_pointer = nullptr;
	Input<std::string> *rt_pointer = nullptr;
	Input<float> *fp_in_pointer = nullptr;
	Input<int> *int_in_pointer = nullptr;
	TabsContainer *tabs_cont_pointer = nullptr;
	TreeView *tv_pointer = nullptr;
	unsigned short ctrl_class_id = 0;
	unsigned short ctrl_index = 0;
	int index = 0;
	LPNMHDR id = 0;
	LPNMTREEVIEW nmtv = 0;

	IO::Mouse::wheel_delta_ = 0;
	IO::Keyboard::typed_char_ = 0;
	
	switch (msg) {

	case WM_CHAR:
		IO::Keyboard::typed_char_ = (char)w_param;
		break;
	case WM_MOUSEWHEEL:

		if (HIWORD(w_param) == 120)
			IO::Mouse::wheel_delta_ = 1;
		else
			IO::Mouse::wheel_delta_ = -1;

		break;
		
	case WM_MOUSEMOVE:

		SetFocus(wnd_handle);
		IO::Mouse::window_with_focus_ = wnd_handle;
		IO::Keyboard::window_with_focus_ = wnd_handle;
		IO::Mouse::mouse_delta_.x = IO::Mouse::mouse_position_.x - LOWORD(l_param);
		IO::Mouse::mouse_delta_.y = IO::Mouse::mouse_position_.y - HIWORD(l_param);
		IO::Mouse::mouse_position_.x = LOWORD(l_param);
		IO::Mouse::mouse_position_.y = HIWORD(l_param);
		
		break;
		
	case WM_LBUTTONDOWN:
		IO::Mouse::left_button_down_ = true;
		break;

	case WM_LBUTTONUP:
		IO::Mouse::left_button_down_ = false;
		break;

	case WM_RBUTTONDOWN:
		IO::Mouse::right_button_down_ = true;
		break;

	case WM_RBUTTONUP:
		IO::Mouse::right_button_down_ = false;
		break;

	case WM_MBUTTONDOWN:

		IO::Mouse::middle_button_down_=true;
		break;

	case WM_MBUTTONUP:
		IO::Mouse::middle_button_down_=false;
		break;
		
	case WM_KEYDOWN:
		IO::Keyboard::keys_down_[(byte)w_param] = true;
		IO::Keyboard::keys_up_[(byte)w_param] = false;
		break;
		
	case WM_KEYUP:
		IO::Keyboard::keys_down_[(byte)w_param] = false;
		IO::Keyboard::keys_up_[(byte)w_param] = true;
		break;

	case WM_SYSKEYDOWN:
		IO::Keyboard::sys_keys_down_[(byte)w_param] = true;
		IO::Keyboard::sys_keys_up_[(byte)w_param] = false;
		break;

	case WM_SYSKEYUP:
		IO::Keyboard::sys_keys_down_[(byte)w_param] = false;
		IO::Keyboard::sys_keys_up_[(byte)w_param] = true;
		break;
		
	case WM_COMMAND:

		ctrl_class_id = LOWORD(w_param) & 0xF000;
		ctrl_index = LOWORD(w_param) & 0x0FFF;
		
		if (ctrl_class_id == BUTTON)
			btn_pointer = Context::get_btn_pointer(ctrl_index);
		else if (ctrl_class_id == CHECK_BOX)
			btn_pointer = Context::get_cb_pointer(ctrl_index);

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
			if (HIWORD(w_param) == WM_MOUSEMOVE) {
				int_in_pointer = Context::get_int_in_pointer(LOBYTE(w_param));
				int_in_pointer->on_value_entered(l_param);
			}
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
		default: break;
		}
	case WM_NOTIFY:

		id = (LPNMHDR)l_param;
		
		switch(w_param) {
		case TABS_CONTAINER:
			if (id->code != TCN_SELCHANGE)
				break;
			tabs_cont_pointer = Context::get_tab_cont_pointer(id->hwndFrom);
			if (tabs_cont_pointer == nullptr)
				break;
			index = TabCtrl_GetCurSel(tabs_cont_pointer->get_handle());
			tabs_cont_pointer->show_distinct(index);

			break;

		case TREE_VIEW:

			switch (id->code) {
				case TVN_SELCHANGED:

					nmtv = (LPNMTREEVIEW)l_param;
					TVITEM new_it = nmtv->itemNew;
					tv_pointer = Context::get_tv_pointer(id->hwndFrom);

					if(tv_pointer == nullptr)	
						break;

					tv_pointer->set_selected_item(new_it.hItem);
					break;
				
				case TVN_ENDLABELEDIT:
					if (((LPNMTVDISPINFO)l_param)->item.pszText != 0)
						return true;
					return false;
				}

			break;
			
		default: break;
			
		}
		
		break;

	case WM_CLOSE:
		PostQuitMessage(404);
		break;

	default: break;
	}

	return DefWindowProc(wnd_handle, msg, w_param, l_param);
}

WinGUI::Window::Window(std::string title, const int &width, const int &height) 
	: title_(std::move(title)),
	  parent_wnd_handle_(nullptr),
	  wnd_class_(),
	  width_(width),
	  height_(height) {

	if(title_.empty())
		title_ = "Window 1";

	if(width_ < 50)
		width_ = 100;

	if(height_ < 50)
		height_ = 100;

	
	instance_ = GetModuleHandle(nullptr);

	wnd_class_.lpszClassName = DEFAULT_WND_CLASS;
	wnd_class_.lpfnWndProc = wnd_proc;
	wnd_class_.hInstance = instance_;
	wnd_class_.hCursor = nullptr;
	wnd_class_.hIcon = nullptr;
	wnd_class_.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW + 1);
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
		width_,
		height_,
		nullptr,
		nullptr,
		instance_,
		0);

}

WinGUI::Window::Window(Window &parent, const int &x, const int &y, const int &width, const int &height) noexcept
	: parent_wnd_handle_(parent.get_handle()),
	  wnd_class_(),
	  instance_(parent.get_instance()),
	  wnd_style_(WS_CHILD),
	  width_(width),
	  height_(height) {

	if(width_ < 50)
		width_ = 100;

	if(height_ < 50)
		height_ = 100;
	
	wnd_class_.lpszClassName = L"content_container";
	wnd_class_.lpfnWndProc = wnd_proc;
	wnd_class_.hInstance = instance_;
	wnd_class_.hCursor = nullptr;
	wnd_class_.hIcon = nullptr;
	wnd_class_.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wnd_class_.lpszMenuName = nullptr;
	wnd_class_.style = 0;
	wnd_class_.cbClsExtra = 0;
	wnd_class_.cbWndExtra = 1;

	RegisterClass(&wnd_class_);
	
	wnd_handle_ = CreateWindow(
		CONTENT_CONTAINER,
		L"",
		wnd_style_,
		x,
		y,
		width_,
		height_,
		parent_wnd_handle_,
		nullptr,
		instance_,
		nullptr);

}

WinGUI::Window::Window(const HWND &parent_handle, const HINSTANCE &instance, const int &x, const int &y, const int &width, const int &height)
	: parent_wnd_handle_(parent_handle),
	  wnd_class_(),
	  instance_(instance),
	  wnd_style_(WS_CHILD),
	  width_(width),
	  height_(height) {

	if(parent_wnd_handle_ == nullptr)
		throw WinGUI_InvalidArgumentException("Cannot construct a child window. Handle to the parent window is not defined.");

	if(instance_ == nullptr)
		throw WinGUI_InvalidArgumentException("Cannot construct a child window. Handle to program instance is not defined.");
	
	if(width_ < 50)
		width_ = 100;

	if(height_ < 50)
		height_ = 100;
	
	wnd_class_.lpszClassName = L"content_container";
	wnd_class_.lpfnWndProc = wnd_proc;
	wnd_class_.hInstance = instance_;
	wnd_class_.hCursor = nullptr;
	wnd_class_.hIcon = nullptr;
	wnd_class_.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wnd_class_.lpszMenuName = nullptr;
	wnd_class_.style = 0;
	wnd_class_.cbClsExtra = 0;
	wnd_class_.cbWndExtra = 1;

	RegisterClass(&wnd_class_);

	
	wnd_handle_ = CreateWindow(
		CONTENT_CONTAINER,
		L"",
		wnd_style_,
		x,
		y,
		width_,
		height_,
		parent_wnd_handle_,
		nullptr,
		instance_,
		nullptr);

}

HWND WinGUI::Window::get_handle() const noexcept { return wnd_handle_; }

HINSTANCE WinGUI::Window::get_instance() const noexcept { return instance_; }

int WinGUI::Window::get_width() const noexcept { return width_; }

int WinGUI::Window::get_height() const noexcept { return height_; }

bool WinGUI::Window::set_icon(std::string file_path) const {
	std::wstring file_path_w = string_to_wstring(std::move(file_path));
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

void WinGUI::Window::show_window() const noexcept { ShowWindow(wnd_handle_, SW_SHOW); }

void WinGUI::Window::hide_window() const noexcept { ShowWindow(wnd_handle_, SW_HIDE); }

void WinGUI::Window::set_cursor(Cursor cursor_type) const noexcept {
	HCURSOR cursor = LoadCursor(nullptr, MAKEINTRESOURCE(cursor_type));
	SetClassLongPtr(wnd_handle_,GCLP_HCURSOR, (LONG_PTR)cursor);
}

void WinGUI::Window::set_style(Style wnd_style) noexcept {
	wnd_style_ |= (DWORD)wnd_style;
	SetWindowLongPtr(wnd_handle_,GWL_STYLE, (LONG_PTR)wnd_style_);
}

bool WinGUI::Window::set_menu(unsigned int resource_id) const noexcept {
	HMENU hmenu = LoadMenu(instance_, MAKEINTRESOURCE(resource_id));

	return SetMenu(wnd_handle_, hmenu);
}



int WinGUI::IO::Mouse::get_wheel_delta() { return wheel_delta_; }

int WinGUI::IO::Mouse::get_position_x() { return mouse_position_.x; }

int WinGUI::IO::Mouse::get_position_y() { return mouse_position_.y; }

int WinGUI::IO::Mouse::get_position_dx() { return mouse_delta_.x; }

int WinGUI::IO::Mouse::get_position_dy() { return mouse_delta_.y; }

bool WinGUI::IO::Mouse::left_button_down() { return left_button_down_; }

bool WinGUI::IO::Mouse::right_button_down() { return right_button_down_; }

bool WinGUI::IO::Mouse::middle_button_down() { return middle_button_down_; }



bool WinGUI::IO::Keyboard::key_down(Key key) { return keys_down_[(byte)key]; }

bool WinGUI::IO::Keyboard::key_up(Key key) { return keys_up_[(byte)key]; }

bool WinGUI::IO::Keyboard::sys_key_down(Key key) { return sys_keys_down_[(byte)key]; }

bool WinGUI::IO::Keyboard::sys_key_up(Key key) { return sys_keys_up_[(byte)key]; }

char WinGUI::IO::Keyboard::get_typed_char() { return typed_char_; }

void WinGUI::IO::Keyboard::key_up_flush(Key key) { keys_up_[(byte)key] = false; }

void WinGUI::IO::Keyboard::sys_key_up_flush(Key key) { sys_keys_up_[(byte)key] = false; }