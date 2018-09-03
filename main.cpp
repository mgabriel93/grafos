#include "Matriz.h"
#include "Matriz.cpp"
#include "Lista.h"
#include "Lista.cpp"

#include <omp.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>
#include <sstream>


using namespace std;

//true = entrada manual. false = pega pelas const abaixo
const bool inOverride = false;
const bool log = false;
const bool runBFS = true;
const bool runDFS = false;
//nome do arquivo padrão
const string arquivoDefault = "as_graph";

//1 = matriz, 2 = lista, qualquer outra coisa = pula direto para as calcular as componentes
const string estruturaDefault = "1";

//1 para paralelizar o carregamento de matriz em memória, 2 para rodar uma bfs em cada thread, qualquer outra coisa para rodar single
//     OBSERVAÇÕES: PARA UM LOG PRECISO, UTILIZAR 1 (Paralelizar apenas o carregamento de matriz)
//     CAUTION: CADA BFS IRÁ ALOCAR SEU PRÓPRIO ESPAÇO EM MEMÓRIA!
const string ompDefault = "1";
//1 para calcular as componentes conexas, qualquer outra coisa para fechar direto
const string componentesDefault = "2";
//1 para calcular o diâmetro, qualquer outra coisa para fechar direto
const string diametroDefault = "2";

//Vértice mínimo para iniciar as BFS/DFS
const int vMin = 1;
//Vértice máximo para iniciar as BFS/DFS
const int vMax = 1;


int main()
{
  //Define o arquivo de entrada
  string inputFile;
  if (inOverride == true)
  {
    cout << "Insira o nome do arquivo: ";
    cin >> inputFile;
  }
  else
  {
    inputFile = arquivoDefault;
  }


  //Define o tipo de estrutura a ser utilizado
  string estrutura;
  if (inOverride == true)
  {
    cout << "Digite o tipo de estrutura a ser utilizado - 1 para matriz, 2 para lista, e qualquer outra tecla para pular: ";
    cin >> estrutura;
  }
  else
  {
    estrutura = estruturaDefault;
  }

  //Define as opções de paralelização
  string ompSettings;
  if (inOverride == true)
  {
    if (estrutura == "1")
    {
      cout << "Digite a parte a ser paralelizada - 1 para  o carregamento da matriz e 2 para as BFS/DFS: ";
      cin >> ompSettings;
    }
    else
    {
      cout << "Digite 1 para rodar em single-thread e 2 para rodar em paralelo: ";
      cin >> ompSettings;
    }
  }
  else
  {
    ompSettings = ompDefault;
  }


  ofstream openTime;
  openTime.open("run_log.txt");
  clock_t tOpen = clock();

  //Caso o usuário escolha MATRIZ:
  if (estrutura == "1")
  {
    clock_t tInicio = clock();

    //Carrega a matriz em memória
    Matriz matriz;
    if (ompSettings == "1")
    {
      matriz.carregar(inputFile, ompSettings);
    }

    //Inicia o timer
    ofstream executionTime;
    executionTime.open((inputFile+"_log_matrizAdj.txt").c_str());
    executionTime << "Tempo para iniciar o vetor: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;

    //BFS e DFS para cada ponto. Roda em paralelo caso seja escolhida a opção 2
    #pragma omp parallel for if (ompSettings == "2")
    for (int i = vMin; i <= vMax; i++)
    {
      if (ompSettings == "1")
      {
        std::ostringstream nomeSaida;
        nomeSaida << i; //Dá cast para string

        //BFS
        if (runBFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a BFS em matriz com inicio no vertice " << i << endl;
          matriz.BFS(i, (inputFile+"_DFS_MatrizAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            matriz.geraEstatisticas((inputFile+"_BFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a BFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }

        //DFS
        if (runDFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a DFS em matriz com inicio no vertice " << i << endl;
          matriz.DFS(i, (inputFile+"_DFS_MatrizAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            matriz.geraEstatisticas((inputFile+"_DFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a DFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }
      }
      else
      {
        std::ostringstream nomeSaida;
        nomeSaida << i; //Dá cast para string

        //Inicializa uma matriz interno
        //TODO: Criar uma classe SÓ PARA A MATRIZ, para poder compartilhar
        Matriz matrizInt;
        matrizInt.carregar(inputFile, ompSettings);

        //BFS
        if (runBFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a BFS em matriz com inicio no vertice " << i << endl;
          matrizInt.BFS(i, (inputFile+"_BFS_MatrizAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            matrizInt.geraEstatisticas((inputFile+"_BFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a BFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }

        //DFS
        if (runDFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a DFS em matriz com inicio no vertice " << i << endl;
          matrizInt.DFS(i, (inputFile+"_DFS_MatrizAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            matrizInt.geraEstatisticas((inputFile+"_DFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a BFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }
      }
    }
    executionTime.close(); //Finaliza o timer
  }
  //Caso o usuário escolha LISTA
  else if (estrutura == "2")
  {
    clock_t tInicio = clock();

    //Carrega a matriz em memória
    Lista lista;
    if (ompSettings == "1")
    {
      lista.carregar(inputFile);
    }

    //Inicia o timer
    ofstream executionTime;
    executionTime.open((inputFile+"_log_listaAdj.txt").c_str());
    executionTime << "Tempo para iniciar o vetor: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;

    //BFS e DFS para cada ponto. Roda em paralelo caso seja escolhida a opção 2
    #pragma omp parallel for if (ompSettings == "2")
    for (int i = vMin; i <= vMax; i++)
    {
      if (ompSettings == "1")
      {
        std::ostringstream nomeSaida;
        nomeSaida << i; //Dá cast para string

        //BFS
        if (runBFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a BFS em lista com inicio no vertice " << i << endl;
          lista.BFS(i, (inputFile+"_BFS_ListaAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            lista.geraEstatisticas((inputFile+"_BFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a BFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }

        //DFS
        if (runDFS == true)
        {
          tInicio = clock();
          // cout << "Iniciando a DFS em lista com inicio no vertice " << i << endl;
          lista.DFS(i, (inputFile+"_DFS_ListaAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            lista.geraEstatisticas((inputFile+"_DFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a DFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }
      }
      else
      {
        std::ostringstream nomeSaida;
        nomeSaida << i; //Dá cast para string

        //Inicializa uma matriz interno
        //TODO: Criar uma classe SÓ PARA A MATRIZ, para poder compartilhar
        Lista listaInt;
        listaInt.carregar(inputFile);

        //BFS
        if (runBFS == true)
        {
          tInicio = clock();
          cout << "Iniciando a BFS em lista com inicio no vertice " << i << endl;
          listaInt.BFS(i, (inputFile+"_BFS_ListaAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            listaInt.geraEstatisticas((inputFile+"_BFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a BFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }

        //DFS
        if (runDFS == true)
        {
          tInicio = clock();
          cout << "Iniciando a DFS em lista com inicio no vertice " << i << endl;
          listaInt.DFS(i, (inputFile+"_DFS_ListaAdj_"+nomeSaida.str()).c_str(), log);
          if (log == true)
          {
            listaInt.geraEstatisticas((inputFile+"_DFS_"+nomeSaida.str()).c_str());
            executionTime << "Tempo para rodar a DFS a partir do vértice " << i << ": " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
          }
        }
      }
    }
    executionTime.close(); //Finaliza o timer
  }

  openTime << "Tempo de execução: " << (clock() - tOpen)/(CLOCKS_PER_SEC/1000) << " ms" << endl;

  //Define se irá ser feito cálculo de componentes
  string calcularComponentes;
  if (inOverride == true)
  {
    cout << "Deseja calcular a quantidade de componentes? Digite 1 para sim, e qualquer outra tecla para sair: ";
    cin >> calcularComponentes;
  }

  else
  {
    calcularComponentes = componentesDefault;
  }

  //Caso inicie, inicia uma lista para calcular as componentes conexas
  if(calcularComponentes == "1")
  {
    Lista lista;
    lista.carregar(inputFile);
    lista.componentes(inputFile);
  }

  //Define se irá ser feito cálculo de diâmetro
  string calcularDiametro;
  if (inOverride == true)
  {
    cout << "Deseja calcular o diâmetro? Digite 1 para sim, e qualquer outra tecla para sair: ";
    cin >> calcularDiametro;
  }

  else
  {
    calcularDiametro = diametroDefault;
  }

  //Caso inicie, inicia uma lista para calcular as componentes conexas
  if(calcularDiametro == "1")
  {
    Lista lista;
    lista.carregar(inputFile);
    lista.diametro(inputFile);
  }
}
