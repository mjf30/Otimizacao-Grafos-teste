//#include "bits/stdc++.h"
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

mt19937_64 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const double tempoLimite = 0.5;
double alpha = 0.7;

int iterationsWithoutIncreasing = 0;

//Função meta-heurística

int ILS(){
    uniform_int_distribution<ll> uid(0, 1LL<<60);

    
    // Vamos usar algumas iterações do GRASP para solução inicial
    
    int best = -1e9;
    
    auto start = chrono::high_resolution_clock::now();
    
    auto agora = chrono::high_resolution_clock::now();
    
    int iter = 0;
    
    vector<pair<double, int>> candidates(itens);
    for(int i = 0; i < itens; i++) candidates[i] = { peso[i] == 0 ? 1e9+lucro[i] : lucro[i]/peso[i] , i};
    sort(candidates.rbegin(), candidates.rend());

    bitset<1000> ilsItems;
    
    while(iter < 50){
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

        if(somaValor - somaPenalidade > best){
            ilsItems = includedItems;
            best = somaValor - somaPenalidade;
        }
    }


    // Com a solução inicial, podemos começar o algoritmo

    vector<int> itemsPorConj(quant_conj, 0);
    int somaValor = 0, somaPenalidade = 0, somaPeso = 0;

    for(int currItem = 0; currItem < itens; currItem++){
        if(ilsItems[currItem] == 1){
            somaValor += lucro[currItem];
            for(int currConj: conju[currItem]){
                itemsPorConj[currConj]++;
                int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
                if(diff <= 0) continue;
                somaPenalidade += inf_conj[currConj].second;
            }
            somaPeso += peso[currItem];
        }
    }

    agora = chrono::high_resolution_clock::now();

    iter = 0;

    while((std::chrono::duration<double>(agora - start)).count() < tempoLimite && iterationsWithoutIncreasing > 50){
        int OLD = best;
        iter++;

        bitset<1000> base = ilsItems;
        int oldAns = somaValor - somaPenalidade;
        vector<int> old_itemsPorConj = itemsPorConj;
        int old_somaValor = somaValor, old_somaPenalidade = somaPenalidade, old_somaPeso = somaPeso;

        // Aqui fazemos a perturbação flipando 4 bits aleatórios
        for(int numOfChanges = 0; numOfChanges < 4; numOfChanges++){
            int itemFlip = uid(rng)%itens;

            if(ilsItems[itemFlip]){
                ilsItems[itemFlip] = 0;
                somaPeso -= peso[itemFlip];
                somaValor -= lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    somaPenalidade += inf_conj[currConj].second;
                }
            }
            else{
                if(somaPeso + peso[itemFlip] > capacidade) { numOfChanges--; continue;}
                ilsItems[itemFlip] = 1;
                somaPeso += peso[itemFlip];
                somaValor += lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    somaPenalidade -= inf_conj[currConj].second;
                }
            }
        }

        best = max(best, somaValor - somaPenalidade);

        // Hill climbing para achar máximo local
        
        for(int i = 0; i < itens; i++){
            int itemFlip = candidates[i].second;
            int novoLucro = somaValor - somaPenalidade;
            if(ilsItems[itemFlip]){
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
            
            if(delta > 0){
                if(ilsItems[itemFlip]){
                    ilsItems[itemFlip] = 0;
                    somaPeso -= peso[itemFlip];
                    somaValor -= lucro[itemFlip];
                    for(int currConj: conju[itemFlip]){
                        int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                        if(diff <= 0) continue;
                        somaPenalidade += inf_conj[currConj].second;
                    }
                }
                else{
                    ilsItems[itemFlip] = 1;
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

        // Critério de aceitação para conjunto base
        agora = chrono::high_resolution_clock::now();

        double randProb = uid(rng)/double(1LL<<60);
        double elapsed = std::chrono::duration<double>(agora - start).count()/tempoLimite;

        // Fica com maior resposta caso randProb < accept
        double accept = 0.5 + elapsed/2;
        if(oldAns > somaValor - somaPenalidade) {
            if(randProb < elapsed){
                ilsItems = base;
                itemsPorConj = old_itemsPorConj;
                somaValor = old_somaValor;
                somaPenalidade = old_somaPenalidade;
                somaPeso = old_somaPeso;
            }
        }
        else{
            if(randProb < elapsed){
                ilsItems = base;
                itemsPorConj = old_itemsPorConj;
                somaValor = old_somaValor;
                somaPenalidade = old_somaPenalidade;
                somaPeso = old_somaPeso;
            }
        }

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
    int sol = ILS();
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