#ifndef MATRIZ_H
#define MATRIZ_H
#include <iostream>
#include <string>
#include <vector>



class Matriz
{
  public:
    carregar(std::string arquivo, std::string ompSettings);
    BFS(int inicio, std::string outputName, bool logFile);
    DFS(int inicio, std::string outputName, bool logFile);
    geraEstatisticas(std::string fileName);
  protected:

  private:
    int nVertices; //Núero de vértices
    int nArestas; //Número de arestas
    std::vector < std::vector<bool> > matrizAdj; //Usado para representar a matriz.
    std::vector <int> nivel; //Lista de níveis
    std::vector <int> pai; //Lista de pais
    std::string inputFile; //Nome do arquivo de entrada
};

#endif // MATRIZ_H
