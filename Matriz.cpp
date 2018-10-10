#include <omp.h>
#include "Matriz.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>



using namespace std;


Matriz::carregar(std::string arquivo, std::string ompSettings)
{
  //Abre o arquivo
  string inputFile = arquivo+".txt";
  std::ifstream inFile(inputFile.c_str());

  //Caso consiga abrir o arquivo, preenche a matriz
  if (inFile.is_open())
  {
    //Carrega o numero de vertices
    string linha;
    getline(inFile, linha);
    //Dá cast para int...
    stringstream linhaInt(linha);
    nArestas = 0;
    linhaInt >> nVertices;
    matrizAdj.resize(nVertices); //Redimensiona a matri

    //Inicializa a matriz e redimensiona os vértices
    // cout << "Alocando espaco para matriz..." << endl;

    //Caso rode apenas uma thread com BFS, carrega na memória em várias threads
    #pragma omp parallel for if (ompSettings == "1")
    for (int i = 0; i <= nVertices; i++)
    {
      matrizAdj[i].resize(nVertices); //Redimensiona cada vértice da matriz
      for (int j = 0; j <= nVertices; j++)
      {
        matrizAdj[i][j] = false; //Inicializa as arestas
      }
    }

    //Preenche a matriz conforme o arquivo
    int v, a;
    while (inFile >> v >> a)
    {
      matrizAdj[v][a] = true;
      matrizAdj[a][v] = true; //Como não é direcionado, marca a volta como existente
      nArestas++;
    }
    cout << "Matriz carregada com sucesso!" << endl;
    inFile.close();
  }
}

Matriz::BFS(int inicio, std::string outputName, bool logFile)
{
  std::vector <bool> explorados; //Cria a lista de vértices explorados
  explorados.resize(nVertices); //Redimensiona a lista conforme necessário

  nivel.clear(); //Limpa a lista de níveis
  nivel.resize(nVertices); //Redimensiona a lista conforme necessário
  nivel[inicio] = 0; //Coloca o nível do vértice inicial como 0

  pai.clear(); //Limpa a lista de pais
  pai.resize(nVertices); //Redimensiona a lista conforme necessário
  pai[inicio] = inicio; //Coloca o pai do vértice inicial como ele mesmo

  std::list <int> fila; //Cria uma fila S

  //Coloca todos os vértices como não explorados
  for (int i = 0; i <= nVertices; i++)
  {
    explorados[i] = false;
  }

  fila.push_back(inicio); //Adiciona o vértice inicial à fila


  std::ostringstream inicioStr;
  inicioStr << inicio; //Dá cast para string.....

  ofstream output;
  if (logFile == true)
  {
    output.open((outputName+".csv").c_str()); //Inicializa o arquivo
    output << "Vertice;Pai;Grau" << endl; //Adiciona o título
    output << inicioStr << ";;0" << endl; //Adiciona a origem
  }

  while (fila.size() != 0)
  {
    int v;
    v = fila.front(); //Pega o vértice atual
    fila.pop_front();  //Remove o vértice atual da fila

    explorados[v] = true;
    for(int i = 0; i <= nVertices; i++) //Para cada vértice
    {
      if(matrizAdj[v][i] == true) //Verifica se é vizinho de V
      {
        if(explorados[i] == false) //Se for, verifica se não foi explorado
        {
          explorados[i] = true; //Caso não tenha sido explorado, marca como explorado
          fila.push_back(i); //Adiciona na fila
          nivel[i] = nivel[v] + 1; //O nível dele será o nível de quem descobiru ele +1
          pai[i] = v; //O pai dele será quem o descobriu
          if (logFile == true)
          {
            output << i << ";" << pai[i] << ";" << nivel[i] << endl; //Adiciona a linha no arquivo
          }
        }
      }
    }
  }
  if (logFile == true)
  {
    output.close(); //Fecha o arquivo
  }
  // cout << "BFS executada com sucesso!" << endl;
}

Matriz::DFS(int inicio, std::string outputName, bool logFile)
{
  std::vector <bool> explorados; //Cria a lista de vértices explorados
  explorados.resize(nVertices); //Redimensiona a lista conforme necessário

  nivel.clear(); //Limpa a lista de níveis
  nivel.resize(nVertices); //Redimensiona a lista conforme necessário
  nivel[inicio] = 0; //Coloca o nível do vértice inicial como 0

  pai.clear(); //Limpa a lista de pais
  pai.resize(nVertices); //Redimensiona a lista conforme necessário
  pai[inicio] = inicio; //Coloca o pai do vértice inicial como ele mesmo

  std::list <int> fila; //Cria uma fila S

  //Coloca todos os vértices como não explorados
  for (int i = 0; i <= nVertices; i++)
  {
    explorados[i] = false;
  }

  fila.push_back(inicio); //Adiciona o vértice inicial à fila


  std::ostringstream inicioStr;
  inicioStr << inicio; //Dá cast para string.....

  ofstream output;
  if (logFile == true)
  {
    output.open((outputName+".csv").c_str()); //Inicializa o arquivo
    output << "Vertice;Pai;Grau" << endl; //Adiciona o título
    output << inicioStr << ";;0" << endl; //Adiciona a origem
  }

  while (fila.size() != 0)
  {
    int v;
    v = fila.back(); //Pega o vértice atual
    fila.pop_back();  //Remove o vértice atual da fila

    explorados[v] = true;
    for(int i = 0; i <= nVertices; i++) //Para cada vértice
    {
      if(matrizAdj[v][i] == true) //Verifica se é vizinho de V
      {
        if(explorados[i] == false) //Se for, verifica se não foi explorado
        {
          explorados[i] = true; //Caso não tenha sido explorado, marca como explorado
          fila.push_back(i); //Adiciona na fila
          nivel[i] = nivel[v] + 1; //O nível dele será o nível de quem descobiru ele +1
          pai[i] = v; //O pai dele será quem o descobriu
          if (logFile == true)
          {
              output << i << ";" << pai[i] << ";" << nivel[i] << endl; //Adiciona a linha no arquivo
          }
        }
      }
    }
  }
  if (logFile == true)
  {
    output.close(); //Fecha o arquivo
  }
  // cout << "DFS executada com sucesso!" << endl;
}


Matriz::geraEstatisticas(std::string fileName)
{
  ofstream output;
  output.open((fileName+".txt").c_str());

  //Retorna o número de vértices e arestas
  output << "Numero de vertices: " << nVertices <<endl;
  output << "Numero de arestas : " << nArestas <<endl;

  //Retorna os pais dos vértices pedidos
  output << "Pai do vertice 10: " << pai[10] << endl;
  output << "Pai do vertice 20: " << pai[20] << endl;
  output << "Pai do vertice 30: " << pai[30] << endl;
  output << "Pai do vertice 40: " << pai[40] << endl;
  output << "Pai do vertice 50: " << pai[50] << endl;
}
