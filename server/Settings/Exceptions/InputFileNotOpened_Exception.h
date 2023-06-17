/**
\file InputFileNotOpened_Exception.h
\brief Класс InputFileNotOpened_Exception - класс-обработчик исключения "Ошибка открытия входного файла"
*/

#pragma once

#include <string>
#include <exception>

class InputFileNotOpened_Exception : public std::exception {
  public:
    InputFileNotOpened_Exception() = delete;

    /**
    \param[in] fileName Название входного файла
    */
    explicit InputFileNotOpened_Exception(const std::string& fileName);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};