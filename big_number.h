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

#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

template <unsigned char Base> class BigUnsigned;
template <unsigned char Base> class BigInteger;
template <unsigned char Base> class BigRational;

template <unsigned char Base> class BigNumber {
 protected:
  virtual std::ostream& write(std::ostream&) const = 0;
  virtual std::istream& read(std::istream&) = 0;
  friend std::ostream& operator<<(std::ostream& os, const BigNumber<Base>& num) {return num.write(os);}
  friend std::istream& operator>>(std::istream& is, BigNumber<Base>& num) { return num.read(is);}

 public:
  virtual BigNumber<Base>& add(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& subtract(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& multiply(const BigNumber<Base>&) const = 0;
  virtual BigNumber<Base>& divide(const BigNumber<Base>&) const = 0;

  virtual operator BigUnsigned<Base>() const = 0;
  virtual operator BigInteger<Base>() const = 0;
  virtual operator BigRational<Base>() const = 0;

  static BigNumber<Base>* create(const char* str);
};



template <unsigned char Base> BigNumber<Base>* BigNumber<Base>::create(const char* str) {
  std::string input(str);
    
  // Verificamos el sufijo para determinar el tipo de número grande
  if (input.back() == 'u') {
    input.pop_back(); // Quitamos el sufijo 'u'
    return new BigUnsigned<Base>(input); // Creamos un BigUnsigned
  } 
  else if (input.back() == 'i') {
    input.pop_back(); // Quitamos el sufijo 'i'
    return new BigInteger<Base>(input); // Creamos un BigInteger
  } 
  else if (input.back() == 'r') {
    input.pop_back(); // Quitamos el sufijo 'r'
    return new BigRational<Base>(input); // Creamos un BigRational
  }
  
  // Si el sufijo no es válido, lanzamos una excepción
  throw std::invalid_argument("Formato incorrecto: El número debe terminar en 'u', 'i' o 'r'");
}



#endif