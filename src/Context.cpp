#include <Context.h>
#include <Buttons.h>
#include <Exceptions.h>
#include "Lists.h"

std::vector<WinGUI::ClickButton*> WinGUI::Context::btn_pointers_(0);
std::vector<WinGUI::CheckBox*> WinGUI::Context::cb_pointers_(0);
std::vector<WinGUI::Input<std::string>*> WinGUI::Context::rt_pointers_(0);
std::vector<WinGUI::Input<float>*> WinGUI::Context::fp_in_pointers_(0);
std::vector<WinGUI::Input<int>*> WinGUI::Context::int_in_pointers_(0);
std::vector<WinGUI::TabsContainer*> WinGUI::Context::tab_container_pointers(0);
std::vector<WinGUI::TreeView*> WinGUI::Context::tv_pointers(0);
unsigned short WinGUI::Context::rb_counter_ = 1;


bool WinGUI::Context::register_gui_object(ClickButton *btn_pointer) {
	if (btn_pointer == nullptr)
		return false;

	btn_pointers_.emplace_back(btn_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(CheckBox *cb_pointer) {
	if (cb_pointer == nullptr)
		return false;

	cb_pointers_.emplace_back(cb_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(Input<std::string> *rt_pointer) {
	if (rt_pointer == nullptr)
		return false;

	rt_pointers_.emplace_back(rt_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(Input<float> *fp_in_pointer) {
	if (fp_in_pointer == nullptr)
		return false;

	fp_in_pointers_.emplace_back(fp_in_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(Input<int> *int_in_pointer) {
	if (int_in_pointer == nullptr)
		return false;

	int_in_pointers_.emplace_back(int_in_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(TabsContainer *tab_container_pointer) {
	if (tab_container_pointer == nullptr)
		return false;

	tab_container_pointers.emplace_back(tab_container_pointer);
	return true;
}

bool WinGUI::Context::register_gui_object(TreeView* tv_pointer) {
	if (tv_pointer == nullptr)
		return false;

	tv_pointers.emplace_back(tv_pointer);
	return true;
}


int WinGUI::Context::get_btn_buffer_size() { return btn_pointers_.size(); }

int WinGUI::Context::get_cb_buffer_size() { return cb_pointers_.size(); }

int WinGUI::Context::get_rt_buffer_size() { return rt_pointers_.size(); }

int WinGUI::Context::get_fp_in_buffer_size() { return fp_in_pointers_.size(); }

int WinGUI::Context::get_int_in_buffer_size() { return int_in_pointers_.size(); }

int WinGUI::Context::get_tab_cont_buffer_size() { return tab_container_pointers.size(); }


unsigned short WinGUI::Context::get_new_rb_id() {

	if(rb_counter_ > 0x0FFF)
		throw WinGUI_ControlCountException("Cannot create a new radio button. Maximum possible count was reached (4095).");

	unsigned short result = RADIO_BUTTON | rb_counter_;
	rb_counter_++;

	return result;
}

WinGUI::ClickButton *WinGUI::Context::get_btn_pointer(int index) {
	if (index >= btn_pointers_.size())
		return nullptr;

	return btn_pointers_[index];
}

WinGUI::ClickButton *WinGUI::Context::get_btn_pointer(HWND handle) {

	for (auto &button : btn_pointers_)
		if (button->get_handle() == handle)
			return button;

	return nullptr;
}

WinGUI::CheckBox *WinGUI::Context::get_cb_pointer(int index) {
	if (index >= cb_pointers_.size())
		return nullptr;

	return cb_pointers_[index];
}

WinGUI::CheckBox *WinGUI::Context::get_cb_pointer(HWND handle) {
	for (auto &check_box : cb_pointers_)
		if (check_box->get_handle() == handle)
			return check_box;

	return nullptr;
}

WinGUI::Input<std::string> *WinGUI::Context::get_rt_pointer(int index) {
	if (index >= rt_pointers_.size())
		return nullptr;

	return rt_pointers_[index];
}

WinGUI::Input<float> *WinGUI::Context::get_fp_in_pointer(int index) {
	if (index >= fp_in_pointers_.size())
		return nullptr;

	return fp_in_pointers_[index];
}

WinGUI::Input<int> *WinGUI::Context::get_int_in_pointer(int index) {
	if (index >= int_in_pointers_.size())
		return nullptr;

	return int_in_pointers_[index];
}

WinGUI::TabsContainer *WinGUI::Context::get_tab_cont_pointer(HWND handle) {
	for(auto &tab_container_pointer : tab_container_pointers)
		if(tab_container_pointer->get_handle() == handle)
			return tab_container_pointer;
	return nullptr;
}

WinGUI::TreeView* WinGUI::Context::get_tv_pointer(HWND handle) {
	for(auto &tv : tv_pointers)
		if(tv->get_handle() == handle)
			return tv;
	return nullptr;
}
