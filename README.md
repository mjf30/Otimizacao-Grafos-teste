# Otimizacao-Grafos

Este reposit\xC3\xB3rio cont\xC3\xA9m implementa\xC3\xA7\xC3\xB5es em C++ de diferentes meta-heur\xC3\xADsticas para o problema da mochila com conjuntos de for\xC3\xA7a (Knapsack Problem with Forfeit Sets).

## Compila\xC3\xA7\xC3\xA3o
Utilize o `g++` com suporte a C++17 para compilar cada algoritmo:

```bash
g++ -std=c++17 grasp.cpp -o grasp
g++ -std=c++17 ils.cpp -o ils
g++ -std=c++17 simulated_annealing.cpp -o simulated_annealing
g++ -std=c++17 tabu.cpp -o tabu
g++ -std=c++17 leitura_exec.cpp -o leitura_exec
```

## Execu\xC3\xA7\xC3\xA3o
O execut\xC3\xA1vel `leitura_exec` percorre as inst\xC3\xA2ncias localizadas na pasta `instances` e gera sa\xC3\xADdas em `resultados/<algoritmo>/<tipo>/saida_<quantidade>.txt`.

```bash
./leitura_exec
```

Os diret\xC3\xB3rios de resultados j\xC3\xA1 est\xC3\xA3o inclu\xC3\xADdos no reposit\xC3\xB3rio para refer\xC3\xAAncia.

