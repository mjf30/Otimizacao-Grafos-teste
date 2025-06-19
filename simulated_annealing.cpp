#include <iostream>
#include <fstream>     
#include <string>      
#include <vector>      
#include <utility>

using namespace std;

typedef long long ll;

signed main(){
    //Entrada (Cenário/tipo/)
    const int scenarios = 4;
    vector<string> pasta_tipos = {"correlated_sc", "fully_correlated_sc", "not_correlated_sc"};
    vector<string> pasta_tamanho = {"300","500","700","800","1000"};

    for (int s_pasta = 1; s_pasta <= scenarios; s_pasta++) {
        for (const auto& ss_pasta : pasta_tipos) {
            for(const auto& sss_pasta : pasta_tamanho){   
                const int total_arquivos = 10;
                for(int i = 1; i <= total_arquivos; i++){
                    string diretorio_arquivo = "instances/scenario"+to_string(s_pasta)+"/"+ss_pasta+to_string(s_pasta)+"/"+sss_pasta+"/"+"kpfs_" + to_string(i)+ ".txt";
        
                    ifstream arquivo(diretorio_arquivo);
        
                    if (!(arquivo.is_open())) {
                        cout << "Erro ao abrir o arquivo: " << diretorio_arquivo << endl;
                        continue;
                    }
        
                    int itens, quant_sets, capacity;
                    arquivo >> itens >> quant_sets >> capacity;
        
                    vector<int> lucro(itens),peso(itens);
        
                    for(int j = 0; j < itens; j++) arquivo >> lucro[j];
                    for(int j = 0; j < itens; j++) arquivo >> peso[j];
        
                    int permitido_set, custo_set, itens_set;
        
                    vector<vector<int> > set(itens);
                    vector<pair<int,int> > inf_sets(quant_sets);
        
                    for(int j = 0; j < quant_sets; j++){
                        arquivo >> permitido_set >> custo_set >> itens_set;
                        inf_sets[j] = make_pair(permitido_set,custo_set);
        
                        for(int k = 0; k < itens_set; k++){
                            int item; arquivo >> item;
                            set[item].push_back(j);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
