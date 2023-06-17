#include "InputFileNotFound_Exception.h"


InputFileNotFound_Exception::InputFileNotFound_Exception(
  const std::string& fileName): std::exception(),
	errorMessage_("Error: Input file '" + fileName + "' not found")
{
}



const char* InputFileNotFound_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}