#include <Context.h>
#include <Buttons.h>

std::vector<WinGui::Window*> WinGui::Context::wnd_pointers_(0);
std::vector<WinGui::ClickButton*> WinGui::Context::btn_pointers_(0);
std::vector<WinGui::CheckBox*> WinGui::Context::cb_pointers_(0);

bool WinGui::Context::register_gui_object(Window* wnd_pointer) {
	if(wnd_pointer==nullptr)
		return false;
	
	wnd_pointers_.emplace_back(wnd_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(ClickButton* btn_pointer) {
	if(btn_pointer==nullptr)
		return false;
	
	btn_pointers_.emplace_back(btn_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(CheckBox* cb_pointer) {
	if(cb_pointer == nullptr)
		return false;
	
	cb_pointers_.emplace_back(cb_pointer);
	return true;
}

int WinGui::Context::get_btn_buffer_size() {
	return btn_pointers_.size();
}

int WinGui::Context::get_cb_buffer_size() {
	return cb_pointers_.size();
}

WinGui::ClickButton* WinGui::Context::get_btn_pointer(int index) {
	if(index>=btn_pointers_.size())
		return nullptr;

	return btn_pointers_[index];
}

WinGui::ClickButton* WinGui::Context::get_btn_pointer(HWND handle) {

	for(auto &button : btn_pointers_) 
		if(button->get_handle() == handle)
			return button;

	return nullptr;
}

WinGui::CheckBox* WinGui::Context::get_cb_pointer(int index) {
	if(index>=cb_pointers_.size())
		return nullptr;

	return cb_pointers_[index];
}

WinGui::CheckBox* WinGui::Context::get_cb_pointer(HWND handle) {
	for(auto &check_box : cb_pointers_) 
		if(check_box->get_handle() == handle)
			return check_box;

	return nullptr;
}
