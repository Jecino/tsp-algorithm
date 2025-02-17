#include "Data.h"
#include "AuxFun.h"
#include <iostream>
#include <vector>

using namespace std;

double calcularCusto(Data& data, vector<int>& v);

Solution construcao(Data& data);
void BuscaLocal(Solution& s, Data& data);
Solution ILS(int maxIter, int maxIterIls, int dimension, Data& data);
Solution Pertubacao(Solution& s, Data& data, int dimension);

//Comando para executar o codigo: ./tsp instances/"nome_da_instancia".tsp
//ex: ./tsp instances/teste.tsp
int main(int argc, char** argv) {

    srand(time(0));

    //Comandos necessarios para leitura da instancia
    auto data = Data(argc, argv[1]);        
    data.read();
    
    //data.getDimension() retorna quantos vertices sao presentes na instancia
    size_t dimension = data.getDimension();
    cout << "Dimensao: " << dimension << endl;

    //data.getDistance(vertice, vertice) retorna a distancia entre os vertices passados como argumentos
    //double custo_1_2 = data.getDistance(1,2);
    //cout << "Distancia entre o vertice 1 e 2: " << custo_1_2 << endl;

    //criando um vector vazio
    //vector<int> solucao_arbitraria;

    //for(int i = 1; i <= dimension; i++){

        //preenchendo o vector com os vertices de 1 ate a dimensao da instancia em ordem cresente
        //solucao_arbitraria.push_back(i);
    //}

    //demonstracao de como criar um vector com tamanho predefinido
    // //o tamanho do vector sera o especificado e seu conteudo sera lixo de memoria
    // vector<int> solucao_arbitraria = vector<int>(dimension);
    //
    // for(int i = 0; i <= dimension; i++){
    //     
    //     //esse loop tera o mesmo resultado que o for anterios, mas adaptado para um vector que ja possui um tamanho
    //     //a sintaxe eh parecida com a de um array em c
    //     solucao_arbitraria[i] = i+1;
    // }

    //exemplo de como usar funcoes com a classe data, implementacao abaixo da main
    //double custo = calcularCusto(data, solucao_arbitraria);

    //cout << "Custo total da solucao (1,2,...,n): " << custo << endl;
    int maxIter = 50;
    int maxIterIls;
    if(dimension >= 150){
        maxIterIls = dimension / 2;
    }
    else{
        maxIterIls = dimension;
    }

    ILS(maxIter, maxIterIls, dimension, data);

    return 0;
}

//o & se chama "referencia" e age como um facilitador de ponteiros
//passar parametros por referencia ajuda na performance no codigo quando seu parametro apresenta muitos dados como e o caso do data, vector em geral apresentam bastante dados
//evitando a copia de todos esses dados e passando apenas uma referencia para o valor original, ou seja o valor dentro da funcao eh o mesmo da fora dela
//passar valores simples como int,double,etc nao compensa ja que esses tipos nao apresentam grande quantidade de dados e podem ser copiados rapidamente
//entretando passar um int por referencia pode ser util para alterar o valor original da variavel
//a referencia apresenta syntaxe semelhante a de uma variavel comum, diferente dos ponteiros
double calcularCusto(Data& data, vector<int>& v){

    double custo = 0;
    
    //maneira de iterar sobre os valores do vector a partir de seu tamanho dado pela funcao size()
    for(int i = 0; i < v.size() - 1; i++){

        custo += data.getDistance(v[i], v[i+1]);
    }

    return custo;
}

Solution ILS(int maxIter, int maxIterIls, int dimension, Data& data){
    Solution bestOfAll;
    bestOfAll.custo = INFINITY;

    for(int i = 0; i < maxIter; i++){
        Solution s = construcao(data);
        Solution best = s;
        //cout << "\nSequencia antes da busca local | custo: " << calcularCusto(data, s.sequencia)  << " | s.custo: " << s.custo << "\n";
        //printVector(s.sequencia);
        //cout << "\n";

        int iterIls = 0;
        while(iterIls < maxIterIls){
            BuscaLocal(s, data);
            if(s.custo < best.custo){
                best = s;
                iterIls = 0;
            }

            //s = Pertubacao(best, data, dimension);
            iterIls++;

            if(best.custo < bestOfAll.custo){
                bestOfAll = best;
            }
        }

        //cout << "Sequência após busca local | custo: "  << calcularCusto(data, s.sequencia) << " | s.custo: " << s.custo << "\n";
        //cout << "\n";
        //printVector(s.sequencia);
    }

    cout << "Melhor Sequência de todas | custo: "  << calcularCusto(data, bestOfAll.sequencia) << " | bestOfAll.custo: " << bestOfAll.custo << "\n";
    cout << "\n";
    printVector(bestOfAll.sequencia);

    return bestOfAll;
}

Solution construcao(Data& data){
    Solution parcial;
    parcial.sequencia = criarSubtour(data);
    vector<int> CL = verticesRestantes(data, parcial.sequencia);
    parcial.custo = calcularCusto(data, parcial.sequencia);

    while(!CL.empty()){
        vector<InfoInsercao> custoInsercao = calcularCustoInsercao(data, parcial.sequencia, CL);
        ordenarCrescente(custoInsercao);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(parcial, custoInsercao[selecionado]);
        removeVector(CL, custoInsercao[selecionado].noInserido);
    }

    return parcial;
}

void BuscaLocal(Solution& s, Data& data){
    vector<int> opcao = {1,2,3,4,5};
    bool melhorou = false;

    while(!opcao.empty()){
        int n = rand() % opcao.size();

        switch(opcao[n]){
            case 1:
                melhorou = bestImprovementSwap(s, data);
                break;
            case 2:
                melhorou = bestImprovement20pt(s, data);
                break;
            case 3:
                melhorou = bestImprovement0r0pt(s, data, 1);
                break;
            case 4:
                melhorou = bestImprovement0r0pt(s, data, 2);
                break;
            case 5:
                melhorou = bestImprovement0r0pt(s, data, 3);
                break;
        }

        if(melhorou == true){

            opcao = {1,2,3,4,5};
        }
        else{

            opcao.erase(opcao.begin() + n);
        }
    }
}

/*
Solution Pertubacao(Solution& s, Data& data, int dimension){
    Solution newS = s;

    int size_i = rand() % ((int) floor(dimension / 10)) + 2;

    int size_j = rand() % ((int) floor(dimension / 10)) + 2;

    int i = rand() % (dimension) + 1;
    //Loop para definir novo valor de i até ele não ocorrer overflow
    while((i + size_i) > dimension - 1){
        i = rand() % (dimension) + 1;
    }

    int j = rand() % (dimension) + 1;


    //Checa se o segmento j está sobreposto no segmento i e gera um j novo até não estar
    while(j == i || (j < i && j + size_j >= i) || j < 2 || j > dimension - 1 || j + size_j >= dimension){
        j = rand() % (dimension) + 1;;
    }

    for(int a = 0; a < size_j; a++){
        if(j + a == i || j + a == i + size_i){
            j = rand() % (dimension) + 1;
        }
    }

    double delta = -data.getDistance() - data.getDistance() - data.getDistance() - data.getDistance();

    cout << "\nSequencia antes da pertubação | i: " << newS.sequencia[i] << " | size_i: " << size_i  << " | j: " << newS.sequencia[j] << " | size_j: " << size_j << "\n";
    printVector(newS.sequencia);

    newS.sequencia.insert(newS.sequencia.begin() + j + size_j, newS.sequencia.begin() + i, newS.sequencia.begin() + i + size_i);
    
    newS.sequencia.insert(newS.sequencia.begin() + i + size_i, newS.sequencia.begin() + j, newS.sequencia.begin() + j + size_j);
    
    newS.sequencia.erase(newS.sequencia.begin() + j + size_j + 1, newS.sequencia.begin() + j + size_j * 2);

    newS.sequencia.erase(newS.sequencia.begin() + i, newS.sequencia.begin() + i + size_i);
    
    cout << "\nSequencia depois da pertubação"<< "\n";
    printVector(newS.sequencia);

    return newS;
}
*/