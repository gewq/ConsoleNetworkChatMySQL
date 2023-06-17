/**
\file Settings.h
\brief Модуль "Настроки" - хранит настройки и предоставляет к ним доступ, загружает из файла
*/

#include <string>


namespace settings{
  /**
  Загрузить настройки из файла *.xml
  \param[in] inputFile Наименование файла с настройками
  */
  void load(const std::string& inputFile);

  namespace network{
    /**
    \return Номер порта сервера чата
    */
    unsigned int getPort();
  }

  namespace database{
    /**
    \return Адрес сервера MySQL
    */
    std::string getAddress();

    /**
    \return Наименование базы данных (MySQL) чата
    */
    std::string getDatabaseName();

    /**
    \return Имя пользователя MySQL
    */
    std::string getUserName();

    /**
    \return Пароль пользователя MySQL
    */
    std::string getUserPassword();
  }
}