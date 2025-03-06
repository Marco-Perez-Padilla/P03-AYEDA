/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 1: Representación de números grandes
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 25/02/2025

** Archivo big_rational.h: Declaracion e implementacion de la plantilla para representar numeros racionales grandes en cualquier base
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      25/02/2025 - Creacion (primera version) del codigo
**      26/02/2025 - Creacion de constructores y operadores de extraccion e insercion
**      26/02/2025 - Creacion del resto de metodos para operar con ellos
**/


#ifndef BIG_RATIONAL_H
#define BIG_RATIONAL_H

#include <algorithm>
#include <sstream>

#include "big_integer.h"


/**
 * @brief Template of BigRational classes, it generates a BigRational number depending on the base passed as parameter.
 * @param unsigned_char base of representation
 */
template <unsigned char Base = 10> class BigRational {
 private:
  BigInteger<Base> numerator_;
  BigInteger<Base> denominator_;; 
 public:
  // Constructors
  BigRational(const BigInteger<Base>& numerator = 0, const BigInteger<Base>& denominator = 1) : numerator_(numerator), denominator_(denominator) {};
  // Getters
  const BigInteger<Base> getNumerator() const {return numerator_;}
  const BigInteger<Base> getDenominator() const {return denominator_;}
  // Setters
  void setNumerator(const BigInteger<Base>& numerator) {numerator_ = numerator;}
  void setDenominator(const BigInteger<Base>& denominator) {denominator_ = denominator;}
  // Assignation operator 
  BigRational<Base>& operator=(const BigRational<Base>&);
  // Insertion and extraction operators
  template <unsigned char B> friend std::ostream& operator<< (std::ostream&, const BigRational<Base>&);
  template <unsigned char B> friend std::istream& operator>> (std::istream&, BigRational<Base>&);
  // Comparation operators
  template <unsigned char B> friend bool operator< (const BigRational<Base>&, const BigRational<Base>&);
  // Arithmetic operators 
  template <unsigned char B> friend BigRational<Base> operator+ (const BigRational<Base>&, const BigRational<Base>&);
  BigRational<Base> operator-(const BigRational<Base>&) const;
  BigRational<Base> operator*(const BigRational<Base>&) const;
  template <unsigned char B> friend BigRational<Base> operator/ (const BigRational<Base>&, const BigRational<Base>&);
};


/**
 * @brief Copy constructor 
 * @param BR to be copied
 * @return BR copied
 */
template <unsigned char Base> BigRational<Base>& BigRational<Base>::operator=(const BigRational<Base>& big_rational) {
  if (this != &big_rational) {
    numerator_ = big_rational.getNumerator();
    denominator_ = big_rational.getDenominator();
  }
  return *this;
}


/**
 * @brief Overload of << operator
 * @param ostream
 * @param BR number to be printed
 * @return ostream
 */
template <unsigned char Base> std::ostream& operator<<(std::ostream& os, const BigRational<Base>& num) {
  
  BigInteger<Base> numerator = num.getNumerator();
  BigInteger<Base> denominator = num.getDenominator();

  os << numerator << " / " << denominator;
   
  return os;
}



/**
 * @brief Overload of >> operator. It requieres a line with format: numerator / denominator
 * @param istream
 * @param BR number to be inserted
 * @return istream
 */
template <unsigned char Base> std::istream& operator>>(std::istream& is, BigRational<Base>& num) {
  BigInteger<Base> numerator;
  BigInteger<Base> denominator;

  // Getting the numerator
  is >> numerator;

  // Skipping the slash in the line ('/')
  char caracter;
  is >> caracter;
  if (caracter != '/') {
    std::cerr << "Error: Incorrect format.It must be 'numerator / denominator'\n";
    exit(EXIT_FAILURE);
  }
  
  // Getting the denominator
  is >> denominator;

  // Checking that the denominator is not 0
  if (denominator == BigInteger<Base>(0)) {
    std::cerr << "Error: Denominator cannot be 0\n";
    exit(EXIT_FAILURE);
  } 

  // Setting the number
  num.setDenominator(denominator);
  num.setNumerator(numerator);

  return is;
}


/**
 * @brief Overload of < operator between two BR
 * @param BR 1 to be compared
 * @param BR 2 to be compared
 * @return if BU_1 is minor than BU_2
 */
template <unsigned char Base> bool operator<(const BigRational<Base>& big_rational_1, const BigRational<Base>& big_rational_2) {
  BigInteger<Base> result_1;
  BigInteger<Base> result_2;

  // If we have: E = a/b and F = c/d, we know that E < F if a*d < b*c
  result_1 = big_rational_1.getNumerator() * big_rational_2.getDenominator();
  result_2 = big_rational_2.getNumerator() * big_rational_1.getDenominator();

  return result_1 < result_2;
}


/**
 * @brief Overload of == operator for BR class
 * @param BR number 1 
 * @param BR number 2
 * @return bool. True if number 1 and number 2 are the same. False otherwise
 */
template <unsigned char Base> bool operator==(const BigRational<Base>& big_rational_1, const BigRational<Base>& big_rational_2) {
  if (big_rational_1.getDenominator() == big_rational_2.getDenominator() && big_rational_1.getNumerator() == big_rational_2.getNumerator()) {
    return true;
  } else {
    return false;
  }
}


/**
 * @brief Overload of >= operator for BR class
 * @param BR number 1 
 * @param BR number 2
 * @return bool. True if number 1 is greater or equal than number 2. False otherwise
 */
template <unsigned char Base> bool operator>=(const BigRational<Base>& big_rational_1, const BigRational<Base>& big_rational_2) {
  return !(big_rational_1 < big_rational_2) || (big_rational_1 == big_rational_2);
}



/**
 * @brief Overload of + operator, it calculates the sum between two BR
 * @param BR number 1 to be summed
 * @param BR number 2 to be summed
 * @return BR result of the sum
 */
template <unsigned char Base> BigRational<Base> operator+(const BigRational<Base>& big_rational_1, const BigRational<Base>& big_rational_2) {
  BigInteger<Base> numerador;
  BigInteger<Base> denominador;

  if (big_rational_1.getDenominator() == big_rational_2.getDenominator()) {
    denominador = big_rational_1.getDenominator();
    numerador = big_rational_1.getNumerator() + big_rational_2.getNumerator();
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  } else {
    denominador = big_rational_1.getDenominator() * big_rational_2.getDenominator();
    numerador = (big_rational_1.getNumerator() * big_rational_2.getDenominator()) + (big_rational_2.getNumerator() * big_rational_1.getDenominator());
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  }
}


/**
 * @brief Overload of - operator, it calculates the rest between two BR
 * @param BR number 1 to be rested
 * @param BR number 2 to be rested
 * @return BR result of the rest
 */
template <unsigned char Base> BigRational<Base> BigRational<Base>::operator-(const BigRational<Base>& big_rational_2) const {
  BigInteger<Base> numerador;
  BigInteger<Base> denominador;

  if (denominator_ == big_rational_2.getDenominator()) {
    denominador = denominator_;
    numerador = numerator_ - big_rational_2.getNumerator();
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  } else {
    denominador = denominator_ * big_rational_2.getDenominator();
    numerador = (numerator_ * big_rational_2.getDenominator()) - (big_rational_2.getNumerator() * denominator_);
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  }
}


/**
 * @brief Overload of * operator. It returns the multiplication between two BR
 * @param BR Number 1
 * @param BR Number 2
 * @return BR result
 */
template <unsigned char Base> BigRational<Base> BigRational<Base>::operator*(const BigRational<Base>& mult) const {
  BigInteger<Base> numerador;
  BigInteger<Base> denominador;

  numerador = numerator_ * mult.getNumerator();
  denominador = denominator_ * mult.getDenominator();

  BigInteger<Base> factor = numerador.mcd(denominador, numerador);
  denominador = denominador / factor;
  numerador = numerador / factor;
  BigRational<Base> result (numerador, denominador);
  return result;
}


/**
 * @brief Overload of / operator. It returns the integer division between two BR
 * @param BR numerator
 * @param BR denominator
 * @return BR result
 */
template <unsigned char Base> BigRational<Base> operator/ (const BigRational<Base>& big_rational_1, const BigRational<Base>& big_rational_2) {
  BigInteger<Base> numerador;
  BigInteger<Base> denominador;

  numerador = big_rational_1.getNumerator() * big_rational_2.getDenominator();
  denominador = big_rational_2.getNumerator() * big_rational_1.getDenominator();

  BigInteger<Base> factor = numerador.mcd(denominador, numerador);
  denominador = denominador / factor;
  numerador = numerador / factor;
  BigRational<Base> result (numerador, denominador);
  return result;
}


#endif