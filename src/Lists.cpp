#include <Lists.h>
#include <Context.h>
#include <Window.h>
#include <StaticControls.h>



WinGUI::List::List(const Window& parent, std::string name, const int& x, const int& y) : Control(parent, x, y, std::move(name)) {}

WinGUI::ComboBox::ComboBox(const Window& parent, std::string name, const int& x, const int& y, const int& width,  const int &list_height) noexcept
	: List(parent,std::move(name), x, y), width_(width), list_height_(list_height) {

	if (width_ <= 0)
		width_ = 80;

	id_ = COMBO_BOX;

	if (!name.empty()) {
		Label name_label(parent, name_, x_, y_);
		y_ += FONT_HEIGHT;
	}
	
	handle_ = CreateWindowEx(
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

bool WinGUI::ComboBox::remove_item(int index) const noexcept {
	int result = SendMessage(handle_,CB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	int count = SendMessage(handle_,CB_GETCOUNT,(WPARAM)index,0);
	
	if(!count)
		EnableWindow(handle_,false);
	
	return true;
}

int WinGUI::ComboBox::get_selected_index() const noexcept {
	int result = SendMessage(handle_,CB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void WinGUI::ComboBox::clear() const noexcept {
	SendMessage(handle_,CB_RESETCONTENT,0,0);
	EnableWindow(handle_,false);
}



WinGUI::ListBox::ListBox(const Window& parent, std::string name, const int& x, const int& y, const int& width, const int& height, bool multiple_selection) noexcept
	: List(parent, std::move(name), x, y ), width_(width), height_(height) {

	id_ = LIST_BOX;

	if (width_ <= 0)
		width_ = 100;

	if (height_ <= 0)
		height_ = 100;
	
	if(!name.empty()) {
		Label name_label(parent,name_,x_,y_);
		y_ += FONT_HEIGHT;
	}
	
	DWORD style = WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_DISABLENOSCROLL;

	if(multiple_selection)
		style|=LBS_MULTIPLESEL;
	
	handle_ = CreateWindowEx(
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

bool WinGUI::ListBox::remove_item(int index) const noexcept{
	
	int result = SendMessage(handle_,LB_DELETESTRING,(WPARAM)index,0);
	
	if(result==LB_ERR)
		return false;

	return true;
}

int WinGUI::ListBox::get_selected_index() const noexcept {
	int result = SendMessage(handle_,LB_GETCURSEL,0,0);

	if(result == LB_ERR)
		return -1;
	
	return result;
}

void WinGUI::ListBox::clear() const noexcept {
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



WinGUI::TreeView::TreeView(const Window& parent, std::string name, const int& x, const int& y, const int& width, const int& height) noexcept
	: Control(parent, x, y, std::move(name)), width_(width), height_(height),selected_item_(-1) {

	id_ = TREE_VIEW;

	if (width_ <= 0)
		width_ = 100;

	if (height_ <= 0)
		height_ = 100;
	
	if(!name.empty()) {
		Label name_label(parent,name_,x_,y_);
		y_ += FONT_HEIGHT;
	}

	handle_ = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		nullptr,
		WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | WS_VISIBLE | TVS_SHOWSELALWAYS | TVS_EDITLABELS,
		x_,
		y_,
		width_, 
		height_,
		parent_handle_,
		(HMENU)TREE_VIEW,
		parent.get_instance(),
		nullptr);


	SendMessage(handle_, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

	Context::register_gui_object(this);
}

bool WinGUI::TreeView::add_item(std::string item_name, int parent_index) {

	TVITEM new_item;
	TVINSERTSTRUCT insert_struct;

	std::wstring item_w_name = string_to_wstring(item_name);

	new_item.mask = TVIF_TEXT | TVIF_HANDLE;
	new_item.pszText = const_cast<wchar_t*>(item_w_name.c_str());

	insert_struct.item = new_item;
	insert_struct.hInsertAfter = nullptr;

	if(parent_index >= 0 && parent_index < items_.size() )
		insert_struct.hParent = items_[parent_index].hItem;
	else
		insert_struct.hParent = nullptr;
	
	new_item.hItem = TreeView_InsertItem(handle_, &insert_struct);
	
	items_.emplace_back(new_item);
	
	return false;
}

bool WinGUI::TreeView::remove_item(int index) {
	TreeView_DeleteItem(handle_, items_[index].hItem);
	items_.erase(items_.begin()+index);
	return false;
}

bool WinGUI::TreeView::clear() {
	TreeView_DeleteAllItems(handle_);
	items_.clear();
	return true;
}

int WinGUI::TreeView::set_selected_item(HTREEITEM item_handle) {

	selected_item_ = 0;

	for (auto &item : items_) {
		if (item.hItem == item_handle)
			return 0;
		selected_item_++;
	}

	return -1;
}

int WinGUI::TreeView::get_selected_index() const noexcept { return selected_item_; }