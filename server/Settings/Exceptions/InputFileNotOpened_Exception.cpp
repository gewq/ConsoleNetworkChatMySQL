#include "InputFileNotOpened_Exception.h"


InputFileNotOpened_Exception::InputFileNotOpened_Exception(
  const std::string& fileName): std::exception(),
	errorMessage_("Error: Input file (" + fileName + ") not opened")
{
}



const char* InputFileNotOpened_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}