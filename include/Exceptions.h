#pragma once
#include <exception>
#include <string>

namespace WinGUI {

	class Exception : public std::exception {
	private:
		std::string type_;
		std::string file_name_;
		std::string description_;
		unsigned int line_;

	public:
		Exception(std::string type, std::string description, std::string file_name, const unsigned int &line);
		char const *what() const override;
		
	};
}

#define WinGUI_InvalidArgumentException(description) WinGUI::Exception("Invalid argument", description, __FILE__, __LINE__)
#define WinGUI_ControlCountException(description) WinGUI::Exception("Maximum count of controls", description, __FILE__, __LINE__)