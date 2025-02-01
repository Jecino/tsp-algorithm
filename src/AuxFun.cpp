#include "AuxFun.h"

bool contem(vector<int>& vector, int target){
    for(int i = 0; i < vector.size(); i++){
        if (vector[i] == target){
            return true;
        }
    }
    return false;
}

void removeVector(vector<int>& vector, int& target){
    for(int i = 0; i < vector.size(); i++){
        if(vector[i] == target){
            vector.erase(vector.begin() + i);
        }
    }
}

void printVector(vector<int>& vector){
    bool first = true;
    for(int i = 0; i < vector.size(); i++){
        if(first){
            cout << vector[i];
            first = false;
        }
        else{
            cout << " -> " << vector[i];
        }
    }
    cout << "\n";
}

vector<int> criarSubtour(Data& data){
    int dimension = data.getDimension();
    vector<int> subtour = {1,1};

    for(int i = 1; i <= 3; i++){
        int arestaQualquer = rand() % (dimension + 1); //Primeiro +1 para corrigir o range

        while(contem(subtour, arestaQualquer) || arestaQualquer == 0) {
            arestaQualquer = rand() % (dimension + 1);
        }

        subtour.insert(subtour.begin() + i, arestaQualquer);
    }

    return subtour;
}

vector<int> verticesRestantes(Data& data, vector<int>& subtour){
    vector<int> CL;

    for(int i = 1; i <= data.getDimension(); i++){
        if(!contem(subtour, i)){
            CL.push_back(i);
        }
    }

    return CL;
}

void ordenarCrescente(vector<InfoInsercao>& vector){
    InfoInsercao temp;

    for(int i = 0; i < vector.size(); i++){
        for(int j = 1 + i; j < vector.size(); j++){
            if(vector[j].custo < vector[i].custo){
                temp = vector[i];
                vector[i] = vector[j];
                vector[j] = temp;
            }
        }
    }
}

vector<InfoInsercao> calcularCustoInsercao(Data& data, vector<int>& sequencia, vector<int>& CL){
    vector<InfoInsercao> custoInsercao((sequencia.size() - 1) * CL.size());

    int l = 0;
    for(int a = 0; a < sequencia.size() - 1; a++){
        int i = sequencia[a];
        int j = sequencia[a+1];

        for(auto k : CL){
            custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = i;
            l++;
        }
    }

    return custoInsercao;
}

void inserirNaSolucao(vector<int>& sequencia, InfoInsercao& noInserido){
    for(int i = 0; i < sequencia.size() - 1; i++){
        if(sequencia[i] == noInserido.arestaRemovida){
            sequencia.insert(sequencia.begin() + i + 1, noInserido.noInserido);
            return;
        }
    }
}