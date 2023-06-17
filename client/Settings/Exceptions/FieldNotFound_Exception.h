/**
\file FieldNotFound_Exception.h
\brief Класс FieldNotFound_Exception - класс-обработчик исключения "Отсутствует поле во входном файле"
*/

#pragma once

#include <string>
#include <exception>

class FieldNotFound_Exception : public std::exception {
  public:
    FieldNotFound_Exception() = delete;

    /**
    \param[in] fileName Название входного файла
    \param[in] fieldName Название поля входного файла
    */
    explicit FieldNotFound_Exception(const std::string& fileName,
                                    const std::string& fieldName);

    virtual const char* what() const noexcept override;

  private:
    std::string errorMessage_;	///<Сообщение об ошибке
};