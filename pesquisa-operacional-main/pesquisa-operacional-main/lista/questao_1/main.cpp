#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace chrono;

static vector<double> sizes;
static mt19937 rng;

/*
Compilação: g++ -std=c++17 -o main main.cpp
Execução: ./main <tempo_limite_segundos> <instancia>
Exemplo: ./main 10 < binpacking_inst.txt

Exemplo de entrada:

10
0.45 0.31 0.27 0.58 0.19 0.72 0.33 0.41 0.15 0.38

Saída:

<número_de_bins>
<índice_do_item_1> <índice_do_item_2> ... <índice_do_item_n>
<índice_do_item_1> <índice_do_item_2> ... <índice_do_item_n>
...
<índice_do_item_1> <índice_do_item_2> ... <índice_do_item_n>

A instância de binpacking_inst.txt não utiliza as trocas e realocações do
algoritmo pois encontra um ótimo local apenas com a construção inicial.

------------------------------------------------------------------------------
a) Representação da solução: Como será modelada e manipulada.
------------------------------------------------------------------------------
Representação da solução: vetor de recipientes (bins) contendo o índice do
  item armazenado nele. Os tamanhos dos itens ficam em um vetor global separado
  (sizes), indexado pelos mesmos índices.

Manipulação:
  - A construção inicial é feita com o algoritmo First Fit Decreasing (FFD),
  ordenando os itens de forma decrescente e alocando-os no primeiro recipiente que couber.
  
  - A busca na vizinhança é feita com as estratégias de realocação e troca. Inicialmente,
  move-se um item de um recipiente para outro (se couber). Se não couber, tenta-se permutar
  dois itens de recipientes distintos (se ambos couberem). Aplicando o first improvement,
  a solução só é atualizada se a nova solução for melhor que a anterior.

------------------------------------------------------------------------------
b) Função de avaliação: Como será calculada a qualidade de uma solução.
------------------------------------------------------------------------------
A função de avaliação se dá por f(s) = k, tal que k é o número de recipientes utilizados.
Soluções inviáveis recebem penalidade alta para nunca serem aceitas.

--------------------------------------------------------------------------------
c) Estratégia de busca local: Como será explorada a vizinhança da solução atual.
--------------------------------------------------------------------------------
A busca local é feita com as estratégias de realocação e troca. Inicialmente,
move-se um item de um recipiente para outro (se couber). Se não couber, tenta-se permutar
dois itens de recipientes distintos (se ambos couberem). Aplicando o first improvement,
a solução só é atualizada se a nova solução for melhor que a anterior.
*/
struct Solution {
    vector<vector<int>> bins;

    void removeEmptyBins() {
        bins.erase(
            remove_if(bins.begin(), bins.end(),
                      [](const vector<int>& b) { return b.empty(); }),
            bins.end());
    }

    double binLoad(const vector<int>& bin) const {
        double sum = 0.0;
        for (int idx : bin) {
            sum += sizes[idx];
        }
        return sum;
    }

    bool isFeasible() const {
        for (const auto& bin : bins) {
            if (binLoad(bin) > 1.0 + 1e-9) { // tolerância numérica
                return false;
            }
        }
        return true;
    }
};

// ---------------------------------------------------------------------------
// (b) Função de avaliação
// ---------------------------------------------------------------------------
// O custo é o número de recipientes utilizados (bins não vazios).
// Soluções inviáveis recebem penalidade alta para nunca serem aceitas.
double evaluate(const Solution& sol) {
    if (!sol.isFeasible()) {
        return 1e9;
    }
    int used = 0;
    for (const auto& bin : sol.bins) {
        if (!bin.empty()) {
            ++used;
        }
    }
    return static_cast<double>(used);
}

// ---------------------------------------------------------------------------
// Construção inicial: First Fit Decreasing (FFD)
// ---------------------------------------------------------------------------
Solution constructInitial() {
    int n = static_cast<int>(sizes.size());
    vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }
    sort(order.begin(), order.end(),
         [](int a, int b) { return sizes[a] > sizes[b]; });

    Solution sol;
    sol.bins.clear();

    for (int item : order) {
        bool placed = false;
        for (auto& bin : sol.bins) {
            if (sol.binLoad(bin) + sizes[item] <= 1.0 + 1e-9) {
                bin.push_back(item);
                placed = true;
                break;
            }
        }
        if (!placed) {
            sol.bins.push_back({item});
        }
    }
    return sol;
}

// ---------------------------------------------------------------------------
// (c) Estratégia de busca local — vizinhança por realocação e troca
// ---------------------------------------------------------------------------
// Vizinhança:
//   1) Realocação: mover um item de um recipiente para outro (se couber).
//   2) Troca: permutar dois itens de recipientes distintos (se ambos couberem).
// Estratégia: first improvement — aplica a primeira melhoria encontrada e
// reinicia a busca a partir da nova solução.
bool tryRelocate(Solution& sol, int fromBin, int itemPos, int toBin) {
    if (fromBin == toBin) {
        return false;
    }

    int item = sol.bins[fromBin][itemPos];
    double toLoad = sol.binLoad(sol.bins[toBin]);

    if (toLoad + sizes[item] > 1.0 + 1e-9) {
        return false;
    }

    sol.bins[toBin].push_back(item);
    sol.bins[fromBin].erase(sol.bins[fromBin].begin() + itemPos);

    int binsBefore = static_cast<int>(sol.bins.size());
    sol.removeEmptyBins();
    int binsAfter = static_cast<int>(sol.bins.size());

    return binsAfter < binsBefore;
}

bool trySwap(Solution& sol, int binA, int posA, int binB, int posB) {
    if (binA == binB) {
        return false;
    }

    int itemA = sol.bins[binA][posA];
    int itemB = sol.bins[binB][posB];

    double loadA = sol.binLoad(sol.bins[binA]);
    double loadB = sol.binLoad(sol.bins[binB]);

    double newLoadA = loadA - sizes[itemA] + sizes[itemB];
    double newLoadB = loadB - sizes[itemB] + sizes[itemA];

    if (newLoadA > 1.0 + 1e-9 || newLoadB > 1.0 + 1e-9) {
        return false;
    }

    sol.bins[binA][posA] = itemB;
    sol.bins[binB][posB] = itemA;

    int binsBefore = static_cast<int>(sol.bins.size());
    sol.removeEmptyBins();
    int binsAfter = static_cast<int>(sol.bins.size());

    return binsAfter < binsBefore;
}

bool localSearchStep(Solution& sol) {
    int numBins = static_cast<int>(sol.bins.size());

    vector<int> binOrder(numBins);
    for (int i = 0; i < numBins; ++i) {
        binOrder[i] = i;
    }
    shuffle(binOrder.begin(), binOrder.end(), rng);

    for (int fromBin : binOrder) {
        int itemsInBin = static_cast<int>(sol.bins[fromBin].size());
        for (int itemPos = 0; itemPos < itemsInBin; ++itemPos) {
            vector<int> targetOrder(numBins);
            for (int i = 0; i < numBins; ++i) {
                targetOrder[i] = i;
            }
            shuffle(targetOrder.begin(), targetOrder.end(), rng);

            for (int toBin : targetOrder) {
                Solution candidate = sol;
                if (tryRelocate(candidate, fromBin, itemPos, toBin)) {
                    sol = move(candidate);
                    return true;
                }
            }
        }
    }

    for (int i = 0; i < numBins; ++i) {
        for (int j = i + 1; j < numBins; ++j) {
            int binA = binOrder[i];
            int binB = binOrder[j];
            for (int posA = 0; posA < static_cast<int>(sol.bins[binA].size()); ++posA) {
                for (int posB = 0; posB < static_cast<int>(sol.bins[binB].size()); ++posB) {
                    Solution candidate = sol;
                    if (trySwap(candidate, binA, posA, binB, posB)) {
                        sol = move(candidate);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

Solution localSearch(Solution initial, steady_clock::time_point deadline) {
    Solution best = move(initial);
    double bestCost = evaluate(best);

    while (steady_clock::now() < deadline) {
        Solution current = best;
        bool improved = false;

        while (steady_clock::now() < deadline) {
            if (!localSearchStep(current)) {
                break;
            }
            improved = true;
            double currentCost = evaluate(current);
            if (currentCost < bestCost) {
                best = current;
                bestCost = currentCost;
            }
        }

        if (!improved) {
            break;
        }
    }

    best.removeEmptyBins();
    return best;
}

void printSolution(Solution sol) {
    sol.removeEmptyBins();
    cout << static_cast<int>(evaluate(sol)) << "\n";
    for (const auto& bin : sol.bins) {
        if (bin.empty()) {
            continue;
        }
        double sum = 0.0;
        for (size_t i = 0; i < bin.size(); ++i) {
            if (i > 0) {
                cout << " ";
            }
            cout << bin[i];
            sum += sizes[bin[i]];
        }
        cout << " (" << sum << ")\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <tempo_limite_segundos>\n";
        cerr << "Entrada (stdin): n seguido de n tamanhos (0 <= s <= 1)\n";
        return 1;
    }

    double timeLimit = stod(argv[1]);
    if (timeLimit <= 0) {
        cerr << "Tempo limite deve ser positivo.\n";
        return 1;
    }

    int n;
    if (!(cin >> n) || n <= 0) {
        cerr << "Erro ao ler n.\n";
        return 1;
    }

    sizes.resize(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> sizes[i]) || sizes[i] < 0.0 || sizes[i] > 1.0) {
            cerr << "Tamanho invalido no item " << i << ".\n";
            return 1;
        }
    }

    rng.seed(static_cast<unsigned>(
        steady_clock::now().time_since_epoch().count()));

    auto deadline = steady_clock::now() +
                    duration_cast<steady_clock::duration>(
                        duration<double>(timeLimit));

    Solution initial = constructInitial();
    Solution best = localSearch(move(initial), deadline);

    printSolution(best);
    return 0;
}
