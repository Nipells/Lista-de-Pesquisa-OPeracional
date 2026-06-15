#include <iostream>
#include <vector>
#include <fstream>
#include <chrono> //criterio de parada por tempo
#include <string>
#include <algorithm> //biblio usada para ordenacao, o sort

//estrutura da representacao de uma bin (caixa)
struct Caixa {
    double capacidade_restante;
    std::vector<int> itens_inside; //guarda os ids dos itens que estao dentro
};

struct ItemAux {
    int id;
    double tamanho;
};

//funcao que ordena do maior para o menor, por comparacao

bool comparar_maior_e_menor(const ItemAux& a, const ItemAux& b) {
    return a.tamanho > b.tamanho;
}

//funcao que conta quantas caixas estao sendo usadas
int calcular_custo(const std::vector <Caixa>& solucao){
    return solucao.size();
}

int main(int argc, char* argv[]){

    //validacao dos argumentos, usa o arquivo e o limite de tempo
    if (argc < 3){
        std::cerr << "Erro: Use assim: " << argv[0] << " <instancia.txt> <tempo_limite_segundos>" << std::endl;
        return 1;
    }

    //leitura do tempo limite
    int tempo_limite = std::stoi(argv[2]);

    //leitura da instancia
    std::ifstream arquivo(argv[1]);

    if (!arquivo.is_open()){
        std::cerr << "Erro ao abrir arquivo" <<  argv[1] << std::endl;
        return 1;
    }

    int itens;
    double capacidade_max;
    arquivo >> itens >> capacidade_max; //?

    std::vector<double> tamanhos(itens);
    std::vector<ItemAux> itens_ordenados(itens);

    for (int i = 0; i < itens; i++){
        arquivo >> tamanhos[i];
        itens_ordenados[i].id = i;
        itens_ordenados[i].tamanho = tamanhos[i];
    }
    arquivo.close();

    std::cout << "=== Meta-heuristica iniciada ===" <<std::endl;
    
    //inicia o cronometro do criterio de parada
    auto tempo_start = std::chrono::steady_clock::now();

    //ordenacao: transforma o ff em ffd
    std::sort(itens_ordenados.begin(), itens_ordenados.end(), comparar_maior_e_menor);

    //solucao inicial  ffd - heuristica construriva

    std::vector <Caixa> solucao;

    for (int i = 0; i < itens; i++){
        int id_original = itens_ordenados[i].id;
        double tam_item = itens_ordenados[i].tamanho;
        bool alocado = false;

        //tenativa de colocar o item ja odenado em alguma caixa ja existente
        for (size_t j = 0; j < solucao.size(); j++){
            if (solucao[j].capacidade_restante >= tam_item){
                solucao[j].itens_inside.push_back(id_original);
                solucao[j].capacidade_restante -= tam_item;
                alocado = true;
                break;
            }
        }

        //se nao coube em nenhuma caixa aberta, cria uma nova caixa
        if(!alocado){
            Caixa new_box;
            new_box.capacidade_restante = capacidade_max - tam_item;
            new_box.itens_inside.push_back(id_original);
            solucao.push_back(new_box);
        }
    }

    std::cout << "Custo da solucao inicial - ffd: " << calcular_custo(solucao) << " caixas." << std::endl;

    //estrategia de bisca local

    bool progrediu = true;

    while (progrediu){
        progrediu = false;

        //verifica se o tempo limite estourou, um criterio de parada
        auto tempo_atual = std::chrono::steady_clock::now();
        auto segundos_passados = std::chrono::duration_cast<std::chrono::seconds>(tempo_atual - tempo_start).count();

        std::cout << "Tempo limkte de " <<tempo_limite << "s atingido! Interrompido busca loical." << std::endl;
        break;
    }

    //olha a vizinhanca e tenta mover pra uma caixa que esteja vazia
    for (size_t c_origem = 0; c_origem < solucao.size() && !progrediu; c_origem++){
        for(size_t idx_item = 0; idx_item < solucao[c_origem].itens_inside.size() && !progrediu; idx_item++){

            int item = solucao[c_origem].itens_inside[idx_item];
            double tam_item = tamanhos[item];

            for(size_t c_destino = 0; c_destino < solucao.size(); c_destino++){
                if(c_origem == c_destino) continue;

                if(solucao[c_destino].capacidade_restante >= tam_item){
                    int custo_antes = calcular_custo(solucao);

                    //faz o movimento na memoria
                    solucao[c_destino].itens_inside.push_back(item);
                    solucao[c_destino].capacidade_restante -= tam_item;
                    solucao[c_origem].itens_inside.erase(solucao[c_origem].itens_inside.begin() + idx_item);
                    solucao[c_origem].capacidade_restante += tam_item;

                    std::vector<Caixa> solucao_temporaria = solucao;
                    
                    if (solucao_temporaria[c_origem].itens_inside.empty()){
                        solucao_temporaria.erase(solucao_temporaria.begin() + c_origem);

                    }

                    if(calcular_custo(solucao_temporaria) < custo_antes) {
                        solucao = solucao_temporaria;
                        progrediu = true;
                        std::cout << "-> Progresso encontrado! Novo custo: " << calcular_custo(solucao) << " caixas." << std::endl;
                        break;
                    }

                    else{

                        //desfaz o movimento
                        solucao[c_origem].itens_inside.insert(solucao[c_origem].itens_inside.begin() + idx_item, item);
                        solucao[c_origem].capacidade_restante -= tam_item;
                        solucao[c_destino].itens_inside.pop_back();
                        solucao[c_destino].capacidade_restante += tam_item;

                    }
                }
            }
        }
    }

    //resultados finais

    std::cout << "\n === Resultado da Meta-heuristica | ffd ===" << std::endl;
    std::cout << "Numero final de caixas: " << calcular_custo(solucao) << std::endl;

    for(size_t j=0; j < solucao.size(); j++){
        std::cout << "Caixa " << j << " (Espaco livre: " << solucao[j].capacidade_restante << ") -> Itens: ";
        for (int item : solucao[j].itens_inside){
            std::cout << item << "(tam: " << tamanhos[item] << ") | ";
        }

        std::cout << std::endl;
    }



    return 0;
}