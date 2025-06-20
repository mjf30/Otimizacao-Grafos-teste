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


//Função meta heuristica
const int tempoLimite = 2;
const double alpha = 0.99999;
const double _div = INT_MAX;

mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

int Simulated_Annealing(){

    bitset<1000> includedItems;

    int somaValor = 0, somaPenalidade = 0, somaPeso = 0;
    vector<int> itemsPorConj(quant_conj, 0);

    // Começa com conjunto aleatório

    uniform_int_distribution<int> uid(0, INT_MAX);
    vector<int> perm(itens);
    for(int i = 0; i < itens; i++) perm[i] = i;

    shuffle(perm.begin(), perm.end(),rng);

    for(int currItem: perm){
        int incluso = uid(rng)%2;

        if(incluso){
            if(peso[currItem] + somaPeso > capacidade) continue;
            includedItems[currItem] = 1;
            somaPeso += peso[currItem];
            somaValor += lucro[currItem];
            for(int currConj: conju[currItem]){
                itemsPorConj[currConj]++;
                int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
                if(diff <= 0) continue;
                somaPenalidade += inf_conj[currConj].second;
            }
        }
    }

    // aqui começa o algoritmo

    int best = somaValor - somaPenalidade;

    double temperature = 1e9;

    auto GetTemperature = [&](){
        temperature = temperature*alpha;
    };

    time_t iniTime = time(NULL);

    time_t agora = time(NULL);
    // while(temperature > 1e-7){
    while(difftime(time(NULL), iniTime) < tempoLimite){
        bool ok = true;
        while(ok){
            int itemFlip = uid(rng)%itens;
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

            if(delta > 0 || (uid(rng) / _div) > exp((delta)/temperature)){
                if(includedItems[itemFlip]){
                    includedItems[itemFlip] = 0;
                    somaPeso -= peso[itemFlip];
                    somaValor -= lucro[itemFlip];
                    for(int currConj: conju[itemFlip]){
                        int diff = itemsPorConj[currConj] - inf_conj[currConj].first;
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

        GetTemperature();
    }

    return best;
}


signed main(){
    //Leitura dos arquivos
    const int cenarios = 4;
    vector<string> pasta_tipos = {"correlated_sc", "fully_correlated_sc", "not_correlated_sc"};
    vector<string> pasta_tamanho = {"300","500","700","800","1000"};

    for (int s_pasta = 1; s_pasta <= cenarios; s_pasta++) {
        for (const auto& ss_pasta : pasta_tipos) {
            for(const auto& sss_pasta : pasta_tamanho){   
                const int total_arquivos = 10;
                vector<int> solucao(total_arquivos);
                vector<double>tempos(total_arquivos);

                for(int i = 1; i <= total_arquivos; i++){

                    string diretorio_arquivo = "instances/scenario"+to_string(s_pasta)+"/"+ss_pasta+to_string(s_pasta)+"/"+sss_pasta+"/"+"kpfs_" + to_string(i)+ ".txt";
                    ifstream arquivo(diretorio_arquivo);
                    if (!(arquivo.is_open())) {
                        //Cenário 1, correlated,500, kpfs_2 não existe
                        if(diretorio_arquivo == "instances/scenario1/correlated_sc1/500/kpfs_2.txt") continue;
                        cout << "Erro ao abrir o arquivo: " << diretorio_arquivo << endl;continue;
                    }

                    //Início da leitura
                    //cout << "Estou lendo o arquivo:" << diretorio_arquivo << endl;
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
                    solucao[i-1] = sol;
                    tempos[i-1] = execution_time;
                }
                
                //Cout com resultados
                string metaHeuristica = "SIMULATEANNEALING";
                string diretorio_saida = "resultados/"+metaHeuristica+"/"+ss_pasta+"/kpfs_saida_"+sss_pasta+"_"+to_string(s_pasta)+".txt";
                ofstream out(diretorio_saida);
                if (out.is_open()) {
                    for(int i = 0; i < total_arquivos; i++) out << solucao[i] << " ";
                    out << '\n';
                    for(int i = 0; i < total_arquivos;i++) out << tempos[i] << " ";
                } else {
                    cerr << "Erro ao criar arquivo de saída." << endl;
                }
            }
        }
    }
    return 0;
}
