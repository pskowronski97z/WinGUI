#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <iostream>
#include <win_gui.h>

#define FONT_HEIGHT 16
#define RB_GROUPS_LIMIT 0xc0
#define BUTTON 0xc1
#define CHECK_BOX 0xc2
#define RICH_TEXT_INPUT 0xc3
#define INT_INPUT 0xc4
#define FLOAT_INPUT 0xc5
#define TAB_CONTROL 0xc6
#define TREE_VIEW 0xc7
#define DEFAULT 0xFF00

/// <summary>
/// This namespace represents whole GUI context runned by thread 
/// </summary>
namespace {

	WNDPROC std_edit_proc = nullptr;
	std::vector<bool*> button_controls;
	std::vector<bool*> check_box_controls;
	std::vector<std::string*> rich_text_buffers;
	std::vector<int*> int_input_controls;
	std::vector<float*> float_input_controls;
	std::vector<int*> rb_group_controls;
	// Przerobiæ na wektory wskaŸników
	std::vector<Win_GUI::Tab> tab_controls;
	std::vector<Win_GUI::Tree_View> tree_views;
	bool is_window_initialized = false;
	unsigned char rb_count = 0;
	
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

		if(!was_separator) {
			result = new wchar_t[length+2];

			for(int i=0;i<length;i++)
				result[i]=source[i];

			result[length + 1]=L'\0';
			result[length]=L'0';
			result[length -1]=L'.';

			delete [] filtered_str;
			return result;
		}
		
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

	std::wstring string_2_wstring(std::string source) {

		std::wstring result;

		for(auto &ch : source) 
			result += (wchar_t)ch;
		
		return result;
	}
	
	/*INT_PTR CALLBACK DlgProc(HWND Window_handle, UINT Message, WPARAM W_param, LPARAM L_param) {
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
	}*/

	DWORD buffer_size = 0;
	wchar_t *text_field_buffer = nullptr;
	std::string number_string;
	LPNMHDR id = 0;
	int index = 0;
	LPNMTREEVIEW nmtv;
	
	LRESULT CALLBACK wnd_procedure(HWND Window_handle, UINT Message, WPARAM W_param, LPARAM L_param) {

		if (!is_window_initialized)
			return DefWindowProcW(Window_handle, Message, W_param, L_param);

		switch (Message) {
		case WM_NOTIFY:
			id = (LPNMHDR)L_param;
			if (id->code == TCN_SELCHANGE) {
				for (auto &tab : tab_controls) {
					if (tab.get_handle() == id->hwndFrom) {
						index = TabCtrl_GetCurSel(tab.get_handle());
						tab.show_distinct(index);
						break;
					}
				}
			}
			if (id->idFrom == TREE_VIEW) {
				switch (id->code) {
				case TVN_SELCHANGED:

					nmtv = (LPNMTREEVIEW)L_param;

					TVITEM new_it = nmtv->itemNew;

					for(auto &tv : tree_views) {
						if(tv.get_handle() == id->hwndFrom) {
							tv.set_selected_item(new_it.hItem);
							break;
						}
					}
		
					break;
				case TVN_ENDLABELEDIT:
					if (((LPNMTVDISPINFO)L_param)->item.pszText != NULL)
						return true;
					return false;
				}
			}
			break;
			
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
				
			case INT_INPUT:   // Przerobiæ na parsowanie w ten sam sposób co float
				if (LOBYTE(W_param) < int_input_controls.size()) {
					buffer_size = GetWindowTextLength((HWND)L_param);
					text_field_buffer = new wchar_t[buffer_size];
					GetWindowText((HWND)L_param, (LPWSTR)text_field_buffer, buffer_size + 1);
					number_string = wchar_t_2_string(text_field_buffer, buffer_size);

					try { *int_input_controls[LOBYTE(W_param)] = std::stoi(number_string); }
					catch (...) { std::cout << "Invalid integer input" << std::endl; }
				}
				break;
				
			case FLOAT_INPUT: 
				if (HIWORD(W_param) == WM_MOUSEMOVE) {
					buffer_size = GetWindowTextLength((HWND)L_param);
					text_field_buffer = new wchar_t[buffer_size];
					GetWindowText((HWND)L_param, (LPWSTR)text_field_buffer, buffer_size + 1);
					number_string = wchar_t_2_string(text_field_buffer, buffer_size);

					*float_input_controls[LOBYTE(W_param)] = std::stof(number_string);
				}
				break;
				
			default:
				if (HIBYTE(W_param) >= 0 && HIBYTE(W_param) <= 0xc0)
					*(rb_group_controls[HIBYTE(W_param)]) = LOBYTE(W_param);
				break;
				
			}
			break;
			
		default:
			return DefWindowProcW(Window_handle, Message, W_param, L_param);
		}
		return 0;
	}

	LRESULT CALLBACK float_input_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {

		int buffer_length;
		wchar_t *buffer;
		wchar_t *format_buffer;
		switch (message) {
		case WM_CHAR:
			if (((w_param < '0') || (w_param > '9')) && (w_param != '.') && (w_param != 8))
				return 0;
		break;
		case WM_KILLFOCUS:	
			buffer_length = GetWindowTextLength(window_handle);
			buffer = new wchar_t[buffer_length+1];
			GetWindowText(window_handle, (LPWSTR)buffer, buffer_length+1);
	
			format_buffer = format_float_input(buffer,buffer_length+1);

			if(format_buffer!=nullptr)
				SendMessage(window_handle,WM_SETTEXT,0,(LPARAM)format_buffer);

			delete [] buffer;
			delete [] format_buffer;
			break;
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
			(HMENU)DEFAULT,
			GetModuleHandle(nullptr),
			nullptr);

		// Je¿eli result==NULL to b³¹d - rzuciæ wyj¹tek
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






Win_GUI::Window::Window(std::string title_label, int width, int height, bool is_size_fixed) {

	DWORD style;
	std::wstring title_w_label = string_2_wstring(title_label);

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
		title_w_label.c_str(),
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

Win_GUI::Window::Window(int x, int y, int width, int height, HWND parent) {

	wnd_class.lpszClassName = L"content_container";
	wnd_class.lpfnWndProc = wnd_procedure;
	wnd_class.hInstance = GetModuleHandle(nullptr);
	wnd_class.hCursor = nullptr;
	wnd_class.hIcon = nullptr;
	wnd_class.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wnd_class.lpszMenuName = nullptr;
	wnd_class.style = 0;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 1;

	RegisterClass(&wnd_class);

	wnd_handle = CreateWindow(
		L"content_container",
		L"",
		WS_CHILD,
		x,
		y,
		width,
		height,
		parent,
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

Win_GUI::Tree_View& Win_GUI::Window::add_tree_view(int x, int y, int width, int height) {

	Tree_View new_tree_view("NON",wnd_handle,x,y,width,height);
	tree_views.emplace_back(new_tree_view);
	return tree_views.at(tree_views.size()-1);
}

void Win_GUI::Window::show_window() const {
	ShowWindow(wnd_handle, SW_SHOW);
	is_window_initialized = true;
}

void Win_GUI::Window::hide_window() {
	ShowWindow(wnd_handle,SW_HIDE);
}

bool Win_GUI::Window::add_button(bool *control, std::string button_label, int x, int y, int width, int height) {

	if ((control == nullptr)
		|| (button_label.empty())
		|| (x < 0)
		|| (y < 0)
		|| (width < 0)
		|| (height < 0))
		return false;

	std::wstring button_w_label = string_2_wstring(button_label);
	HWND new_button_handle = create_button((unsigned char)button_controls.size(), wnd_handle, button_w_label.c_str(), x, y, width,
	                                       height);
	button_handles.emplace_back(new_button_handle);
	button_controls.emplace_back(control);

	return true;
}

bool Win_GUI::Window::add_group_box(std::string group_box_label, int x, int y, int width, int height) const {

	if ((group_box_label.empty())
		|| (x < 0)
		|| (y < 0)
		|| (width < 0)
		|| (height < 0))
		return false;

	std::wstring gbox_w_label = string_2_wstring(group_box_label);
	create_group_box(wnd_handle, gbox_w_label.c_str(), x, y, width, height);
	return true;
}

bool Win_GUI::Window::add_check_box(bool *control, std::string check_box_label, int x, int y) {

	if ((check_box_label.empty())
		|| (control == nullptr)
		|| (x < 0)
		|| (y < 0))
		return false;

	std::wstring check_box_w_label = string_2_wstring(check_box_label);
	HWND new_check_box_handle = create_check_box((unsigned char)check_box_controls.size(), wnd_handle, check_box_w_label.c_str(), x, y);

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
		(HMENU)DEFAULT,
		GetModuleHandle(nullptr),
		nullptr);
	return true;
}

bool Win_GUI::Window::add_rich_text_input(std::string *buffer, std::string name, int x, int y, int width, int height, bool v_scroll, bool h_scroll, bool is_password) {

	if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0))
		return false;

	if (!name.empty()) {
		add_label(name, x, y);
		height -= FONT_HEIGHT;
		y += FONT_HEIGHT;
	}
	
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE;

	if (v_scroll)
		dwStyle |= WS_VSCROLL;

	if (h_scroll)
		dwStyle |= WS_HSCROLL;

	if(is_password) {
		dwStyle = WS_CHILD | WS_VISIBLE | ES_PASSWORD;
		height = 22;
	}
		
	HWND new_text_field = create_rich_text_input((unsigned char)text_field_handles.size(), wnd_handle, x, y, width, height, dwStyle);
	text_field_handles.emplace_back((new_text_field));
	rich_text_buffers.emplace_back(buffer);

	return true;
}

bool Win_GUI::Window::add_int_input(int *control, std::string name, int x, int y, int width, int min, int max) {

	if (!name.empty()) {
		add_label(name, x, y);
		y += FONT_HEIGHT;
	}
	
	HWND buddy_wnd_handle = create_int_input((unsigned char)int_input_controls.size(), wnd_handle, x, y, width, min,
	                                         max);
	int_input_controls.emplace_back(control);

	return true;
}

bool Win_GUI::Window::add_float_input(float *control, std::string name, int x, int y, int width) {

	if (!name.empty()) {
		add_label(name, x, y);
		y += FONT_HEIGHT;
	}
	
	create_float_input(float_input_controls.size(), wnd_handle, x, y, width);

	float_input_controls.emplace_back(control);
	
	return true;
}

bool Win_GUI::Window::add_label(std::string label_text, int x, int y) {


	std::wstring label_w_text = string_2_wstring(label_text);
	HWND result = CreateWindowExW(
		0,
		WC_STATIC,
		label_w_text.c_str(),
		WS_CHILD|WS_VISIBLE|SS_LEFT,
		x,
		y,
		label_w_text.size()*6,
		FONT_HEIGHT,
		wnd_handle,
		(HMENU)DEFAULT,
		GetModuleHandle(nullptr),
		0);

	SendMessage(result, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

	return true;
	
}

bool Win_GUI::Window::add_radio_button(int* control, std::string rb_label, int x, int y, bool is_first)
{
	if ((rb_label.empty())
		|| (control == nullptr && is_first)
		|| (x < 0)
		|| (y < 0)
		|| (is_first && (rb_group_controls.size() > RB_GROUPS_LIMIT))
		|| (rb_count > 0xFF)
		|| (!is_first && rb_group_controls.empty()))
		return false;

	std::wstring rb_w_label = string_2_wstring(rb_label);
	DWORD style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;

	if (is_first) {
		style |= WS_GROUP;
		rb_group_controls.emplace_back(control);
		rb_count=0;
	}

	unsigned short w_param = (rb_group_controls.size() - 1) << 8 | rb_count;

	HWND result = CreateWindowExW(
		0,
		L"BUTTON",
		rb_w_label.c_str(),
		style,
		x,
		y,
		12 + rb_label.length() * 6,
		12,
		wnd_handle,
		(HMENU)w_param,
		GetModuleHandle(nullptr),
		nullptr);

	SendMessage(result, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	rb_count++;

	return true;
}

Win_GUI::Tab& Win_GUI::Window::add_tab(int x, int y, int width, int height) {

	Tab result(x,y,width,height,wnd_handle);
	tab_controls.emplace_back(result);
	return tab_controls.at(tab_controls.size()-1);
}

Win_GUI::Window Win_GUI::Window::add_content_container(int x, int y, int width, int height) {

	Window content_pane(x,y,width,height,wnd_handle);
	return content_pane;
}

bool Win_GUI::Window::add_progress_bar(int x, int y, int width, int height) {

	HWND result = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		PROGRESS_CLASS,
		0,
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_SMOOTHREVERSE, 
		x ,
		y, 
		width,
		height,
		wnd_handle,
		(HMENU)0xc8, 
		GetModuleHandle(nullptr), 
		0);

	SendMessage(result,PBM_SETPOS, (WPARAM)50,0);
	
	return true;
	
}






Win_GUI::List_Box Win_GUI::Window::add_list_box(std::string name, int x, int y, int width, int height, bool multiple_selection) {

	if (!name.empty()) {
		add_label(name, x, y);
		y += FONT_HEIGHT;
		height -= FONT_HEIGHT;
	}
	List_Box result(x, y, width, height, wnd_handle, multiple_selection);

	return result;
}

Win_GUI::List_Box::List_Box(int x, int y, int width, int height, HWND parent, bool multiple_selection) {

	DWORD style = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_DISABLENOSCROLL;

	if(multiple_selection)
		style|=LBS_MULTIPLESEL;
	
	handle = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WC_LISTBOX,
		nullptr,
		style,
		x,
		y,
		width,
		height,
		parent,
		(HMENU)DEFAULT,
		GetModuleHandle(nullptr),
		0);

	SendMessage(handle, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
}

bool Win_GUI::List_Box::add_item(std::string item) const {

	if (item.empty())
		return false;

	std::wstring w_item = string_2_wstring(item);
	SendMessage(handle,LB_ADDSTRING, 0, (LPARAM)w_item.c_str());
	return true;
}

bool Win_GUI::List_Box::remove_item(int index) const {

	int result = SendMessage(handle,LB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	return true;
}

int Win_GUI::List_Box::get_selected_index() const {

	int result = SendMessage(handle,LB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void Win_GUI::List_Box::clear() const {
	SendMessage(handle,LB_RESETCONTENT,0,0);
}

std::vector<int> Win_GUI::List_Box::get_selected_indexes() const {

	int count = SendMessage(handle,LB_GETSELCOUNT, 0, 0);

	if (count < 0)
		return std::vector<int>(1, -1);

	std::vector<int> result(count);
	SendMessage(handle,LB_GETSELITEMS, count, (LPARAM)result.data());

	return result;
}






Win_GUI::Combo_Box Win_GUI::Window::add_combo_box(std::string name, int x, int y, int width) {

	if (!name.empty()) {
		add_label(name, x, y);
		y += FONT_HEIGHT;
	}
	Combo_Box result(x, y, width, wnd_handle);

	return result;
}

Win_GUI::Combo_Box::Combo_Box(int x, int y, int width, HWND parent) {
	
	handle = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WC_COMBOBOX,
		nullptr,
		WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
		x,
		y,
		width,
		100,
		parent,
		(HMENU)DEFAULT,
		GetModuleHandle(nullptr),
		nullptr);

	SendMessage(handle, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	EnableWindow(handle,false);
}

bool Win_GUI::Combo_Box::add_item(std::string item) const {

	if (item.empty())
		return false;

	int count = SendMessage(handle,CB_GETCOUNT,0,0);
	
	if(!count)
		EnableWindow(handle,true);
	
	std::wstring w_item = string_2_wstring(item);
	SendMessage(handle,CB_ADDSTRING, 0, (LPARAM)w_item.c_str());

	return true;
}

bool Win_GUI::Combo_Box::remove_item(int index) const {

	int result = SendMessage(handle,CB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	int count = SendMessage(handle,CB_GETCOUNT,(WPARAM)index,0);
	
	if(!count)
		EnableWindow(handle,false);
	
	return true;
}

int Win_GUI::Combo_Box::get_selected_index() const {

	int result = SendMessage(handle,CB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void Win_GUI::Combo_Box::clear() const {
	SendMessage(handle,CB_RESETCONTENT,0,0);
	EnableWindow(handle,false);
}






Win_GUI::Tab::Tab(int x, int y, int width, int height, HWND parent) : x(x), y(y), width(width), height(height), parent(parent) {

	handle = CreateWindowEx(
		0,
		WC_TABCONTROL,
		0,
		WS_CHILD | WS_VISIBLE ,
		x,
		y,
		width,
		height,
		parent,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	SendMessage(handle, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
}

bool Win_GUI::Tab::add_content(std::string tab_name, Window content) {
	std::wstring tab_w_name = string_2_wstring(tab_name);
	wchar_t *text = new wchar_t[tab_w_name.size() + 1];

	int i = 0;

	for (auto &wchar : tab_w_name) {
		text[i] = wchar;
		i++;
	}

	text[i] = L'\0';
	TCITEM tcitem;
	tcitem.mask = TCIF_TEXT;
	tcitem.pszText = text;
	tcitem.cchTextMax = tab_w_name.size();
	TabCtrl_InsertItem(handle, contents.size(), & tcitem);

	contents.emplace_back(content);
	delete [] text;
	return true;
}

bool Win_GUI::Tab::show_distinct(int index) {
	if(index >= contents.size())
		return false;

	for(auto &wnd : contents)
		wnd.hide_window();

	contents[index].show_window();
	return true;
}

HWND Win_GUI::Tab::get_handle() const {
	return handle;
}






Win_GUI::Tree_View::Tree_View(std::string name, HWND parent, int x, int y, int width, int height)
	: name(name),
	  parent_wnd_handle(parent),
	  x(x),
	  y(y),
	  width(width),
	  height(height) {

	tv_handle = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		nullptr,
		WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | WS_VISIBLE | TVS_SHOWSELALWAYS | TVS_EDITLABELS,
		x,
		y,
		width, 
		height,
		parent,
		(HMENU)TREE_VIEW,
		GetModuleHandle(nullptr),
		nullptr);

}

bool Win_GUI::Tree_View::add_item(std::string item_name, int parent_index) {

	TVITEM new_item;
	TVINSERTSTRUCT insert_struct;

	std::wstring item_w_name = string_2_wstring(item_name);

	new_item.mask = TVIF_TEXT | TVIF_HANDLE;
	new_item.pszText = const_cast<wchar_t*>(item_w_name.c_str());

	insert_struct.item = new_item;
	insert_struct.hInsertAfter = nullptr;

	if(parent_index >= 0 && parent_index < items.size() )
		insert_struct.hParent = items[parent_index].hItem;
	else
		insert_struct.hParent = nullptr;
	
	new_item.hItem = TreeView_InsertItem(tv_handle, &insert_struct);
	
	items.emplace_back(new_item);
	
	return false;
}

bool Win_GUI::Tree_View::remove_item(int index) {

	TreeView_DeleteItem(tv_handle, items[index].hItem);
	items.erase(items.begin()+index);
	return false;
}

bool Win_GUI::Tree_View::clear() {
	TreeView_DeleteAllItems(tv_handle);
	items.clear();
	return true;
}

int Win_GUI::Tree_View::set_selected_item(HTREEITEM item_handle) {

	selected_item = 0;
	
	for(auto &item : items) {
		if(item.hItem == item_handle)
			return 0;
		selected_item++;
	}

	return -1;

}

int Win_GUI::Tree_View::get_selected_item() {
	return selected_item;
}

HWND Win_GUI::Tree_View::get_handle() {
	return tv_handle;
}
