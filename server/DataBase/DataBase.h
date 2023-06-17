/**
\file DataBase.h
\brief Модуль "Взаимодествие с базой данных чата (MySQL)"
*/

#pragma once

#include <string>
#include <memory>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "../Message/Message.h"


namespace database {
  /**
  Задать настройки для работы с базой даных
  \param[in] url Адрес сервера с базой данных ("localhost"- если тот же ПК)
  \param[in] name Ник пользователя базы данных
  \param[in] password Пароль пользователя базы данных
  \param[in] database Название базы в MySQL
  */
  void initialize(const std::string& url,
                  const std::string& user,
                  const std::string& password,
                  const std::string& database);
                  
  /**
  Проверить есть ли в базе заданный Логин
  \param[in] login Логин
  \return Признак наличия заданного Логина в базе
  */
  bool isLoginRegistered(const std::string& login);

  /**
  Проверить соответствует ли Пароль заданному Логину
  \param[in] login Логин
  \param[in] passwordHash Хэш пароля
  \return Признак правильный ли Пароль
  */
  bool isPasswordRight(const std::string& login,
                        const std::string& passwordHash);

  /**
  Проверить есть ли в базе заданный Ник
  \param[in] name Ник пользователя
  \return Признак наличия заданного Ника в базе
  */
  bool isNicknameRegistered(const std::string& name);

  /**
  Вернуть Ник по Логину
  Если пользователь не зарегистрирован - возвращает пустую строку
  \param[in] login Логин
  \return Ник пользователя
  */
  std::string getNickname(const std::string& login);

  /**
  \return Количество зарегистрированных пользователей
  */
  size_t getNumberUsers();

  /**
  Загрузить имена зарегистрированных пользователей
  \param[in] userNames Умный указатель на вектор в который поместить имена пользователей
  */
  void loadUserNames(std::shared_ptr<std::vector<std::string> > userNames);

  /**
  Загрузить сообщения, адресованные заданному пользователю
  \param[in] login Логин пользователя
  \param[in] destination Указатель на список в который поместить сообщения
  */
  void loadMessages(const std::string& login,
                    std::shared_ptr<std::list<Message> >& messages);

  /**
  Добавить нового пользователя в базу
  \param[in] name Ник пользователя
  \param[in] login Логин пользователя
  \param[in] hash Хэш пароля
  */
  void addUser(const std::string& name,
              const std::string& login,
              const std::string& hash);

  /**
  Поместить в базу сообщение от одного пользователя другому
  \param[in] sender Ник отправителя
  \param[in] receiver Ник получателя
  \param[in] text Текст сообщения
  */
  void addMessage(const std::string& sender,
                  const std::string& receiver,
                  const std::string& text);

  /**
  Удалить аккаунт пользователя
  \param[in] login Логин пользователя
  */
  void removeUser(const std::string& login);

  /**
  Протестировать методы модуля
  \param[in] url Адрес сервера с базой данных ("localhost"- если тот же ПК)
  \param[in] name Ник пользователя базы данных
  \param[in] password Пароль пользователя базы данных
  */
  void test(const std::string& url,
            const std::string& user,
            const std::string& password);
}