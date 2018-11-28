#include <bits/stdc++.h>
#include <omp.h>
#include <time.h>
#include "Euclidiana.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <cmath>

//TODO:

using namespace std;
bool sortcol(const vector<int>& v1, const vector<int>& v2) {
 return v1[1] < v2[1];
}


float Euclidiana::calculaDistancia(int x1, int y1, int x2, int y2)
{
  return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

std::vector<int> Euclidiana::retornaCoordenadas(int n1, int n2)
{
  std::vector<int> temp;
  temp.push_back(listaAdj[n1][0]);
  temp.push_back(listaAdj[n1][1]);
  temp.push_back(listaAdj[n2][0]);
  temp.push_back(listaAdj[n2][1]);
  return temp;
}

Euclidiana::calcular(std::string arquivo, bool heur = false)
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
    linhaInt >> nVertices;
    listaAdj.resize(nVertices); //Redimensiona a lista

    float v, a; int linePos;
    linePos = 0;

    while (inFile >> v >> a)
    {
      //Adiciona a coordenada
      listaAdj[linePos].push_back(v);
      listaAdj[linePos].push_back(a);
      listaAdj[linePos].push_back(linePos+1);
      linePos++;
    }
  }
  inFile.close(); //Fiinaliza a leitura do arquivo

  //Ordena a lista
  float distancia;
  ofstream output;
  output.open("result.txt");
  clock_t tInicio;


  //MÉTODO 1 (Com Heurística):
  if (heur == true)
  {
    tInicio = clock();
    distancia = 0;
    std::vector <bool> explorados(nVertices, false);
    explorados[0] = true;
    bool completo = false;

    int v = 0;
    int vAnt;
    while (completo == false)
    {
      output << listaAdj[v][2] << " ";

      distancia = distancia + calculaDistancia(listaAdj[v][0],listaAdj[v][1],listaAdj[vAnt][0],listaAdj[vAnt][1]);
      explorados[v] = true;
      completo = true;
      float tempDist = std::numeric_limits<float>::max();
      vAnt = v;
      for (int i = 0; i <= nVertices-1; i++)
      {
        if (explorados[i] == false)
        {
          completo = false;
          float tempDistI = calculaDistancia(listaAdj[0][0],listaAdj[0][1],listaAdj[i][0],listaAdj[i][1]);
          if (tempDistI < tempDist)
          {
            tempDist = tempDistI;
            v = i;
          }
        }
      }
      if (completo)
      {
        distancia = distancia + calculaDistancia(listaAdj[v][0],listaAdj[v][1],listaAdj[0][0],listaAdj[0][1]);
      }
    }
    cout << distancia << endl;
    cout << "Tempo de execucao: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
  }

  //MÉTODO 2 (Sem Heurística):
  if(heur == false)
  {
    tInicio = clock();
    sort(listaAdj.begin(), listaAdj.end(),sortcol);
    distancia = 0;
    for (int i = 0; i < nVertices; i++)
    {
      output << listaAdj[i][2] << " ";
      if (i > 0)
      {
        distancia = distancia + calculaDistancia(listaAdj[i][0],listaAdj[i][1],listaAdj[i-1][0],listaAdj[i-1][1]);
      }
      if (i == (nVertices-1))
      {
        distancia = distancia + calculaDistancia(listaAdj[i][0],listaAdj[i][1],listaAdj[0][0],listaAdj[0][1]);
      }
    }
    cout << distancia << endl;
    cout << "Tempo de execucao: " << (clock() - tInicio)/(CLOCKS_PER_SEC/1000) << " ms" << endl;
  }


  output.close();
}
