#include "TooManyInputParameters_Exception.h"



TooManyInputParameters_Exception::TooManyInputParameters_Exception() : std::exception()
{
}



const char* TooManyInputParameters_Exception::what() const noexcept
{
	return "Error: Too many input parameters";
}