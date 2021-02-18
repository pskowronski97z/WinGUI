#include <Lists.h>
#include <Context.h>
#include <Window.h>
#include <CommCtrl.h>
#include <StaticControls.h>

WinGUI::List::List(const Window& parent, const int& x, const int& y, std::string name) : Control(parent, x, y, name) {}

WinGUI::ComboBox::ComboBox(const Window& parent, const std::string& name, const int& x, const int& y, const int& width,  const int &list_height)
	: List(parent, x, y, name), width_(width), list_height_(list_height) {

	id_ = COMBO_BOX;

	if(!name.empty()) {
		Label name_label(parent,x_,y_,name);
		y_ += FONT_HEIGHT;
	}
	
	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WC_COMBOBOX,
		nullptr,
		WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
		x_,
		y_,
		width_,
		list_height_,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	EnableWindow(handle_,false);
}

bool WinGUI::ComboBox::add_item(std::string item) const {
	if (item.empty())
		return false;

	int count = SendMessage(handle_,CB_GETCOUNT,0,0);
	
	if(!count)
		EnableWindow(handle_,true);
	
	std::wstring w_item = string_to_wstring(item);
	SendMessage(handle_,CB_ADDSTRING, 0, (LPARAM)w_item.c_str());

	return true;
}

bool WinGUI::ComboBox::remove_item(int index) const {
	int result = SendMessage(handle_,CB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	int count = SendMessage(handle_,CB_GETCOUNT,(WPARAM)index,0);
	
	if(!count)
		EnableWindow(handle_,false);
	
	return true;
}

int WinGUI::ComboBox::get_selected_index() const {
	int result = SendMessage(handle_,CB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void WinGUI::ComboBox::clear() const {
	SendMessage(handle_,CB_RESETCONTENT,0,0);
	EnableWindow(handle_,false);
}



WinGUI::ListBox::ListBox(const Window& parent, const std::string& name, const int& x, const int& y, const int& width, const int& height, bool multiple_selection)
	: List(parent, x, y, name), width_(width), height_(height) {

	id_ = LIST_BOX;

	if(!name.empty()) {
		Label name_label(parent,x_,y_,name);
		y_ += FONT_HEIGHT;
	}
	
	DWORD style = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_DISABLENOSCROLL;

	if(multiple_selection)
		style|=LBS_MULTIPLESEL;
	
	handle_ = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WC_LISTBOX,
		nullptr,
		style,
		x_,
		y_,
		width_,
		height_,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		0);

	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	
}

bool WinGUI::ListBox::add_item(std::string item) const {

	if (item.empty())
		return false;

	std::wstring w_item = string_to_wstring(item);
	SendMessage(handle_,LB_ADDSTRING, 0, (LPARAM)w_item.c_str());
	return true;

}

bool WinGUI::ListBox::remove_item(int index) const {
	
	int result = SendMessage(handle_,LB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	return true;
}

int WinGUI::ListBox::get_selected_index() const {
	int result = SendMessage(handle_,LB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void WinGUI::ListBox::clear() const {
	SendMessage(handle_,LB_RESETCONTENT,0,0);
}

std::vector<int> WinGUI::ListBox::get_selected_indexes() const {
	int count = SendMessage(handle_,LB_GETSELCOUNT, 0, 0);

	if (count < 0)
		return std::vector<int>(1, -1);

	std::vector<int> result(count);
	SendMessage(handle_,LB_GETSELITEMS, count, (LPARAM)result.data());

	return result;
}



