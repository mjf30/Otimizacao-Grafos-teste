#include "bits/stdc++.h"
// #include <iostream>
// #include <fstream>     
// #include <string>      
// #include <vector>      
// #include <utility>
// #include <chrono>
// #include <ctime>
// #include <cmath>         
// #include <bitset>
// #include <random>
// #include <algorithm>

using namespace std;

typedef long long ll;

int itens, quant_conj, capacidade;
vector<int> lucro,peso; //Lucro e peso do item i
vector<vector<int> > conju; //Lista de conjunto que o item i pertence (conj[i] = lista)
vector<pair<int,int> > inf_conj;//(Limite de itens na solução,Penalidade) do conjunto i

mt19937_64 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const double tempoLimite = 0.5;
double alpha = 0.7;

int iterationsWithoutIncreasing = 0;

//Função meta-heurística

int GRASP(){

    uniform_int_distribution<ll> uid(0, 1LL<<60);

    
    // Começa o algoritmo
    
    int best = -1e9;
    
    auto start = chrono::high_resolution_clock::now();
    
    auto agora = chrono::high_resolution_clock::now();
    
    int iter = 0;
    
    vector<pair<double, int>> candidates(itens);
    for(int i = 0; i < itens; i++) candidates[i] = { peso[i] == 0 ? 1e9+lucro[i] : lucro[i]/peso[i] , i};
    sort(candidates.rbegin(), candidates.rend());
    
    while((std::chrono::duration<double>(agora - start)).count() < tempoLimite && iterationsWithoutIncreasing > 50){
        int OLD = best;
        iter++;
        
        bitset<1000> includedItems;
        vector<int> itemsPorConj(quant_conj, 0);
        int somaValor = 0, somaPenalidade = 0, somaPeso = 0;
        
        
        // Algoritmo construtivo que utiliza métrica gulosa lucro/peso
        
        double prob = 0.9;

        int cnt = 0, cnt2 = 0;;

        for(auto[comp, currItem]: candidates){
            double randProb = uid(rng)/double(1LL<<60);
            
            if(randProb < prob){
                if(peso[currItem] + somaPeso > capacidade) continue;
    
                somaValor += lucro[currItem];
                includedItems[currItem] = 1;
                for(int currConj: conju[currItem]){
                    itemsPorConj[currConj]++;
                    int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    somaPenalidade += inf_conj[currConj].second;
                }
                somaPeso += peso[currItem];

                // decai mais rápido ao longo do tempo
                prob *= alpha;
                alpha *= 0.9;
            }
        }

        best = max(somaValor - somaPenalidade, best);

        // Hill climbing para achar máximo local

        for(int i = 0; i < itens; i++){
            int itemFlip = candidates[i].second;
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

            if(delta > 0){
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
            }
            else continue;
        }

        agora = chrono::high_resolution_clock::now();

        if(best == OLD) iterationsWithoutIncreasing++;
        else iterationsWithoutIncreasing = 0;
    }

    cout << best << ' ' << iter << endl;

    return best;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Falta algumentos";
        return 1; 
    }
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
    int sol = GRASP();
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