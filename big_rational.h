/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 3: Calculadora para números grandes
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
**      06/03/2025 - Adaptacion del codigo para usar la clase abstracta BigNumber (big_number.h)
**      07/03/2025 - Creacion de la especializacion para base 2 para arreglar errores al adaptar al uso de BigNumber
**      07/03/2025 - Adicion de manejo de excepciones
**      11/03/2025 - Cambio de implementacion de add, subtract, multiply y divide
**      11/03/2025 - Implementación de dichos metodos de BigInteger<2>
**/


#ifndef BIG_RATIONAL_H
#define BIG_RATIONAL_H

#include <algorithm>
#include <sstream>

#include "big_number.h"
#include "big_integer.h"

template <unsigned char Base> class BigRational;


/**
 * @brief Specialization of BigRational for the base 2
 */
template<> class BigRational<2> : public BigNumber<2> {
 private:
  BigInteger<2> numerator_;
  BigInteger<2> denominator_;; 
 public:
  // Constructors
  BigRational(const BigInteger<2>& numerator = 0, const BigInteger<2>& denominator = 1) : numerator_(numerator), denominator_(denominator) {};
  // Getters
  const BigInteger<2> getNumerator() const {return numerator_;}
  const BigInteger<2> getDenominator() const {return denominator_;}
  // Setters
  void setNumerator(const BigInteger<2>& numerator) {numerator_ = numerator;}
  void setDenominator(const BigInteger<2>& denominator) {denominator_ = denominator;}
  // Assignation operator 
  BigRational<2>& operator=(const BigRational<2>&);
  // Insertion and extraction operators
  std::ostream& write (std::ostream&) const override;
  std::istream& read (std::istream&) override;
  // Comparation operators
  friend bool operator< (const BigRational<2>&, const BigRational<2>&);
  // Arithmetic operators 
  friend BigRational<2> operator+ (const BigRational<2>&, const BigRational<2>&);
  BigRational<2> operator-(const BigRational<2>&) const;
  BigRational<2> operator*(const BigRational<2>&) const;
  friend BigRational<2> operator/ (const BigRational<2>&, const BigRational<2>&);
  // Virtual methods to override
  BigNumber<2>& add(const BigNumber<2>&) const override;
  BigNumber<2>& subtract(const BigNumber<2>&) const override;
  BigNumber<2>& multiply(const BigNumber<2>&) const override;
  BigNumber<2>& divide(const BigNumber<2>&) const override;
  // Virtual change-type operators to override 
   operator BigUnsigned<2>() const override;
   operator BigInteger<2>() const override;
   operator BigRational<2>() const override;
 };
 
 
 /**
  * @brief Copy constructor 
  * @param BR to be copied
  * @return BR copied
  */
  BigRational<2>& BigRational<2>::operator=(const BigRational<2>& big_rational) {
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
  std::ostream& BigRational<2>::write(std::ostream& os) const {
   
   BigInteger<2> numerator = getNumerator();
   BigInteger<2> denominator = getDenominator();
 
   os << numerator << "/" << denominator;
    
   return os;
 }
 
 
 /**
  * @brief Overload of >> operator. It requieres a line with format: numerator / denominator
  * @param istream
  * @param BR number to be inserted
  * @return istream
  */
  std::istream& BigRational<2>::read(std::istream& is) {
   BigInteger<2> numerator;
   BigInteger<2> denominator;
   try {
    // Getting the numerator
    is >> numerator;
  
    // Skipping the slash in the line ('/')
    char caracter;
    is >> caracter;
    if (caracter != '/') {
      throw BigNumberRationalFormat();
    }
    
    // Getting the denominator
    is >> denominator;
  
    // Checking that the denominator is not 0
    if (denominator == BigInteger<2>(0)) {
      throw BigNumberDivisionByZero();
    } 
  
    // Setting the number
    setDenominator(denominator);
    setNumerator(numerator);
  
    return is;
  } catch (const BigNumberRationalFormat& error) {
    std::cerr << error.what() << std::endl;
    return is;
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    return is;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    return is;
  }
 }
 
 
 /**
  * @brief Overload of < operator between two BR
  * @param BR 1 to be compared
  * @param BR 2 to be compared
  * @return if BU_1 is minor than BU_2
  */
  bool operator<(const BigRational<2>& big_rational_1, const BigRational<2>& big_rational_2) {
   BigInteger<2> result_1;
   BigInteger<2> result_2;
 
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
  bool operator==(const BigRational<2>& big_rational_1, const BigRational<2>& big_rational_2) {
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
  bool operator>=(const BigRational<2>& big_rational_1, const BigRational<2>& big_rational_2) {
   return !(big_rational_1 < big_rational_2) || (big_rational_1 == big_rational_2);
 }
 
 
 
 /**
  * @brief Overload of + operator, it calculates the sum between two BR
  * @param BR number 1 to be summed
  * @param BR number 2 to be summed
  * @return BR result of the sum
  */
  BigRational<2> operator+(const BigRational<2>& big_rational_1, const BigRational<2>& big_rational_2) {
   BigInteger<2> numerador;
   BigInteger<2> denominador;
   try {
    if (big_rational_1.getDenominator() == big_rational_2.getDenominator()) {
      denominador = big_rational_1.getDenominator();
      numerador = big_rational_1.getNumerator() + big_rational_2.getNumerator();
      BigInteger<2> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<2>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<2> result (numerador, denominador);
      return result;
    } else {
      denominador = big_rational_1.getDenominator() * big_rational_2.getDenominator();
      numerador = (big_rational_1.getNumerator() * big_rational_2.getDenominator()) + (big_rational_2.getNumerator() * big_rational_1.getDenominator());
      BigInteger<2> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<2>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<2> result (numerador, denominador);
      return result;
    }
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  }
 }
 
 
 /**
  * @brief Overload of - operator, it calculates the rest between two BR
  * @param BR number 1 to be rested
  * @param BR number 2 to be rested
  * @return BR result of the rest
  */
  BigRational<2> BigRational<2>::operator-(const BigRational<2>& big_rational_2) const {
   BigInteger<2> numerador;
   BigInteger<2> denominador;
   try {
    if (denominator_ == big_rational_2.getDenominator()) {
      denominador = denominator_;
      numerador = numerator_ - big_rational_2.getNumerator();
      BigInteger<2> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<2>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<2> result (numerador, denominador);
      return result;
    } else {
      denominador = denominator_ * big_rational_2.getDenominator();
      numerador = (numerator_ * big_rational_2.getDenominator()) - (big_rational_2.getNumerator() * denominator_);
      BigInteger<2> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<2>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<2> result (numerador, denominador);
      return result;
    }
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  }
 }
 
 
 /**
  * @brief Overload of * operator. It returns the multiplication between two BR
  * @param BR Number 1
  * @param BR Number 2
  * @return BR result
  */
  BigRational<2> BigRational<2>::operator*(const BigRational<2>& mult) const {
   BigInteger<2> numerador;
   BigInteger<2> denominador;
   try {
    numerador = numerator_ * mult.getNumerator();
    denominador = denominator_ * mult.getDenominator();
    if (denominador == BigInteger<2>()) {
      throw BigNumberDivisionByZero();
    }
    BigInteger<2> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<2> result (numerador, denominador);
    return result;
   } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  }
 }
 
 
 /**
  * @brief Overload of / operator. It returns the integer division between two BR
  * @param BR numerator
  * @param BR denominator
  * @return BR result
  */
  BigRational<2> operator/ (const BigRational<2>& big_rational_1, const BigRational<2>& big_rational_2) {
   BigInteger<2> numerador;
   BigInteger<2> denominador;
   try {
    numerador = big_rational_1.getNumerator() * big_rational_2.getDenominator();
    denominador = big_rational_2.getNumerator() * big_rational_1.getDenominator();
  
    BigInteger<2> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    if (denominador == BigInteger<2>()) {
      throw BigNumberDivisionByZero();
    }
    numerador = numerador / factor;
    BigRational<2> result (numerador, denominador);
    return result;
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<2>();
    denominador = BigInteger<2>(1);
    BigRational<2> result (numerador, denominador);
    return result;
  }
 }
 
 
/**
 * @brief Overriding of add method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the sum
 */
  BigNumber<2>& BigRational<2>::add(const BigNumber<2>& other) const {
    try {
      // Intenta hacer el cast al mismo tipo
      const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
      BigRational<2>* result = new BigRational<2>(*this + other_rational);
      return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
        BigRational<2> converted(other_integer);
        BigRational<2>* result = new BigRational<2>(*this + converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> integer(other_unsigned);
        BigRational<2> converted(integer);
        BigRational<2>* result = new BigRational<2>(*this + converted);
        return *result;
      }
    }
 }
 

/**
 * @brief Overriding of subtract method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the rest
 */
  BigNumber<2>& BigRational<2>::subtract(const BigNumber<2>& other) const {
    try {
      // Intenta hacer el cast al mismo tipo
      const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
      BigRational<2>* result = new BigRational<2>(*this - other_rational);
      return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
        BigRational<2> converted(other_integer);
        BigRational<2>* result = new BigRational<2>(*this - converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> integer(other_unsigned);
        BigRational<2> converted(integer);
        BigRational<2>* result = new BigRational<2>(*this - converted);
        return *result;
      }
    }
 }
 
 
/**
 * @brief Overriding of multiply method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the multiplication
 */
  BigNumber<2>& BigRational<2>::multiply(const BigNumber<2>& other) const {
    try {
      // Intenta hacer el cast al mismo tipo
      const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
      BigRational<2>* result = new BigRational<2>(*this * other_rational);
      return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
        BigRational<2> converted(other_integer);
        BigRational<2>* result = new BigRational<2>(*this * converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> integer(other_unsigned);
        BigRational<2> converted(integer);
        BigRational<2>* result = new BigRational<2>(*this * converted);
        return *result;
      }
    }
 }
 
 
/**
 * @brief Overriding of divide method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the divition
 */
  BigNumber<2>& BigRational<2>::divide(const BigNumber<2>& other) const {
    try {
      // Intenta hacer el cast al mismo tipo
      const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
      BigRational<2>* result = new BigRational<2>(*this / other_rational);
      return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
        BigRational<2> converted(other_integer);
        BigRational<2>* result = new BigRational<2>(*this / converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> integer(other_unsigned);
        BigRational<2> converted(integer);
        BigRational<2>* result = new BigRational<2>(*this / converted);
        return *result;
      }
    }
 }
 
 
/**
 * @brief Operator to change type to BigUnsigned
 * @return Equivalent BigUnsigned<2>
 */
  BigRational<2>::operator BigUnsigned<2>() const {
   BigInteger<2> div = numerator_/denominator_;

   std::string binary_string;
   for (bool bit : div.getModule()) {
     binary_string += (bit ? '1' : '0');
   }
 
   std::vector<unsigned char> binary_chars;
   for (char c : binary_string) {
     binary_chars.push_back(static_cast<unsigned char>(c));
   }
 
   return BigUnsigned<2>(binary_chars.data());
   
 }
 

/**
 * @brief Operator to change type to BigInteger
 * @return Equivalent BigInteger
 */
BigRational<2>::operator BigInteger<2>() const {
  BigInteger<2> div = numerator_/denominator_;
  return div;
}
 
 
/**
 * @brief Operator to change type to BigRational
 * @return Equivalent to BigRational (itself)
 */
BigRational<2>::operator BigRational<2>() const {
  return *this;
}


/**
 * @brief Operator of the class BigInteger<2> to change type to BigRational
 * @return Equivalent to BigRational
 */
BigInteger<2>::operator BigRational<2>() const {
  BigInteger<2> denominador (1);
  return BigRational<2>(*this, denominador);
}


/**
 * @brief Overriding of add method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the sum
 */
BigNumber<2>& BigInteger<2>::add(const BigNumber<2>& other) const {
  try {
    const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
    BigInteger<2>* result = new BigInteger<2>(*this + other_integer);
    return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> converted(other_unsigned);  
        BigInteger<2>* result = new BigInteger<2>(*this + converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
        BigRational<2> rational = this->operator BigRational<2>();
        BigRational<2>* result = new BigRational<2>(rational + other_rational);
        return *result;
      }
    }
 }
 
 
/**
 * @brief Overriding of subtract method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the rest
 */
 BigNumber<2>& BigInteger<2>::subtract(const BigNumber<2>& other) const {
  try {
    const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
    BigInteger<2>* result = new BigInteger<2>(*this - other_integer);
    return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> converted(other_unsigned);  
        BigInteger<2>* result = new BigInteger<2>(*this - converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
        BigRational<2> rational = this->operator BigRational<2>();
        BigRational<2>* result = new BigRational<2>(rational - other_rational);
        return *result;
      }
    }
 }
 

/**
 * @brief Overriding of multiply method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the multiplication
 */
 BigNumber<2>& BigInteger<2>::multiply(const BigNumber<2>& other) const {
  try {
    const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
    BigInteger<2>* result = new BigInteger<2>(*this * other_integer);
    return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> converted(other_unsigned);  
        BigInteger<2>* result = new BigInteger<2>(*this * converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
        BigRational<2> rational = this->operator BigRational<2>();
        BigRational<2>* result = new BigRational<2>(rational * other_rational);
        return *result;
      }
    }
 }
 
 
/**
 * @brief Overriding of divide method, allocating the result in dynamic memory
 * @param BigNumber<2> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<2> result of the divition
 */
 BigNumber<2>& BigInteger<2>::divide(const BigNumber<2>& other) const {
  try {
    const BigInteger<2>& other_integer = dynamic_cast<const BigInteger<2>&>(other);
    BigInteger<2>* result = new BigInteger<2>(*this / other_integer);
    return *result;
    } catch (const std::bad_cast&) {
      try {
        const BigUnsigned<2>& other_unsigned = dynamic_cast<const BigUnsigned<2>&>(other);
        BigInteger<2> converted(other_unsigned);  
        BigInteger<2>* result = new BigInteger<2>(*this / converted);
        return *result;
      } catch (const std::bad_cast&) {
        const BigRational<2>& other_rational = dynamic_cast<const BigRational<2>&>(other);
        BigRational<2> rational = this->operator BigRational<2>();
        BigRational<2>* result = new BigRational<2>(rational / other_rational);
        return *result;
      }
    }
 }


/**
 * @brief Template of BigRational classes, it generates a BigRational number depending on the base passed as parameter.
 * @param unsigned_char base of representation
 */
template <unsigned char Base = 10> class BigRational : public BigNumber<Base>{
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
  std::ostream& write (std::ostream&) const override;
  std::istream& read (std::istream&) override;
  // Comparation operators
  template <unsigned char B> friend bool operator< (const BigRational<Base>&, const BigRational<Base>&);
  // Arithmetic operators 
  template <unsigned char B> friend BigRational<Base> operator+ (const BigRational<Base>&, const BigRational<Base>&);
  BigRational<Base> operator-(const BigRational<Base>&) const;
  BigRational<Base> operator*(const BigRational<Base>&) const;
  template <unsigned char B> friend BigRational<Base> operator/ (const BigRational<Base>&, const BigRational<Base>&);
  // Virtual methods to override
  BigNumber<Base>& add(const BigNumber<Base>&) const override;
  BigNumber<Base>& subtract(const BigNumber<Base>&) const override;
  BigNumber<Base>& multiply(const BigNumber<Base>&) const override;
  BigNumber<Base>& divide(const BigNumber<Base>&) const override;
  // Virtual change-type operators to override
  operator BigUnsigned<Base>() const override;
  operator BigInteger<Base>() const override;
  operator BigRational<Base>() const override;
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
template <unsigned char Base> std::ostream& BigRational<Base>::write(std::ostream& os) const {
  
  BigInteger<Base> numerator = getNumerator();
  BigInteger<Base> denominator = getDenominator();

  os << numerator << "/" << denominator;
   
  return os;
}


/**
 * @brief Overload of >> operator. It requieres a line with format: numerator / denominator
 * @param istream
 * @param BR number to be inserted
 * @return istream
 */
template <unsigned char Base> std::istream& BigRational<Base>::read(std::istream& is) {
  BigInteger<Base> numerator;
  BigInteger<Base> denominator;
  try {
    // Getting the numerator
    is >> numerator;
  
    // Skipping the slash in the line ('/')
    char caracter;
    is >> caracter;
    if (caracter != '/') {
      throw BigNumberRationalFormat();
    }
    
    // Getting the denominator
    is >> denominator;
  
    // Checking that the denominator is not 0
    if (denominator == BigInteger<Base>(0)) {
      throw BigNumberDivisionByZero();
    } 
  
    // Setting the number
    setDenominator(denominator);
    setNumerator(numerator);
  
    return is;
  } catch (const BigNumberRationalFormat& error) {
    std::cerr << error.what() << std::endl;
    return is;
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    return is;
  } 
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
  try {
    if (big_rational_1.getDenominator() == big_rational_2.getDenominator()) {
      denominador = big_rational_1.getDenominator();
      numerador = big_rational_1.getNumerator() + big_rational_2.getNumerator();
      BigInteger<Base> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<Base>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<Base> result (numerador, denominador);
      return result;
    } else {
      denominador = big_rational_1.getDenominator() * big_rational_2.getDenominator();
      numerador = (big_rational_1.getNumerator() * big_rational_2.getDenominator()) + (big_rational_2.getNumerator() * big_rational_1.getDenominator());
      BigInteger<Base> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<Base>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<Base> result (numerador, denominador);
      return result;
    }
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
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

  try {
    if (denominator_ == big_rational_2.getDenominator()) {
      denominador = denominator_;
      numerador = numerator_ - big_rational_2.getNumerator();
      BigInteger<Base> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<Base>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<Base> result (numerador, denominador);
      return result;
    } else {
      denominador = denominator_ * big_rational_2.getDenominator();
      numerador = (numerator_ * big_rational_2.getDenominator()) - (big_rational_2.getNumerator() * denominator_);
      BigInteger<Base> factor = numerador.mcd(denominador, numerador);
      denominador = denominador / factor;
      if (denominador == BigInteger<Base>()) {
        throw BigNumberDivisionByZero();
      }
      numerador = numerador / factor;
      BigRational<Base> result (numerador, denominador);
      return result;
    }
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
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

  try {
    numerador = numerator_ * mult.getNumerator();
    denominador = denominator_ * mult.getDenominator();
    if (denominador == BigInteger<Base>()) {
      throw BigNumberDivisionByZero();
    }
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  }
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

  try {
    numerador = big_rational_1.getNumerator() * big_rational_2.getDenominator();
    denominador = big_rational_2.getNumerator() * big_rational_1.getDenominator();
    BigInteger<Base> factor = numerador.mcd(denominador, numerador);
    denominador = denominador / factor;
    if (denominador == BigInteger<Base>()) {
      throw BigNumberDivisionByZero();
    }
    numerador = numerador / factor;
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const BigNumberDivisionByZero& error) {
    std::cerr << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  } catch (const std::exception& error) {
    std::cerr << "Unexpected error: " << error.what() << std::endl;
    numerador = BigInteger<Base>();
    denominador = BigInteger<Base>(1);
    BigRational<Base> result (numerador, denominador);
    return result;
  }
}


/**
 * @brief Overriding of add method, allocating the result in dynamic memory
 * @param BigNumber<Base> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<Base> result of the sum
 */
template <unsigned char Base> BigNumber<Base>& BigRational<Base>::add(const BigNumber<Base>& other) const {
  try {
    // Intenta hacer el cast al mismo tipo
    const BigRational<Base>& other_rational = dynamic_cast<const BigRational<Base>&>(other);
    BigRational<Base>* result = new BigRational<Base>(*this + other_rational);
    return *result;
  } catch (const std::bad_cast&) {
    try {
      const BigInteger<Base>& other_integer = dynamic_cast<const BigInteger<Base>&>(other);
      BigRational<Base> converted(other_integer);
      BigRational<Base>* result = new BigRational<Base>(*this + converted);
      return *result;
    } catch (const std::bad_cast&) {
      const BigUnsigned<Base>& other_unsigned = dynamic_cast<const BigUnsigned<Base>&>(other);
      BigInteger<Base> integer(other_unsigned);
      BigRational<Base> converted(integer);
      BigRational<Base>* result = new BigRational<Base>(*this + converted);
      return *result;
    }
  }
}


/**
 * @brief Overriding of subtract method, allocating the result in dynamic memory
 * @param BigNumber<Base> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<Base> result of the rest
 */
template <unsigned char Base> BigNumber<Base>& BigRational<Base>::subtract(const BigNumber<Base>& other) const {
  try {
    // Intenta hacer el cast al mismo tipo
    const BigRational<Base>& other_rational = dynamic_cast<const BigRational<Base>&>(other);
    BigRational<Base>* result = new BigRational<Base>(*this - other_rational);
    return *result;
  } catch (const std::bad_cast&) {
    try {
      const BigInteger<Base>& other_integer = dynamic_cast<const BigInteger<Base>&>(other);
      BigRational<Base> converted(other_integer);
      BigRational<Base>* result = new BigRational<Base>(*this - converted);
      return *result;
    } catch (const std::bad_cast&) {
      const BigUnsigned<Base>& other_unsigned = dynamic_cast<const BigUnsigned<Base>&>(other);
      BigInteger<Base> integer(other_unsigned);
      BigRational<Base> converted(integer);
      BigRational<Base>* result = new BigRational<Base>(*this - converted);
      return *result;
    }
  }
}


/**
 * @brief Overriding of multiply method, allocating the result in dynamic memory
 * @param BigNumber<Base> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<Base> result of the multiplication
 */
template <unsigned char Base> BigNumber<Base>& BigRational<Base>::multiply(const BigNumber<Base>& other) const {
  try {
    // Intenta hacer el cast al mismo tipo
    const BigRational<Base>& other_rational = dynamic_cast<const BigRational<Base>&>(other);
    BigRational<Base>* result = new BigRational<Base>(*this * other_rational);
    return *result;
  } catch (const std::bad_cast&) {
    try {
      const BigInteger<Base>& other_integer = dynamic_cast<const BigInteger<Base>&>(other);
      BigRational<Base> converted(other_integer);
      BigRational<Base>* result = new BigRational<Base>(*this * converted);
      return *result;
    } catch (const std::bad_cast&) {
      const BigUnsigned<Base>& other_unsigned = dynamic_cast<const BigUnsigned<Base>&>(other);
      BigInteger<Base> integer(other_unsigned);
      BigRational<Base> converted(integer);
      BigRational<Base>* result = new BigRational<Base>(*this * converted);
      return *result;
    }
  }
}


/**
 * @brief Overriding of divide method, allocating the result in dynamic memory
 * @param BigNumber<Base> BigUnsigned, BigInteger or BigRational
 * @return BigNumber<Base> result of the division
 */
template <unsigned char Base> BigNumber<Base>& BigRational<Base>::divide(const BigNumber<Base>& other) const {
  try {
    // Intenta hacer el cast al mismo tipo
    const BigRational<Base>& other_rational = dynamic_cast<const BigRational<Base>&>(other);
    BigRational<Base>* result = new BigRational<Base>(*this / other_rational);
    return *result;
  } catch (const std::bad_cast&) {
    try {
      const BigInteger<Base>& other_integer = dynamic_cast<const BigInteger<Base>&>(other);
      BigRational<Base> converted(other_integer);
      BigRational<Base>* result = new BigRational<Base>(*this / converted);
      return *result;
    } catch (const std::bad_cast&) {
      const BigUnsigned<Base>& other_unsigned = dynamic_cast<const BigUnsigned<Base>&>(other);
      BigInteger<Base> integer(other_unsigned);
      BigRational<Base> converted(integer);
      BigRational<Base>* result = new BigRational<Base>(*this / converted);
      return *result;
    }
  }
}


/**
 * @brief Operator to change type to BigUnsigned
 * @return Equivalent BigUnsigned
 */
template <unsigned char Base> BigRational<Base>::operator BigUnsigned<Base>() const {
  BigInteger<Base> div = numerator_/denominator_;
  return div.getModule();
}


/**
 * @brief Operator to change type to BigInteger
 * @return Equivalent BigInteger
 */
template <unsigned char Base> BigRational<Base>::operator BigInteger<Base>() const {
  BigInteger<Base> div = numerator_/denominator_;
  return div;
}


/**
 * @brief Operator to change type to BigRational
 * @return Equivalent BigRational (itself)
 */
template <unsigned char Base> BigRational<Base>::operator BigRational<Base>() const {
  return *this;
}


#endif