#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <iostream>
#include <win_gui_framework.h>
#include <CommCtrl.h>
#include <string>

#define BUTTON 0x01
#define CHECK_BOX 0x02
#define RICH_TEXT_INPUT 0x03
#define INT_INPUT 0x04
#define FLOAT_INPUT 0x05

namespace {

	WNDPROC std_edit_proc = nullptr;
	std::vector<bool*> button_controls;
	std::vector<bool*> check_box_controls;
	std::vector<std::string*> rich_text_buffers;
	std::vector<int*> int_input_controls;
	bool is_window_initialized = false;

	std::string wchar_t_2_string(wchar_t *text, int length) {

		std::string result;
		char char_buffer;

		for (int i = 0; i < length; i++) {
			char_buffer = (char)text[i];
			result += char_buffer;
		}

		return result;
	}
	/// <summary>
	/// Formats input wide char array to be displayed as a decimal float number
	/// </summary>
	/// <param name="source">Pointer to the input wide char array</param>
	/// <param name="length">Length of the array including \0 terminating char</param>
	/// <returns></returns>
	wchar_t *format_float_input(const wchar_t *source, int length) {

		wchar_t *padding_buffer;
		wchar_t *result;
		int buffer_itr = 0;
		int source_itr = 0;
		bool was_separator = false;

		// 1. String preprocessing
		// 1.1 Return default string if source is empty

		if (length == 1) {
			result = new wchar_t[4];

			result[0] = L'0';
			result[1] = L'.';
			result[2] = L'0';
			result[3] = L'\0';
			return result;
		}

		// 1.2. Filter redundant decimal separators - only first will be relevant

		wchar_t *filtered_str = new wchar_t[length];
		int filtered_str_len = length;

		while (source[source_itr] != L'\0') {
			if (was_separator && source[source_itr] == L'.') {
				source_itr++;
				filtered_str_len--;
				continue;
			}

			filtered_str[buffer_itr] = source[source_itr];

			if (source[source_itr] == L'.')
				was_separator = true;

			source_itr++;
			buffer_itr++;
		}

		filtered_str[buffer_itr] = L'\0';

		// filtered_str contains string which is filtered out of redundant decimal separators
		// Amount of array elements is still the same like origin length, but \0 is placed earlier which trims the string
		// Real length (pointed by \0) is stored in filtered_str_len

		// 2. Padding front with 0 when the first char is "."
		
		result = filtered_str;

		if (filtered_str[0] == L'.' && filtered_str_len >= 2) {
			padding_buffer = new wchar_t[filtered_str_len + (int)1];
			padding_buffer[0] = L'0';
			padding_buffer[1] = L'.';

			for (source_itr = 1, buffer_itr = 2; source_itr < filtered_str_len; source_itr++, buffer_itr++)
				padding_buffer[buffer_itr] = filtered_str[source_itr];

			delete [] filtered_str;
			filtered_str = padding_buffer;
			filtered_str_len++;
			result = filtered_str;
		}

		// 3. Padding back with 0 when the last char before \0 is "."

		if (filtered_str[filtered_str_len - 2] == L'.' && filtered_str_len >= 2) {
			padding_buffer = new wchar_t[filtered_str_len + (int)1];
			padding_buffer[filtered_str_len] = L'\0';
			padding_buffer[filtered_str_len - 1] = L'0';

			for (source_itr = 0, buffer_itr = 0; source_itr < filtered_str_len - 1; source_itr++, buffer_itr++)
				padding_buffer[buffer_itr] = filtered_str[source_itr];

			delete [] filtered_str;
			result = padding_buffer;
		}

		return result;
	}

	INT_PTR CALLBACK DlgProc(HWND Window_handle, UINT Message, WPARAM W_param, LPARAM L_param) {
		switch (Message) {
		case WM_CLOSE:
			PostQuitMessage(404);
			break;
		case WM_COMMAND: {
			std::cout << Message << std::endl;
			std::cout << W_param << std::endl;
			switch (LOWORD(W_param)) {
			case IDOK:
				EndDialog(Window_handle, IDOK);
				break;
			case IDCANCEL:
				EndDialog(Window_handle, IDCANCEL);
				break;
			case 1002:
				std::cout << "Button 1 clicked" << std::endl;
				break;
			}
		}
		break;

		default: return FALSE;
		}
	}

	LRESULT CALLBACK wnd_procedure(HWND Window_handle, UINT Message, WPARAM W_param, LPARAM L_param) {

		if (!is_window_initialized)
			return DefWindowProcW(Window_handle, Message, W_param, L_param);

		DWORD buffer_size;
		wchar_t *text_field_buffer=nullptr;
		wchar_t *text_field_buffer_2=nullptr;
		std::string number_string;

		switch (Message) {
		case WM_DESTROY:
			PostQuitMessage(404);
			std::cout << "Quit" << std::endl;
			break;
		case WM_LBUTTONDOWN:
			std::cout << "Mouse down" << std::endl;
			break;
		case WM_LBUTTONUP:
			std::cout << "Mouse up" << std::endl;
			break;
		case WM_COMMAND:
			bool *control_ptr;
			switch (HIBYTE(W_param)) {
			case BUTTON:
				control_ptr = button_controls[LOBYTE(W_param)];
				*control_ptr = true;
				break;
			case CHECK_BOX:
				control_ptr = check_box_controls[LOBYTE(W_param)];
				(*control_ptr) = !(*control_ptr);
				if (*control_ptr)
					CheckDlgButton(Window_handle, W_param,BST_CHECKED);
				else
					CheckDlgButton(Window_handle, W_param,BST_UNCHECKED);
				break;
			case RICH_TEXT_INPUT:
				buffer_size = GetWindowTextLength((HWND)L_param);
				text_field_buffer = new wchar_t[buffer_size];
				GetWindowText((HWND)L_param, (LPWSTR)text_field_buffer, buffer_size + 1);
				*rich_text_buffers[LOBYTE(W_param)] = wchar_t_2_string(text_field_buffer, buffer_size);
				break;
			case INT_INPUT:
				if (LOBYTE(W_param) < int_input_controls.size()) {
					buffer_size = GetWindowTextLength((HWND)L_param);
					text_field_buffer = new wchar_t[buffer_size];
					GetWindowText((HWND)L_param, (LPWSTR)text_field_buffer, buffer_size + 1);
					number_string = wchar_t_2_string(text_field_buffer, buffer_size);

					try { *int_input_controls[LOBYTE(W_param)] = std::stoi(number_string); }
					catch (...) { std::cout << "Invalid integer input" << std::endl; }
				}
				break;
			case FLOAT_INPUT: {
				buffer_size = GetWindowTextLength((HWND)L_param);
				text_field_buffer = new wchar_t[buffer_size+1];
				GetWindowText((HWND)L_param, (LPWSTR)text_field_buffer, buffer_size+1);

				if(!buffer_size)
					SendMessage((HWND)L_param,WM_SETTEXT,0,(LPARAM)L"0.0");
			
				delete [] text_field_buffer_2;
				text_field_buffer_2 = format_float_input(text_field_buffer,buffer_size+1);

				if(text_field_buffer_2!=nullptr)
					std::wcout<<text_field_buffer_2<<std::endl;
			break;	
			}
			default:
				break;
			}
			break;
		default:
			return DefWindowProcW(Window_handle, Message, W_param, L_param);
		}
		return 0;
	}

	LRESULT CALLBACK float_input_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {

		switch (message) {
		case WM_CHAR:
			if (((w_param < '0') || (w_param > '9')) && (w_param != '.') && (w_param != 8))
				return 0;

		default:
			break;
		}

		return CallWindowProc(std_edit_proc, window_handle, message, w_param, l_param);
	}

	// TODO: Add CreateWindow  fail exception
	HWND create_button(unsigned char id, HWND parent, LPCWSTR button_label, int x, int y, int width, int height) {

		unsigned short w_param = (BUTTON << 8) | id;
		HWND result = CreateWindowExW(
			0,
			L"BUTTON",
			button_label,
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			x,
			y,
			width,
			height,
			parent,
			(HMENU)w_param,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(result, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
		return result;
	}

	// TODO: Add CreateWindow  fail exception
	HWND create_group_box(HWND parent, LPCWSTR button_label, int x, int y, int width, int height) {

		HWND result = CreateWindowExW(
			0,
			L"BUTTON",
			button_label,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			x,
			y,
			width,
			height,
			parent,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		// Je�eli result==NULL to b��d - rzuci� wyj�tek
		SendMessage(result, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
		return result;
	}

	// TODO: Add CreateWindow  fail exception
	HWND create_check_box(unsigned char id, HWND parent, LPCWSTR check_box_label, int x, int y) {

		unsigned short w_param = (CHECK_BOX << 8) | id;
		std::wstring label(check_box_label);

		HWND result = CreateWindowExW(
			0,
			L"BUTTON",
			check_box_label,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			x,
			y,
			12 + label.length() * 6,
			12,
			parent,
			(HMENU)w_param,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(result, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
		return result;
	}

	// TODO: Add CreateWindow  fail exception
	HWND create_rich_text_input(unsigned char id, HWND parent, int x, int y, int width, int height, DWORD dwStyle) {

		unsigned short w_param = (RICH_TEXT_INPUT << 8) | id;
		HWND result = CreateWindowExW(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			nullptr,
			dwStyle,
			x
			,
			y,
			width,
			height,
			parent,
			(HMENU)w_param,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(result, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
		return result;
	}

	HWND create_int_input(unsigned char id, HWND parent, int x, int y, int width, int min, int max) {

		unsigned short w_param = (INT_INPUT << 8) | id;
		HWND result = CreateWindowExW(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			nullptr,
			WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT,
			x,
			y,
			width,
			22,
			parent,
			(HMENU)w_param,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(result, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

		HWND ud_control_handle = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			UPDOWN_CLASS,
			nullptr,
			WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
			0,
			0,
			0,
			0,
			parent,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(ud_control_handle,UDM_SETRANGE, 0,MAKELPARAM(max, min));

		return result;
	}

	HWND create_float_input(unsigned char id, HWND parent, int x, int y, int width) {

		unsigned short w_param = (FLOAT_INPUT << 8) | id;
		HWND result = CreateWindowExW(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			nullptr,
			WS_CHILD | WS_VISIBLE | ES_LEFT,
			x,
			y,
			width,
			22,
			parent,
			(HMENU)w_param,
			GetModuleHandle(nullptr),
			nullptr);

		SendMessage(result, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
		SendMessage(result,WM_SETTEXT,0,(LPARAM)L"0.0");
		if (std_edit_proc == nullptr)
			std_edit_proc = (WNDPROC)SetWindowLongPtr(result,GWLP_WNDPROC, (LONG_PTR)float_input_proc);

		return result;

	}
}

Win_GUI::Window::Window(LPCWSTR title_label, int width, int height, bool is_size_fixed) {

	DWORD style;

	if (is_size_fixed)
		style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	else
		style = WS_OVERLAPPEDWINDOW;

	wnd_class.lpszClassName = L"win_gui_class";
	wnd_class.lpfnWndProc = wnd_procedure;
	wnd_class.hInstance = GetModuleHandle(nullptr);
	wnd_class.hCursor = nullptr;
	wnd_class.hIcon = nullptr;
	wnd_class.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW + 1);
	wnd_class.lpszMenuName = nullptr;
	wnd_class.style = 0;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 1;

	RegisterClass(&wnd_class);

	wnd_handle = CreateWindow(
		L"win_gui_class",
		title_label,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);
}

void Win_GUI::Window::show_window_async() const {
	ShowWindow(wnd_handle, 1);
	MSG msg;
	while (GetMessage(&msg, wnd_handle, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Win_GUI::Window::show_window() const {
	ShowWindow(wnd_handle, 1);
	is_window_initialized = true;
	return wnd_handle;
}

bool Win_GUI::Window::add_button(bool *control, LPCWSTR button_label, int x, int y, int width, int height) {

	if ((control == nullptr)
		|| (button_label == nullptr)
		|| (x < 0)
		|| (y < 0)
		|| (width < 0)
		|| (height < 0))
		return false;

	HWND new_button_handle = create_button((unsigned char)button_controls.size(), wnd_handle, button_label, x, y, width,
	                                       height);
	button_handles.emplace_back(new_button_handle);
	button_controls.emplace_back(control);

	return true;
}

bool Win_GUI::Window::add_group_box(LPCWSTR button_label, int x, int y, int width, int height) const {

	if ((button_label == nullptr)
		|| (x < 0)
		|| (y < 0)
		|| (width < 0)
		|| (height < 0))
		return false;

	create_group_box(wnd_handle, button_label, x, y, width, height);
	return true;
}

bool Win_GUI::Window::add_check_box(bool *control, LPCWSTR check_box_label, int x, int y) {

	if ((check_box_label == nullptr)
		|| (control == nullptr)
		|| (x < 0)
		|| (y < 0))
		return false;

	HWND new_check_box_handle = create_check_box((unsigned char)check_box_controls.size(), wnd_handle, check_box_label,
	                                             x, y);

	check_box_controls.emplace_back(control);
	check_box_handles.emplace_back(new_check_box_handle);

	return true;
}

bool Win_GUI::Window::add_pane(int x, int y, int width, int height) const {
	if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0))
		return false;
	CreateWindowExW(
		0,
		L"STATIC",
		L"Pane",
		WS_CHILD | WS_VISIBLE | SS_BITMAP,
		x,
		y,
		width,
		height,
		wnd_handle,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);
	return true;
}

bool Win_GUI::Window::add_rich_text_input(std::string *buffer, int x, int y, int width, int height, bool v_scroll,
                                          bool h_scroll) {

	if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0))
		return false;

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE;

	if (v_scroll)
		dwStyle |= WS_VSCROLL;

	if (h_scroll)
		dwStyle |= WS_HSCROLL;

	HWND new_text_field = create_rich_text_input((unsigned char)text_field_handles.size(), wnd_handle, x, y, width,
	                                             height,
	                                             dwStyle);
	text_field_handles.emplace_back((new_text_field));
	rich_text_buffers.emplace_back(buffer);

	return true;
}

bool Win_GUI::Window::add_int_input(int *control, int x, int y, int width, int min, int max) {

	HWND buddy_wnd_handle = create_int_input((unsigned char)int_input_controls.size(), wnd_handle, x, y, width, min,
	                                         max);
	int_input_controls.emplace_back(control);

	return true;
}

bool Win_GUI::Window::add_float_input(float *control, int x, int y, int width) {

	create_float_input(0, wnd_handle, x, y, width);

	return true;
}
