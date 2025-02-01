#ifndef AUXFUN_H
#define AUXFUN_H

#include "Data.h"
#include <vector>

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

vector<int> criarSubtour(Data& data);
void printVector(vector<int>& vector);
bool contem(vector<int>& vector, int target);
vector<int> verticesRestantes(Data& data, vector<int>& subtour);
vector<InfoInsercao> calcularCustoInsercao(Data& data, vector<int>& sequencia, vector<int>& CL);
void ordenarCrescente(vector<InfoInsercao>& vector);
void inserirNaSolucao(vector<int>& sequencia, InfoInsercao& noInserido);
void removeVector(vector<int>& vector, int& target);

#endif