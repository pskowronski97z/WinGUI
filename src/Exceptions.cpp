#include <Exceptions.h>

WinGUI::Exception::Exception(std::string type, std::string description, std::string file_name, const unsigned int &line)
	: type_("< WinGUI EXCEPTION: "), file_name_(std::move(file_name)), description_(std::move(description)), line_(line) {

	type_ += type;

	int lenght = 0;
	char *ch;
	for(ch = const_cast<char*>(file_name_.c_str()) + file_name_.size() + 1; *ch != '\\'; ch-- ){}

	lenght = file_name_.c_str() + file_name_.size() + 1 - ch; 

	file_name_ = std::string(file_name_.c_str() + file_name_.size() + 2 - lenght);
}

char const* WinGUI::Exception::what() const{

	std::string message = type_+ " >< " + file_name_ + " at line " + std::to_string(line_) + " >: " + description_ + "\n";

	char *result = new char[message.size() + 1];
	strcpy_s(result, message.size() + 1, message.c_str());

	return result;
}
