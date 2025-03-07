/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 3: Calculadora para números grandes
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 06/03/2025

** Archivo big_number_exception.h: Declaracion de la clase que maneja los errores 
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      07/03/2025 - Creacion (primera version) del codigo
**      07/03/2025 - Adicion de excepcion para formato de racional incorrecto
**/

#ifndef BIG_NUMBER_EXCEPTION_H
#define BIG_NUMBER_EXCEPTION_H

#include <exception>
#include <string>


/**
 * @brief Class BigNumberException that heritages from std:exception, it has a protected member which is the message to be thrown if an error occurs
 */
class BigNumberException : public std::exception {
 protected:
  std::string error_message_;
 public:
  explicit BigNumberException (const std::string& error_message) : error_message_(error_message) {}
  const char* what() const throw() {return error_message_.c_str();}
};


/**
 * @brief Class BigNumberBadDigit. Throws an exception if there is an invalid digit
 */
class BigNumberBadDigit : public BigNumberException {
 public:
  explicit BigNumberBadDigit(char digit) : BigNumberException("Error: Invalid digit in the number: " + std::string(1, digit)) {}
};


/**
 * @brief Class BigNumberNotRecognized. Throws an exception if sufix is not recognized when creating an object
 */
class BigNumberNotRecognized : public BigNumberException {
  public:
   explicit BigNumberNotRecognized(char digit) : BigNumberException("Error: Format not recognized, sufix must be 'u', 'i' or 'r'. Sufix detected: " + std::string(1, digit)) {}
 };
 

/**
 * @brief Class BigNumberDivisionByZero. Throws an expection if there is a division by zero
 */
class BigNumberDivisionByZero : public BigNumberException {
 public:
  BigNumberDivisionByZero() : BigNumberException("Error: Division by cero") {}
};


/**
 * @brief Class BigNumberDivisionByZero. Throws an expection if there is a division by zero
 */
class BigNumberRationalFormat : public BigNumberException {
 public:
  BigNumberRationalFormat() : BigNumberException("Error: Incorrect format.It must be 'numerator / denominator'") {}
};


#endif