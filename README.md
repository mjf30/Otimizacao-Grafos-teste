# Otimização de Grafos

Este repositório contém implementações em C++ de diferentes heurísticas para o problema de mochila com penalidades. Os programas disponíveis são:

- **grasp** – Algoritmo GRASP
- **ils** – Iterated Local Search
- **tabu** – Busca Tabu
- **simulated_annealing** – Simulated Annealing
- **leitura_exec** – Script auxiliar para executar todos os algoritmos nas instâncias

## Compilação

Para compilar todas as heurísticas utilize o comando abaixo (é necessário o `g++`):

```bash
make
```

## Execução

Cada algoritmo recebe dois argumentos: caminho para o arquivo de entrada e caminho do arquivo de saída. Exemplo:

```bash
./grasp instances/scenario1/correlated_sc1/300/kpfs_1.txt saida.txt
```

O programa `leitura_exec` automatiza a execução em todas as instâncias presentes no diretório `instances` e salva os resultados em `resultados/`.

