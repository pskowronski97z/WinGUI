#include <Context.h>

#include "Button.h"

std::vector<WinGui::Window*> WinGui::Context::wnd_pointers_(0);
std::vector<WinGui::Button*> WinGui::Context::btn_pointers_(0);
  
bool WinGui::Context::register_gui_object(Window* wnd_pointer) {
	if(wnd_pointer==nullptr)
		return false;
	
	wnd_pointers_.emplace_back(wnd_pointer);
	return true;
}

bool WinGui::Context::register_gui_object(Button* btn_pointer) {
	if(btn_pointer==nullptr)
		return false;
	
	btn_pointers_.emplace_back(btn_pointer);
	return true;
}

int WinGui::Context::get_btn_buffer_size() {
	return btn_pointers_.size();
}

WinGui::Button* WinGui::Context::get_btn_pointer(int index) {
	if(index>=btn_pointers_.size())
		return nullptr;

	return btn_pointers_[index];
}

WinGui::Button* WinGui::Context::get_btn_pointer(HWND handle) {

	for(auto &button : btn_pointers_) 
		if(button->get_btn_handle() == handle)
			return button;

	return nullptr;
}
