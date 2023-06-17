#include "NoInputParameters_Exceptions.h"



NoInputParameters_Exceptions::NoInputParameters_Exceptions() : std::exception()
{
}



const char* NoInputParameters_Exceptions::what() const noexcept
{
	return "Error: No input parameters. Specify input file";
}