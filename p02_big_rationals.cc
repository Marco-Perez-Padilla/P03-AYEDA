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

** Archivo p02_big_rationals.cc: programa cliente.
**      Contiene la funcion main del proyecto que usa las templates para trabajar con numeros racionales en bases 2, 8, 10 y 16
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      05/02/2025 - Creacion (primera version) del codigo
**      12/02/2025 - Version final de la primera version del codigo
**      24/02/2026 - Adaptacion provisional para trabajar con plantillas
**      26/02/2025 - Adicion de funciones para instanciar las plantillas en tiempo de compilacion
**      26/02/2025 - Adaptacion final del codigo para trabajar con ficheros de entrada y salida
**/

#include <iostream>
#include <fstream>

#include "big_unsigned.h"
#include "big_integer.h"
#include "big_rational.h"

#include "check_functions.h"


/**
 * @brief Template function that will do the required operations based on the templates. This function is not responsive of the instantiation
          of the templates in compilation times. It's merely responsive of the operations themselves, requiring itself a proper instantiation 
          by other function
 * @param ofstream file where the results will be written in
 * @param ifstream file where the numbers will be read from
 */
template <unsigned char B> void ProcessBaseRationals(std::ofstream& outfile, std::ifstream& infile) {
  std::string line;
  BigRational<B> numero_1, numero_2; // Template BigRational

  if (getline(infile, line)) {      // Getting the first line
    if (line.substr(0, 2) == "N1") {
      // Substracting the number, without any other tags
      std::string rationalStr = line.substr(line.find("=") + 1);
      std::istringstream iss(rationalStr);
      // Using the >> operator from BigRational
      iss >> numero_1;
    }
  }
  
  // Same with the second number
  if (getline(infile, line)) {
      if (line.substr(0, 2) == "N2") {
          std::string rationalStr = line.substr(line.find("=") + 1);
          std::istringstream iss(rationalStr);
          iss >> numero_2;
      }
  }
  
  // Print the base into the output file 
  outfile << "Base = " << static_cast<int>(B) << std::endl;

  // Printing the numbers into the output file 
  outfile << "N1 = " << numero_1 << std::endl;
  outfile << "N2 = " << numero_2 << std::endl;
  
  // Operations
  outfile << "N1 + N2 = " << (numero_1 + numero_2) << std::endl;
  outfile << "N1 - N2 = " << (numero_1 - numero_2) << std::endl;
  outfile << "N1 * N2 = " << (numero_1 * numero_2) << std::endl;
  outfile << "N1 / N2 = " << (numero_1 / numero_2) << std::endl;
  outfile << "N1 < N2 : "<< ((numero_1 < numero_2) ? "true" : "false") << std::endl;
  outfile << "N1 == N2 : " << ((numero_1 == numero_2) ? "true" : "false") << std::endl;
}



/**
 * @brief Function that, given a base, it instantiates the correspondent template, considering bases 2, 8, 10 and 16
 * @param unsigned_char base in which we're working
 * @param string name of the output file
 * @param string name of the input file
 */
void ProcessRationals(unsigned char base, const std::string& output_file, const std::string& input_file) {
  // Opening the files
  std::ifstream in_file(input_file);
  std::ofstream out_file(output_file);
  std::string line;
  
  // The first line contains the base. Since we read it in the main, we skip it
  getline(in_file, line); 
  
  // Instantiation depending on the base
  switch(base) {
    case 2:
      std::cout << "Generating file '" << output_file << "'..."<< std::endl;
      ProcessBaseRationals<2>(out_file, in_file);
      break;
    case 8:
      std::cout << "Generating file '" << output_file << "'..."<< std::endl;
      ProcessBaseRationals<8>(out_file, in_file);
      break;
    case 10:
      std::cout << "Generating file '" << output_file << "'..."<< std::endl;
      ProcessBaseRationals<10>(out_file, in_file);
      break;
    case 16:
      std::cout << "Generating file '" << output_file << "'..."<< std::endl;
      ProcessBaseRationals<16>(out_file, in_file);
      break;
    default:
      // If none of above, abort
      std::cerr << "non-available base: " << static_cast<int>(base) << std::endl;
      exit(EXIT_FAILURE);
  }
}


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
  
  if (getline(in_file, line)) {
    if (line.substr(0, 5) == "Base ") {
      base = static_cast<unsigned char>(std::stoi(line.substr(6)));
    }
  }
  
  in_file.close(); 

  ProcessRationals(base, output, input);

  out_file.close();

  return 0;
}