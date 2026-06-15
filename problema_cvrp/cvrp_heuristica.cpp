#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

int main() {
    std::cout << "=== Heuristica gulosa: Roteamento de veiculos ===" << std::endl;

    // defini o deposito (id 0) e 4 clientes (ids 1 a 4)
    int pontos = 5; 
    int deposito = 0;
    double capacidade_veiculo = 15.0;

    // demanda de cada ponto (deposito tem demanda 0)
    double demanda[] = {0.0, 5.0, 8.0, 6.0, 4.0};

    // matriz de distancias entre todos os pontos
    double distancias[5][5] = {
        {0.0, 10.0, 12.0, 15.0, 20.0}, // distancias a partir do deposito (0)
        {10.0, 0.0,  5.0, 18.0, 25.0}, // distancias a partir do cliente 1
        {12.0,  5.0, 0.0,  14.0, 17.0}, // distancias a partir do cliente 2
        {15.0, 18.0, 14.0, 0.0,  8.0},  // distancias a partir do cliente 3
        {20.0, 25.0, 17.0,  8.0, 0.0}   // distancias a partir do cliente 4
    };

    // vetor para controlar quais clientes ja foram atendidos
    std::vector<bool> atendido(pontos, false);
    atendido[deposito] = true;

    int clientes_restantes = 4;
    int veiculo_atual = 1;

    // laco principal roda ate que todos os clientes sejam atendidos
    while (clientes_restantes > 0) {
        std::cout << "\n[ Rota do Veiculo " << veiculo_atual << " ]" << std::endl;
        std::cout << "Deposito";

        int ponto_atual = deposito;
        double carga_atual = 0.0;
        bool rota_ativa = true;

        // monta a rota do veiculo atual com base no vizinho mais proximo
        while (rota_ativa) {

            int proximo_cliente = -1;
            double menor_distancia = std::numeric_limits<double>::max();

            // procura o cliente mais proximo que caiba na capacidade de carga
            for (int i = 1; i < pontos; i++) {

                if (!atendido[i] && (carga_atual + demanda[i] <= capacidade_veiculo)) {
                    
                    if (distancias[ponto_atual][i] < menor_distancia) {

                        menor_distancia = distancias[ponto_atual][i];
                        proximo_cliente = i;

                    }
                }
            }

            // se achei um cliente valido, faço a entrega
            if (proximo_cliente != -1) {

                std::cout << " -> Cliente " << proximo_cliente;

                carga_atual += demanda[proximo_cliente];

                atendido[proximo_cliente] = true;

                clientes_restantes--;
                
                ponto_atual = proximo_cliente;

            } 
            
            else {

                // se nao couber mais ninguem ou nao houver clientes, volto para o deposito
                std::cout << " -> Deposito" << std::endl;
                std::cout << "Carga total transportada: " << carga_atual << " unidades" << std::endl;
                rota_ativa = false;

            }
        }

        veiculo_atual++;

    }

    return 0;

}