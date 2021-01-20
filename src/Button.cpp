#include <Button.h>
#include <Context.h>

WinGui::Button::Button(Window parent, std::string btn_label, int x, int y, int width, int height)
	: parent_(parent.get_handle()),
	btn_label_(btn_label),
	x_(x),
	y_(y),
	width_(width),
	height_(height),
	is_focus_set_(false),
	was_clicked_(false),
	was_dbclicked_(false) {

	
	id_ = Context::get_btn_buffer_size();

	if (id_ <= 255) 
		id_ |= BUTTON;
	// else exception

	std::wstring btn_w_label = string_to_wstring(btn_label_);
	
	btn_handle_ = CreateWindowExW(
			0,
			L"BUTTON",
			btn_w_label.c_str(),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_NOTIFY,
			x_,
			y_,
			width_,
			height_,
			parent_,
			(HMENU)id_,
			parent.get_instance(),
			nullptr);

	SendMessage(btn_handle_, WM_SETFONT, (WPARAM)((HFONT)GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));

	Context::register_gui_object(this);
	
}

HWND WinGui::Button::get_btn_handle() const {
	return btn_handle_;
}

void WinGui::Button::on_click() {
	was_clicked_ = true;
}

void WinGui::Button::on_double_click() {
	was_dbclicked_ = true;
}

void WinGui::Button::on_focus_set() {
	is_focus_set_ = true;
}

void WinGui::Button::on_focus_lost() {
	is_focus_set_ = false;
}

bool WinGui::Button::was_clicked() {
	if(was_clicked_) {
		was_clicked_ = false;
		return true;
	}	
	return false;
}

bool WinGui::Button::was_dbclicked()
{
	if(was_dbclicked_) {
		was_dbclicked_ = false;
		return true;
	}	
	return false;
}

bool WinGui::Button::is_focused() const {
	return is_focus_set_;
}

std::string WinGui::Button::get_name() const {
	return btn_label_;
}
