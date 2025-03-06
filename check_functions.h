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

** Archivo check_functions.h: Declaracion de las funciones de chequeo de parámetros.
**
** Referencias:
**      Enlaces de interes

** Historial de revisiones:
**      05/02/2025 - Creacion (primera version) del codigo
**      12/02/2025 - Finalizacion de la primera version codigo
**      26/02/2025 - Adicion de checkfile functions
**/

#include <iostream>

bool ValidateFile (const std::string& name);
void CheckFileError (const std::string& name);
void ValidateCommand(int argc, char* argv[]);
void Help ();
void Usage();