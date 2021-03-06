#include <bits/stdc++.h>
#include <omp.h>
#include <time.h>
#include "Lista.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>

//TODO:

using namespace std;

typedef pair<int, int> par;



Lista::bubbleSort(std::vector < vector < int> > &lista, int tamanho) //Ordena um vetor por bubblesort, começando por i = 1
{
  for (int i = 1; i <= tamanho-1; i++)
  {
    for (int j = 1; j <= tamanho-i-1; j++)
    {
      if (lista[j].size() < lista[j+1].size())
      {
         vector<int> aux = lista[j];
         lista[j] = lista[j+1];
         lista[j+1] = aux;
      }
    }
  }
}

Lista::carregar(std::string arquivo)
{
  //Abre o arquivo
  string inputFile = arquivo+".txt";
  std::ifstream inFile(inputFile.c_str());

  //Caso consiga abrir o arquivo, preenche a Lista
  if (inFile.is_open())
  {
    //Carrega o numero de vertices
    string linha;
    getline(inFile, linha);
    //Dá cast para int...
    stringstream linhaInt(linha);
    nArestas = 0;
    linhaInt >> nVertices;
    vertMax = nVertices+1; //Índice para auxiliar loops e matrizes
    listaAdj.resize(vertMax); //Redimensiona a lista
    if (true == true)
    {
      listaPesos.resize(vertMax);
      negativo = false;
    }

    for (int i = 1; i <= nVertices; i++)
    {
      listaAdj[i].push_back(i); //Adiciona um vértice do elemento para ele mesmo. Pode ser útil no futuro, e evita bugs com índice 0
      listaPesos[i].push_back(i); //Adiciona um vértice do elemento para ele mesmo. Pode ser útil no futuro, e evita bugs com índice 0
    }

    //Inicializa a Lista e redimensiona os vértices
    // cout << "Carregando a Lista..." << endl;

    //Caso rode apenas uma thread com BFS, carrega na memória em várias threads
    //Preenche a Lista conforme o arquivo

    int v, a;
    float w;

    while (inFile >> v >> a >> w)
    {
      //Adiciona a aresta ao vetor
      if(v <= vertMax && a <= vertMax)
      {
        nArestas++;
        listaAdj[v].push_back(a);
        listaAdj[a].push_back(v);
        if (w >= 0)
        {
          listaPesos[v].push_back(w);
          listaPesos[a].push_back(w);
        }
        else
        {
          negativo = true;
        }
      }
    }
    // cout << "Lista carregada com sucesso!" << endl;
  }
  inFile.close();
}

Lista::BFS(int inicio, std::string outputName, bool logFile)
{
  std::vector <bool> explorados(vertMax, false);

  nivel.clear(); //Limpa a lista de níveis
  nivel.resize(vertMax); //Redimensiona a lista conforme necessário
  nivel[inicio] = 0; //Coloca o nível do vértice inicial como 0

  pai.clear(); //Limpa a lista de pais
  pai.resize(vertMax); //Redimensiona a lista conforme necessário
  pai[inicio] = inicio; //Coloca o pai do vértice inicial como ele mesmo

  std::list <int> fila; //Cria uma fila S
  fila.push_back(inicio); //Adiciona o vértice inicial à fila

  ofstream output;
  if(logFile == true)
  {
    output.open((outputName+".csv").c_str()); //Inicializa o arquivo
    output << "Vertice;Pai;Grau" << endl; //Adiciona o título
    output << inicio << ";;0" << endl; //Adiciona a origem
  }

  while (fila.size() != 0)
  {
    int v;
    v = fila.front(); //Pega o vértice atual
    fila.pop_front();  //Remove o vértice atual da fila

    explorados[v] = true;

    for (int i = 1; i <= listaAdj[v].size()-1; i++) //Varre o array de vértices
    {
      int w;
      w = listaAdj[v][i];

      if (explorados[w] == false)
      {
        explorados[w] = true;
        fila.push_back(w);
        nivel[w] = nivel[v] + 1; //O nível dele será o nível de quem descobiru ele +1
        pai[w] = v; //O pai dele será quem o descobriu

        if (logFile == true)
        {
          output << w << ";" << pai[w] << ";" << nivel[w] << endl; //Adiciona a linha no arquivo
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

Lista::DFS(int inicio, std::string outputName, bool logFile)
{
  std::vector <bool> explorados(vertMax, false); //Cria a lista de vértices explorados

  nivel.clear(); //Limpa a lista de níveis
  nivel.resize(vertMax); //Redimensiona a lista conforme necessário
  nivel[inicio] = 0; //Coloca o nível do vértice inicial como 0

  pai.clear(); //Limpa a lista de pais
  pai.resize(vertMax); //Redimensiona a lista conforme necessário
  pai[inicio] = inicio; //Coloca o pai do vértice inicial como ele mesmo

  std::list <int> fila; //Cria uma fila S
  fila.push_back(inicio); //Adiciona o vértice inicial à fila


  std::ostringstream inicioStr;
  inicioStr << inicio; //Dá cast para string.....

  ofstream output;
  if(logFile == true)
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
    for (int i = 0; i <= listaAdj[v].size()-1; i++) //Varre o array de vértices
    {
      int w;
      w = listaAdj[v][i];

      if (explorados[w] == false)
      {
        explorados[w] = true;
        fila.push_back(w);
        nivel[w] = nivel[v] + 1; //O nível dele será o nível de quem descobiru ele +1
        pai[w] = v; //O pai dele será quem o descobriu
        if (logFile == true)
        {
          output << w << ";" << pai[w] << ";" << nivel[w] << endl; //Adiciona a linha no arquivo
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

Lista::BFScomponentes(int inicio, std::vector<bool> &explorados, std::vector<int> &elementos)
{
  int aux = 0;
  std::vector <int> nivelLight(vertMax, -1); //Cria uma lista de níveis interna

  std::list <int> fila; //Cria uma fila S
  fila.push_back(inicio); //Adiciona o vértice inicial à fila
  elementos.push_back(inicio); //Adiciona o vértice inicial à lista de componentes
  nivelLight[inicio] = 0;

  while (fila.size() != 0)
  {
    int v;
    v = fila.front(); //Pega o vértice atual
    fila.pop_front();  //Remove o vértice atual da fila
    explorados[v] = true;

    for (int i = 1; i <= listaAdj[v].size()-1; i++) //Varre o array de vértices
    {
      int w;
      w = listaAdj[v][i];
      if (explorados[w] == false)
      {
        explorados[w] = true;
        elementos.push_back(w); //Adiciona W na lsita de elementos da componente
        fila.push_back(w);
        nivelLight[w] = nivelLight[v] + 1; //O nível dele será o nível de quem descobiru ele +1
        if (nivelLight[w] > aux) //Verifica se o nível novo é maior do que o atual máximo
        {
          aux = nivelLight[w];
        }
      }
    }
  }
  return aux;
}

int Lista::BFSdiametro(int inicio, std::vector<bool> explorados)
{
  int aux = 0;
  std::vector <int> nivelLight(vertMax, -1); //Cria uma lista de níveis interna

  std::list <int> fila; //Cria uma fila S
  fila.push_back(inicio); //Adiciona o vértice inicial à fila
  nivelLight[inicio] = 0;

  while (fila.size() != 0)
  {
    int v;
    v = fila.front(); //Pega o vértice atual
    fila.pop_front();  //Remove o vértice atual da fila
    explorados[v] = true;

    for (int i = 1; i <= listaAdj[v].size()-1; i++) //Varre o array de vértices
    {
      int w;
      w = listaAdj[v][i];
      // cout << w << endl;
      if (explorados[w] == false)
      {
        explorados[w] = true;
        fila.push_back(w);
        nivelLight[w] = nivelLight[v] + 1; //O nível dele será o nível de quem descobiru ele +1
        if (nivelLight[w] > aux) //Verifica se o nível novo é maior do que o atual máximo
        {
          aux = nivelLight[w];
        }
      }
    }
  }
  return aux;
}

Lista::geraEstatisticas(std::string fileName)
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

  int grauMin, grauMax;
  float grauMed, grauMediano;

  output.close();
}

Lista::componentes(std::string outputName)
{
  std::vector <bool> explorados(vertMax, false);
  std::vector < vector < int > > componentes;
  int nComponentes = 0;
  clock_t tInicio = clock();

  for (int i = 1; i < nVertices; i++)
  {
    if (explorados[i] == false)
    {
      std::vector<int> elementos;
      nComponentes++;
      BFScomponentes(i, explorados, elementos);
      componentes.push_back(elementos);
    }
  }

  //Calcula a ordem dos elementos
  bubbleSort(componentes, nComponentes);

  ofstream outputComponentes;
  outputComponentes.open((outputName+"_componentes.txt").c_str());
  cout << nComponentes << endl;
  //Imprime os elementos no arquivo de saída
  for (int i = 1; i <= nComponentes; i++)
  {
    outputComponentes << "N de elementos: " << componentes[i-1].size() << ". Elementos: ";
    for (int j = 0; j <= componentes[i-1].size()-1; j++)
    {
      outputComponentes << componentes[i-1][j] << " ";
    }
    outputComponentes << endl;
  }
  outputComponentes << "Tempo de execucao: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
  outputComponentes.close();
  grauCalc(outputName);
}

Lista::grauCalc(std::string outputName)
{
 //Cálculo de grau
 ofstream outputGraus;
 outputGraus.open((outputName+"_graus.txt").c_str());

 //Calcula o grau máximo e a média
 int grauMin, grauMax;
 float grauMed, grauMediano;
 std::vector <int> arrayGrau;

 grauMax = 0;
 grauMed = 0;
 for (int i = 1; i <= nVertices; i++)
 {
   arrayGrau.push_back(listaAdj[i].size()-1);
   grauMed = grauMed + listaAdj[i].size() - 1;
   if ((listaAdj[i].size() - 1) > grauMax) //Se o vértice i tiver grau maior do que grauMax, atualiza
   {
     grauMax = listaAdj[i].size() - 1;
   }
 }
 grauMed = grauMed / nVertices; //Divide a soma total dos graus pelo número de vértices para calcular a média

 //Calcula o grau mínimo
 grauMin = grauMax; //Coloca o grau mínimo igual ao máximo
 for (int i = 1; i <= vertMax; i++)
 {
   if ((listaAdj[i].size()-1) < grauMin) //Se o vértice i tiver grau menor do que grauMin e não for a origem, atualiza
   {
     grauMin = listaAdj[i].size() - 1;
   }
 }

 //Calcula a mediana
 sort(arrayGrau.begin(), arrayGrau.end());
 if (arrayGrau.size() % 2)
 {
   grauMediano = (arrayGrau[nVertices/2-1] + arrayGrau[nVertices/2]) / 2;
 }
 else
 {
   grauMediano = arrayGrau[nVertices/2];
 }

 outputGraus << "Grau minimo: " << grauMin << endl;
 outputGraus << "Grau maximo: " << grauMax << endl;
 outputGraus << "Grau Medio : " << grauMed << endl;
 outputGraus << "Mediana    : " << grauMediano << endl;
}

Lista::diametro(string outputName)
{
  //Cria um arquivo de saída
  ofstream outputDiametro;
  outputDiametro.open((outputName+"_diametro.txt").c_str());

  clock_t tInicio = clock();

  int diametro = 0;
  std::vector<int> auxGraus(vertMax, 0); //Cria um vetor para armazenar os gruas máximos inicializando em cada vértice
  std::vector<int> elementos;

  #pragma omp parallel for
  for(int i = 1; i <= nVertices; i++)
  {
    std::vector <bool> explorados(vertMax, false);  //Coloca todos os vértices como não explorados
    auxGraus[i] = BFSdiametro(i, explorados); //Calcula a BFS a partir de cada vértice
  }

  //For utilizado para calcular o maior valor. Poderia colocar dentro do for anterior, mas assim não perde tempo sincronizando as threads
  for (int i = 1; i <= nVertices; i++)
  {
    if (auxGraus[i] > diametro)
    {
        diametro = auxGraus[i];
    }
  }
  outputDiametro << "Diametro do grafo: " << diametro << endl;
  outputDiametro << "Tempo de execucao: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
  outputDiametro.close();
}

Lista::caminho(int in)
{
  cout << distOrigem[in] << " - ";
  while (in != pai[in])
  {
    cout << in << " ";
    in = pai[in];
  }
  cout << in << endl;
}

Lista::Pesos(int inicio, bool logFile, bool mst = false)
{

  std::vector <bool> explorados(vertMax, false);
  std::vector <float> distancia(vertMax, 10000);

  distancia[inicio] = 0;
  pai.clear(); //Limpa a lista de pais
  pai.resize(vertMax); //Redimensiona a lista conforme necessário
  pai[inicio] = inicio; //Coloca o pai do vértice inicial como ele mesmo

  distOrigem.resize(vertMax); //Redimensiona a lista conforme necessário

  priority_queue< par, vector <par> , greater<par> > fila;

  fila.push(make_pair(distancia[inicio], inicio));

  while (!fila.empty())
  {
    int u;
    u = fila.top().second; //Pega o vértice atual
    fila.pop();  //Remove o vértice atual da fila

    explorados[u] = true;
    distOrigem[u] = distancia[u];

    for (int i = 1; i <= listaAdj[u].size()-1; i++) //Varre o array de vértices
    {

      int v;
      float pesoUV;
      v = listaAdj[u][i];
      pesoUV = listaPesos[u][i];

      if (explorados[v] == false)
      {
        if (mst == false)
        //Dijkstra
        {
          if(distancia[v] > (distancia[u] + pesoUV))
          {
            distancia[v] = distancia[u] + pesoUV;
            fila.push(make_pair(distancia[v], v));
            pai[v] = u;
            // cout << u << " " << listaAdj[u][i] << " " << distancia[u] << endl; //Debug
          }
        }
        else
        //Prim
        {
          if(distancia[v] > pesoUV)
          {
            distancia[v] = pesoUV;
            fila.push(make_pair(distancia[v], v));
            pai[v] = u;
          }
        }
      }
    }
  }
}

Lista::excentricidade()
{
  float distMax = 0;
  for(int i= 1; i <= nVertices; i++)
  {
    if (distOrigem[i] > distMax)
    {
      distMax = distOrigem[i];
    }
  }
  cout << distMax << endl;
}

Lista::minimumSpanningTree()
{
  ofstream output;
  output.open("runlog.txt");

  int peso = 0;
  for (int i = 1; i <= nVertices; i++)
  {
    output << pai[i] << " " << i << " " << distOrigem[i] << endl;
    peso = peso + distOrigem[i];
  }
  output.close();
  cout << peso << endl;;
}

Lista::distanciaMedia()
{
  double soma;
  int counter = 0;
  for (int i=1; i <= nVertices; i++)
  {
      Pesos(i, false);
      for (int i = 1; i <= nVertices; i++)
      {
        soma = soma + distOrigem[i];
        counter++;
      }
  }

  cout << "Distancia media: " << soma/counter << endl;
}

Lista::maiorGrau(int quant)
{
  std::vector <int> grau(vertMax, 0);

  for (int i = 1; i <= nVertices; i++)
  {
    grau[pai[i]]++;
    grau[i]++;
  }

  for (int i = 1; i <= quant; i++)
  {
    int max = *std::max_element(grau.begin(), grau.end());
    int temp = 0;
    int it = 0;
    while (temp != max)
    {
      temp = grau[it];
      it++;
    }
    cout << it-1 << " " << grau[it-1] << endl;
    grau[it-1] = 0;
  }
}

Lista::vizinhos(int vert)
{
  cout << pai[vert] << endl;
  for(int i = 1; i <= nVertices; i++)
  {
    if(pai[i] == vert)
    {
      cout << i << endl;
    }
  }
}

Lista::buscaNomes(std::string pesquisador)
{
  // string inputFile = "rede_colaboracao_vertices.txt";
  // std::ifstream inFile(inputFile.c_str());
  // vector<std::string> listaNomes;
  //
  // listaNomes.push_back(" ");
  //
  // if (inFile.is_open())
  // {
  //
  //   int v;
  //   string nome;
  //
  //   while (inFile.good())
  //   {
  //     getline(inFile, v, ',');
  //     getline(inFile, nome, '\n');
  //     listaNomes.push_back(nome);
  //     cout << v << "/" << listaNomes[v] << endl;
  //   }
  // }
  // inFile.close();
}
