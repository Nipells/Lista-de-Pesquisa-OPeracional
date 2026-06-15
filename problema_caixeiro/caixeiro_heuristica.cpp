#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

int main() {
    std::cout << "=== Heuristica do vizinho mais proximo ===" << std::endl;

    //matriz de distâncias
    // matriz[i][j] guarda a distância da cidade i para a cidade j

    int cidades = 4;
    double matriz_distancia[4][4] = {

        {0.0, 10.0, 15.0, 20.0}, //distâncias a partir da Cidade 0
        {10.0, 0.0, 35.0, 25.0}, //distâncias a partir da Cidade 1
        {15.0, 35.0, 0.0, 30.0}, //distâncias a partir da Cidade 2
        {20.0, 25.0, 30.0, 0.0}  //distâncias a partir da Cidade 3

    };

    //vetor para controlar quem já foi visitado - td falso ate agr
    std::vector<bool> visitada(cidades, false);
    
    //etor para guardar a ordem da rota
    std::vector<int> rota;

    //começando a viagem pela cidade 0
    int cidade_atual = 0;

    rota.push_back(cidade_atual);
    visitada[cidade_atual] = true;

    double distancia_total = 0.0;

    //procurar o vizinho mais próximo para as próximas (cidades - 1) paradas

    for (int passo = 1; passo < cidades; passo++) {

        int melhor_vizinho = -1;
        double menor_distancia = std::numeric_limits<double>::max();

        //olha pra todas as cidades possieis

        for (int vizinho = 0; vizinho < cidades; vizinho++) {

            //se ainda não foi visitada e a distância for menor do que a conhecida

            if (!visitada[vizinho] && matriz_distancia[cidade_atual][vizinho] < menor_distancia) {

                menor_distancia = matriz_distancia[cidade_atual][vizinho];

                melhor_vizinho = vizinho;

            }
        }

        //viajar para o melhor vizinho encontrado

        if (melhor_vizinho != -1) {

            rota.push_back(melhor_vizinho);
            visitada[melhor_vizinho] = true;
            distancia_total += menor_distancia;
            cidade_atual = melhor_vizinho; //se achar muda pra la

        }
    }

    //voltar para a cidade de origem, cidade 0

    int cidade_origem = rota[0];

    distancia_total += matriz_distancia[cidade_atual][cidade_origem];
    rota.push_back(cidade_origem);

    //resultados

    std::cout << "\n=== Resultado da heuristica construtiva ===" << std::endl;

    std::cout << "Rota calculada: ";

    for (size_t i = 0; i < rota.size(); i++) {

        std::cout << "Cidade " << rota[i];
        if (i < rota.size() - 1) std::cout << " -> ";

    }
    
    std::cout << std::endl;

    std::cout << "Distancia Total do Circuito: " << distancia_total << " km" << std::endl;

    return 0;
}