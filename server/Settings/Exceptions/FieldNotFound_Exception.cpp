#include "FieldNotFound_Exception.h"


FieldNotFound_Exception::FieldNotFound_Exception(
  const std::string& fileName,
  const std::string& fieldName): std::exception(),
	errorMessage_("Error: Input file '" + fileName +
                "' : field '" + fieldName + "' not found")
{
}



const char* FieldNotFound_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}