#include <iostream>
#include <vector>
#include <fstream>
#include <chrono> //criterio de parada por tempo

//estrutura da representacao de uma bin (caixa)
struct Caixa {
    double capacidade_restante;
    std::vector<int> itens_inside; //guarda os ids dos itens que estao dentro
};

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
        std::cerr << "Erro ao abrir arquivo" << std::endl;
        return 1;
    }

    int itens;
    double capacidade_max;
    arquivo >> itens >> capacidade_max; //?

    std::vector<double> tamanhos(itens);

    for (int i = 0; i < itens; i++){
        arquivo >> tamanhos[i];
    }
    arquivo.close();

    std::cout << "=== Meta-heuristica iniciada ===" <<std::endl;
    std::cout << "Itens para empacotar: " << itens << " | Tempo limite: " << tempo_limite << "s" << std::endl;

    //solucao inicial - heuristica construriva

    return 0;
}