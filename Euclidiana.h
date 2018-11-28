#ifndef EUCLIDIANA_H
#define EUCLIDIANA_H
#include <iostream>
#include <string>
#include <vector>



class Euclidiana
{
  public:
    calcular(std::string arquivo, bool heur);
  protected:

  private:
    float calculaDistancia(int x1, int y1, int x2, int y2); //Bubble Sort
    std::vector<int> retornaCoordenadas(int n1, int n2); //Retorna as coordenadas de N1 e N2
    int nVertices; //Núero de vértices
    std::vector < std::vector<int> > listaAdj; //Usado para representar a lista.
    std::vector < std::vector<int> > listaOriginal; //Usado para representar a lista.
    std::string inputFile; //Nome do arquivo de entrada
};

#endif // LISTA_H
