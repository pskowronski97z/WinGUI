#include <Buttons.h>
#include <CommCtrl.h>
#include <Context.h>
#include <Window.h>
#include <Exceptions.h>

WinGUI::Button::Button(const Window &parent, const int &x, const int &y, std::string name)
	: Control(parent, x, y, std::move(name)),
	  state_(false),
	  is_focus_set_(false) {}

void WinGUI::Button::on_click()
{
}

void WinGUI::Button::on_focus_set() { is_focus_set_ = true; }

void WinGUI::Button::on_focus_lost() { is_focus_set_ = false; }


WinGUI::ClickButton::ClickButton(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height)
	: Button(parent, x, y, std::move(name)),
	  width_(width),
	  height_(height) {

	id_ = Context::get_btn_buffer_size();

	if (id_ <= 0x0FFF)
		id_ |= BUTTON;
	else
		throw WinGUI_ControlCountException("Cannot create a new click button. Maximum possible count was reached (4096).");

	const auto w_name = string_to_wstring(name_);

	handle_ = CreateWindowEx(
		0,
		L"BUTTON",
		w_name.c_str(),
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_NOTIFY,
		x_,
		y_,
		width_,
		height_,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));

	Context::register_gui_object(this);

}

void WinGUI::ClickButton::on_click() { state_ = true; }

bool WinGUI::ClickButton::was_clicked() {
	if (state_) {
		state_ = false;
		return true;
	}
	return false;
}

bool WinGUI::ClickButton::is_focused() const { return is_focus_set_; }


WinGUI::CheckBox::CheckBox(const Window &parent, std::string name, const int &x, const int &y) : Button(parent, x, y, std::move(name)) {

	id_ = Context::get_cb_buffer_size();

	if (id_ <= 0x0FFF)
		id_ |= CHECK_BOX;
	else
		throw WinGUI_ControlCountException("Cannot create a new check box. Maximum possible count was reached (4096).");

	const auto w_name = string_to_wstring(name_);


	handle_ = CreateWindowExW(
		0,
		L"BUTTON",
		w_name.c_str(),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		x_,
		y_,
		12 + name_.length() * 7,
		12,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	Context::register_gui_object(this);
}

void WinGUI::CheckBox::on_click() {
	state_ = !state_;

	if (state_)
		CheckDlgButton(parent_handle_, id_,BST_CHECKED);
	else
		CheckDlgButton(parent_handle_, id_,BST_UNCHECKED);
}

bool WinGUI::CheckBox::is_checked() const { return state_; }


WinGUI::RadioButton::RadioButton(const Window &parent, std::string name, const int &x, const int &y, bool new_group) :
	Control(parent, x, y, std::move(name)) {

	std::wstring rb_w_label = string_to_wstring(name_);
	DWORD style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;

	if (new_group) 
		style |= WS_GROUP;
	
	id_ = Context::get_new_rb_id();

	handle_ = CreateWindowExW(
		0,
		L"BUTTON",
		rb_w_label.c_str(),
		style,
		x,
		y,
		12 + name_.length() * 6,
		12,
		parent_handle_,
		(HMENU)id_,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
}

bool WinGUI::RadioButton::is_checked() const {
	UINT result = IsDlgButtonChecked(parent_handle_,id_);
	if(result == BST_CHECKED)
		return true;
	return false;
}


void WinGUI::TabsContainer::show_distinct(int index) const{

	for(auto &tab : tabs_)
		tab->hide_window();

	tabs_[index]->show_window();
	
}

WinGUI::TabsContainer::TabsContainer(const Window &parent, const int &x, const int &y, const int &width, const int &height)
	: Control(parent, x, y, ""), width_(width), height_(height), container_x_(x_ + 1), container_y_(y + 21),
	  container_width_(width_ - 4), container_height_(height_ - 23) {

	handle_ = CreateWindowEx(
		0,
		WC_TABCONTROL,
		0,
		WS_CHILD | WS_VISIBLE ,
		x_,
		y_,
		width_,
		height_,
		parent_handle_,
		(HMENU)TABS_CONTAINER,
		parent.get_instance(),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));

	Context::register_gui_object(this);
}

WinGUI::Window * WinGUI::TabsContainer::add_tab(std::string name) {


	Window *result = new Window(parent_handle_, GetModuleHandle(nullptr), container_x_, container_y_, container_width_,container_height_);
	
	std::wstring tab_w_name = string_to_wstring(name);
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
	TabCtrl_InsertItem(handle_, tabs_.size(), &tcitem);
	tabs_.emplace_back(result);
	delete [] text;
	return result;
	
}

int WinGUI::TabsContainer::get_container_x() const { return container_x_; }

int WinGUI::TabsContainer::get_container_y() const { return container_y_; }

int WinGUI::TabsContainer::get_container_width() const { return container_width_; }

int WinGUI::TabsContainer::get_container_height() const { return container_height_; }
