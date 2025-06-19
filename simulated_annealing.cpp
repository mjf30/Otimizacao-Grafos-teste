#include "bits/stdc++.h"
using namespace std;

typedef long long ll;

signed main(){
    //Entrada (Cenário/tipo/)
    const int total_arquivos = 10;git push origin main
    for(int i = 1; i < total_arquivos; i++){
        string nome_arquivo = "kpfs_"+i+".txt";

        ifstream arquivo(nome_arquivo);

        if (!(arquivo.is_open())) {
            cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
            continue;
        }

        int itens, sets, capacity;
        arquivo >> itens >> sets >> capacity;

        vector<int> lucro(itens),peso(itens);

        for(int j = 0; j < itens; j++) arquivo >> lucro[j];
        for(int j = 0; j < itens; j++) arquivo >> peso[j];

        int permitido_set, custo_set, itens_set;
        //declarar estrutura que guarda itens do set
        for(int j = 0; j < sets; j++){
            arquivo >> permitido_set >> custo_set >> itens_set;
            for(int k = 0; k < itens_set; k++){
                int iten; arquivo >> item;
                //Guardar na estrutura 
            }
        }
    }

    return 0;
}
