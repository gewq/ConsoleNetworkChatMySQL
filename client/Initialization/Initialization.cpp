#include "Initialization.h"

#include <string>

#include "../Settings/Settings.h"
#include "../User/User.h"
#include "../Message/Message.h"
#include "../Server/Server.h"

#include "Exceptions/TooManyInputParameters_Exception.h"
#include "Exceptions/NoInputParameters_Exceptions.h"



void ini::initialization(int argc, char** argv)
{
  //Не переданы параметры через командную строку
  if (argc < 2) {
    throw NoInputParameters_Exceptions();
  }
  //Слишком много параметров командной строки
  else if (argc > 2) {
    throw TooManyInputParameters_Exception();
  }

  const std::string inputFile = argv[1];
  settings::load(inputFile);

	user::test();
	message::test();

  server::initialize(settings::network::getPort(),
                      settings::network::getAddress());
}