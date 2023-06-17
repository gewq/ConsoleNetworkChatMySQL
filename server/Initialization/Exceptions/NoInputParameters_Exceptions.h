/**
\file NoInputParameters_Exceptions.h
\brief Класс NoInputParameters_Exceptions - класс-обработчик исключения "Не переданы параметры через командную строку"
*/

#pragma once

#include <string>
#include <exception>

class NoInputParameters_Exceptions : public std::exception {
  public:
    NoInputParameters_Exceptions();

    virtual const char* what() const noexcept override;
};