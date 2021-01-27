#include <Buttons.h>

WinGui::Button::Button(const Window &parent, const int &x, const int &y, std::string name)
	: Control(parent, x, y, std::move(name)),
	  state_(false),
	  is_focus_set_(false) {}

void WinGui::Button::on_click()
{
}

void WinGui::Button::on_focus_set() { is_focus_set_ = true; }

void WinGui::Button::on_focus_lost() { is_focus_set_ = false; }


WinGui::ClickButton::ClickButton(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height)
	: Button(parent, x, y, std::move(name)),
	  width_(width),
	  height_(height) {

	id_ = Context::get_btn_buffer_size();

	if (id_ <= 255)
		id_ |= BUTTON;
	// else exception

	const auto w_name = string_to_wstring(name_);

	handle_ = CreateWindowExW(
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

void WinGui::ClickButton::on_click() { state_ = true; }

bool WinGui::ClickButton::was_clicked() {
	if (state_) {
		state_ = false;
		return true;
	}
	return false;
}

bool WinGui::ClickButton::is_focused() const { return is_focus_set_; }


WinGui::CheckBox::CheckBox(const Window &parent, std::string name, const int &x, const int &y) : Button(parent, x, y, std::move(name)) {

	id_ = Context::get_cb_buffer_size();

	if (id_ <= 255)
		id_ |= CHECK_BOX;
	// else exception

	const auto w_name = string_to_wstring(name_);


	handle_ = CreateWindowExW(
		0,
		L"BUTTON",
		w_name.c_str(),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		x,
		y,
		12 + name_.length() * 6,
		12,
		parent_handle_,
		(HMENU)id_,
		GetModuleHandle(nullptr),
		nullptr);

	SendMessage(handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	Context::register_gui_object(this);
}

void WinGui::CheckBox::on_click() {
	state_ = !state_;

	if (state_)
		CheckDlgButton(parent_handle_, id_,BST_CHECKED);
	else
		CheckDlgButton(parent_handle_, id_,BST_UNCHECKED);
}

bool WinGui::CheckBox::is_checked() const { return state_; }


WinGui::RadioButton::RadioButton(const Window &parent, std::string name, const int &x, const int &y, bool new_group) :
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

bool WinGui::RadioButton::is_checked() const {
	UINT result = IsDlgButtonChecked(parent_handle_,id_);
	if(result == BST_CHECKED)
		return true;
	return false;
}