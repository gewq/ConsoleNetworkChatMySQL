#include "Settings.h"

#include <iostream>

#include "TinyXML/tinyxml2.h" //XML парсер
#include "Exceptions/InputFileNotFound_Exception.h"
#include "Exceptions/InputFileNotOpened_Exception.h"
#include "Exceptions/InputFileEmpty_Exception.h"
#include "Exceptions/FieldNotFound_Exception.h"
#include "Exceptions/FieldNotInteger_Exception.h"


namespace{
  namespace network{
    unsigned int PORT = 9999;
  }

  namespace database{
    std::string ADDRESS = "";
    std::string NAME = "";
    std::string USER = "";
    std::string PASSWORD = "";
  }
}


//Загрузить из файла значение порта сервера
static void loadPort(const std::string& fileName,
                      tinyxml2::XMLNode* root);

//Загрузить из файла значение адреса сервера MySQL
static void loadAddress(const std::string& fileName,
                        tinyxml2::XMLNode* root);

//Загрузить из файла наименование базы данных (MySQL) чата
static void loadDatabaseName(const std::string& fileName,
                              tinyxml2::XMLNode* root);

//Загрузить из файла имя пользователя MySQL
static void loadUserName(const std::string& fileName,
                          tinyxml2::XMLNode* root);

//Загрузить из файла имя пользователя MySQL
static void loadUserPassword(const std::string& fileName,
                              tinyxml2::XMLNode* root);

//Проверить ошибки чтения входного файла
static void checkErrors(const std::string& fileName,
                        const tinyxml2::XMLError& errorCode);


void settings::load(const std::string& inputFile)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError errorCode = doc.LoadFile(inputFile.c_str());
  checkErrors(inputFile, errorCode);

  tinyxml2::XMLNode* root = doc.FirstChild();
  //Файл пуст
  if (!root){
    throw InputFileEmpty_Exception(inputFile);
  }

  loadPort(inputFile, root);
  loadAddress(inputFile, root);
  loadDatabaseName(inputFile, root);
  loadUserName(inputFile, root);
  loadUserPassword(inputFile, root);
}



unsigned int settings::network::getPort()
{
  return ::network::PORT;
}



std::string settings::database::getAddress()
{
  return ::database::ADDRESS;
}



std::string settings::database::getDatabaseName()
{
  return ::database::NAME;
}



std::string settings::database::getUserName()
{
  return ::database::USER;
}



std::string settings::database::getUserPassword()
{
  return ::database::PASSWORD;
}


//=============================================================================
static void loadPort(const std::string& fileName,
                    tinyxml2::XMLNode* root)
{
  //Поле не найдено
  tinyxml2::XMLElement* pElement = root->FirstChildElement("NETWORK");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "NETWORK");
  }
  pElement = pElement->FirstChildElement("PORT");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "NETWORK-PORT");
  }

  const std::string fieldValue = pElement->GetText();
  const std::string fieldName = "NETWORK-PORT";

  //Не целое число
  size_t pos = fieldValue.find_first_not_of("0123456789");
  if (pos != std::string::npos) {
    throw FieldNotInteger_Exception(fieldName, fieldValue);
  }

  ::network::PORT = std::stoi(fieldValue);
}



static void loadAddress(const std::string& fileName,
                    tinyxml2::XMLNode* root)
{
  //Поле не найдено
  tinyxml2::XMLElement* pElement = root->FirstChildElement("DATABASE");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE");
  }
  pElement = pElement->FirstChildElement("ADDRESS");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE-ADDRESS");
  }

  ::database::ADDRESS = pElement->GetText();
}



static void loadDatabaseName(const std::string& fileName,
                    tinyxml2::XMLNode* root)
{
  //Поле не найдено
  tinyxml2::XMLElement* pElement = root->FirstChildElement("DATABASE");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE");
  }
  pElement = pElement->FirstChildElement("NAME");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE-NAME");
  }

  ::database::NAME = pElement->GetText();
}



static void loadUserName(const std::string& fileName,
                    tinyxml2::XMLNode* root)
{
  //Поле не найдено
  tinyxml2::XMLElement* pElement = root->FirstChildElement("DATABASE");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE");
  }
  pElement = pElement->FirstChildElement("USER");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE-USER");
  }

  ::database::USER = pElement->GetText();
}



static void loadUserPassword(const std::string& fileName,
                    tinyxml2::XMLNode* root)
{
  //Поле не найдено
  tinyxml2::XMLElement* pElement = root->FirstChildElement("DATABASE");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE");
  }
  pElement = pElement->FirstChildElement("PASSWORD");
  if (!pElement){
    throw FieldNotFound_Exception(fileName, "DATABASE-PASSWORD");
  }

  ::database::PASSWORD = pElement->GetText();
}



static void checkErrors(const std::string& fileName,
                        const tinyxml2::XMLError& errorCode)
{
  if (errorCode != tinyxml2::XMLError::XML_SUCCESS){
    switch (errorCode){
      case tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND:
        throw InputFileNotFound_Exception(fileName);
        break;
      case tinyxml2::XMLError::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
        throw InputFileNotOpened_Exception(fileName);
        break;
      default:
        break;
    }    
  }
}