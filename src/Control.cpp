#include <Control.h>
#include <Window.h>

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