#include "InputFileEmpty_Exception.h"


InputFileEmpty_Exception::InputFileEmpty_Exception(
  const std::string& fileName): std::exception(),
	errorMessage_("Error: Input file '" + fileName + "' empty")
{
}



const char* InputFileEmpty_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}