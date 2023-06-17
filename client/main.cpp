#include <memory>

#include "Initialization/Initialization.h"
#include "Chat/Chat.h"



int main(int argc, char** argv)
{
  try{
		ini::initialization(argc, argv);

    std::shared_ptr<bool> isRun = std::make_shared<bool>(true);
		Chat::getInstance()->attach(isRun);

		while (*isRun) {
			Chat::getInstance()->process();
		}
  }
	catch (std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Undefined exception" << std::endl;
	}
  return EXIT_SUCCESS;
}