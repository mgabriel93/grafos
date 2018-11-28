#include "Matriz.h"
#include "Matriz.cpp"
#include "Lista.h"
#include "Lista.cpp"
#include "Euclidiana.h"
#include "Euclidiana.cpp"

#include <omp.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>
#include <sstream>


using namespace std;
int main()
{

  Euclidiana lista;
  lista.calcular("points-20",false);
}
