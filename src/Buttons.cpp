#include <Buttons.h>
#include <Context.h>


WinGui::Button::Button(const Window &parent, const int &x, const int &y, std::string name)
	:
	handle_(nullptr),
	parent_handle_(parent.get_handle()),
	id_(0),
	x_(x),
	y_(y),
	name_(std::move(name)),
	state_(false),
	is_focus_set_(false){}

int WinGui::Button::get_x_pos() const {
	return x_;
}

int WinGui::Button::get_y_pos() const {
	return y_;
}

std::string WinGui::Button::get_name() const {
	return name_;
}

HWND WinGui::Button::get_handle() const {
	return handle_;
}

void WinGui::Button::on_focus_set() { is_focus_set_ = true; }

void WinGui::Button::on_focus_lost() { is_focus_set_ = false; }

DWORD WinGui::Button::get_id() const {
	return id_;
}



WinGui::ClickButton::ClickButton(const Window &parent, std::string name, const int &x, const int &y, const int &width, const int &height)
	: Button(parent, x, y, std::move(name)),
	  width_(width),
	  height_(height){

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

void WinGui::ClickButton::on_click() {
	state_ = true;
}

bool WinGui::ClickButton::was_clicked() {
	if(state_) {
		state_ = false;
		return true;
	}	
	return false;
}

bool WinGui::ClickButton::is_focused() const {
	return is_focus_set_;
}



WinGui::CheckBox::CheckBox(const Window& parent, std::string name, const int& x, const int& y) : Button(parent, x, y, std::move(name)) {

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
	
}

void WinGui::CheckBox::on_click() {
	state_ = true;
}