/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 1: Representación de números grandes
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 05/02/2025

** Archivo big_integer.h: Declaracion e implementacion de la plantilla para representar numeros enteros grandes en cualquier base
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      05/02/2025 - Creacion (primera version) del codigo
**      11/02/2025 - Finalizacion de la primera version del codigo
**      21/02/2025 - Inicio de segunda version del codigo. Adaptación a plantillas
**      24/02/2025 - Creación de la especificación para la base dos
**      24/02/2025 - Creacion de constructores y operadores =, << y >> para la base dos
**      25/02/2025 - Creacion del resto de operadores y metodos para la base dos
**      25/02/2025 - Adicion del constructor de cambio de tipo de BigUnsigned<2> a BigInteger<2>
**/


#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <algorithm>

#include "big_unsigned.h"


/**
 * @brief Template of BigInteger classes, it generates a BigUnsigned number depending on the base passed as parameter.
 *        It is made out of the BigInteger class, being this modified to adapt it to template.
 *        Important note: There is a specialization for the base 2 on the line 420 of this file
 * @param unsigned_char base of representation
 */
template <unsigned char Base = 10> class BigInteger {
 private:
  BigUnsigned<Base> module_;
  bool sign_; // 1 if positive, 0 if negative
 public:
  // Constructors
  BigInteger(int n = 0);
  BigInteger(const BigUnsigned<Base>& module, bool sign = 1) : module_(module), sign_(sign) {};
  // Getters
  const BigUnsigned<Base> getModule() const {return module_;}
  const bool getSign() const {return sign_;}
  // Setters
  void setModule(const BigUnsigned<Base>& module) {module_ = module;}
  void setSign(const bool sign) {sign_ = sign;}
  // Assignation operator 
  BigInteger<Base>& operator=(const BigInteger<Base>&);
  // Insertion and extraction operators
  template <unsigned char B> friend std::ostream& operator<< (std::ostream&, const BigInteger<Base>&);
  template <unsigned char B> friend std::istream& operator>> (std::istream&, BigInteger<Base>&);
  // Comparation operators
  template <unsigned char B> friend bool operator< (const BigInteger<Base>&, const BigInteger<Base>&);
  // Arithmetic operators 
  template <unsigned char B> friend BigInteger<Base> operator+ (const BigInteger<Base>&, const BigInteger<Base>&);
  BigInteger<Base> operator-(const BigInteger<Base>&) const;
  BigInteger<Base> operator*(const BigInteger<Base>&) const;
  BigInteger<Base> operator%(const BigInteger<Base>&) const;
  template <unsigned char B> friend BigInteger<Base> operator/ (const BigInteger<Base>&, const BigInteger<Base>&);
  BigInteger<Base> mcd(const BigInteger<Base>&, const BigInteger<Base>&) const;
};


/**
 * @brief Default constructor that builds a BigInteger<Base> (aka BI) object
 * @param int to be converted
 */
template <unsigned char Base> BigInteger<Base>::BigInteger(int numero) {
  // If zero (base case), build 0 with sign positive
  if (numero == 0) {
    module_.AddDigit(0);
    sign_ = 1;
  } else {
    // If negative
    if (numero < 0) {
      // Get the positive number and add it to the module
      numero = numero * (-1);
      while (numero > 0) {
        unsigned char digit = static_cast<char>(numero % 10);
        module_.AddDigit(digit);
        numero /= 10;
      }
      // Set the sign to negative
      sign_ = 0;
    } else { // If positive, add it and set the sign at positive
      while (numero > 0) {
        unsigned char digit = static_cast<char>(numero % 10);
        module_.AddDigit(digit);
        numero /= 10;
      }
      sign_ = 1;
    }
  }
}


/**
 * @brief Copy constructor 
 * @param BI to be copied
 * @return BI copied
 */
template <unsigned char Base> BigInteger<Base>& BigInteger<Base>::operator=(const BigInteger<Base>& big_integer) {
  if (this != &big_integer) {
    module_ = big_integer.getModule();
    sign_ = big_integer.getSign();
  }
  return *this;
}


/**
 * @brief Overload of << operator
 * @param ostream
 * @param BI number to be printed
 * @return ostream
 */
template <unsigned char Base> std::ostream& operator<<(std::ostream& os, const BigInteger<Base>& num) {
  if (num.getModule().getDigits().empty()) {
    os << '0';
  } else {
    if (num.getSign() == 0) { // If negative, print "-" before the number
      os << "-";
    }
    for (int i {num.getModule().getDigits().size() - 1}; i >= 0; --i) {
      // Same as with BigUnsigned, we get the representation of the letters when the base is greater than 10
      if (num.getModule().getDigits()[i] >= 'A' && num.getModule().getDigits()[i] <= 'Z') {
        os << num.getModule().getDigits()[i];
      } else {
        int digit = num.getModule().getDigits()[i];
        os << digit;
      }
    }
  }
  return os;
}


/**
 * @brief Overload of >> operator
 * @param istream
 * @param BI number to be inserted
 * @return istream
 */
template <unsigned char Base> std::istream& operator>>(std::istream& is, BigInteger<Base>& num) {
  std::string input;
  is >> input;
  // Initialize a bool to create the BI number
  bool not_negative = true;
  // If not empty and the string starts with "-"
  if (!(input.empty()) && input[0] == '-') {
    not_negative = false; //It's negative
    input.erase(0, 1); // Erase one position starting from the position zero ("-")
  }
  
  num = BigInteger<Base>(BigUnsigned<Base>(reinterpret_cast<const unsigned char*>(input.data())), not_negative);
  
  return is;
}


/**
 * @brief Overload of < operator between two BI
 * @param BI 1 to be compared
 * @param BI 2 to be compared
 * @return if BI_1 is minor than BI_2
 */
template <unsigned char Base> bool operator<(const BigInteger<Base>& big_integer_1, const BigInteger<Base>& big_integer_2) {
  // Case: Both negative, the one with greater module will be greater
  if (big_integer_1.getSign() == 0 && big_integer_2.getSign() == 0) {
    return !(big_integer_1.getModule() < big_integer_2.getModule());
  } else if (big_integer_1.getSign() == 1 && big_integer_2.getSign() == 1) { // Case: Both positive, return the lesser of both modules
    return big_integer_1.getModule() < big_integer_2.getModule();
  } else if (big_integer_1.getSign() == 0 && big_integer_2.getSign() == 1) { // Case: One negative, other positive: the negative is lesser
    return true;
  } else {
    return false; // Case: Viceversa
  }
}


/**
 * @brief Overload of == operator for BI class
 * @param BI number 1 
 * @param BI number 2
 * @return bool. True if number 1 and number 2 are the same. False otherwise
 */
template <unsigned char Base> bool operator==(const BigInteger<Base>& big_integer_1, const BigInteger<Base>& big_integer_2) {
  // If both sign and module are the same in both numbers, they're the same
  if (big_integer_1.getSign() == big_integer_2.getSign() && big_integer_1.getModule() == big_integer_2.getModule()) {
    return true;
  } else {
    return false;
  }
}


/**
 * @brief Overload of >= operator for BI class
 * @param BI number 1 
 * @param BI number 2
 * @return bool. True if number 1 is greater or equal than number 2. False otherwise
 */
template <unsigned char Base> bool operator>=(const BigInteger<Base>& big_integer_1, const BigInteger<Base>& big_integer_2) {
  return !(big_integer_1 < big_integer_2) || (big_integer_1 == big_integer_2);
}


/**
 * @brief Overload of + operator, it calculates the sum between two BI
 * @param BI number 1 to be summed
 * @param BI number 2 to be summed
 * @return BI result of the sum
 */
template <unsigned char Base> BigInteger<Base> operator+(const BigInteger<Base>& big_integer_1, const BigInteger<Base>& big_integer_2) {
  // Case 1: Both negative. Sum is the sum of the modules, with negative sign (-5 + (-8))
  if (big_integer_1.getSign() == 0 && big_integer_2.getSign() == 0) {
    BigUnsigned<Base> sum = big_integer_1.getModule() + big_integer_2.getModule();
    BigInteger<Base> result (sum, 0);
    return result;
  } else if (big_integer_1.getSign() == 1 && big_integer_2.getSign() == 1) { // Case 2: Both positive. Sum is the sum of the modules, with positive sign (5 + 8)
    BigUnsigned<Base> sum = big_integer_1.getModule() + big_integer_2.getModule();
    BigInteger<Base> result (sum, 1);
    return result;
  } else if (big_integer_1.getSign() == 0 && big_integer_2.getSign() == 1) { // Case 3: Number 1 negative, 2 positive. 
    if (big_integer_1.getModule() >= big_integer_2.getModule()) { // Case 3.1: If 1 is greater than 2, sum is the rest of the modules in that order (number1-number2), with negative sign (-8+5)
      BigUnsigned<Base> sum = big_integer_1.getModule() - big_integer_2.getModule();
      BigInteger<Base> result (sum, 0);
      return result;
    } else {
      BigUnsigned<Base> sum = big_integer_2.getModule() - big_integer_1.getModule(); // Case 3.2: Else, sum is the rest of the modules in reversed (Number2-number1), with positive sign (-3+5)
      BigInteger<Base> result (sum, 1);
      return result;
    }
  } else { // Case 4: Number 1 positive, 2 negative. 
    if (big_integer_1.getModule() >= big_integer_2.getModule()) { // Case 4.1: If 1 is greater than 2, sum is the rest of the modules in that order (number1-number2), with positive sign (8-5)
      BigUnsigned<Base> sum = big_integer_1.getModule() - big_integer_2.getModule();
      BigInteger<Base> result (sum, 1);
      return result;
    } else {
      BigUnsigned<Base> sum = big_integer_2.getModule() - big_integer_1.getModule();// Case 4.2: Else, sum is the rest of the modules in reversed (Number2-number1), with negative sign (3-5)
      BigInteger<Base> result (sum, 0);
      return result;
    }
  }
}


/**
 * @brief Overload of - operator, it calculates the rest between two BI
 * @param BI number 1 to be rested
 * @param BI number 2 to be rested
 * @return BI result of the rest
 */
template <unsigned char Base> BigInteger<Base> BigInteger<Base>::operator-(const BigInteger<Base>& big_integer_2) const {
  // Case: Both negatives (-5 - (-8)) = (-5+8). We're on the case 3 of the sum
  if (sign_ == 0 && big_integer_2.getSign() == 0) {
    BigInteger<Base> seg_term (big_integer_2.getModule(), 1);
    BigInteger<Base> result = *this + seg_term;
    return result;
  } else if (sign_ == 1 && big_integer_2.getSign() == 1) { // Case: Both positive (5 - 8). We're actually on the case 4 of the sum
    if (module_ >= big_integer_2.getModule()) {
      BigUnsigned<Base> sum = module_ - big_integer_2.getModule();
      BigInteger<Base> result (sum, 1);
      return result;
    } else {
      BigUnsigned<Base> sum = big_integer_2.getModule() - module_;
      BigInteger<Base> result (sum, 0);
      return result;
    }
  } else if (sign_ == 0 && big_integer_2.getSign() == 1) { // Case: Number 1 negative, 2 positive (-5 - 8). We're actually on the case 1 of the sum
    BigUnsigned<Base> sum = module_ + big_integer_2.getModule();
    BigInteger<Base> result (sum, 0);
    return result;
  } else { // Case: Viceversa (5 - (-8)) = (5 + 8). We're actually on the case 2 of the sum
    BigInteger<Base> seg_term (big_integer_2.getModule(), 1);
    BigInteger<Base> result = *this + seg_term;
    return result;
  }
}


/**
 * @brief Pre-increment operator. It adds 1 to a BI in pre-order
 * @param BI to be added 1
 * @return BI summed
 */
template <unsigned char Base> BigInteger<Base>& operator++(BigInteger<Base>& big_integer_1) {
  BigInteger<Base> unit (1);
  big_integer_1 = big_integer_1 + unit;
  return big_integer_1;
}


/**
 * @brief Post-increment operator. It adds 1 to a BI in post-order
 * @param BI to be added 1
 * @return BI summed
 */
template <unsigned char Base> BigInteger<Base> operator++(BigInteger<Base>& big_integer_1, int) {
  BigInteger<Base> post_integer = big_integer_1;
  BigInteger<Base> unit (1);
  big_integer_1 = big_integer_1 + unit;
  return post_integer;
}


/**
 * @brief Pre-decrement operator. It rests 1 to a BI in pre-order
 * @param BI to be rested 1
 * @return BI rested
 */
template <unsigned char Base> BigInteger<Base>& operator--(BigInteger<Base>& big_integer_1) {
  BigInteger<Base> unit (1);
  big_integer_1 = big_integer_1 - unit;
  return big_integer_1;
}


/**
 * @brief Post-decrement operator. It rests 1 to a BI in post-order
 * @param BI to be rested 1
 * @return BI rested
 */
template <unsigned char Base> BigInteger<Base> operator--(BigInteger<Base>& big_integer_1, int) {
  BigInteger<Base> post_integer = big_integer_1;
  BigInteger<Base> unit (1);
  big_integer_1 = big_integer_1 - unit;
  return post_integer;
}


/**
 * @brief Overload of % operator for BI class
 * @param BI denominator
 * @return BI module result
 */
template <unsigned char Base> BigInteger<Base> BigInteger<Base>::operator%(const BigInteger<Base>& big_integer) const {
  // Result is the module of the modules
  BigUnsigned<Base> mod = module_ % big_integer.getModule();
  BigInteger<Base> result (mod, 1);
  return result;
}


/**
 * @brief Overload of * operator. It returns the multiplication between two BI
 * @param BI Number 1
 * @param BI Number 2
 * @return BI result
 */
template <unsigned char Base> BigInteger<Base> BigInteger<Base>::operator*(const BigInteger<Base>& mult) const {
  // Result will be the multiplication of the modules. Positive if both signs are the same, negative otherwise
  BigUnsigned<Base> prod = module_ * mult.getModule();
  if ((sign_ == 0 && mult.getSign() == 0) || (sign_ == 1 && mult.getSign() == 1)) {
    BigInteger<Base> result (prod, 1);
    return result;
  } else {
    BigInteger<Base> result (prod, 0);
    return result;
  }
}


/**
 * @brief Overload of / operator. It returns the integer division between two BI
 * @param BI numerator
 * @param BI denominator
 * @return BI integer result
 */
template <unsigned char Base> BigInteger<Base> operator/(const BigInteger<Base>& big_integer_1, const BigInteger<Base>& big_integer_2) {
  // Result will be the multiplication of the modules. Positive if both signs are the same, negative otherwise
  BigUnsigned<Base> div = big_integer_1.getModule() / big_integer_2.getModule();
  if ((big_integer_1.getSign() == 0 && big_integer_2.getSign() == 0) || (big_integer_1.getSign() == 1 && big_integer_2.getSign() == 1)) {
    BigInteger<Base> result (div, 1);
    return result;
  } else {
    BigInteger<Base> result (div, 0);
    return result;
  }
}


/**
 * @brief GCD implementation between two BI
 * @param BI Number 1
 * @param BI Number 2
 * @return Greatest common divisor between both BI
 */
template <unsigned char Base> BigInteger<Base> BigInteger<Base>::mcd(const BigInteger<Base>& num_1, const BigInteger<Base>& num_2) const {
  // Getting two temporal BI to modify in the iterative version of the GCD, with motivation not to overflow the stack with recursive calls
  BigInteger<Base> temp_num_1 = num_1;
  BigInteger<Base> temp_num_2 = num_2;
  // Null Integer to compare in the while
  BigInteger<Base> null (BigUnsigned<Base>(), 1);
  // While the second number is not 0
  while (!(temp_num_2 == null)) {
    // Auxiliar number is the second number
    BigInteger<Base> temp = temp_num_2;
    // Second number is module of first and second number
    temp_num_2 = temp_num_1 % temp_num_2;
    // Getting the previous second number, that will be the first number in the first steps
    temp_num_1 = temp;
  }
  return temp_num_1;
}


/**
 * @brief Specialization of BigInteger for the base 2
 */
template<> class BigInteger<2> {
 private:
  std::vector<bool> module_; // Vector of bools to improve efficiency
  bool sign_; // 1 if positive, 0 if negative

  std::vector<bool> UnsignedSum(const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) const; // Sum not considering the signs
  BigInteger<2> TwosComplement() const; // Two's complement

  void ProcessZeros();
  void Clear() {module_.clear();}
 public:
  // Constructors
  BigInteger(int n = 0);
  BigInteger(const unsigned char* );
  BigInteger(const BigUnsigned<2>&);
  BigInteger(const std::vector<bool>& module, bool sign) : module_(module), sign_(sign) {};
  BigInteger(const BigInteger<2>& big_binary) : module_(big_binary.getModule()), sign_(big_binary.getSign()) {}; // Constructor de copia
  
  // Getters
  const std::vector<bool> getModule() const {return module_;}
  const bool getSign() const {return sign_;}
  // Setters
  void setModule(const std::vector<bool>& module) {module_ = module;}
  void setSign(const bool sign) {sign_ = sign;}
  // Assignation operator 
  BigInteger<2>& operator=(const BigInteger<2>&);
  // Insertion and extraction operators
  friend std::ostream& operator<< (std::ostream&, const BigInteger<2>&);
  friend std::istream& operator>> (std::istream&, BigInteger<2>&);
  // Comparation operators
  friend bool operator< (const BigInteger<2>&, const BigInteger<2>&);
  // Arithmetic operators 
  friend BigInteger<2> operator+ (const BigInteger<2>&, const BigInteger<2>&);
  BigInteger<2> operator-(const BigInteger<2>&) const;
  BigInteger<2> operator*(const BigInteger<2>&) const;
  BigInteger<2> operator%(const BigInteger<2>&) const;
  friend BigInteger<2> operator/ (const BigInteger<2>&, const BigInteger<2>&);
  BigInteger<2> mcd(const BigInteger<2>&, const BigInteger<2>&) const;
  // Auxiliar method
  void AddDigit (unsigned char digit) {module_.push_back(digit);} // Adds a digit
};



/**
 * @brief Default constructor that builds a BigInteger<Base> (aka BI) object
 * @param int to be converted
 */
BigInteger<2>::BigInteger(int numero) {
  // If zero (base case), build 0 with sign positive
  if (numero == 0) {
    module_.push_back(0);
    sign_ = 1;
  } else {
    // If negative
    if (numero < 0) {
      // Get the positive number and add it to the module
      numero = numero * (-1);
      while (numero > 0) {
        unsigned char digit = static_cast<char>(numero % 10);
        module_.push_back(digit);
        numero /= 10;
      }
      // Set the sign to negative
      sign_ = 0;
    } else { // If positive, add it and set the sign at positive
      while (numero > 0) {
        unsigned char digit = static_cast<char>(numero % 10);
        module_.push_back(digit);
        numero /= 10;
      }
      sign_ = 1;
    }
  }
}


/**
 * @brief Constructor to build a BigUnsigned object
 * @param unsigned_char* array of chars to be converted
 */
BigInteger<2>::BigInteger(const unsigned char* char_array) {
  // Temporal vector. We'll use it to revert the addition order 
  std::vector<unsigned char> temp_digits;
  unsigned i {0};
  // For each char in the array, until it reaches '<\0'
  while (char_array[i] != '\0') {
    // If it's not a number, abort
    if (char_array[i] != '0' && char_array[i] != '1') { // REVISAR
      std::cerr << "The array must not contain a non-numeric character" << std::endl;
      return;
    } else {
      // Convert the digit
      bool digit;
      if (char_array[i] == '0') {
        digit = 0;
      } else {
        digit = 1;
      }
      temp_digits.push_back(digit);
    }
    ++i;    
  }

  // Revert the order
  for (int j = temp_digits.size() - 1; j >= 0; j--) {
    module_.push_back(temp_digits[j]);
  }
}


/**
 * @brief Constructor from a BigUnsigned number (copying the digits into module_)
 * @param BigUnsigned in base two to be copied
 */
BigInteger<2>::BigInteger(const BigUnsigned<2>& to_be_binary) {
  for (long unsigned int i {0}; i < to_be_binary.getDigits().size(); ++i) {
    module_.push_back(to_be_binary.getDigits()[i]);
  }
  sign_ = 1;
}


/**
 * @brief Copy constructor 
 * @param BI to be copied
 * @return BI copied
 */
BigInteger<2>& BigInteger<2>::operator=(const BigInteger<2>& big_integer) {
  if (this != &big_integer) {
    module_ = big_integer.getModule();
    sign_ = big_integer.getSign();
  }
  return *this;
}


/**
 * @brief Overload of << operator
 * @param ostream
 * @param BI number to be printed
 * @return ostream
 */
std::ostream& operator<<(std::ostream& os, const BigInteger<2>& num) {
  if (num.getModule().empty()) {
    os << '0';
  } else {
    if (num.getSign() == 0) { // If negative, print "-" before the number
      os << "-";
    }
    for (int i {num.getModule().size() - 1}; i >= 0; --i) {
      bool digit = num.getModule()[i];
      os << digit;  
    }
  }
  return os;
}


/**
 * @brief Overload of >> operator
 * @param istream
 * @param BI number to be inserted
 * @return istream
 */
std::istream& operator>>(std::istream& is, BigInteger<2>& num) {
  std::string input;
  is >> input;
  // Initialize a bool to create the BI number
  num.module_.clear();
  bool not_negative = true;

  std::vector <bool> temp;
  // If not empty and the string starts with "-"
  if (!(input.empty()) && input[0] == '-') {
    not_negative = false; //It's negative
    input.erase(0, 1); // Erase one position starting from the position zero ("-")
  }
  
  // Adding bools to the vector
  for (char caracter : input) {
    if (caracter != '1' && caracter != '0') {
      std::cerr << "Error" << std::endl;
      return is;
    } else if (caracter == '1') {
      temp.push_back(1);
    } else {
      temp.push_back(0);
    }
  }

  // Revert the order
  for (int i {temp.size()-1}; i >= 0; --i) {
    num.module_.push_back(temp[i]);
  }

  // Fix the sign
  num.sign_ = not_negative;

  return is;
}


/**
 * @brief Private method that clears the leading zeros resulting from sum and rest
 */
void BigInteger<2>::ProcessZeros() {
  while (module_.size() > 1 && module_.back() == 0) {
    module_.pop_back();
  }
}


/**
 * @brief Private method that calculates the unsigned sum of two binaries, but spreading the last bit (sign spreading)
 * @param BigInteger in base two, first number
 * @param BigInteger in base two, second number
 * @return BigInteger in base two, result of summing both
 */
std::vector<bool> BigInteger<2>::UnsignedSum(const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) const {
  // Getting copies, since we could add digits
  BigInteger<2> big_binary_1_copy = big_binary_1;
  BigInteger<2> big_binary_2_copy = big_binary_2;

  std::vector<bool> result;
  result.clear();

  // Getting the maximum size
  unsigned max_size = std::max(big_binary_1.getModule().size(), big_binary_2.getModule().size());

  // Before summing, sign extension
  if (big_binary_1.getModule().size() != max_size && big_binary_1.getSign() == 0) {
    for (long unsigned int i {big_binary_1.getModule().size()}; i < max_size; ++i) {
      big_binary_1_copy.AddDigit(1);
    }
  } else if (big_binary_2.getModule().size() != max_size && big_binary_2.getSign() == 0) {
    for (long unsigned int i {big_binary_2.getModule().size()}; i < max_size; ++i) {
      big_binary_2_copy.AddDigit(1);
    }
  }

  bool carry = false;

  // For each digit we get the bit of each position (if exists) and 0 if it does not exist
  for (unsigned int i {0}; i < max_size || carry; ++i) {
    bool bit1 = (i < big_binary_1_copy.getModule().size()) ? big_binary_1_copy.getModule()[i] : false; // Get the bit from the first number
    bool bit2 = (i < big_binary_2_copy.getModule().size()) ? big_binary_2_copy.getModule()[i] : false; // Get the bit from the second number
    // The sum is the XOR of bits and carry, and it exists a carry if any of the combinations is true (1)
    bool sum = bit1 ^ bit2 ^ carry;
    carry = (bit1 & bit2) | (bit1 & carry) | (bit2 & carry);
    // Insert it 
    result.push_back(sum);
  }

  return result;
}


/**
 * @brief Private method that returns the complement of the BigInteger that invoked this method
 * @return complement
 */
BigInteger<2> BigInteger<2>::TwosComplement() const {
  BigInteger<2> result = *this;
  
  // Bool that will show us when the first '1' is found
  bool first_found = false;
  for (unsigned int i {0}; i < result.module_.size(); ++i) {
    // If we don't find it, continue the loop
    if (first_found == false) {
      if (result.module_[i] != 1) {
        continue;
      } else {
        first_found = true; // If we find it, set the bool as true
      }
    } else {
      result.module_[i] = !result.module_[i]; // If it's been found, revert the bits of each position (0 -> 1, 1 -> 0)
    }
  }

  // Revert the sign
  result.sign_ = !result.sign_;

  return result;
}


/**
 * @brief Overload of < operator for BigInteger in base two. Calculates which one between two BigIntegers in base two
 * @param BI number 1 
 * @param BI number 2
 * @return bool. True if number 1 is less than number 2. False otherwise
 */
 bool operator<(const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) {
  if (big_binary_1.getSign() == 0 && big_binary_2.getSign() == 1) {
    return true;
  } else if (big_binary_1.getSign() == 1 && big_binary_2.getSign() == 0) {
    return false;
  } else if (big_binary_1.getSign() == 1 && big_binary_2.getSign() == 1) {
    if (big_binary_1.getModule().size() != big_binary_2.getModule().size()) {
      return big_binary_1.getModule().size() < big_binary_2.getModule().size();
    } else { // If same size then
      // For each digit (from more to less important)
      for (int i = big_binary_1.getModule().size() - 1; i >= 0; --i) {
        // If a digit is different, the one with minor value of digit will be the minor one
        if (big_binary_1.getModule()[i] < big_binary_2.getModule()[i]) {
          return true;
        } else if (big_binary_1.getModule()[i] > big_binary_2.getModule()[i]) {
          return false;
        }
      }
      return false;
    }
  } else {
    if (big_binary_1.getModule().size() != big_binary_2.getModule().size()) {
      return big_binary_1.getModule().size() > big_binary_2.getModule().size();
    } else { // If same size then
      // For each digit (from more to less important)
      for (int i = big_binary_1.getModule().size() - 1; i >= 0; --i) {
        // If a digit is different, the one with minor value of digit will be the minor one
        if (big_binary_1.getModule()[i] < big_binary_2.getModule()[i]) {
          return false;
        } else if (big_binary_1.getModule()[i] > big_binary_2.getModule()[i]) {
          return true;
        }
      }
      return true;
    }
  }
}


/**
 * @brief Overload of == operator for BigInteger in base two
 * @param BI number 1 
 * @param BI number 2
 * @return bool. True if number 1 and number 2 are the same. False otherwise
 */
bool operator==(const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) {
  if (big_binary_1.getModule().size() != big_binary_2.getModule().size() || big_binary_1.getSign() != big_binary_2.getSign()) {
    return false;
  } else {
    for (long unsigned int i {0}; i < big_binary_1.getModule().size(); ++i) {
      if (big_binary_1.getModule()[i] != big_binary_2.getModule()[i]) {
        return false;
      } 
    }
    return true;
  }
}


/**
 * @brief Overload of >= operator for BigInteger in base two
 * @param BI number 1 
 * @param BI number 2
 * @return bool. True if number 1 is greater or equal than number 2. False otherwise
 */
bool operator>=(const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) {
  return !(big_binary_1 < big_binary_2) || (big_binary_1 == big_binary_2);
}


/**
 * @brief Overload of + operator for BigInteger in base two. It sums both numbers using Twos complement when one of them  is negative
 */
BigInteger<2> operator+ (const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) {
  unsigned int max_size = std::max(big_binary_1.getModule().size(), big_binary_2.getModule().size());
  if (big_binary_1.getSign() == 0 && big_binary_2.getSign() == 0) {
    BigInteger<2> additional_1 = big_binary_1;
    additional_1.setSign(1);
    BigInteger<2> additional_2 = big_binary_2;
    additional_2.setSign(1);
    std::vector<bool> sum = big_binary_1.UnsignedSum(additional_1, additional_2);
    BigInteger<2> result (sum, 0);
    result.ProcessZeros();
    return result;
  } else if (big_binary_1.getSign() == 0 && big_binary_2.getSign() == 1) {
    if (big_binary_1.getModule() == big_binary_2.getModule()) {
      BigInteger<2> result;
      result.ProcessZeros();
      return result;
    } else {
      BigInteger<2> additional_1 = big_binary_1;
      additional_1.setSign(1);
      BigInteger<2> additional_2 = big_binary_2;
      additional_2.setSign(1);
      BigInteger<2> complement = additional_2.TwosComplement();
      std::vector<bool> sum = big_binary_1.UnsignedSum(additional_1, complement);
      if (sum.size() > max_size) {
        sum.pop_back();
      }
      bool sum_sign = (big_binary_2 < big_binary_1 || big_binary_2 == big_binary_1) ? 1 : 0;
      BigInteger<2> result (sum, sum_sign);
      result.ProcessZeros();
      return result;
    }
  } else if (big_binary_1.getSign() == 1 && big_binary_2.getSign() == 0) {
    if (big_binary_1.getModule() == big_binary_2.getModule()) {
      BigInteger<2> result;
      result.ProcessZeros();
      return result;
    } else {
      BigInteger<2> additional = big_binary_2;
      additional.setSign(1);
      BigInteger<2> complement = additional.TwosComplement();
      std::vector<bool> sum = big_binary_1.UnsignedSum(big_binary_1, complement);
      if (sum.size() > max_size) {
        sum.pop_back();
      }
      bool sum_sign = (big_binary_2 < big_binary_1 || big_binary_2 == big_binary_1) ? 1 : 0;
      BigInteger<2> result (sum, sum_sign);
      result.ProcessZeros();
      return result;
    }
  } else {
    std::vector<bool> sum = big_binary_1.UnsignedSum(big_binary_1, big_binary_2);
    BigInteger<2> result (sum, 1);
    result.ProcessZeros();
    return result;
  }
}



/**
 * @brief Pre-increment operator. It adds 1 to a BI in base two in pre-order
 * @param BI to be added 1
 * @return BU summed
 */
BigInteger<2>& operator++(BigInteger<2>& big_binary_1) {
  BigInteger<2> unit (1);
  big_binary_1 = big_binary_1 + unit;
  return big_binary_1;
}


/**
 * @brief Post-increment operator. It adds 1 to a BI in base two in post-order
 * @param BI to be added 1
 * @return BU summed
 */
BigInteger<2> operator++(BigInteger<2>& big_binary_1, int) {
  BigInteger<2> post_binary = big_binary_1;
  BigInteger<2> unit (1);
  big_binary_1 = big_binary_1 + unit;
  return post_binary;
}


/**
 * @brief Overload of - operator, it calculates the rest between two BI in base two
 * @param BI number to be rested
 * @return BI result of the rest
 */
BigInteger<2> BigInteger<2>::operator- (const BigInteger<2>& big_binary_2) const {
  unsigned int max_size = std::max(module_.size(), big_binary_2.getModule().size());
  if (sign_ == 0 && big_binary_2.getSign() == 0) {
    if (module_ == big_binary_2.getModule()) {
      BigInteger<2> result;
      result.ProcessZeros();
      return result;
    } else {
      BigInteger<2> partial = big_binary_2;
      partial.setSign(1);
      BigInteger<2> additional = *this;
      additional.setSign(0);
      BigInteger<2> complement = partial.TwosComplement();
      std::vector<bool> sum = big_binary_2.UnsignedSum(complement, additional);
      if (sum.size() > max_size) {
        sum.pop_back();
      }
      bool sum_sign = (big_binary_2 < *this || big_binary_2 == *this) ? 1 : 0;
      BigInteger<2> result (sum, sum_sign);
      result.ProcessZeros();
      return result;
    }
  } else if (sign_ == 0 && big_binary_2.getSign() == 1) {
    std::vector<bool> sum = big_binary_2.UnsignedSum(*this, big_binary_2);
    BigInteger<2> result (sum, 0);
    result.ProcessZeros();
    return result;
  } else if (sign_ == 1 && big_binary_2.getSign() == 0) {
    BigInteger<2> additional = big_binary_2;
    additional.setSign(1);
    std::vector<bool> sum = big_binary_2.UnsignedSum(*this, additional);
    BigInteger<2> result (sum, 1);
    result.ProcessZeros();
    return result;
  } else {
    if (module_ == big_binary_2.getModule()) {
      BigInteger<2> result;
      result.ProcessZeros();
      return result;
    } else {
      BigInteger<2> complement = big_binary_2.TwosComplement();
      std::vector<bool> sum = big_binary_2.UnsignedSum(*this, complement);
      if (sum.size() > max_size) {
        sum.pop_back();
      }
      bool sum_sign = (big_binary_2 < *this || big_binary_2 == *this) ? 1 : 0;
      BigInteger<2> result (sum, sum_sign);
      result.ProcessZeros();
      return result;
    }
  }
}


/**
 * @brief Pre-decrement operator. It rests 1 to a BI in base two in pre-order
 * @param BI to be rested 1
 * @return BI rested
 */
BigInteger<2>& operator--(BigInteger<2>& big_binary_1) {
  BigInteger<2> unit (1);
  big_binary_1 = big_binary_1 - unit;
  return big_binary_1;
}


/**
 * @brief Post-decrement operator. It rests 1 to a BI in base two in post-order
 * @param BI to be rested 1
 * @return BI rested
 */
BigInteger<2> operator--(BigInteger<2>& big_binary_1, int) {
  BigInteger<2> post_binary = big_binary_1;
  BigInteger<2> unit (1);
  big_binary_1 = big_binary_1 - unit;
  return post_binary;
}


/**
 * @brief Overload of * operator. It returns the multiplication between two BI in base two
 * @param BI Number 1
 * @param BI Number 2
 * @return BI result
 */
BigInteger<2> BigInteger<2>::operator* (const BigInteger<2>& mult) const {
  BigInteger<2> result;
  result.Clear();

  if (mult.getModule().empty() || module_.empty() || (mult.getModule()[0] == 0 && mult.getModule().size() == 1) || (module_[0] == 0 && module_.size() == 1)) {
    result.AddDigit(0);
    return result;
  }

  BigInteger<2> counter;
  BigInteger<2> mult_copy = mult;
  mult_copy.setSign(1);
  BigInteger<2> additional = *this;
  additional.setSign(1);

  while (counter < mult_copy) {
    result = result + additional; 
    ++counter; 
  }

  if (sign_ != mult.getSign()) {
    result.setSign(0);
  } else {
    result.setSign(1);
  }

  return result;
}


/**
 * @brief Overload of % operator. It returns the module resulting from the integer division between two BI
 * @param BI denominator
 * @return BI module result
 */
BigInteger<2> BigInteger<2>::operator% (const BigInteger<2>& big_binary) const {
  BigInteger<2> temp_num = *this;
  BigInteger<2> big_binary_copy = big_binary;

  temp_num.setSign(1);
  big_binary_copy.setSign(1);

  while (temp_num >= big_binary_copy) {
    temp_num = temp_num - big_binary_copy;
    temp_num.ProcessZeros();
  }

  return temp_num;
}


/**
 * @brief Overload of / operator. It returns the integer division between two BI
 * @param BI numerator
 * @param BI denominator
 * @return BI integer result
 */
BigInteger<2> operator/ (const BigInteger<2>& big_binary_1, const BigInteger<2>& big_binary_2) {
  BigInteger<2> temp_num;

  if ((big_binary_2.getModule().size() == 1 && big_binary_2.getModule()[0] == 0) || (big_binary_1.getModule().size() == 1 && big_binary_1.getModule()[0] == 0) || (big_binary_1.getModule().empty()) || (big_binary_2.getModule().empty())) {
    temp_num.AddDigit(0);
    return temp_num;
  }

  temp_num.Clear();
  BigInteger<2> counter (0);
  BigInteger<2> big_binary_2_copy = big_binary_2;
  temp_num = big_binary_1;
  temp_num.setSign(1);
  big_binary_2_copy.setSign(1);

  while (temp_num >= big_binary_2_copy) {
    temp_num = temp_num - big_binary_2_copy;
    temp_num.ProcessZeros();
    ++counter; 
  }

  if (big_binary_1.getSign() != big_binary_2.getSign()) {
    counter.setSign(0);
  } else {
    counter.setSign(1);
  }

  return counter;
}


/**
 * @brief GCD implementation between two BI in base two
 * @param BI Number 1
 * @param BI Number 2
 * @return Greatest common divisor between both BI in base two
 */
BigInteger<2> BigInteger<2>::mcd(const BigInteger<2>& num_1, const BigInteger<2>& num_2) const {
  // Getting two temporal BI to modify in the iterative version of the GCD, with motivation not to overflow the stack with recursive calls
  BigInteger<2> temp_num_1 = num_1;
  BigInteger<2> temp_num_2 = num_2;
  // Null Integer to compare in the while
  BigInteger<2> null;
  
  temp_num_1.setSign(1);
  temp_num_2.setSign(1);

  // While the second number is not 0
  while (!(temp_num_2 == null) && !(temp_num_2 < null)) {
    // Auxiliar number is the second number
    BigInteger<2> temp = temp_num_2;
    // Second number is module of first and second number
    temp_num_2 = temp_num_1 % temp_num_2;
    // Getting the previous second number, that will be the first number in the first steps
    temp_num_1 = temp;
  }
  return temp_num_1;
}


#endif