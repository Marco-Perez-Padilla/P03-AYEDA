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

** Archivo big_unsigned.h: Declaracion e implementacion de la plantilla para representar numeros naturales grandes en cualquier base
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      05/02/2025 - Creacion (primera version) del codigo
**      11/02/2025 - Finalizacion de la primera version del codigo
**      21/02/2025 - Inicio de segunda version del codigo. Adaptación a plantillas
**      23/02/2025 - Adaptación de suma y resta para trabajar entre las bases 2 y 36
**/

#ifndef BIG_UNSIGNED_H
#define BIG_UNSIGNED_H

#include <iostream>
#include <vector>


/**
 * @brief Template of BigUnsigned classes, it generates a BigUnsigned number depending on the base passed as parameter.
 *        It is made out of the BigUnsigned class, being this modified to adapt it to template, finding general rules of
 *        calculation and representation
 * @param unsigned_char base of representation
 */
template <unsigned char Base = 10> class BigUnsigned {
 private:
  std::vector<unsigned char> digits_;

  // Auxiliar private methods
  void Clear() {digits_.clear();}
  void ProcessZeros();
 public:
  // Constructors
  BigUnsigned(unsigned n = 0); 
  BigUnsigned(const unsigned char* );
  BigUnsigned(const BigUnsigned& big_unsigned) : digits_(big_unsigned.getDigits()) {}; // Constructor de copia
  // Getters and setters
  std::vector<unsigned char> getDigits() const {return digits_;}
  void setDigits(const std::vector<unsigned char> digits) {digits_ = digits;}
  // Assignation operator 
  BigUnsigned& operator=(const BigUnsigned&);
  // Insertion and extraction operators
  friend std::ostream& operator<< <Base> (std::ostream&, const BigUnsigned<Base>&);
  friend std::istream& operator>> <Base> (std::istream&, BigUnsigned<Base>&);
  // Comparation operators
  friend bool operator< <Base> (const BigUnsigned<Base>&, const BigUnsigned<Base>&);
  // Arithmetic operators 
  friend BigUnsigned<Base> operator+ <Base> (const BigUnsigned<Base>&, const BigUnsigned<Base>&);
  BigUnsigned<Base> operator-(const BigUnsigned<Base>&) const;
  BigUnsigned<Base> operator*(const BigUnsigned<Base>&) const;
  BigUnsigned<Base> operator%(const BigUnsigned<Base>&) const;
  friend BigUnsigned<Base> operator/ <Base> (const BigUnsigned<Base>&, const BigUnsigned<Base>&);
  // Auxiliar method
  void AddDigit (unsigned char digit) {digits_.push_back(digit);} // Adds a digit
};


/**
 * @brief Default constructor that builds a BigUnsigned object
 * @param unsigned_int number to be converted
 */
template <unsigned char Base> BigUnsigned<Base>::BigUnsigned(unsigned numero) {
  // Base case and default behaviour
  if (numero == 0) {
    digits_.push_back(0);
  } else {
    while (numero > 0) {
      // Getting the digit and converting it to char type
      unsigned char digit = numero % 10;
      digits_.push_back(digit);
      // Advance to the next digit
      numero /= 10;
    }
  }
}


/**
 * @brief Constrcutor to build a BigUnsigned object
 * @param unsigned_char* array of chars to be converted
 */
template <unsigned char Base> BigUnsigned<Base>::BigUnsigned(const unsigned char* char_array) {
  // Temporal vector. We'll use it to revert the addition order 
  std::vector<unsigned char> temp_digits;
  unsigned i {0};
  // For each char in the array, until it reaches '<\0'
  while (char_array[i] != '\0') {
    // If it's not a number, abort
    if ((char_array[i] < '0' || char_array[i] > (Base + '0')) && (char_array[i] < 'A' || char_array[i] > 'Z' || Base < 10)) { // REVISAR
      std::cerr << "The array must not contain a non-numeric character" << std::endl;
      return;
    } else {
      // Convert the digit
      if (char_array[i] >= 'A' && char_array[i] <= 'Z') {
        unsigned char digit = char_array[i];
        temp_digits.push_back(digit);
      } else {
        unsigned char digit = char_array[i] - '0';;
        temp_digits.push_back(digit);
      }
      ++i;
    }
  }

  // Revert the order
  for (int j = temp_digits.size() - 1; j >= 0; j--) {
    digits_.push_back(temp_digits[j]);
  }
}


/**
 * @brief Copy constructor 
 * @param BigUnsigned to be copied
 * @return BigUnsigned copied
 */
template <unsigned char Base> BigUnsigned<Base>& BigUnsigned<Base>::operator=(const BigUnsigned<Base>& big_unsigned) {
  if (this != &big_unsigned) {
    digits_ = big_unsigned.digits_;
  }
  return *this;
}


/**
 * @brief Overload of << operator
 * @param ostream
 * @param BigUnsigned number to be printed
 * @return ostream
 */
template <unsigned char Base> std::ostream& operator<<(std::ostream& os, const BigUnsigned<Base>& num) {
  if (num.getDigits().empty()) {
    os << '0';
  } else {
    for (int i {num.getDigits().size() - 1}; i >= 0; --i) {
      // If the digit's value is greater than 9, we're on higher than 10 bases. We need letters to represent it. We get here its representation (not its ascii code)
      if (num.getDigits()[i] >= 'A' && num.getDigits()[i] <= 'Z') {
        os << num.getDigits()[i];
      } else {
        int digit = num.getDigits()[i];
        os << digit;
      }
    } 
  }
  return os;
}


/**
 * @brief Overload of >> operator
 * @param istream
 * @param BigUnsigned number to be inserted
 * @return istream
 */
template <unsigned char Base> std::istream& operator>>(std::istream& is, BigUnsigned<Base>& num) {
  std::string input;
  is >> input;
  num = BigUnsigned<Base>(reinterpret_cast<const unsigned char*>(input.data()));
  return is;
}


/**
 * @brief Overload of < operator for BigUnsigned class. Calculates which one between two BigUnsigneds (aka BU) is the minor one
 * @param BU number 1 
 * @param BU number 2
 * @return bool. True if number 1 is less than number 2. False otherwise
 */
template <unsigned char Base> bool operator<(const BigUnsigned<Base>& big_unsigned_1, const BigUnsigned<Base>& big_unsigned_2) {
  // The one with minor size will be the minor one between them
  if (big_unsigned_1.getDigits().size() != big_unsigned_2.getDigits().size()) {
    return big_unsigned_1.getDigits().size() < big_unsigned_2.getDigits().size();
  } else { // If same size then
    // For each digit (from more to less important)
    for (int i = big_unsigned_1.getDigits().size() - 1; i >= 0; --i) {
      // If a digit is different, the one with minor value of digit will be the minor one
      if (big_unsigned_1.getDigits()[i] < big_unsigned_2.getDigits()[i]) {
        return true;
      } else if (big_unsigned_1.getDigits()[i] > big_unsigned_2.getDigits()[i]) {
        return false;
      }
    }
    return false;
  }
}


/**
 * @brief Overload of == operator for BigUnsigned class
 * @param BU number 1 
 * @param BU number 2
 * @return bool. True if number 1 and number 2 are the same. False otherwise
 */
template <unsigned char Base> bool operator==(const BigUnsigned<Base>& big_unsigned_1, const BigUnsigned<Base>& big_unsigned_2) {
  if (big_unsigned_1.getDigits().size() != big_unsigned_2.getDigits().size()) {
    return false;
  } else {
    for (long unsigned int i {0}; i < big_unsigned_1.getDigits().size(); ++i) {
      if (big_unsigned_1.getDigits()[i] != big_unsigned_2.getDigits()[i]) {
        return false;
      } 
    }
    return true;
  }
}


/**
 * @brief Overload of >= operator for BigUnsigned class
 * @param BU number 1 
 * @param BU number 2
 * @return bool. True if number 1 is greater or equal than number 2. False otherwise
 */
template <unsigned char Base> bool operator>=(const BigUnsigned<Base>& big_unsigned_1, const BigUnsigned<Base>& big_unsigned_2) {
  return !(big_unsigned_1 < big_unsigned_2) || (big_unsigned_1 == big_unsigned_2);
}


/**
 * @brief Private method that clears the leading zeros resulting from sum and rest
 */
template <unsigned char Base> void BigUnsigned<Base>::ProcessZeros() {
  while (digits_.size() > 1 && digits_.back() == 0) {
    digits_.pop_back();
  }
}


/**
 * @brief Overload of + operator, it calculates the sum between two BU
 * @param BU number 1 to be summed
 * @param BU number 2 to be summed
 * @return BU result of the sum
 */
template <unsigned char Base> BigUnsigned<Base> operator+(const BigUnsigned<Base>& big_unsigned_1, const BigUnsigned<Base>& big_unsigned_2) {
  BigUnsigned<Base> result;
  // Clearing the BU result
  result.digits_.clear();
  // Declaring a carry to count while summing
  bool carry = 0;

  // Case: The two numbers have the same size
  if (big_unsigned_1.getDigits().size() == big_unsigned_2.getDigits().size()) {
    // For each digit
    for (unsigned int i {0}; i < big_unsigned_1.getDigits().size(); ++i) {
      // Getting the carry from the previous step (0 initially)
      int partial_result = carry;
      // If the digit's value is greater than 9, we're on higher than 10 bases. We need letters to represent it. We get here its real value
      int digit_1 = big_unsigned_1.getDigits()[i];
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      } 
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' +10;
      }
      // Summing the two digits
      partial_result += digit_1 + digit_2;
      // If the sum has two digits, we've got a carry for next steps
      if (partial_result / Base >= 1) {
        // Add the module and update the carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        }
        result.digits_.push_back(digit_to_add); 
        carry = 1;
      } else { // Otherwise, there is no carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 0;
      }
    }
    // If in the final step we still got a carry, add it
    if (carry) {
      result.digits_.push_back(carry);
    }
  } else if (big_unsigned_1.getDigits().size() > big_unsigned_2.getDigits().size()) { // Case: Number 1 size is greater than number 2 size
    unsigned int i {0};
    // For each digit of number 2 (the minor BU), we repeat the algorithm for same sizes
    while (i < big_unsigned_2.getDigits().size()) {
      int partial_result = carry;
      int digit_1 = big_unsigned_1.getDigits()[i];
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      } 
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' +10;
      }
      // Summing the two digits
      partial_result += digit_1 + digit_2;
      if (partial_result / Base >= 1) {
        // Add the module and update the carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        }
        result.digits_.push_back(digit_to_add); 
        carry = 1;
      } else {
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 0;
      }
      ++i;
    } // Where it was the final step before, we continue by propagating the carry through the rest of the digits of the greater number
    while (i < big_unsigned_1.getDigits().size()) {
      int partial_result = carry;
      int digit_1 = big_unsigned_1.getDigits()[i];
      int digit_2 = 0;
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      } 
      // Summing the two digits
      partial_result += digit_1 + digit_2;
      if (partial_result / Base >= 1) {
        // Add the module and update the carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 1;
      } else {
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 0;
      }
      ++i;
    }
    // If we still have a carry in the final step, add it
    if (carry) {
      result.digits_.push_back(carry);
    }
  } else { // Case: Number 2 size is greater than number 1 size. Repeat the previous algorithm
    unsigned int i {0};
    while (i < big_unsigned_1.getDigits().size()) {
      int partial_result = carry;
      int digit_1 = big_unsigned_1.getDigits()[i];
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      } 
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' +10;
      }
      // Summing the two digits
      partial_result += digit_1 + digit_2;
      if (partial_result / Base >= 1) {
        // Add the module and update the carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        }
        result.digits_.push_back(digit_to_add); 
        carry = 1;
      } else {
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 0;
      }
      ++i;
    }
    while (i < big_unsigned_2.getDigits().size()) {
      int partial_result = carry;
      int digit_1 = 0;
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' + 10;
      } 
      // Summing the two digits
      partial_result += digit_1 + digit_2;
      if (partial_result / Base >= 1) {
        // Add the module and update the carry
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        }
        result.digits_.push_back(digit_to_add); 
        carry = 1;
      } else {
        unsigned char digit_to_add = partial_result % Base;
        // Adapt it to a letter if it's bigger than 9
        if (digit_to_add > 9) {
          digit_to_add = (digit_to_add) + 'A' - 10;
        } 
        result.digits_.push_back(digit_to_add); 
        carry = 0;
      }
      ++i;
    }
    
    if (carry) {
      result.digits_.push_back(carry);
    }
  }

  // Eliminate possible leading zeros
  result.ProcessZeros();

  return result;
}


/**
 * @brief Pre-increment operator. It adds 1 to a BU in pre-order
 * @param BU to be added 1
 * @return BU summed
 */
template <unsigned char Base> BigUnsigned<Base>& operator++(BigUnsigned<Base>& big_unsigned_1) {
  BigUnsigned<Base> unit (1);
  big_unsigned_1 = big_unsigned_1 + unit;
  return big_unsigned_1;
}


/**
 * @brief Post-increment operator. It adds 1 to a BU in post-order
 * @param BU to be added 1
 * @return BU summed
 */
template <unsigned char Base> BigUnsigned<Base> operator++(BigUnsigned<Base>& big_unsigned_1, int) {
  BigUnsigned<Base> post_unsigned = big_unsigned_1;
  BigUnsigned unit (1);
  big_unsigned_1 = big_unsigned_1 + unit;
  return post_unsigned;
}


/**
 * @brief Overload of - operator, it calculates the rest between two BU. If the result is less than zero, it'll show zero
 * @param BU number to be rested
 * @return BU result of the rest
 */
template <unsigned char Base> BigUnsigned<Base> BigUnsigned<Base>::operator-(const BigUnsigned<Base>& big_unsigned_2) const {
  BigUnsigned<Base> result;
  // If the number to be rested is greater than the current one, return 0
  if (*this < big_unsigned_2 || *this == big_unsigned_2) {
    return result;
  }
  // Clear the result
  result.Clear();
  // Initialize a "carry" (for cases like when: 18-9, the carry will count that "1" after doing "8-9")
  int carry = 0;
  // Case: Both sizes are the same
  if (digits_.size() == big_unsigned_2.getDigits().size()) {
    // For each digit
    for (unsigned int i {0}; i < big_unsigned_2.getDigits().size(); ++i) {
      // Get the carry from previous step (initially zero)
      int partial_digit = carry;
      int digit_1 = digits_[i];
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      }
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' + 10;
      }
      // Sum the carry to the "inferior" digit
      partial_digit += digit_2;
      // If the "superior" limit is lesser than the "inferior" (case 8-9)
      if (digit_1 < partial_digit) {
        // The digit to be added will be result of the following formula: 10 - ("inferior" - "superior")
        partial_digit = Base - (partial_digit - digit_1);
        if (partial_digit / Base >= 1) {
          // Add the module and update the carry
          unsigned char digit_to_add = partial_digit % Base;
          // Adapt it to a letter if it's bigger than 9
          if (digit_to_add > 9) {
            digit_to_add = (digit_to_add) + 'A' - 10;
          }
          result.digits_.push_back(digit_to_add); 
        } else {
          if (partial_digit > 9) {
            partial_digit = partial_digit + 'A' -10;
          }
          result.AddDigit(partial_digit); 
        }
        // Propagate the carry
        carry = 1;
      } else { // Otherwise, rest normally without carry for the next steps
        partial_digit = digits_[i] - partial_digit; 
        if (partial_digit > 9) {
          partial_digit = partial_digit + 'A' -10;
        }
        result.AddDigit(partial_digit);
        carry = 0;
      }
    }
  } else if (digits_.size() > big_unsigned_2.getDigits().size()) { // Case: Current size is greater than number 2 size
    unsigned int i {0};
    // For each digit of number 2, do the previous algorithm
    while (i < big_unsigned_2.getDigits().size()) {
      int digit_1 = digits_[i];
      int digit_2 = big_unsigned_2.getDigits()[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      }
      if (digit_2 > 9) {
        digit_2 = digit_2 - 'A' + 10;
      }
      // Sum the carry to the "inferior" digit
      int partial_digit = digit_1 - digit_2 - carry;
      if (partial_digit < 0) {
        partial_digit += Base;
        carry = 1;                    
      } else {
        carry = 0;
      }
      if (partial_digit > 9) {
        partial_digit = partial_digit + 'A' -10;
      }
      result.AddDigit(partial_digit);
      ++i;
    }
    // For the rest of the digits
    while (i < digits_.size()) {
      int digit_1 = digits_[i];
      if (digit_1 > 9) {
        digit_1 = digit_1 - 'A' + 10;
      }
      int partial_digit = digit_1 - carry; 
      if (partial_digit < 0) {
        partial_digit += Base;
        carry = 1;                   // REVISAR AQUI
      } else {
          carry = 0;
      }
      if (partial_digit > 9) {
        partial_digit = partial_digit + 'A' -10;
      }
      result.AddDigit(partial_digit);
      ++i;
    }
  }
  // Clean zeros
  result.ProcessZeros();
  return result;
}


/**
 * @brief Pre-decrement operator. It rests 1 to a BU in pre-order
 * @param BU to be rested 1
 * @return BU rested
 */
template <unsigned char Base> BigUnsigned<Base>& operator--(BigUnsigned<Base>& big_unsigned_1) {
  BigUnsigned<Base> unit (1);
  big_unsigned_1 = big_unsigned_1 - unit;
  return big_unsigned_1;
}


/**
 * @brief Post-decrement operator. It rests 1 to a BU in post-order
 * @param BU to be rested 1
 * @return BU rested
 */
template <unsigned char Base> BigUnsigned<Base> operator--(BigUnsigned<Base>& big_unsigned_1, int) {
  BigUnsigned<Base> post_unsigned = big_unsigned_1;
  BigUnsigned<Base> unit (1);
  big_unsigned_1 = big_unsigned_1 - unit;
  return post_unsigned;
}


/**
 * @brief Overload of * operator. It returns the multiplication between two BU
 * @param BU Number 1
 * @param BU Number 2
 * @return BU result
 */
template <unsigned char Base> BigUnsigned<Base> BigUnsigned<Base>::operator*(const BigUnsigned<Base>& mult) const {
  BigUnsigned<Base> result;
  result.Clear();

  if (mult.getDigits().empty() || digits_.empty() || (mult.getDigits()[0] == 0 && mult.getDigits().size() == 1) || (digits_[0] == 0 && digits_.size() == 1)) {
    result.AddDigit(0);
    return result;
  }

  BigUnsigned<Base> counter;
  while (counter < mult) {
    result = result + *this; // Suma el número actual al resultado
    ++counter; // Incrementa el contador
  }

  return result;
}


/**
 * @brief Overload of % operator. It returns the module resulting from the integer division between two BU
 * @param BU denominator
 * @return BU module result
 */
template <unsigned char Base> BigUnsigned<Base> BigUnsigned<Base>::operator%(const BigUnsigned<Base>& big_unsigned) const {
  BigUnsigned<Base> temp_num;
  temp_num = *this;
  while (temp_num >= big_unsigned) {
    temp_num = temp_num - big_unsigned;
    temp_num.ProcessZeros();
  }

  return temp_num;
}


/**
 * @brief Overload of / operator. It returns the integer division between two BU
 * @param BU numerator
 * @param BU denominator
 * @return BU integer result
 */
template <unsigned char Base> BigUnsigned<Base> operator/(const BigUnsigned<Base>& big_unsigned_1, const BigUnsigned<Base>& big_unsigned_2) {
  BigUnsigned<Base> temp_num;

  if ((big_unsigned_2.getDigits().size() == 1 && big_unsigned_2.getDigits()[0] == 0) || (big_unsigned_1.getDigits().size() == 1 && big_unsigned_1.getDigits()[0] == 0) || (big_unsigned_1.getDigits().empty()) || (big_unsigned_2.getDigits().empty())) {
    temp_num.AddDigit(0);
    return temp_num;
  } else if (big_unsigned_2.getDigits().size() == 1 && (big_unsigned_2.getDigits()[0] == 1)) {
    return big_unsigned_1; 
  }

  temp_num.Clear();
  BigUnsigned<Base> counter;
  temp_num = big_unsigned_1;
  
  while (temp_num >= big_unsigned_2) {
    temp_num = temp_num - big_unsigned_2;
    ++counter; 
  }

  return counter;
}

#endif