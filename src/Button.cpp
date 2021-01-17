#include <Button.h>
#include <Context.h>

WinGui::Button::Button(Window parent, std::string btn_label, int x, int y, int width, int height)
	: parent_(parent.get_handle()),
	btn_label_(btn_label),
	x_(x),
	y_(y),
	width_(width),
	height_(height) {

	id_ = Context::get_btn_buffer_size();

	if (id_ <= 255) 
		id_ |= BUTTON;

	std::wstring btn_w_label = string_to_wstring(btn_label_);
	
	btn_handle_ = CreateWindowExW(
			0,
			L"BUTTON",
			btn_w_label.c_str(),
			WS_CHILD | WS_TABSTOP | WS_VISIBLE,
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
