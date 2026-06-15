#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

const int INF = std::numeric_limits<int>::max();

//função auxiliar para encontrar se existe um caminho com capacidade livre usando bfs

bool bfs(int grafo_resid[6][6], int fonte, int destino, int pai[], int n) {

    std::vector<bool> visitado(n, false);
    std::queue<int> fila;

    fila.push(fonte);
    visitado[fonte] = true;
    pai[fonte] = -1;

    while (!fila.empty()) {

        int u = fila.front();

        fila.pop();

        for (int v = 0; v < n; v++) {

            //se ainda não foi visitado e tem capacidade disponível no cano

            if (!visitado[v] && grafo_resid[u][v] > 0) {

                if (v == destino) {

                    pai[v] = u;

                    return true;
                }

                fila.push(v);
                pai[v] = u;
                visitado[v] = true;

            }
        }
    }

    return false;

}

int main() {

    std::cout << "=== Heuristica do fluxo maximo ===" << std::endl;

    // 6 nós, 0 = fonte, 5 = destino final

    int n = 6;
    int fonte = 0;
    int destino = 5;

    //matriz de capacidade dos canos, frafo original

    int grafo[6][6] = {

        {0, 16, 13,  0,  0,  0}, // Saídas do nó 0
        {0,  0, 10, 12,  0,  0}, // Saídas do nó 1
        {0,  4,  0,  0, 14,  0}, // Saídas do nó 2
        {0,  0,  9,  0,  0, 20}, // Saídas do nó 3
        {0,  0,  0,  7,  0,  4}, // Saídas do nó 4
        {0,  0,  0,  0,  0,  0}  // Nó 5 (destino final)

    };

    //criar o grafo residual (uma cópia que vai sendo esvaziada conforme a água passa)
    
    int grafo_resid[6][6];

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {

            grafo_resid[i][j] = grafo[i][j];

        }
    }

    int pai[6]; //vetor para guardar o caminho encontrado pelo bfs
    int fluxo_maximo = 0;

    // enquanto existir um caminho com capacidade disponível da Fonte até o destino
    while (bfs(grafo_resid, fonte, destino, pai, n)) {
        
        //encontrar a menor capacidade (o "gargalo") ao longo do caminho achado

        int fluxo_caminho = INF;

        for (int v = destino; v != fonte; v = pai[v]) {

            int u = pai[v];

            fluxo_caminho = std::min(fluxo_caminho, grafo_resid[u][v]);

        }

        //atualizar as capacidades residuais dos canos (subtrai no sentido do fluxo, soma no reverso)
        for (int v = destino; v != fonte; v = pai[v]) {

            int u = pai[v];

            grafo_resid[u][v] -= fluxo_caminho;
            grafo_resid[v][u] += fluxo_caminho;

        }

        //somar o fluxo desse caminho no fluxo máximo total
        fluxo_maximo += fluxo_caminho;

    }

    //resultado
    std::cout << "\n=== Resultado do Algoritmo ===" << std::endl;
    std::cout << "O fluxo maximo que consegue passar pela rede e: " << fluxo_maximo << " unidades/segundo." << std::endl;

    return 0;
    
}