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

** Archivo big_number.h: Declaracion de la clase interfaz que tiene como clases derivadas a BigInteger, BigUnsigned y BigRational
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      06/03/2025 - Creacion (primera version) del codigo
**      09/03/2025 - Adicion del destructor
**      11/03/2025 - Manejo de excepciones de constructores en metodo create
**/

#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <sstream>
#include "big_number_exception.h"

// Forward declarations
template <unsigned char Base> class BigUnsigned;
template <unsigned char Base> class BigInteger;
template <unsigned char Base> class BigRational;

template <unsigned char Base = 10> class BigNumber {
 protected:
  virtual std::ostream& write(std::ostream&) const = 0; // Write method, it'll be specified in each sub-class
  virtual std::istream& read(std::istream&) = 0; // Read method, it'll be specified in each sub-class
  friend std::ostream& operator<<(std::ostream& os, const BigNumber<Base>& num) {return num.write(os);} // General << operator, invokes "write"
  friend std::istream& operator>>(std::istream& is, BigNumber<Base>& num) { return num.read(is);} // General >> operator, invokes "read"

 public:
  // Virtual destructor
  virtual ~BigNumber() {}
  // Virtual calculation methods to override in each sub-class
  virtual BigNumber<Base>& add(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& subtract(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& multiply(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& divide(const BigNumber<Base>&) const = 0;
  // Virtual change-type operators to override in each sub-class
  virtual operator BigUnsigned<Base>() const = 0;
  virtual operator BigInteger<Base>() const = 0;
  virtual operator BigRational<Base>() const = 0;
  // Method to create an object depending on the sufix of a string
  static BigNumber<Base>* create(const char* string);
};


/**
 * @brief Method that creates an object depending on the sufix of the array:
 *        -u: Creates a BigUnsigned object
 *        -i: Creates a BigInteger object
 *        -r: Creates a BigRational object
 * @param const_char* array of characters to be read
 * @return pointer to the new memory direction
 */
template <unsigned char Base> BigNumber<Base>* BigNumber<Base>::create(const char* string) {
  std::string input(string);
  try {
    if (input.back() == 'u') {
      input.pop_back(); // Remove it
      try {
        const unsigned char* number = reinterpret_cast<const unsigned char*>(input.c_str());
        return new BigUnsigned<Base>(number); // Create a BigUnsigned
      } catch (const BigNumberBadDigit& error) {
        std::cerr << error.what() << std::endl;
        return nullptr;
      } 
    } 
    else if (input.back() == 'i') {
      input.pop_back(); // Remove it
      try {
        BigUnsigned<Base>number(reinterpret_cast<const unsigned char*>(input.c_str()));
        return new BigInteger<Base>(number); // Create a BigInteger
      } catch (const BigNumberBadDigit& error) {
        std::cerr << error.what() << std::endl;
        return nullptr;
      } 
    } 
    else if (input.back() == 'r') {
      try {
        input.pop_back(); // Remove it
        // get numerator and denominator
        std::istringstream iss(input);
        std::string num_str, den_str;
        if (!std::getline(iss, num_str, '/') || !std::getline(iss, den_str)) {
          throw BigNumberRationalFormat(); 
        }

        BigInteger<Base> numerator(BigUnsigned<Base>(reinterpret_cast<const unsigned char*>(num_str.c_str())));
        BigInteger<Base> denominator(BigUnsigned<Base>(reinterpret_cast<const unsigned char*>(den_str.c_str())));

        if (denominator == BigInteger<Base>(0)) {
          throw BigNumberDivisionByZero();
        }

        return new BigRational<Base>(numerator, denominator);
      } catch (const BigNumberRationalFormat& error) {
        std::cerr << error.what() << std::endl;
        return nullptr;
      } catch (const BigNumberDivisionByZero& error) {
        std::cerr << error.what() << std::endl;
        return nullptr;
      } catch (const BigNumberBadDigit& error) {
        std::cerr << error.what() << std::endl;
        return nullptr;
      } 
    }

    throw BigNumberNotRecognized(input.back());
  } catch (const BigNumberNotRecognized& error) {
    std::cerr << error.what() << std::endl;
    return nullptr;
  } 
}


#endif