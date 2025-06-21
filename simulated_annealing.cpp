#include "bits/stdc++.h"
#include <iostream>
#include <fstream>     
#include <string>      
#include <vector>      
#include <utility>
#include <chrono>
#include <ctime>
#include <cmath>         
#include <bitset>
#include <random>
#include <algorithm>

using namespace std;

typedef long long ll;

int itens, quant_conj, capacidade;
vector<int> lucro,peso; //Lucro e peso do item i
vector<vector<int> > conju; //Lista de conjunto que o item i pertence (conj[i] = lista)
vector<pair<int,int> > inf_conj;//(Limite de itens na solução,Penalidade) do conjunto i


//Função meta-heurística
const double tempoLimite = 0.5;
const double alpha = 0.9999;
const double _div = INT_MAX;

int iterationsWithoutIncreasing = 0;

mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

int Simulated_Annealing(){

    bitset<1000> includedItems;

    int somaValor = 0, somaPenalidade = 0, somaPeso = 0;
    vector<int> itemsPorConj(quant_conj, 0);

    uniform_int_distribution<int> uid(0, INT_MAX);

    // aqui começa o algoritmo

    int best = somaValor - somaPenalidade;
    cout << best << endl;

    double temperature = 1e4;

    auto GetTemperature = [&](){
        temperature = max(temperature*alpha, 1e-3);
    };

    auto start = chrono::high_resolution_clock::now();

    auto agora = chrono::high_resolution_clock::now();
    // // while(temperature > 1e-7){
    while((std::chrono::duration<double>(agora - start)).count() < tempoLimite && temperature > 1.5 && iterationsWithoutIncreasing > 70){
        int OLD = best;
        bool ok = true;
        while(ok && temperature > 1.5){
            int itemFlip = uid(rng)%itens;
            int novoLucro = somaValor - somaPenalidade;
            if(includedItems[itemFlip]){
                novoLucro -= lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    novoLucro += inf_conj[currConj].second;
                }
            }
            else{
                if(somaPeso + peso[itemFlip] > capacidade) continue;
                novoLucro += lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    novoLucro -= inf_conj[currConj].second;
                }
            }

            int delta = novoLucro - (somaValor - somaPenalidade);
            best = max(best, novoLucro);

            // cout << novoLucro << endl;
            // return novoLucro;

            if(delta > 0 || (uid(rng) / _div) < exp((delta)/temperature)){
                if(includedItems[itemFlip]){
                    includedItems[itemFlip] = 0;
                    somaPeso -= peso[itemFlip];
                    somaValor -= lucro[itemFlip];
                    for(int currConj: conju[itemFlip]){
                        int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                        if(diff <= 0) continue;
                        somaPenalidade += inf_conj[currConj].second;
                    }
                }
                else{
                    includedItems[itemFlip] = 1;
                    somaPeso += peso[itemFlip];
                    somaValor += lucro[itemFlip];
                    for(int currConj: conju[itemFlip]){
                        int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                        if(diff <= 0) continue;
                        somaPenalidade -= inf_conj[currConj].second;
                    }
                }
                ok = false;
            }
            else continue;
        }

        GetTemperature();
        agora = chrono::high_resolution_clock::now();
        //cout << (std::chrono::duration<double>(agora - start)).count() << endl;

        if(best == OLD) iterationsWithoutIncreasing++;
        else iterationsWithoutIncreasing = 0;
    }
    
    // cout << best << endl;
    // cout << "Terminou uma iteração! " << (std::chrono::duration<double>(agora - start)).count() << endl;
    return best;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Falta algumentos";
        return 1; 
    }
    cout << "Entrou!" << endl;
    string dir_entrada = argv[1];
    string dir_saida = argv[2];

    //Abre arquivo de entrada
    ifstream arquivo(dir_entrada);
    if (!(arquivo.is_open())) {
        cout << "Erro ao abrir o arquivo: " << dir_entrada << endl;
        return -1;
    }

    //Início da leitura
    //cout << "Estou lendo o arquivo:" << dir_entrada << endl;
    arquivo >> itens >> quant_conj >> capacidade;
    lucro.assign(itens,0);peso.assign(itens,0);
    
    for(int j = 0; j < itens; j++) arquivo >> lucro[j];
    for(int j = 0; j < itens; j++) arquivo >> peso[j];

    conju.assign(itens,vector<int>()); 
    inf_conj.assign(quant_conj,make_pair(0,0));

    for(int j = 0; j < quant_conj; j++){
        int lim_conj, penalidade_conj, itens_conj;
        arquivo >> lim_conj >> penalidade_conj >> itens_conj;
        inf_conj[j].first = lim_conj; 
        inf_conj[j].second = penalidade_conj;

        for(int k = 0; k < itens_conj; k++){
            int item; arquivo >> item;
            conju[item].push_back(j);
        }
    }
    //Fim na leitura

    //Calcular o tempo e solução
    auto start = chrono::high_resolution_clock::now();
    int sol = Simulated_Annealing();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;
    double execution_time = time.count();

    //Abre arquivo de saída
    ofstream saida_arquivo(dir_saida,ios::app);
    if (!saida_arquivo.is_open()) {
        cout << "Erro ao abrir " << dir_saida << " para escrita.\n";
        return 1;
    }
    saida_arquivo << sol << " " << execution_time << '\n';
    saida_arquivo.close();
    return 0;
}