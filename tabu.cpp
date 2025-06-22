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

mt19937_64 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

const int tabuListMaxSize = 10000;

const double tempoLimite = 0.6;
double alpha = 0.8;

int iterationsWithoutIncreasing = 0;

//Função meta-heurística

int TABU(){
    iterationsWithoutIncreasing = 0;

    //Will be used to xor hash tabu list
    
    uniform_int_distribution<ll> uid(0, 1LL<<60);
    vector<ll> rndVal(itens, 0);
    for(ll &i: rndVal) i = uid(rng);
    set<ll> tabulist;
    queue<ll> moveQueue;

    bitset<1000> includedItems;
    ll currHash = 0;
    int somaValor = 0, somaPenalidade = 0, somaPeso = 0;
    vector<int> itemsPorConj(quant_conj, 0);

    // Começa com conjunto aleatório

    vector<int> perm(itens);
    for(int i = 0; i < itens; i++) perm[i] = i;

    shuffle(perm.begin(), perm.end(), rng);

    // O tabu não tem solução

    // for(int currItem: perm){     
    //     int incluso = uid(rng)%60;

    //     if(currItem >= itens) return -1;

    //     if(incluso == 1){
    //         if(peso[currItem] + somaPeso > capacidade) continue;
    //         int nValor = somaValor, nPenalidade = somaPenalidade;
    //         nValor += lucro[currItem];
    //         for(int currConj: conju[currItem]){
    //             int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
    //             if(diff <= 0) continue;
    //             nPenalidade += inf_conj[currConj].second;
    //         }
            
    //         if(somaValor - somaPenalidade > nValor - nPenalidade) {  // não quero pegar solução inicial muito ruim
    //             continue;
    //         }

    //         currHash ^= rndVal[currItem];

    //         somaValor += lucro[currItem];
    //         includedItems[currItem] = 1;
    //         for(int currConj: conju[currItem]){
    //             itemsPorConj[currConj]++;
    //             int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
    //             if(diff <= 0) continue;
    //             somaPenalidade += inf_conj[currConj].second;
    //         }
    //         somaPeso += peso[currItem];
    //     }
    // }

    vector<pair<double, int>> candidates(itens);
    for(int i = 0; i < itens; i++) candidates[i] = { peso[i] == 0 ? 1e9+lucro[i] : lucro[i]/peso[i] , i};
    sort(candidates.rbegin(), candidates.rend());

    double prob = 0.8;

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
            alpha *= 0.2;
        }
    }

    // Começa o algoritmo

    int best = somaValor - somaPenalidade;

    moveQueue.push(currHash);
    tabulist.insert(currHash);

    auto start = chrono::high_resolution_clock::now();

    auto agora = chrono::high_resolution_clock::now();

    int iter = 0;

    while((std::chrono::duration<double>(agora - start)).count() < tempoLimite && iterationsWithoutIncreasing < 300){
        int OLD = best;
        pair<int,int> currBest = {-1e9,-1e9};
        iter++;

        for(int itemFlip = 0; itemFlip < itens; itemFlip++){
            if(std::chrono::duration<double>(agora - start).count() > tempoLimite) break;
            ll novaHash = currHash ^ rndVal[itemFlip];
            if(tabulist.find(novaHash) != tabulist.end()) {continue;}
            
            int novoLucro = somaValor - somaPenalidade;
            if(includedItems[itemFlip]){
                novoLucro -= lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    novoLucro += inf_conj[currConj].second;
                }
            }
            else{
                if(somaPeso + peso[itemFlip] > capacidade) {continue;}
                novoLucro += lucro[itemFlip];
                for(int currConj: conju[itemFlip]){
                    int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                    if(diff <= 0) continue;
                    novoLucro -= inf_conj[currConj].second;
                }
            }
            currBest = max(currBest, make_pair(novoLucro, itemFlip));
        }

        auto [bestAns, itemFlip] = currBest;
        best = max(best, bestAns);

        if(bestAns == -1e9) break;

        currHash ^= rndVal[itemFlip];
        while(tabulist.size() > tabuListMaxSize){
            if(moveQueue.size() == 0) {tabulist.clear(); break;}
            ll lastMov = moveQueue.front(); moveQueue.pop();
            tabulist.erase(lastMov);
        }
        moveQueue.push(currHash);
        tabulist.insert(currHash);

        if(includedItems[itemFlip]){
            includedItems[itemFlip] = 0;
            somaPeso -= peso[itemFlip];
            somaValor -= lucro[itemFlip];
            for(int currConj: conju[itemFlip]){
                itemsPorConj[currConj]--;
                int diff = itemsPorConj[currConj]+1 - inf_conj[currConj].first;
                if(diff <= 0) continue;
                somaPenalidade -= inf_conj[currConj].second;
            }
        }
        else{
            includedItems[itemFlip] = 1;
            somaPeso += peso[itemFlip];
            somaValor += lucro[itemFlip];
            for(int currConj: conju[itemFlip]){
                itemsPorConj[currConj]++;
                int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
                if(diff <= 0) continue;
                somaPenalidade += inf_conj[currConj].second;
            }
        }

        agora = chrono::high_resolution_clock::now();

        if(best == OLD) iterationsWithoutIncreasing++;
        else iterationsWithoutIncreasing = 0;
    }

    cout << iter << ' ' << std::chrono::duration<double>(agora - start).count() << endl;

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
    int sol = TABU();
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