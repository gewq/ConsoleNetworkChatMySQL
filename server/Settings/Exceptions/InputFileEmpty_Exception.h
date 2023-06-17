/**
\file InputFileEmpty_Exception.h
\brief Класс InputFileEmpty_Exception - класс-обработчик исключения "Входной файл пуст"
*/

#pragma once

#include <string>
#include <exception>

class InputFileEmpty_Exception : public std::exception {
  public:
    InputFileEmpty_Exception() = delete;

    /**
    \param[in] fileName Название входного файла
    */
    explicit InputFileEmpty_Exception(const std::string& fileName);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};