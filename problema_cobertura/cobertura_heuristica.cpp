#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

int main() {

    std::cout << "=== Heuristica gulosa: Problema de cobertura ===" << std::endl;

    //definição de 5 bairros que preciso cobrir (id de 0 a 4)
    int n_elementos = 5;
    
    //dai crii 3 antenas onde cada uma cobre um grupo de bairros

    std::vector<std::set<int>> conjuntos = {

        {0, 1, 2}, 
        {2, 3},    
        {3, 4}     

    };

    //bairros que ainda estao sem sinal

    std::set<int> universo_restante;

    for (int i = 0; i < n_elementos; i++) {

        universo_restante.insert(i);

    }

    std::vector<int> conjuntos_escolhidos;

    //rodo o laco enquanto houver bairro sem cobertura

    while (!universo_restante.empty()) {

        int melhor_conjunto = -1;
        int max_cobertura = -1;

        //procuro a antena que atende mais bairros sem sinal no momento

        for (size_t i = 0; i < conjuntos.size(); i++) {

            int cobertura_atual = 0;

            for (int elemento : conjuntos[i]) {

                if (universo_restante.count(elemento)) {

                    cobertura_atual++;

                }
            }

            if (cobertura_atual > max_cobertura) {

                max_cobertura = cobertura_atual;
                melhor_conjunto = i;

            }
        }

        //se ñ cobrir mais nada, para o laco

        if (melhor_conjunto == -1 || max_cobertura == 0) {
            break;

        }

        //poe-se a melhor antena na minha solucao
        conjuntos_escolhidos.push_back(melhor_conjunto);

        //tirabairros atendidos da minha lista de pendencias
        for (int elemento : conjuntos[melhor_conjunto]) {

            universo_restante.erase(elemento);

        }
    }

    std::cout << "\n=== Resultado da Cobertura ===" << std::endl;
    std::cout << "Conjuntos escolhidos para garantir a cobertura: ";

    for (int c : conjuntos_escolhidos) {

        std::cout << "C" << c << " ";

    }

    std::cout << std::endl;

    if (universo_restante.empty()) {

        std::cout << "Sucesso: Todos os elementos foram 100% cobertos!" << std::endl;
    
    } 
    
    else {

        std::cout << "Atencao: Nao foi possivel cobrir todos os elementos." << std::endl;
    
    }

    return 0;
    
}