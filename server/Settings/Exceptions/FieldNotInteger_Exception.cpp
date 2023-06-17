#include "FieldNotInteger_Exception.h"


FieldNotInteger_Exception::FieldNotInteger_Exception(
  const std::string& fieldName,
  const std::string& fieldValue): std::exception(),
	errorMessage_("Error: Field '" + fieldName +
                "' : value '" + fieldValue + "' not integer")
{
}



const char* FieldNotInteger_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}