#ifndef AUXFUN_H
#define AUXFUN_H

#include "Data.h"
#include <vector>
#include <algorithm>

using namespace std;

struct Solution{
    vector<int> sequencia;
    double custo;
};

struct InfoInsercao{
    int noInserido;
    int arestaRemovida;
    double custo;
};

//Genericas
vector<int> criarSubtour(Data& data);
void printVector(vector<int>& vector);
bool contem(vector<int>& vector, int target);

//Construcao
vector<int> verticesRestantes(Data& data, vector<int>& subtour);
vector<InfoInsercao> calcularCustoInsercao(Data& data, vector<int>& sequencia, vector<int>& CL);
void ordenarCrescente(vector<InfoInsercao>& vector);
void inserirNaSolucao(Solution& s, InfoInsercao& noInserido);
void removeVector(vector<int>& vector, int& target);

//BuscaLocal
bool bestImprovementSwap(Solution& s, Data& data);
bool bestImprovement20pt(Solution& s, Data& data);
bool bestImprovement0r0pt(Solution& s, Data& data, int block);

#endif