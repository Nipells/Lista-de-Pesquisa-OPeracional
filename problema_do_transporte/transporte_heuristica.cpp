#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "heuristica canto noroeste iniciada" << std::endl;

    //cenário: 2 fabricas e 3 clientes
    int fabricas = 2;
    int clientes = 3;

    //dados do problema
    std::vector<double> capacidade = {500.0, 700.0};
    std::vector<double> demanda = {300.0, 400.0, 500.0};

    //matriz de custos - apenas para calcular o custo final da solução gerada

    double custo[2][3] = {

        {4.0, 2.0, 5.0},
        {3.0, 6.0, 1.0}
    };

    //matriz para guardar a solução gerada pela heurística

    std::vector<std::vector<double>> x(fabricas, std::vector<double>(clientes, 0.0));

    //indices para rastrear onde estamos na matriz - começa no canto noroeste: 0,0
    int i = 0; //linha (fabrica)
    int j = 0; //coluna (cliente)

   //canto noroeste

    while (i < fabricas && j < clientes) {

        //envia o máximo possível: o mínimo entre o que a fábrica tem e o que o cliente quer
        double quantidade = std::min(capacidade[i], demanda[j]);

        //aloca a quantidade na matriz de solução
        x[i][j] = quantidade;

        //atualiza os saldos de oferta e demanda
        capacidade[i] -= quantidade;
        demanda[j] -= quantidade;

        //se a fábrica esgotou sua capacidade, desce para a próxima linha
        if (capacidade[i] == 0) {

            i++;

        }
        //se o cliente teve sua demanda atendida, passa para a próxima coluna
        else if (demanda[j] == 0) {

            j++;

        }
    }

    //solucao

    double custo_total = 0;
    std::cout << "\n=== Resultado da heuristica - canto noroeste ===" << std::endl;
    
    for (int f = 0; f < fabricas; f++) {

        std::cout << "\n[ Fabrica " << f + 1 << " ]" << std::endl;

        for (int c = 0; c < clientes; c++) {

            if (x[f][c] > 0) {

                std::cout << " -> Enviou " << x[f][c] << " unidades para o Cliente " << c + 1 << std::endl;
                custo_total += x[f][c] * custo[f][c];

            }
        }
    }

    std::cout << "\nCusto Logistico Total Obtido: R$ " << custo_total << std::endl;

    return 0;
}