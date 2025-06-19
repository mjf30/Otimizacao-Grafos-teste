#include <iostream>
#include <fstream>     
#include <string>      
#include <vector>      
#include <utility>
#include <time.h>

using namespace std;

typedef long long ll;

int itens, quant_conj, capacidade;
vector<int> lucro,peso; //Lucro e peso do item i
int lim_conj, penalidade_conj, itens_conj;
vector<vector<int> > conj; //Lista de conjunto que o item i pertence (conj[i] = lista)
vector<pair<int,int> > inf_conj;//(Limite de itens na solução,Penalidade) do conjunto i

//Função meta heuristica

signed main(){
    //Leitura dos arquivos
    const int cenarios = 4;
    vector<string> pasta_tipos = {"correlated_sc", "fully_correlated_sc", "not_correlated_sc"};
    vector<string> pasta_tamanho = {"300","500","700","800","1000"};

    for (int s_pasta = 1; s_pasta <= cenarios; s_pasta++) {
        for (const auto& ss_pasta : pasta_tipos) {
            for(const auto& sss_pasta : pasta_tamanho){   
                const int total_arquivos = 10;
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
        
                    conj.assign(itens,vector<int>()); inf_conj.assign(quant_conj,make_pair(0,0));
                    for(int j = 0; j < quant_conj; j++){
                        arquivo >> lim_conj >> penalidade_conj >> itens_conj;
                        inf_conj[j].first = lim_conj; inf_conj[j].second = penalidade_conj;
        
                        for(int k = 0; k < itens_conj; k++){
                            int item; arquivo >> item;
                            conj[item].push_back(j);
                        }
                    }
                    //Fim na leitura

                    //Calcular o tempo
                    
                    //Chamada da função
                    //int sol = meta_her();

                    //Cout com resultados
                }
            }
        }
    }
    return 0;
}
