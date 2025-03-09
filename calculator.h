/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 3: Calculadora para números grandes
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 09/03/2025

** Archivo calculator.h: Declaracion e implementacion de las funciones necesarias para la instanciacion y utilizacion de las plantillas en una calculadora RPN
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      09/03/2025 - Creacion (primera version) del codigo
**/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stack>

#include "big_unsigned.h"
#include "big_integer.h"
#include "big_rational.h"
#include "big_number.h"
#include "big_number_exception.h"

// Tabla de operandos con punteros a BigNumber<Base>
template <unsigned char Base> std::unordered_map<std::string, BigNumber<Base>*> map;

// Función para limpiar memoria
template <unsigned char Base> void ClearBoard() {
  for (auto& pair : map<Base>) {
    delete pair.second;
  }
  map<Base>.clear();
}

// Crear un número grande en la tabla
template <unsigned char Base> void CreateBigNumber(const std::string& label, const std::string& value) {
  if (map<Base>.count(label)) {
    delete map<Base>[label]; // Liberamos memoria anterior
  }

  map<Base>[label] = BigNumber<Base>::create(value.c_str());

  if (!map<Base>[label]) {
    std::cerr << "Error: Invalid number format for label " << label << '\n';
    map<Base>[label] = BigNumber<Base>::create("0u"); // En caso de error, asignamos "0u"
  }
}


// Evaluar una expresión en Notación Polaca Inversa
template <unsigned char Base> void EvaluateExpression(const std::string& label, std::istringstream& expression) {
  std::stack<BigNumber<Base>*> stack;
  std::string command;

  try {
    // While we have the expression
    while (expression >> command) {
      // If the "command" is on the map, then it is an operand. Therefore, we push it onto the stack
      if (map<Base>.count(command)) {
        stack.push(map<Base>[command]);
      } else { // Otherwise, it is an operation. We consider only operations with arity 2
        // If there are less than two operands on the stack, operation cannot be made
        if (stack.size() < 2) {
          throw BigNumberException("Not enough operands.");
        }
        // Getting the operands
        BigNumber<Base>* operand_2 = stack.top(); 
        stack.pop();
        BigNumber<Base>* operand_1 = stack.top(); 
        stack.pop();

        BigNumber<Base>* result = nullptr;

        // Doing the operation
        if (command == "+") {
          result = &operand_1->add(*operand_2);
        } else if (command == "-") {
          result = &operand_1->subtract(*operand_2);
        } else if (command == "*") {
          result = &operand_1->multiply(*operand_2);
        } else if (command == "/") {
          result = &operand_1->divide(*operand_2);
        } else {
          throw BigNumberException("Unknown operator: " + command);
        }
        // Push the result onto the stack
        stack.push(result);
      }
    }
    // If, after the calculations, there are more than one elements on the stack, the expression is not valid
    if (stack.size() != 1) {
      throw BigNumberException("Invalid expression.");
    }
    // Clearing the memory
    if (map<Base>.count(label)) {
      delete map<Base>[label];
    }
    // Associating the label with the result
    map<Base>[label] = stack.top();
  } catch (const BigNumberException& e) { // If any exception occurred, we notify it and associate the label with "0u"
    std::cerr << "Error: " << e.what() << '\n';
    map<Base>[label] = BigNumber<Base>::create("0u");
  }
}


// Procesamiento del archivo según la base
void ProcessFile(unsigned char base, const std::string& input_file, const std::string& output_file) {
  std::ifstream in_file(input_file);
  std::ofstream out_file(output_file);
  std::string line;

  // The first line contains the base. Since we read it in the main, we skip it
  getline(in_file, line); 

  // We don't have a limit of lines. Hence, we read until the EOF.
  while (std::getline(in_file, line)) {
    std::istringstream reading_line(line);
    std::string label; // Storages the name of the operands
    std::string value; // Storages the value of the operands
    char operation;

    reading_line >> label >> operation;

    // If operation is '=', next will be value of an operand. Otherwise next will be an expression to evaluate
    if (operation == '=') {
      reading_line >> value;
      switch(base) {
        case 2:
          CreateBigNumber<2>(label, value);
          break;
          case 8:
          CreateBigNumber<8>(label, value);
          break;
        case 10:
          CreateBigNumber<10>(label, value);
          break;
        case 16:
          CreateBigNumber<16>(label, value);
          break;
        default:
          // If none of above, abort
          std::cerr << "non-available base: " << static_cast<int>(base) << std::endl;
          exit(EXIT_FAILURE);
      }
      out_file << label << " = " << value << '\n';
    } else if (operation == '?') {
      switch(base) {
        case 2:
          std::cout << "Reading expression in base 2 '" << label << "'..."<< std::endl;
          EvaluateExpression<2>(label, reading_line);
          break;
          case 8:
          std::cout << "Reading expression in base 2 '" << label << "'..."<< std::endl;
          EvaluateExpression<8>(label, reading_line);
          break;
        case 10:
          std::cout << "Reading expression in base 2 '" << label << "'..."<< std::endl;
          EvaluateExpression<10>(label, reading_line);
          break;
        case 16:
          std::cout << "Reading expression in base 2 '" << label << "'..."<< std::endl;
          EvaluateExpression<16>(label, reading_line);
          break;
        default:
          // If none of above, abort
          std::cerr << "non-available base: " << static_cast<int>(base) << std::endl;
          exit(EXIT_FAILURE);
      }
    }
  }

  switch(base) {
    case 2:
      std::cout << "Clearing map ..." << std::endl;
      ClearBoard<2>();
      break;
      case 8:
      std::cout << "Clearing map ..." << std::endl;
      ClearBoard<2>();
      break;
    case 10:
      std::cout << "Clearing map ..." << std::endl;
      ClearBoard<2>();
      break;
    case 16:
      std::cout << "Clearing map ..." << std::endl;
      ClearBoard<2>();
      break;
    default:
      // If none of above, abort
      std::cerr << "non-available base: " << static_cast<int>(base) << std::endl;
      exit(EXIT_FAILURE);
  }
}


#endif