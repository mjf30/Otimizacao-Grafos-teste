#include <iostream>
#include <fstream>     
#include <string>      
#include <vector>      
#include <utility>
#include <time.h>

using namespace std;

typedef long long ll;

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
                        cout << "Erro ao abrir o arquivo: " << diretorio_arquivo << endl;continue;
                    }

                    //Início da leitura
                    //cout << "Estou lendo o arquivo:" << diretorio_arquivo << endl;
                    int itens, quant_conj, capacidade; arquivo >> itens >> quant_conj >> capacidade;
    
                    vector<int> lucro(itens),peso(itens); //Lucro e peso do item i
                    for(int j = 0; j < itens; j++) arquivo >> lucro[j];
                    for(int j = 0; j < itens; j++) arquivo >> peso[j];
        
                    int lim_conj, penalidade_conj, itens_conj;
                    vector<vector<int> > conj(itens); //Lista de conjunto que o item i pertence (conj[i] = lista)
                    vector<pair<int,int> > inf_conj(quant_conj);//(Limite de itens na solução,Penalidade) do conjunto i

                    for(int j = 0; j < quant_conj; j++){
                        arquivo >> lim_conj >> penalidade_conj >> itens_conj;
                        inf_conj[j] = make_pair(lim_conj,penalidade_conj);
        
                        for(int k = 0; k < itens_conj; k++){
                            int item; arquivo >> item;
                            conj[item].push_back(j);
                        }
                    }

                    //Fim na leitura

                    //Chamada da função
                }
            }
        }
    }

    return 0;
}
