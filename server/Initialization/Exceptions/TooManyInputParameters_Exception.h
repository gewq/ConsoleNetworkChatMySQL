/**
\file TooManyInputParameters_Exception.h
\brief Класс TooManyInputParameters_Exception - класс-обработчик исключения "Слишком много входных аргументов"
*/

#pragma once

#include <string>
#include <exception>

class TooManyInputParameters_Exception : public std::exception {
  public:
    TooManyInputParameters_Exception();

    virtual const char* what() const noexcept override;
};