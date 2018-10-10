#ifndef LISTA_H
#define LISTA_H
#include <iostream>
#include <string>
#include <vector>



class Lista
{
  public:
    carregar(std::string arquivo);
    caminho(int in);
    excentricidade();
    minimumSpanningTree();
    maiorGrau(int quant);
    distanciaMedia();
    vizinhos(int vert);
    BFS(int inicio, std::string outputName, bool logFile);
    DFS(int inicio, std::string outputName, bool logFile);
    Pesos(int inicio, bool logFile, bool mst);
    BFScomponentes(int inicio, std::vector<bool> &explorados, std::vector<int> &elementos);
    int BFSdiametro(int inicio, std::vector<bool> explorados);
    geraEstatisticas(std::string fileName);
    componentes(std::string outputName);
    diametro(std::string inputFile);
    grauCalc(std::string outputName);
    bool negativo;
    buscaNomes(std::string pesquisador);
  protected:

  private:
    bubbleSort(std::vector < vector < int> > &lista, int tamanho); //Bubble Sort
    std::vector <int> elemComponentes; //Elementos (vértices) pertencentes às componentes
    int nVertices; //Núero de vértices
    int vertMax;   //Índice do maior vértice. Será nVertices+1, utilizado para auxiliar loops
    int nArestas; //Número de arestas
    std::vector < std::vector<int> > listaAdj; //Usado para representar a lista.
    std::vector < std::vector<float> > listaPesos; //Usado para representar os pesos.
    std::vector <int> grau; //Lista de níveis
    std::vector <int> nivel; //Lista de níveis
    std::vector <int> pai; //Lista de pais
    std::vector <float> distOrigem; //Lista da distância até a origem
    std::string inputFile; //Nome do arquivo de entrada
};

#endif // LISTA_H
