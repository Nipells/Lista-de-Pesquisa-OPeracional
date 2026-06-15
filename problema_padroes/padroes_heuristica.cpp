#include <iostream>
#include <vector>
#include <algorithm>

//estrutura para representar os pedidos de corte das latinhas
struct Pedido {

    double tamanho;
    int quantidade;

};

int main() {

    std::cout << "=== Heuristica Gulosa: padroes de corte ===" << std::endl;

    //cada rolo grande de aluminio tem 100 cm de largura
    double largura_rolo_padrao = 100.0;

    //pedidos de tiras de aluminio para as latinhas

    std::vector<Pedido> pedidos = {

        {45.0, 3}, // preciso de 3 cortes de 45 cm
        {35.0, 4}, // preciso de 4 cortes de 35 cm
        {20.0, 5}  // preciso de 5 cortes de 20 cm

    };

    int rolos_utilizados = 0;
    
    std::cout << "\n=== Esquema de corte dos rolos ===" << std::endl;

    // laco principal para processar os pedidos ate atender todos
    while (true) {

        double espaco_livre = largura_rolo_padrao;
        bool cortou_algo = false;

        //encaixar o maior numero de cortes possivel no rolo atual

        for (size_t i = 0; i < pedidos.size(); i++) {

            while (pedidos[i].quantidade > 0 && espaco_livre >= pedidos[i].tamanho) {
                
                if (!cortou_algo) {

                    rolos_utilizados++;
                    std::cout << "Rolo " << rolos_utilizados << " -> Cortes feitos: ";
                    cortou_algo = true;

                }
                
                std::cout << pedidos[i].tamanho << "cm ";
                espaco_livre -= pedidos[i].tamanho;
                pedidos[i].quantidade--;

            }
        }

        if (cortou_algo) {

            std::cout << "| Sobra de aluminio: " << espaco_livre << "cm" << std::endl;
        }

        //erifico se ainda resta algum pedido pendente

        bool tem_pedido = false;

        for (size_t i = 0; i < pedidos.size(); i++) {

            if (pedidos[i].quantidade > 0) {

                tem_pedido = true;

                break;

            }
        }

        // se nao houver mais pedidos, finalizo o laco
        if (!tem_pedido) {

            break;
            
        }
    }

    std::cout << "\nTotal de rolos grandes de 100cm utilizados: " << rolos_utilizados << std::endl;

    return 0;
}