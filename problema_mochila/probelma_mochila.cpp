#include <iostream>
#include <vector>
#include <algorithm>

//representação de um item da mochila
struct Item {

    int id;
    double valor;
    double peso;
    double densidade; //valor por quilo

};

//função para ordenar do maior para o menor

bool comparar_densidade(const Item& a, const Item& b) {

    return a.densidade > b.densidade;

}

bool comparar_correto(const Item& a, const Item& b) {

    return a.densidade > b.densidade;

}

int main() {

    std::cout << "=== heuristica gulosa da mochila iniciada ===" << std::endl;

    //mochila com capacidade máxima de 50 kg
    double capacidade_maxima = 50.0;

    // teste com 5 itens (valor, peso)

    std::vector<Item> itens = {

        {1, 60.0,  10.0, 0.0}, //item 1: R$ 60 e 10kg  densidade: 6.0
        {2, 100.0, 20.0, 0.0}, //item 2: R$ 100 e 20kg densidade: 5.0
        {3, 120.0, 30.0, 0.0}, //item 3: R$ 120 e 30kg densidade: 4.0
        {4, 40.0,  10.0, 0.0}, //item 4: R$ 40 e 10kg  densidade: 4.0
        {5, 90.0,  15.0, 0.0}  //item 5: R$ 90 e 15kg  densidade: 6.0

    };

    //calculo da densidade

    for (size_t i = 0; i < itens.size(); i++) {

        itens[i].densidade = itens[i].valor / itens[i].peso;

    }

    //ordenação
    std::sort(itens.begin(), itens.end(), comparar_correto);

    //preenchendo a mochila
    double peso_atual = 0.0;
    double valor_total_ganho = 0.0;

    std::vector<int> itens_escolhidos;

    for (size_t i = 0; i < itens.size(); i++) {

        //se o item couber
        if (peso_atual + itens[i].peso <= capacidade_maxima) {

            itens_escolhidos.push_back(itens[i].id);
            peso_atual += itens[i].peso;
            valor_total_ganho += itens[i].valor;

        }
    }

    
    //resultados

    std::cout << "\n=== Resultado da heuristica gulosa ===" << std::endl;
    std::cout << "Peso total na mochila: " << peso_atual << " kg / " << capacidade_maxima << " kg" << std::endl;
    std::cout << "Valor total arrecadado: R$ " << valor_total_ganho << std::endl;
    
    std::cout << "itens colocados na mochila: ";

    for (int id : itens_escolhidos) {

        std::cout << id << " ";

    }
    
    std::cout << std::endl;

    return 0;
}