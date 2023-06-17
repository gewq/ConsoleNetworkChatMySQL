/**
\file InputFileNotFound_Exception.h
\brief Класс InputFileNotFound_Exception - класс-обработчик исключения "Входной файл не найден"
*/

#pragma once

#include <string>
#include <exception>

class InputFileNotFound_Exception : public std::exception {
  public:
    InputFileNotFound_Exception() = delete;

    /**
    \param[in] fileName Название входного файла
    */
    explicit InputFileNotFound_Exception(const std::string& fileName);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};