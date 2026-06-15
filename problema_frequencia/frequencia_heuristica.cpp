#include <iostream>
#include <vector>
#include <set>

int main() {

    std::cout << "=== Heuristica Gulosa: Atribuicao de Frequencia ===" << std::endl;

    // defini 4 antenas na minha rede
    int antenas = 4;

    // matriz de interferência: se for 1, as antenas estao perto e nao podem usar a mesma frequencia
    int interferencia[4][4] = {

        {0, 1, 1, 0}, // antena 0 tem conflito com 1 e 2
        {1, 0, 1, 1}, // antena 1 tem conflito com 0, 2 e 3
        {1, 1, 0, 1}, // antena 2 tem conflito com 0, 1 e 3
        {0, 1, 1, 0}  // antena 3 tem conflito com 1 e 2
    };

    // vetor para guardar a frequencia de cada antena (-1 significa sem frequencia)
    std::vector<int> frequencia_atribuida(antenas, -1);

    // atribuo a frequencia da primeira antena
    frequencia_atribuida[0] = 0;

    // laco para definir as frequencias das outras antenas
    for (int i = 1; i < antenas; i++) {

        // conjunto para rastrear as frequencias que os vizinhos já estao usando
        std::set<int> frequencias_bloqueadas;
        
        for (int j = 0; j < antenas; j++) {

            if (interferencia[i][j] == 1 && frequencia_atribuida[j] != -1) {
                frequencias_bloqueadas.insert(frequencia_atribuida[j]);
            }
        }

        // procuro a menor frequencia livre (começando do canal 0)
        int canal = 0;
        while (frequencias_bloqueadas.count(canal)) {
            canal++;
        }

        // atribuo o canal encontrado para a antena atual
        frequencia_atribuida[i] = canal;

    }

    //resultados
    std::cout << "\n=== Canais de Frequencia Distribuidos ===" << std::endl;

    int max_canais = 0;

    for (int i = 0; i < antenas; i++) {

        std::cout << " -> Antena " << i << " | Canal de Frequencia: " << frequencia_atribuida[i] << std::endl;
        
        if (frequencia_atribuida[i] > max_canais) {

            max_canais = frequencia_atribuida[i];
            
        }
    }

    std::cout << "\nTotal de frequencias distintas utilizadas: " << max_canais + 1 << std::endl;

    return 0;
}