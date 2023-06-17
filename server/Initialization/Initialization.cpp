#include "Initialization.h"

#include "../Settings/Settings.h"
#include "../Network/Network.h"
#include "../DataBase/DataBase.h"

#include "Exceptions/TooManyInputParameters_Exception.h"
#include "Exceptions/NoInputParameters_Exceptions.h"



void initialization(int argc, char** argv)
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

  database::test(settings::database::getAddress(),
                  settings::database::getUserName(),
                  settings::database::getUserPassword());
  database::initialize(settings::database::getAddress(),
                        settings::database::getUserName(),
                        settings::database::getUserPassword(),
                        settings::database::getDatabaseName());
  network::initialize(settings::network::getPort());
}