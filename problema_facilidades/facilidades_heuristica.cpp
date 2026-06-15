#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

int main() {
    std::cout << "=== Heuristica Gulosa: Problema das Facilidades ===" << std::endl;

    //2 locais possiveis para depósitos e 3 clientes
    int locais = 2;
    int clientes = 3;

    // custo fixo para abrir o deposito no Local 1 e no Local 2
    double custo_abertura[] = {1000.0, 1500.0};

    // matriz de custos de transporte: custo[local][cliente]

    double custo_transporte[2][3] = {
        {300.0, 500.0, 200.0}, // custos saindo do Local 1
        {100.0, 200.0, 150.0}  // custos saindo do Local 2

    };

    // passo 1: calculo do custo total simulando a abertura de cada local individualmente
    int melhor_local = -1;
    double menor_custo_total = std::numeric_limits<double>::max();

    for (int i = 0; i < locais; i++) {

        double custo_simulado = custo_abertura[i];
        
        for (int j = 0; j < clientes; j++) {

            custo_simulado += custo_transporte[i][j];
        }

        if (custo_simulado < menor_custo_total) {

            menor_custo_total = custo_simulado;
            melhor_local = i;
        }
    }

    //resultados
    std::cout << "\n=== Decisao de Instalacao e Atendimento ===" << std::endl;
    std::cout << "Abri o deposito no Local: " << melhor_local + 1 << std::endl;
    std::cout << "Custo fixo de abertura: R$ " << custo_abertura[melhor_local] << std::endl;
    
    std::cout << "Clientes atendidos por esse deposito:" << std::endl;
    double custo_logistico = 0.0;

    for (int j = 0; j < clientes; j++) {

        std::cout << " -> Cliente " << j + 1 << " | Custo de envio: R$ " << custo_transporte[melhor_local][j] << std::endl;
        custo_logistico += custo_transporte[melhor_local][j];

    }

    std::cout << "\nCusto Total da Solucao: R$ " << menor_custo_total << std::endl;

    return 0;
}