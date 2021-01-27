#include <Context.h>
#include <Buttons.h>


WinGui::Control::Control(const Window& parent, const int& x, const int& y, std::string name)
	: handle_(nullptr),
	  parent_handle_(parent.get_handle()),
	  id_(0),
	  x_(x),
	  y_(y),
	  name_(std::move(name)) {}

int WinGui::Control::get_x_pos() const { return x_; }

int WinGui::Control::get_y_pos() const { return y_; }

std::string WinGui::Control::get_name() const { return name_; }

HWND WinGui::Control::get_handle() const { return handle_; }

DWORD WinGui::Control::get_id() const { return id_; }


std::vector<WinGui::Window*> WinGui::Context::wnd_pointers_(0);
std::vector<WinGui::ClickButton*> WinGui::Context::btn_pointers_(0);
std::vector<WinGui::CheckBox*> WinGui::Context::cb_pointers_(0);
byte WinGui::Context::rb_counter_ = 0;

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

int WinGui::Context::get_btn_buffer_size() { return btn_pointers_.size(); }

int WinGui::Context::get_cb_buffer_size() { return cb_pointers_.size(); }

WinGui::ClickButton* WinGui::Context::get_btn_pointer(int index) {
	if(index>=btn_pointers_.size())
		return nullptr;

	return btn_pointers_[index];
}

unsigned short WinGui::Context::get_new_rb_id() {

	//if(radio_button_counter_ == 255)
		//exc

	unsigned short result = RADIO_BUTTON | rb_counter_;
	rb_counter_++;

	return result;
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
