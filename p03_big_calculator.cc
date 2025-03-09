/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Asignatura: Algoritmos y Estructuras de Datos Avanzadas
** Curso: 2º
** Practica 3: Representación de números grandes
** Autor: Marco Pérez Padilla
** Correo: alu0101469348@ull.edu.es
** Fecha: 05/02/2025

** Archivo p03_big_calculator.cc: programa cliente.
**      Contiene la funcion main del proyecto que usa las templates para trabajar con numeros racionales en bases 2, 8, 10 y 16
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      05/02/2025 - Creacion (primera version) del codigo
**      12/02/2025 - Version final de la primera version del codigo
**      24/02/2026 - Adaptacion provisional para trabajar con plantillas
**      26/02/2025 - Adicion de funciones para instanciar las plantillas en tiempo de compilacion
**      26/02/2025 - Adaptacion final del la version 2.0 del codigo para trabajar con ficheros de entrada y salida
**      07/03/2025 - Adicion de las clases BigNumber y BigNumberException
**      09/03/2025 - Adicion de ProcessFile
**/

#include <iostream>
#include <fstream>

#include "big_unsigned.h"
#include "big_integer.h"
#include "big_rational.h"
#include "big_number.h"
#include "calculator.h"

#include "check_functions.h"


/**
 * @brief main function, invokes the needed function to work as a client function, managing minor errors
 */
int main (int argc, char* argv[]) {  
  ValidateCommand(argc, argv);

  std::string input = std::string(argv[1]);
  std::string output = std::string(argv[2]);

  std::ifstream in_file(input);
  std::ofstream out_file(output);

  if (!in_file) {
    std::cerr << "Error: File " << input << " couldn't be opened" << std::endl;
    return EXIT_FAILURE;
  }
  
  if (!out_file) {
    std::cerr << "Error: File " << output << " couldn't be opened" << std::endl;
    return EXIT_FAILURE;
  }

  // Read the base, with default base = 10
  std::string line;
  unsigned char base = 10; 
  try {
    if (getline(in_file, line)) {
      if (line.substr(0, 5) == "Base ") {
        base = static_cast<unsigned char>(std::stoi(line.substr(6)));
        if (base != 2 && base != 8 && base != 10 && base != 16) {
          throw BigNumberNotSupportedBase();
        }
      }
    }
  } catch (const BigNumberNotSupportedBase& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }

  in_file.close(); 

  ProcessFile(base, input, output);

  out_file.close();

  return 0;
}