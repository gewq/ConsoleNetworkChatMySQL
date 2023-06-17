/**
\file FieldNotInteger_Exception.h
\brief Класс FieldNotInteger_Exception - класс-обработчик исключения "Поле во входном файле не целое число"
*/

#pragma once

#include <string>
#include <exception>

class FieldNotInteger_Exception : public std::exception {
  public:
    FieldNotInteger_Exception() = delete;

    /**
    Параметризованный конструктор.
    На основе переданного названия поля
    формирует сообщение об ошибке
    \param[in] fieldName Название поля
    \param[in] fieldValue Значение в поле
    */
    explicit FieldNotInteger_Exception(const std::string& fieldName,
                                      const std::string& fieldValue);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};