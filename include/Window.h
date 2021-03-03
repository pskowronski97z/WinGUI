#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

namespace WinGUI {

	enum class Cursor {
		ARROW = 32512,
		ARROW_WAIT = 32650,
		CROSS = 32515,
		HAND = 32649,
		HELP = 32651,
		NO = 32648,
		WAIT = 32514,
		SIZE_WIDTH = 32644,
		SIZE_HEIGHT = 32645,
		SIZE_TO_LEFT = 32642,
		SIZE_TO_RIGHT = 32643,
		SIZE_ALL = 32646
	};

	enum class Style {
		MAXIMIZE = WS_MAXIMIZEBOX,
		MINIMIZE = WS_MINIMIZEBOX,
		SIZEABLE = WS_SIZEBOX,
		H_SCROLL = WS_HSCROLL,
		V_SCROLL = WS_VSCROLL
	};

	// Functions >
	std::wstring string_to_wstring(std::string source);
	std::string wchar_to_string(const wchar_t *text, const int &length);
	// < Functions

	// TODO: Zabezpieczenia + wyj¹tki
	class Window {
	private:
		std::string title_;
		HWND wnd_handle_;
		HWND parent_wnd_handle_;
		WNDCLASS wnd_class_;
		HINSTANCE instance_;
		DWORD wnd_style_;
		int width_;
		int height_;
		static LRESULT CALLBACK wnd_proc(HWND wnd_handle, UINT msg, WPARAM w_param, LPARAM l_param);
	public:
		Window(std::string title, const int &width, const int &height);
		Window(Window &parent, const int &x, const int &y, const int &width, const int &height) noexcept;
		Window(const HWND &parent_handle, const HINSTANCE &instance, const int &x, const int &y, const int &width,
		       const int &height);
		void show_window() const noexcept;
		void hide_window() const noexcept;
		HWND get_handle() const noexcept;
		HINSTANCE get_instance() const noexcept;
		int get_width() const noexcept;
		int get_height() const noexcept;
		bool set_icon(std::string file_path) const;
		void set_cursor(Cursor cursor_type) const noexcept;
		void set_style(Style wnd_style) noexcept;
		bool set_menu(unsigned int resource_id) const noexcept;
	};

	namespace IO {

		class Mouse {
			friend class Window;
		private:
			static HWND window_with_focus_;
			static POINT mouse_position_;
			static POINT mouse_delta_;
			static bool left_button_down_;
			static bool middle_button_down_;
			static bool right_button_down_;
			static int wheel_delta_;
		public:
			static int get_wheel_delta();
			static int get_position_x();
			static int get_position_y();
			static int get_position_dx();
			static int get_position_dy();
			static bool left_button_down();
			static bool right_button_down();
			static bool middle_button_down();
		};

		enum class Key : byte {
			BACKSPACE = 0X08,
			TAB = 0X09,
			CLEAR = 0X0C,
			ENTER = 0X0D,
			SHIFT = 0X10,
			CONTROL = 0X11,
			ALT = 0X12,
			PAUSE = 0X13,
			CAPSLOCK = 0X14,
			ESCAPE = 0X1B,
			SPACE = 0X20,
			PAGEUP = 0X21,
			PAGEDOWN = 0X22,
			END = 0X23,
			HOME = 0X24,
			LEFT = 0X25,
			UP = 0X26,
			RIGHT = 0X27,
			DOWN = 0X28,
			SELECT = 0X29,
			PRINT = 0X2A,
			EXECUTE = 0X2B,
			PRINTSCREEN = 0X2C,
			INSERT = 0X2D,
			DELETE_KEY = 0X2E,
			HELP = 0X2F,
			ZERO = 0X30,
			ONE = 0X31,
			TWO = 0X32,
			THREE = 0X33,
			FOUR = 0X34,
			FIVE = 0X35,
			SIX = 0X36,
			SEVEN = 0X37,
			EIGHT = 0X38,
			NINE = 0X39,
			A = 0X41,
			B = 0X42,
			C = 0X43,
			D = 0X44,
			E = 0X45,
			F = 0X46,
			G = 0X47,
			H = 0X48,
			I = 0X49,
			J = 0X4A,
			K = 0X4B,
			L = 0X4C,
			M = 0X4D,
			N = 0X4E,
			O = 0X4F,
			P = 0X50,
			Q = 0X51,
			R = 0X52,
			S = 0X53,
			T = 0X54,
			U = 0X55,
			V = 0X56,
			W = 0X57,
			X = 0X58,
			Y = 0X59,
			Z = 0X5A,
			LEFTWINDOWSKEY = 0X5B,
			RIGHTWINDOWSKEY = 0X5C,
			APPLICATIONSKEY = 0X5D,
			SLEEP = 0X5F,
			NUMPAD0 = 0X60,
			NUMPAD1 = 0X61,
			NUMPAD2 = 0X62,
			NUMPAD3 = 0X63,
			NUMPAD4 = 0X64,
			NUMPAD5 = 0X65,
			NUMPAD6 = 0X66,
			NUMPAD7 = 0X67,
			NUMPAD8 = 0X68,
			NUMPAD9 = 0X69,
			MULTIPLY = 0X6A,
			ADD = 0X6B,
			SEPERATOR = 0X6C,
			SUBTRACT = 0X6D,
			DECIMAL = 0X6E,
			DIVIDE = 0X6F,
			F1 = 0X70,
			F2 = 0X71,
			F3 = 0X72,
			F4 = 0X73,
			F5 = 0X74,
			F6 = 0X75,
			F7 = 0X76,
			F8 = 0X77,
			F9 = 0X78,
			F10 = 0X79,
			F11 = 0X7A,
			F12 = 0X7B,
			F13 = 0X7C,
			F14 = 0X7D,
			F15 = 0X7E,
			F16 = 0X7F,
			F17 = 0X80,
			F18 = 0X81,
			F19 = 0X82,
			F20 = 0X83,
			F21 = 0X84,
			F22 = 0X85,
			F23 = 0X86,
			F24 = 0X87,
			NUMLOCK = 0X90,
			SCROLLLOCK = 0X91,
			LEFTSHIFT = 0XA0,
			RIGHTSHIFT = 0XA1,
			LEFTCONTROL = 0XA2,
			RIGHTCONTOL = 0XA3,
			LEFTMENU = 0XA4,
			RIGHTMENU = 0XA5,
			BROWSERBACK = 0XA6,
			BROWSERFORWARD = 0XA7,
			BROWSERREFRESH = 0XA8,
			BROWSERSTOP = 0XA9,
			BROWSERSEARCH = 0XAA,
			BROWSERFAVORITES = 0XAB,
			BROWSERHOME = 0XAC,
			VOLUMEMUTE = 0XAD,
			VOLUMEDOWN = 0XAE,
			VOLUMEUP = 0XAF,
			NEXTTRACK = 0XB0,
			PREVIOUSTRACK = 0XB1,
			STOPMEDIA = 0XB2,
			PLAYPAUSE = 0XB3,
			LAUNCHMAIL = 0XB4,
			SELECTMEDIA = 0XB5,
			LAUNCHAPP1 = 0XB6,
			LAUNCHAPP2 = 0XB7,
			OEM1 = 0XBA,
			OEMPLUS = 0XB8,
			OEMCOMMA = 0XBC,
			OEMMINUS = 0XBD,
			OEMPERIOD = 0XBE,
			OEM2 = 0XBF,
			OEM3 = 0XC0,
			OEM4 = 0XDB,
			OEM5 = 0XDC,
			OEM6 = 0XDD,
			OEM7 = 0XDE,
			OEM8 = 0XDF,
			OEM102 = 0XE2,
			PROCESS = 0XE5,
			PACKET = 0XE7,
			ATTN = 0XF6,
			CRSEL = 0XF7,
			EXSEL = 0XF8,
			ERASEEOF = 0XF9,
			PLAY = 0XFA,
			ZOOM = 0XFB,
			PA1 = 0XFD,
			OEMCLEAR = 0XFE
		};

		class Keyboard {
			friend class Window;
		private:
			static HWND window_with_focus_;		
			static char typed_char_;
			static bool keys_down_[255];
			static bool keys_up_[255];
			static bool sys_keys_down_[255];
			static bool sys_keys_up_[255];

		public:
			static bool key_down(Key key);
			static bool key_up(Key key);
			static bool sys_key_down(Key key);
			static bool sys_key_up(Key key);
			static char get_typed_char();
			static void key_up_flush(Key key);
			static void sys_key_up_flush(Key key);
		};

	}
}
