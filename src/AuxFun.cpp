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

void inserirNaSolucao(Solution& s, InfoInsercao& noInserido){
    for(int i = 0; i < s.sequencia.size() - 1; i++){
        if(s.sequencia[i] == noInserido.arestaRemovida){
            s.sequencia.insert(s.sequencia.begin() + i + 1, noInserido.noInserido);
            s.custo += noInserido.custo;
            return;
        }
    }
}


bool bestImprovementSwap(Solution& s, Data& data){
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s.sequencia.size() - 1; i++){
        int vi = s.sequencia[i];
        int vi_next = s.sequencia[i+1];
        int vi_prev = s.sequencia[i-1];

        for(int j = i + 1; j < s.sequencia.size() -1; j++){

            int vj = s.sequencia[j];
            int vj_next = s.sequencia[j+1];
            int vj_prev = s.sequencia[j-1];
            double delta;

            if(vj_prev == vi){
                delta = -data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj) 
                        +data.getDistance(vi, vj_next);  
            }
            else{
                delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) + data.getDistance(vi_prev, vj) 
                        + data.getDistance(vj, vi_next) - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next)
                        + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);  
            }
            

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
        swap(s.sequencia[best_i], s.sequencia[best_j]);
        s.custo = s.custo + bestDelta;
        return true;
    }

    return false;
}

bool bestImprovement20pt(Solution& s, Data& data){
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s.sequencia.size() - 1; i++){
        int vi = s.sequencia[i];
        int vi_next = s.sequencia[i+1];
        int vi_prev = s.sequencia[i-1];

        for(int j = i + 2; j < s.sequencia.size() -1; j++){

            int vj = s.sequencia[j];
            int vj_next = s.sequencia[j+1];
            int vj_prev = s.sequencia[j-1];

            double delta = - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next) 
                            + data.getDistance(vi_next, vj_next) + data.getDistance(vi, vj);  

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    //DEBUG: cout << bestDelta << " | " << best_i << " | " << best_j << "\n";
    if(bestDelta < 0){
        reverse(s.sequencia.begin() + best_i + 1, s.sequencia.begin() + best_j + 1);
        s.custo = s.custo + bestDelta;
        return true;
    }

    return false;
}

bool bestImprovement0r0pt(Solution& s, Data& data, int block){
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s.sequencia.size() - 1 * block; i++){
        int vi = s.sequencia[i];
        int vi_next = s.sequencia[i+1];
        int vi_prev = s.sequencia[i-1];

        for(int j = i + 1; j < s.sequencia.size() - 1; j++){

            int vj = s.sequencia[j];
            int vj_next = s.sequencia[j+1];
            int vj_prev = s.sequencia[j-1];

            double delta = -data.getDistance(vi, vi_next) -data.getDistance(vi_prev, vi) -data.getDistance(vj, vj_next)
                            +data.getDistance(vj, vi) + data.getDistance(vi, vj_next) + data.getDistance(vi_prev, vi_next);

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    //DEBUG: cout << bestDelta << " | " << best_i << " | " << best_j << "\n";
    if(bestDelta < 0){
        s.sequencia.insert(s.sequencia.begin() + best_j + 1, s.sequencia[best_i]);
        s.sequencia.erase(s.sequencia.begin() + best_i);
        s.custo = s.custo + bestDelta;
        return true;
    }

    return false;
}