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


//Função meta-heurística

int GRASP(){
    int sol;
    return sol;
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