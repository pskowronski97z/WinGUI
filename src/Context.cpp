#include <Context.h>
#include <Buttons.h>

std::vector<WinGui::Window*> WinGui::Context::wnd_pointers_(0);
std::vector<WinGui::ClickButton*> WinGui::Context::btn_pointers_(0);
std::vector<WinGui::CheckBox*> WinGui::Context::cb_pointers_(0);
std::vector<WinGui::Input<std::string>*> WinGui::Context::rt_pointers_(0);
std::vector<WinGui::Input<float>*> WinGui::Context::fp_in_pointers_(0);
std::vector<WinGui::Input<int>*> WinGui::Context::int_in_pointers_(0);
byte WinGui::Context::rb_counter_ = 0;


bool WinGui::Context::register_gui_object(Window *wnd_pointer) {
	if (wnd_pointer == nullptr)
		return false;

	wnd_pointers_.emplace_back(wnd_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(ClickButton *btn_pointer) {
	if (btn_pointer == nullptr)
		return false;

	btn_pointers_.emplace_back(btn_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(CheckBox *cb_pointer) {
	if (cb_pointer == nullptr)
		return false;

	cb_pointers_.emplace_back(cb_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(Input<std::string> *rt_pointer) {
	if (rt_pointer == nullptr)
		return false;

	rt_pointers_.emplace_back(rt_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(Input<float> *fp_in_pointer) {
	if (fp_in_pointer == nullptr)
		return false;

	fp_in_pointers_.emplace_back(fp_in_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(Input<int> *int_in_pointer) {
	if (int_in_pointer == nullptr)
		return false;

	int_in_pointers_.emplace_back(int_in_pointer);
	return true;
}



int WinGui::Context::get_btn_buffer_size() { return btn_pointers_.size(); }

int WinGui::Context::get_cb_buffer_size() { return cb_pointers_.size(); }

int WinGui::Context::get_rt_buffer_size() { return rt_pointers_.size(); }

int WinGui::Context::get_fp_in_buffer_size() { return fp_in_pointers_.size(); }

int WinGui::Context::get_int_in_buffer_size() { return int_in_pointers_.size(); }


unsigned short WinGui::Context::get_new_rb_id() {

	//if(radio_button_counter_ == 255)
	//exc

	unsigned short result = RADIO_BUTTON | rb_counter_;
	rb_counter_++;

	return result;
}


WinGui::ClickButton *WinGui::Context::get_btn_pointer(int index) {
	if (index >= btn_pointers_.size())
		return nullptr;

	return btn_pointers_[index];
}

WinGui::ClickButton *WinGui::Context::get_btn_pointer(HWND handle) {

	for (auto &button : btn_pointers_)
		if (button->get_handle() == handle)
			return button;

	return nullptr;
}

WinGui::CheckBox *WinGui::Context::get_cb_pointer(int index) {
	if (index >= cb_pointers_.size())
		return nullptr;

	return cb_pointers_[index];
}

WinGui::CheckBox *WinGui::Context::get_cb_pointer(HWND handle) {
	for (auto &check_box : cb_pointers_)
		if (check_box->get_handle() == handle)
			return check_box;

	return nullptr;
}

WinGui::Input<std::string> *WinGui::Context::get_rt_pointer(int index) {
	if (index >= rt_pointers_.size())
		return nullptr;

	return rt_pointers_[index];
}

WinGui::Input<float> *WinGui::Context::get_fp_in_pointer(int index) {
	if (index >= fp_in_pointers_.size())
		return nullptr;

	return fp_in_pointers_[index];
}

WinGui::Input<int> *WinGui::Context::get_int_in_pointer(int index) {
	if (index >= int_in_pointers_.size())
		return nullptr;

	return int_in_pointers_[index];
}