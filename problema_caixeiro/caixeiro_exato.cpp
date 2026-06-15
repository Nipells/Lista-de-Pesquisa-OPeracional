#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

int main() {

    IloEnv env;

    try {

        IloModel modelo(env);

        //usando a mesma matriz de distâncias de 4 cidades da heurística

        int cidades = 4;

        double matriz_distancia[4][4] = {

            {0.0, 10.0, 15.0, 20.0},
            {10.0, 0.0, 35.0, 25.0},
            {15.0, 35.0, 0.0, 30.0},
            {20.0, 25.0, 30.0, 0.0}

        };

        //variaveis
        //x[i][j] = 1 se formos da cidade i direto para a cidade j, 0 caso contrário
  
        IloArray<IloNumVarArray> x(env, cidades);

        for (int i = 0; i < cidades; i++) {

            x[i] = IloNumVarArray(env, cidades, 0, 1, ILOINT);

        }

        //variáveis auxiliares u[i] para a eliminação de subrotas
        IloNumVarArray u(env, cidades, 0, cidades, ILOFLOAT);

        //fo - minimizar a distancia total
        IloExpr distancia_total(env);

        for (int i = 0; i < cidades; i++) {

            for (int j = 0; j < cidades; j++) {

                if (i != j) {

                    distancia_total += matriz_distancia[i][j] * x[i][j];

                }
            }
        }

        modelo.add(IloMinimize(env, distancia_total));
        distancia_total.end();

        //restriçoes

        //ptroibir ir de uma cidade para ela mesma x[i][i] = 0

        for (int i = 0; i < cidades; i++) {

            modelo.add(x[i][i] == 0);

        }

        //cada cidade deve ter exatamente 1 saída
        for (int i = 0; i < cidades; i++) {

            IloExpr saidas(env);

            for (int j = 0; j < cidades; j++) {

                saidas += x[i][j];

            }

            modelo.add(saidas == 1);
            saidas.end();

        }

        //cada cidade deve ter exatamente 1 entrada
        for (int j = 0; j < cidades; j++) {

            IloExpr entradas(env);

            for (int i = 0; i < cidades; i++) {

                entradas += x[i][j];

            }

            modelo.add(entradas == 1);
            entradas.end();

        }

        //eliminação de subrotas - mtz
        //regra: u[i] - u[j] + n * x[i][j] <= n - 1 (para i != j e i, j > 0)

        for (int i = 1; i < cidades; i++) {

            for (int j = 1; j < cidades; j++) {

                if (i != j) {

                    modelo.add(u[i] - u[j] + cidades * x[i][j] <= cidades - 1);

                }
            }
        }

        //solver
        IloCplex cplex(modelo);

        if (cplex.solve()) {

            std::cout << "\n=== Caixeiro viajante: Solucao exata encontrada ===" << std::endl;
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Distancia Otima Minima: " << cplex.getObjValue() << " km" << std::endl;

            //reconstruir a ordem exata da rota, apenas por estetica mesmo
            std::cout << "Rota Otima: Cidade 0";

            int atual = 0;
            bool continuar = true;
            
            while (continuar) {

                for (int proxima = 0; proxima < cidades; proxima++) {

                    if (cplex.getValue(x[atual][proxima]) > 0.5) {

                        std::cout << " -> Cidade " << proxima;

                        atual = proxima;

                        if (atual == 0) continuar = false; //volta ao inicio

                        break;

                    }
                }
            }

            std::cout << std::endl;

        } 
        
        else {

            std::cout << "Nao foi possivel encontrar uma solucao." << std::endl;

        }

    } 
    
    catch (IloException& e) {

        std::cerr << "Erro no CPLEX: " << e << std::endl;

    } 
    
    catch (...) {

        std::cerr << "Erro desconhecido." << std::endl;

    }

    env.end();
    
    return 0;
}